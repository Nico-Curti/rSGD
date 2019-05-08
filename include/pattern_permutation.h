#ifndef __ppermutation__
#define __ppermutation__
#include <memory>
#include <numeric>
#include <cmath>
#include <utils.h>

class PatternPermutation
{
  // Private members

  long int a;
  long int M;
  long int batch;

  std :: unique_ptr < long int[] > MM;

public:

  // Constructors

  PatternPermutation ();
  PatternPermutation (const long int & a, const long int & M, const long int & batch);

  // Destructors

  ~PatternPermutation () = default;

  // Copy operators

  PatternPermutation (const PatternPermutation & p);
  PatternPermutation & operator = (const PatternPermutation & p);

  // Public members

  long int get_batch (long int * & perm);

};

#endif // __ppermutation__
