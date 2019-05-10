#include <pattern.h>

Patterns :: Patterns () : Nrow(0L), Ncol(0L), Nout(0L), output(nullptr), input(nullptr)
{
}

Patterns :: Patterns (const std :: string & filename, bool bin, const std :: string & del)
{
  if (bin)
  {
    std::ifstream is(filename, std::ios::binary);
    if (!is) error_pattern(filename);
    is.read(reinterpret_cast <char * >(&Nrow), sizeof(long int));
    is.read(reinterpret_cast <char * >(&Ncol), sizeof(long int));
    Nout   = Nrow;
    input  = new double[Nrow * Ncol];
    output = new long int[Nout];

    is.read(reinterpret_cast < char * >(output), sizeof(long int) * Nout);
    is.read(reinterpret_cast < char * >(input),  sizeof(double)   * Nrow * Ncol);
    is.close();
  }
  else
  {
    std :: vector < std :: string > row_;
    std :: ifstream is(filename);
    if ( !is ) error_pattern(filename);
    std::stringstream buff;
    buff << is.rdbuf();
    is.close();

    // Get M
    std :: string row;
    std :: getline(buff, row);
    row_ = split(row, del);
    Nout = row_.size();
    Nrow = Nout;

    // Read outputs
    output = new long int [Nout];
    std :: transform( row_.begin(), row_.end(), output, [](const std :: string & i){return std :: stod(i);} );

    // Get N
    std :: getline(buff, row);
    row_ = split(row, del);
    Ncol = row_.size();
    input = new double[Nrow * Ncol];

    // Read first pattern
    std :: transform( row_.begin(), row_.end(), input, [](const std :: string & i){return std :: stod(i);} );

    // Read all others
    for (long int i = Ncol; i < Nrow * Ncol; ++i)
    {
      std :: getline(buff, row);
      row_ = split(row, del);
      std :: transform( row_.begin(), row_.end(), input + i * Ncol, [](const std :: string & rr){return std :: stod(rr);} );
    }

  }

#ifdef DEBUG
  check_binary();
#endif
}


Patterns :: Patterns(const long int & N, const long int & M) : Nrow (N), Ncol (M), Nout (N)
{
  input  = new double[Nrow * Ncol];
  output = new long int [Nrow];

  std :: default_random_engine engine;
  std :: bernoulli_distribution dist(.5);
  std :: fill_n(output, Nout, 1L);
  std :: generate_n(input, Nrow * Ncol,
                    [&]()
                    {
                      return (dist(engine)) ? 1. : -1.;
                    });
}

Patterns :: Patterns (long double ** data, long int * label, const int & Nrow, const int & Ncol) : Nrow (Nrow), Ncol (Ncol), Nout (Nrow)
{
  input  = new double[Nrow * Ncol];
  output = new long int[Nrow];

  for (int i = 0; i < Nrow; ++i) std :: copy_n(data[i], Ncol, input + i * Ncol);
  std :: copy_n(label, Nrow, output);
}

Patterns & Patterns :: operator = (const Patterns & p)
{
  Nrow   = p.Nrow;
  Ncol   = p.Ncol;
  Nout   = p.Nrow;
  input  = new double[Nrow * Ncol];
  output = new long int[Nrow];

  std :: copy_n(p.input, Nrow * Ncol, input);
  std :: copy_n(p.output, Nrow, output);

  return *this;
}

Patterns :: Patterns (const Patterns & p)
{
  Nrow   = p.Nrow;
  Ncol   = p.Ncol;
  Nout   = p.Nrow;
  input  = new double[Nrow * Ncol];
  output = new long int[Nrow];

  std :: copy_n(p.input, Nrow * Ncol, input);
  std :: copy_n(p.output, Nrow, output);
}

Patterns :: ~Patterns ()
{
  if (Nrow)
  {
    delete[] input;
  }
  if (Nout) delete[] output;
}


#ifdef DEBUG
void Patterns :: check_binary ()
{
  assert(Nout == Nrow);
  int cnt = 0;
  for (long int i = 0L; i < Nrow; ++i)
    cnt += std :: count_if(input + i * Ncol, input + i * Ncol + Ncol,
                           [](const double & v)
                           {
                            return std :: abs(v);
                           });
  assert(cnt == Nrow * Ncol);
  cnt = std :: accumulate(output, output + Nout,
                          0, [](const int & res, const long int & v)
                          {
                            return res + static_cast < int >(std :: abs(v));
                          });
  assert(cnt == Nout);
}
#endif
