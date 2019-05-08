#include <cmd.h>
#include <rsgd.h>

int main (int argc, char *argv[])
{
  bool waitcenter, init_equal, center,
       bin, bin_w;
  int  nth;
  long int K, y,batch,
           max_iters,
           seed, formula;
  double eta, lambda, gamma,
         eta_factor, lambda_factor, gamma_step;
  std :: string patternsfile,
                output,
                weightsfile,
                del;

  parse_training_rsgd(argc, argv,
                      patternsfile,
                      K,
                      y,
                      batch,
                      max_iters,
                      seed,
                      eta,
                      lambda,
                      gamma,
                      eta_factor,
                      lambda_factor,
                      gamma_step,
                      waitcenter,
                      init_equal,
                      center,
                      output,
                      weightsfile,
                      bin_w,
                      formula,
                      bin,
                      del,
                      nth);

  Patterns pattern(patternsfile, bin, del);
  parameters params(y, eta, lambda, gamma, eta_factor, lambda_factor, gamma_step);

  long int * bin_weights = rSGD(pattern, K, params, batch, formula, seed, max_iters, init_equal, waitcenter, center, output, weightsfile, bin_w);

  delete[] bin_weights;

  return 0;
}
