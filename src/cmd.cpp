#include <cmd.h>

void parse_training_rsgd( int argc, char *argv[],
                          std :: string & patternsfile,
                          long int & K,
                          long int & y,
                          long int & batch,
                          long int & max_iters,
                          long int & seed,
                          double & eta,
                          double & lambda,
                          double & gamma,
                          double & eta_factor,
                          double & lambda_factor,
                          double & gamma_step,
                          bool & waitcenter,
                          bool & init_equal,
                          bool & center,
                          std :: string & output,
                          std :: string & weightsfile,
                          bool & bin_w,
                          long int & formula,
                          bool & bin,
                          std :: string & del,
                          int & nth
                          )
{
  ArgumentParser argparse("Training Replicated SGD 2.0");

#ifdef _OPENMP
  nth  = omp_get_max_threads();
  nth -= nth % 2;
  argparse.add_argument < int >(          "tArg",  "t",  "threads",     "Max number of threads exploitable",                                                false, nth);
#else
  nth = 1;
#endif

  argparse.add_argument < std :: string >("fArg",  "f",  "file",        "Pattern Filename (with extension)",                                                true,  "");
  argparse.add_argument < std :: string >("oArg",  "o",  "output",      "Output Filename (with extension)",                                                 false, ""); // TODO: set true!
  argparse.add_argument < std :: string >("WArg",  "W",  "weightsfile", "Output weights filename (with extension)",                                         false, "rsgd_weights");
  argparse.add_argument < bool >(         "BArg",  "B",  "bin_w",       "File format for weights"
                                                                        "(0) Textfile(default), "
                                                                        "(1) Binary",                                                                       false, false);
  argparse.add_argument < bool >(         "bArg",  "b",  "bin",         "File format for inputs: "
                                                                        "(0) Textfile(default), "
                                                                        "(1) Binary",                                                                       false, false);
  argparse.add_argument < std :: string >("dlArg", "dl", "delimiter",   "Delimiter for text files(default: \"\\t\")",                                       false, "\t");
  argparse.add_argument < long int >(     "kArg",  "k",  "hidden",      "Number of Hidden Layers(default:3)",                                               false, 3L);
  argparse.add_argument < long int >(     "yArg",  "y",  "replica",     "Number of replicas",                                                               false, 1L);
  argparse.add_argument < long int >(     "bArg",  "b",  "batch",       "Minibatch size",                                                                   false, 5L);
  argparse.add_argument < long int >(     "iArg",  "i",  "epochs",      "Max Number of Epochs(default: 1000)",                                              false, 1000L);
  argparse.add_argument < long int >(     "rArg",  "r",  "seed",        "Seed random generator(default: 135)",                                              false, 135L);
  argparse.add_argument < double >(       "eArg",  "e",  "eta",         "Initial value of the step for the energy (loss) term gradient",                    false, 2.);
  argparse.add_argument < double >(       "lArg",  "l",  "lambda",      "Initial value of the step for the interaction gradient",                           false, .1);
  argparse.add_argument < double >(       "gArg",  "g",  "gamma",       "Initial value of the interaction strength",                                        false, inf);
  argparse.add_argument < double >(       "xArg",  "x",  "factor",      "Factors used to update 'eta', 'lambda' and 'gamma' respectively after each epoch", false);
  argparse.add_argument < std :: string >("zArg",  "z",  "formula",     "Choose Iteration Update Scheme: "
                                                                        "'simple', 'hard', 'corrected', 'continuous'",                                      false, "simple");
  argparse.add_argument < bool >(         "wArg",   "w", "waitcenter",  "Whether to only exit successfully if the center replica has solved the problem",   false, false);
  argparse.add_argument < bool >(         "qArg",   "q", "equal",       "Whether to initialize all replicated networks equally",                            false, true);
  argparse.add_argument < bool >(         "cArg",   "c", "center",      "Whether to explicity use a central replica (if 'false', it is traced out)",        false, false);


  argparse.parse_args(argc, argv);

  std :: string formula_fmt;
  std :: vector < double > factors;

#ifdef _OPENMP
  argparse.get < int >(          "tArg",  nth);
#endif

  argparse.get < std :: string >("fArg",  patternsfile);
  argparse.get < std :: string >("oArg",  output);
  argparse.get < std :: string >("WArg",  weightsfile);
  argparse.get < bool >(         "BArg",  bin_w);
  argparse.get < bool >(         "bArg",  bin);
  argparse.get < std :: string >("dlArg", del);
  argparse.get < long int >(     "kArg",  K);
  argparse.get < long int >(     "yArg",  y);
  argparse.get < long int >(     "bArg",  batch);
  argparse.get < long int >(     "iArg",  max_iters);
  argparse.get < long int >(     "rArg",  seed);
  argparse.get < double >(       "eArg",  eta);
  argparse.get < double >(       "lArg",  lambda);
  argparse.get < double >(       "gArg",  gamma);

  argparse.get < bool >(         "wArg",  waitcenter);
  argparse.get < bool >(         "qArg",  init_equal);
  argparse.get < bool >(         "cArg",  center);
  argparse.get < std :: string >("zArg",  formula_fmt);
  argparse.get < double >(       "xArg",  factors);

  formula = _formulas_[formula_fmt];

  switch ( factors.size() )
  {
    case 0:
    {
      eta_factor = 1.;
      lambda_factor = 1.;
      gamma_step = .01;
    } break;
    case 1:
    {
      eta_factor = factors[0];
      lambda_factor = 1.;
      gamma_step = .01;
    } break;
    case 2:
    {
      eta_factor = factors[0];
      lambda_factor = factors[1];
      gamma_step = .01;
    } break;
    case 3:
    {
      eta_factor = factors[0];
      lambda_factor = factors[1];
      gamma_step = factors[2];
    } break;
    default:
      error_factors();
  }

  if ( ! file_exists(patternsfile) ) error_pattern(patternsfile);

  return;
}

