# distutils: language = c++
# cython: language_level=2

cdef extern from "parameters.h":
  cppclass parameters:
    parameters (const long int & y, const double & eta, const double & lamda, const double & gamma, const double & eta_factor, const double & lambda_factor, const double & gamma_step);
