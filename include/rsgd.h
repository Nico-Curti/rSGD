#ifndef __rsgd__
#define __rsgd__
#include <vector>
#include <iterator>
#include <functional>

#include <netnk.h>
#include <kick.h>
#include <parameters.h>
#include <pattern.h>
#include <pattern_permutation.h>
#include <utils.h>


long int compute_errs (const NetNK & net, const Patterns & p);
void subepoch (NetNK & net, const Patterns & p, PatternPermutation & patt_perm, const parameters & params);
long int * predict (const long int * weights, const Patterns & p, const int & N);
long int * rSGD (const Patterns & pattern, const long int & K, parameters & params, const int & batch, const long int & formula, const long int & seed, const long int & max_epochs, const bool & init_equal, const bool & waitcenter, const bool & center, std :: string outfile = "", std :: string weightsfile = "", bool bin = true);

#endif // __rsgd__
