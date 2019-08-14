#ifndef __kick__
#define __kick__
#include <netnk.h>
#include <parameters.h>
#include <utils.h>
#ifdef DEBUG

#include <cassert>

#endif

namespace kick
{

double tanh (const double & x);
double hardtanh (const double & x);

double compute_kick (const long int & kick, const double & x);
void kickboth (NetNK & n1, NetNK & n2, const parameters & p);
void kickboth_traced (NetNK & n1, NetNK & n2, const parameters & p, const long int & kick);
void kickboth_traced_continuous (NetNK & n1, NetNK & n2, const parameters & p);

}

#endif  // __kick__
