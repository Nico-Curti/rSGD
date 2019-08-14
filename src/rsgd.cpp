#include <rsgd.h>

long int compute_errs (const NetNK & net, const Patterns & p)
{
  long int errs = 0L;

  for (long int i = 0L; i < p.Nrow; ++i)
  {
    long int h_out = 0L;

    for (long int j = 0L; j < net.K; ++j)
    {
      long int dot = 0L;
      long int sum = 0L;

      for (long int k = 0L; k < net.N; ++k)
      {
        long int idx1 = i * p.Ncol + k;
        long int idx2 = j * net.N + k;
        dot += p.input[idx1] * net.J[idx2];
        sum += p.input[idx1] + net.J[idx2];
      }

      h_out += 4L * dot - 2L * sum + net.N > 0L ? 1L : -1L;
    }

    long int t_out = h_out > 0L ? 1L : -1L;
    errs += t_out != p.output[i];
  }
  return errs;
}


void subepoch (NetNK & net, const Patterns & p, PatternPermutation & patt_perm, const parameters & params)
{
  long int * Mbatch = nullptr;
  long int size_mb = patt_perm.get_batch(Mbatch);
  long int * h = new long int [net.K];

  std :: vector < long int > wrongh;
  std :: vector < long int > indh;

  std :: fill_n (net.DeltaH.get(), net.size, 0.);

  for (long int i = 0L; i < size_mb; ++i)
  {
    long int h_out = 0L;
    long int batch_idx = Mbatch[i];

    for (long int j = 0L; j < net.K; ++j)
    {
      long int dot = 0L;
      long int sum = 0L;

      for (long int k = 0L; k < net.N; ++k)
      {
        long int idx1 = batch_idx * p.Ncol + k;
        long int idx2 = j * net.N + k;
        dot += p.input[idx1] * net.J[idx2];
        sum += p.input[idx1] + net.J[idx2];
      }

      h[i] = 4L * dot - 2L * sum + net.N;
    }

    long int t_out = h_out > 0L ? 1L : -1L;

    if ( t_out != p.output[batch_idx] )
    {
      // compute_gd
      long int tofix = (-p.output[batch_idx] * h_out + 1L) / 2L;

      for (long int j = 0L; j < net.K; ++j)
      {
        long int tmp = h[i] * p.output[batch_idx];
        if (tmp > 0L) continue;
        wrongh.push_back (-tmp);
        indh.push_back (i);
      }

      std :: sort (indh.begin(), indh.end(), [&](const long int & a, const long int & b) { return wrongh[a] < wrongh[b]; });

      for (long int j = 0L; j < tofix; ++j)
        for (long int k = 0L; k < net.N; ++k)
        {
          long int idx = batch_idx * p.Ncol + k;
          long int ind = indh[j];
          net.DeltaH[ ind ] += params.eta * p.output[ batch_idx ] * (2. * p.input[ idx ] - 1.);
        }

      wrongh.clear();
      indh.clear();
    }
  }

  NetNK :: update (net);

  delete [] Mbatch;
  delete [] h;
}

long int * predict (const long int * weights, const Patterns & p, const int & N)
{
  long int * predition = new long int [p.Nrow];

  for (long int i = 0L; i < p.Nrow; ++i)
  {
    long int h_out = 0L;

    for (long int j = 0L; j < p.Ncol; ++j)
    {
      long int dot = 0L;
      long int sum = 0L;

      for (long int k = 0L; k < N; ++k)
      {
        long int idx1 = i * p.Ncol + k;
        long int idx2 = j * N + k;

        dot += p.input[idx1] * weights[idx2];
        sum += p.input[idx1] + weights[idx2];
      }

      h_out += 4L * dot - 2L * sum + N > 0L ? 1L : -1L;
    }
    predition[i] = h_out > 0L ? 1L : -1L;
  }

  return predition;
}

