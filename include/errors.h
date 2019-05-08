#ifndef __errors__
#define __errors__

#include <iostream>

static constexpr int ERROR_KPOSITIVE        = 10;
static constexpr int ERROR_YPOSITIVE        = 11;
static constexpr int ERROR_BPOSITIVE        = 12;

static constexpr int ERROR_FORMULA          = 2;
static constexpr int ERROR_FACTORS          = 3;
static constexpr int ERROR_PATTERN          = 4;
static constexpr int ERROR_WEIGHTS          = 5;

// ERRORS 101-105 are related to raising errors in parse_args.h


inline void error_Kpositive(const long int & K)
{
  std :: cerr << "K must be positive and odd; given: " << K << std :: endl;
  std :: exit(ERROR_KPOSITIVE);
}

inline void error_ypositive(const long int & y)
{
  std :: cerr << "y must be positive; given: " << y << std :: endl;
  std :: exit(ERROR_YPOSITIVE);
}

inline void error_batchpositive(const long int & batch)
{
  std :: cerr << "batch must be positive; given: " << batch << std :: endl;
  std :: exit(ERROR_BPOSITIVE);
}

inline void error_formula(const long int & formula)
{
  std :: cerr << "Invalid iteration scheme! Given : " << formula << " but possible values are 'simple'(0), 'hard'(1), 'corrected'(2), 'continuous'(3)" << std :: endl;
  std :: exit(ERROR_FORMULA);
}

inline void error_factors()
{
  std :: cerr << "Invalid number of factors! Required values are associated to 'eta', 'lambda' and 'gamma', respectively." << std :: endl;
  std :: exit(ERROR_FACTORS);
}

inline void error_pattern(const std :: string & filename)
{
  std :: cerr << "Input file not found! Given: " << filename << std :: endl;
  std :: exit(ERROR_PATTERN);
}

inline void error_weights(const std :: string & filename)
{
  std :: cerr << "Weights file not found! Given : " << filename << std :: endl;
  std :: exit(ERROR_WEIGHTS);
}

inline void warning_lamdba(const double & lambda)
{
  std :: cerr << "Warning : lambda = " << lambda << " with waitcenter enabled" << std :: endl;
}

inline void warning_batch(const long int & batch, const long int & M)
{
  std :: cerr << "Warning : batch = " << batch << ", M = " << M << " with init_equal enabled" << std :: endl;
}

#endif // __errors__
