#ifndef __netnk__
#define __netnk__
#include <memory>    // std::unique_ptr
#include <random>    // std::bernoulli_distribution
#include <algorithm> // std::copy_n
#include <fstream>   // std::ifstream
#include <iterator>  // std::ostream_iterator

#if __GNUC__ == 4 && __GNUC_MINOR__ < 9
#include <utils.h>
#endif

class NetNK
{
  // Private members
public:

  int N;
  int K;
  int size;

  std :: unique_ptr < double[] > H;
  std :: unique_ptr < double[] > DeltaH;
  std :: unique_ptr < long int[] > J;
  std :: unique_ptr < long int[] > oldJ;


  // Constructors

  NetNK ();
  NetNK (const long int & N,  const long int & K);

  // Destructors

  ~NetNK () = default;

  // Copy operators

  NetNK (const NetNK & n);
  NetNK & operator = (const NetNK & n);
  NetNK & operator = (NetNK && n);

  // Operators
  long int operator - (const NetNK & n) const;
  bool operator == (const NetNK & n) const;

  // Public members

  void save_weights (const std :: string & filename, bool bin = false);
  void init (std :: bernoulli_distribution & binary_dist, std :: mt19937 & engine);

  // Static members

  static void update (NetNK & n);
  static NetNK mean (const NetNK * nets, const long int & n_nets, const long int & N, const long int & K);
  static long int * read_weights (const std :: string & filename, long int & K, long int & N, bool bin = false);

};

#endif // __netnk__
