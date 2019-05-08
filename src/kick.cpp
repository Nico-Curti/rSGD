#include <kick.h>

namespace kick
{

double tanh (const double & x)
{
  return std :: tanh (x);
}

double hardtanh (const double & x)
{
  return -1. <= x && x <= 1. ? x   :
         x < 1.              ? -1. :
                                1.;
}

double compute_kick (const long int & kick, const double & x)
{
  double value;
  switch (kick)
  {
    case _hard: value = kick :: hardtanh (x);
    break;
    default :   value = kick :: tanh (x);
    break;
  }
  return value;
}

void kickboth (NetNK & n1, NetNK & n2, const parameters & p)
{
  assert (n1 == n2);

  for (long int i = 0L; i < n1.size; ++i)
  {
    long int j_diff = n2.J[i] - n1.J[i];

    n1.H[i] += p.lambda * j_diff;
    n2.H[i] -= p.lambda * j_diff;

    n1.J[i]  = n1.H[i] > 0. ? 1L : 0L;
    n2.J[i]  = n2.H[i] > 0. ? 1L : 0L;
  }
}

void kickboth_traced (NetNK & n1, NetNK & n2, const parameters & p, const long int & kick)
{
  assert (n1 == n2);

  double factor = p.gamma * p.y;
  double correction = kick == _corrected ? kick :: tanh (factor) : 1.;

  for (long int i = 0L; i < n1.size; ++i)
  {
    double c_kick = kick :: compute_kick (kick, factor * n2.H[i]);

    n1.H[i] += p.lambda * (c_kick - correction * (2. * n1.J[i] - 1.));
    n1.J[i]  = n1.H[i] > 0. ? 1L : 0L;

    n2.H[i] += 2. * static_cast < double > (n1.J[i] - n1.oldJ[i]) / p.y;
    n2.J[i]  = n2.H[i] > 0. ? 1L : 0L;
  }
}

void kickboth_traced_continuous (NetNK & n1, NetNK & n2, const parameters & p)
{
  assert (n1 == n2);

  for (long int i = 0L; i < n1.size; ++i)
  {
    n1.H[i] += p.lambda * (n2.H[i] - 2. * n1.J[i] + 1.);
    n1.J[i]  = n1.H[i] > 0. ? 1L : 0L;

    n2.H[i] += 2. * static_cast < double >(n1.J[i] - n1.oldJ[i]) / p.y;
    n2.J[i]  = n2.H[i] > 0. ? 1L : 0L;
  }
}


}

