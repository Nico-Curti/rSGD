#ifndef __cmd__
#define __cmd__
#include <parse_args.h>
#include <errors.h>
#include <utils.h>

void parse_training_rsgd (int argc, char *argv[], std :: string & patternsfile, long int & K, long int & y, long int & batch, long int & max_iters, long int & seed, double & eta, double & lambda, double & gamma, double & eta_factor, double & lambda_factor, double & gamma_step, bool & waitcenter, bool & init_equal, bool & center, std :: string & output, std :: string & weights, bool & bin_w, long int & formula, bool & bin, std :: string & del, int & nth);
void parse_test_args (    int argc, char *argv[], std :: string & patternsfile, std:: string & del, bool & bin, std :: string &weight_file, std :: string & output_file, int & nth);

#endif // __cmd__
