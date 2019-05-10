#include <netnk.h>

NetNK :: NetNK () : N (0L), K (0L), size (0L), H (nullptr), DeltaH (nullptr), J (nullptr), oldJ (nullptr)
{
}

NetNK :: NetNK (const long int & N, const long int & K) : N (N), K (K), size (N * K)
{
  H      = std :: make_unique < double[] >(size);
  DeltaH = std :: make_unique < double[] >(size);
  J    = std :: make_unique < long int[] >(size);
  oldJ = std :: make_unique < long int[] >(size);
}

NetNK :: NetNK (const NetNK & n)
{
  N = n.N;
  K = n.K;
  size = n.size;

  H.reset ( new double [size] );
  DeltaH.reset ( new double [size] );
  J.reset ( new long int [size] );
  oldJ.reset ( new long int [size] );

  std :: copy_n (n.H.get(), size, H.get());
  std :: copy_n (n.DeltaH.get(), size, DeltaH.get());
  std :: copy_n (n.J.get(), size, J.get());
  std :: copy_n (n.oldJ.get(), size, oldJ.get());
}

NetNK & NetNK :: operator = (const NetNK & n)
{
  N = n.N;
  K = n.K;
  size = n.size;

  H.reset ( new double [size] );
  DeltaH.reset ( new double [size] );
  J.reset ( new long int [size] );
  oldJ.reset ( new long int [size] );

  std :: copy_n (n.H.get(), size, H.get());
  std :: copy_n (n.DeltaH.get(), size, DeltaH.get());
  std :: copy_n (n.J.get(), size, J.get());
  std :: copy_n (n.oldJ.get(), size, oldJ.get());

  return *this;
}

NetNK & NetNK :: operator = (NetNK && n)
{
  N = n.N;
  K = n.K;
  size = n.size;

  H = std :: move(n.H);
  DeltaH = std :: move(n.DeltaH);
  J = std :: move(n.J);
  oldJ = std :: move(n.oldJ);

  return *this;
}

long int NetNK :: operator - (const NetNK & n) const
{
  long int dist = 0;

  for ( long int i = 0L; i < N; ++i)
  {
    long int dot = 0;
    long int sum = 0;

    for (long int j = 0L; j < K; ++j)
    {
      long int idx = i * K + j;
      dot += J[idx] * n.J[idx];
      sum += J[idx] + n.J[idx];
    }

    dist += -2L * dot + sum;
  }

  return dist;
}

bool NetNK :: operator == (const NetNK & n) const
{
  return size == n.size;
}

long int * NetNK :: read_weights (const std :: string & filename, long int & K, long int & N, bool bin)
{
  std :: ifstream is;
  long int * weights = nullptr;
  if ( bin )
  {
    is.open (filename, std :: ios :: binary);

    is.read (reinterpret_cast < char * > (& K), sizeof (long int) );
    is.read (reinterpret_cast < char * > (& N), sizeof (long int) );

    weights = new long int [N * K];

    is.read (reinterpret_cast < char * > (&weights), sizeof (long int) * K * N);
  }
  else
  {
    is.open (filename);

    is >> K >> N;

    weights = new long int [N * K];

    std :: for_each (weights, weights + N * K, [&](long int & val){ is >> val; });
  }

  is.close();

  return weights;
}

void NetNK :: save_weights (const std :: string & filename, bool bin)
{
  std :: ofstream os;
  if ( bin )
  {
    os.open (filename, std :: ios :: out | std :: ios :: binary );

    os.write ( reinterpret_cast < char * > (& K), sizeof (long int));
    os.write ( reinterpret_cast < char * > (& N), sizeof (long int));

    os.write ( reinterpret_cast < char * > (J.get()), sizeof (long int) * N * K);
  }
  else
  {
    os.open (filename);

    os << K << " " << N << std :: endl;
    std :: copy_n (J.get(), K * N, std :: ostream_iterator < long int >(os, " "));
  }
  os.close();
}

void NetNK :: init (std :: bernoulli_distribution & binary_dist, std :: mt19937 & engine)
{
  std :: generate_n (H.get(), size, [&](){ return binary_dist(engine) ? 1. : -1.; });
  std :: fill_n (DeltaH.get(), size, 0.);
  std :: transform (H.get(), H.get() + size, J.get(), [](const double & i){ return i > 0. ? 1L : 0L; });
  std :: copy_n (J.get(), size, oldJ.get());
}

void NetNK :: update (NetNK & n)
{
  for (long int i = 0L; i < n.size; ++i)
  {
    n.H[i] += n.DeltaH[i];
    n.J[i]  = n.H[i] > 0. ? 1L : 0L;
  }
}

NetNK NetNK :: mean (const NetNK * nets, const long int & n_nets, const long int & N, const long int & K)
{
  NetNK mean (N, K);

  for (long int i = 0L; i < mean.size; ++i)
  {
    long int sum = std :: accumulate (nets, nets + n_nets, 0L,
                                      [&](const long int & res, const NetNK & n)
                                      {
                                        return res + n.J[i];
                                      });
    mean.H[i] = 2. * sum;
    mean.DeltaH[i] = 0.;

    mean.H[i] /= n_nets - 1.;
    mean.J[i]  = mean.H[i] > 0. ? 1L : 0L;
    mean.oldJ[i] = mean.J[i];
  }

  return mean;
}

