#include <pattern_permutation.h>

PatternPermutation :: PatternPermutation () : a (0L), M (0L), batch (0L), MM (nullptr)
{
}

PatternPermutation :: PatternPermutation (const long int & a, const long int & M, const long int & batch) : a (a), M (M), batch (batch)
{
  MM = std :: make_unique < long int[] >(M);
  std :: iota (MM.get(), MM.get() + M, 0L);
  std :: shuffle (MM.get(), MM.get() + M, global_engine);
}

PatternPermutation :: PatternPermutation (const PatternPermutation & p)
{
  a = p.a;
  M = p.M,
  batch = p.batch;
  MM.reset (new long int [M]);

  std :: copy_n (p.MM.get(), M, MM.get());
}

PatternPermutation & PatternPermutation :: operator = (const PatternPermutation & p)
{
  a = p.a;
  M = p.M,
  batch = p.batch;
  MM.reset (new long int [M]);

  std :: copy_n (p.MM.get(), M, MM.get());

  return *this;
}

long int PatternPermutation :: get_batch (long int * & perm)
{
  long int b = std :: min(a + batch - 1, M);

  if ( b == M )
  {
    std :: shuffle (MM.get(), MM.get() + M, global_engine);
    a = 1L;
  }
  else
  {
    a = b + 1L;
  }

  perm = new long int [b - a + 1L];
  std :: copy_n (MM.get() + a - 1L, b, perm);
  return b - a + 1L;
}