void parse_test_args(int argc, char *argv[],
                     std :: string & patternsfile,
                     std :: string & del,
                     bool & bin,
                     std :: string & weight_file,
                     std :: string & output_file,
                     int & nth)
{
  ArgumentParser argparse("Test Replicated SGD 2.0");
#ifdef _OPENMP
  nth  = omp_get_max_threads();
  nth -= nth % 2;
  argparse.add_argument < int >(           "tArg",  "t",  "threads", "Max number of threads exploitable",          false, nth);
#else
  nth = 1;
#endif

  argparse.add_argument < std :: string> ( "fArg", "f", "test_file", "Test Filename (with extension)",             true,  "");
  argparse.add_argument < bool >(          "bArg",  "b",  "bin",     "File format: "
                                                                     "(0) Textfile(default), "
                                                                     "(1) Binary",                                 false, false);
  argparse.add_argument < std :: string> ( "wArg", "w", "weights",   "Weights Matrix Filename (with extension)",   true,  "");
  argparse.add_argument < std :: string >("dlArg", "dl", "delimiter","Delimiter for text files(default: \"\\t\")", false, "\t");
  argparse.add_argument < std :: string> ( "oArg", "o", "output",    "Output Filename (with extension)",           false, "");

  argparse.parse_args(argc, argv);

  argparse.get < std :: string > ("fArg", patternsfile);
  argparse.get < bool >(         "bArg",  bin);
  argparse.get < std :: string > ("wArg", weight_file);
  argparse.get < std :: string >("dlArg", del);
  argparse.get < std :: string > ("oArg", output_file);

  if ( ! file_exists(patternsfile) ) error_pattern(patternsfile);
  if ( ! file_exists(weight_file)  ) error_weights(weight_file);

#ifdef _OPENMP
  argparse.get < int >(          "tArg",  nth);
#endif
  return;
}