long int * rSGD (const Patterns & pattern, const long int & K, parameters & params, const int & batch, const long int & formula, const long int & seed, const long int & max_epochs, const bool & init_equal, const bool & waitcenter, const bool & center, std :: string outfile, std :: string weightsfile, bool bin)
{
  global_engine = std :: mt19937(seed);
  // check variables
#ifdef DEBUG
  if (K < 1L || K % 2L == 0L)              error_Kpositive(K);
  if (params.y < 1L)                       error_ypositive(params.y);
  if (batch < 1L)                          error_batchpositive(batch);
  if (formula > 3L)                        error_formula(formula);
  if (params.lambda == 0L && waitcenter)   warning_lamdba(params.lambda);
  if (init_equal && batch >= pattern.Nrow) warning_batch(batch, pattern.Nrow);
#endif

  bool ok = false;

  long int epoch = 0L;
  long int N = pattern.Ncol;
  long int M = pattern.Nrow;

  NetNK netc(N, K);

  std :: unique_ptr < long int[] > shuffle_idx (new long int [params.y]);
  std :: unique_ptr < long int[] > errs        (new long int [params.y]);
  std :: unique_ptr < long int[] > dist        (new long int [params.y]);
  std :: unique_ptr < long int[] > min_errs    (new long int [params.y]);

  std :: unique_ptr < PatternPermutation[] > p_perm (new PatternPermutation[params.y]);
  std :: unique_ptr < NetNK[] > nets (new NetNK [params.y]);

  std :: bernoulli_distribution binary_dist;
  std :: ofstream os;


  if ( center || init_equal ) netc.init (binary_dist, global_engine);

  for (long int i = 0L; i < params.y; ++i)
  {
    if ( init_equal ) nets[i] = netc;
    else              nets[i].init(binary_dist, global_engine);
  }

  if ( !center ) netc = NetNK :: mean(nets.get(), params.y, N, K);

  long int errc = compute_errs(netc, pattern);

  std :: iota (shuffle_idx.get(), shuffle_idx.get() + params.y, 0L);
  std :: transform (nets.get(), nets.get() + params.y, errs.get(), [&](NetNK & n){ return compute_errs(n, pattern); });
  std :: transform (nets.get(), nets.get() + params.y, dist.get(), [&](NetNK & n){ return netc - n; });
  std :: generate_n (p_perm.get(), params.y, [&](){ return PatternPermutation(1L, M, batch); });
  std :: copy_n (errs.get(), params.y, min_errs.get());

  long int m_errs = * std :: min_element (errs.get(), errs.get() + params.y);
  long int minerr = errc < m_errs ? errc : m_errs;
  long int mean_errs = std :: accumulate (errs.get(), errs.get() + params.y, 0L);
  long int mean_dist = std :: accumulate (dist.get(), dist.get() + params.y, 0L);
  long int minerrc = 0L;

  if ( ! outfile.empty() )
  {
    os.open (outfile);

    // Header
    os << "#epoch" << "\t" << "err(Wc)" << "\t" << "err(best)" << "\t";
    for (long int i = 0L; i < params.y; ++i)
      os << "err(W" << i << ")" << "\t";
    os << "lambda" << "\t" << "gamma" << "\t";
    for (long int i = 0L; i < params.y; ++i)
      os << "d(W" << i << "\t";
    os << std :: endl;

    // Content
    os << epoch << "\t" << errc << "\t" << ( errc < m_errs ? errc : m_errs ) << "\t";
    std :: copy_n (errs.get(), params.y, std :: ostream_iterator < long int > (os, "\t"));
    os << std :: setprecision (3) << params.lambda << "\t"
       << std :: setprecision (3) << params.gamma  << "\t";
    std :: copy_n (dist.get(), params.y, std :: ostream_iterator < long int > (os, "\t"));
    os << std :: endl;
  }


#ifdef VERBOSE
  std :: cout << "ep: " << epoch
              << " lambda: " << std :: setprecision (3) << params.lambda
              << " gamma : " << std :: setprecision (3) << params.gamma
              << " eta: "    << std :: setprecision (3) << params.eta
              << std :: endl;
  std :: cout << "  errc " << errc << " [" << errc << "]" << std :: endl;
  std :: cout << "  errs " << m_errs << " [";
  std :: copy_n (errs.get(), params.y, std :: ostream_iterator < int > ( std :: cout, ","));
  std :: cout << "] (mean = " << std :: setprecision (3) << mean_errs / params.y
              << ")" << std :: endl;
  std :: cout << "  dist = [";
  std :: copy_n (dist.get(), params.y, std :: ostream_iterator < int > ( std :: cout, ","));
  std :: cout << "] (mean = " << std :: setprecision (3) << mean_dist / params.y
              << ")" << std :: endl;
#endif

  long int sub_epochs = (M + batch - 1L) / batch;

  if ( ( ok = (errc == 0L) ) || ( !waitcenter && minerr == 0L ) ) epoch = 0L;

  while ( !ok && epoch < max_epochs )
  {
    ++ epoch;

    for ( long int i = 0L; i < sub_epochs; ++i)
    {
      std :: shuffle (shuffle_idx.get(), shuffle_idx.get() + params.y, global_engine);

      for ( long int j = 0L; j < params.y; ++j)
      {
        long int idx = shuffle_idx[j];
        std :: copy_n (nets[idx].oldJ.get(), nets[idx].size, nets[idx].J.get());

        subepoch (nets[idx], pattern, p_perm[idx], params);

        ( ! center && formula == _continuous ) ? kick :: kickboth_traced_continuous( nets[idx], netc, params )          :
        ( ! center && formula != _continuous ) ? kick :: kickboth_traced(            nets[idx], netc, params, formula ) :
        (   center && params.lambda > 0.     ) ? kick :: kickboth(                   nets[idx], netc, params )          :
        void();
      }
    }

    ok = ( errc = compute_errs(netc, pattern) ) == 0L ? true : false;

    minerrc = minerrc < errc ? minerrc : errc;


    if ( ! outfile.empty() )
    {
      os << epoch << "\t" << errc << "\t" << ( errc < m_errs ? errc : m_errs ) << "\t";
      std :: copy_n (errs.get(), params.y, std :: ostream_iterator < long int > (os, "\t"));
      os << std :: setprecision (3) << params.lambda << "\t"
         << std :: setprecision (3) << params.gamma  << "\t";
      std :: copy_n (dist.get(), params.y, std :: ostream_iterator < long int > (os, "\t"));
      os << std :: endl;
    }

#ifdef VERBOSE
    std :: cout << "ep: " << epoch
                << " lambda: " << std :: setprecision (3) << params.lambda
                << " gamma : " << std :: setprecision (3) << params.gamma
                << " eta: "    << std :: setprecision (3) << params.eta
                << std :: endl;
    std :: cout << "  errc " << errc << " [" << errc << "]" << std :: endl;
    std :: cout << "  errs " << m_errs << " [";
    std :: copy_n (errs.get(), params.y, std :: ostream_iterator < long int > ( std :: cout, ","));
    std :: cout << "] (mean = " << std :: setprecision (3) << mean_errs / params.y
                << ")" << std :: endl;
    std :: cout << "  dist = [";
    std :: copy_n (dist.get(), params.y, std :: ostream_iterator < long int > ( std :: cout, ","));
    std :: cout << "] (mean = " << std :: setprecision (3) << mean_dist / params.y
                << ")" << std :: endl;
#endif

    parameters :: update (params);
  }

  if ( ! outfile.empty() ) os.close();

  if ( ok )
  {
#ifdef VERBOSE
    std :: cout << "SOLVED" << std :: endl;
#endif

    if ( ! weightsfile.empty() ) netc.save_weights (weightsfile, bin);
  }
  else
  {
#ifdef VERBOSE
    std :: cout << "FAILED" << std :: endl;
#endif
  }

  return netc.J.get();
}
