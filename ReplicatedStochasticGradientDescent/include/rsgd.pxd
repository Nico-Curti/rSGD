# distutils: language = c++
# cython: language_level=2

from libcpp.string cimport string
from pattern cimport Patterns
from params cimport parameters
from libcpp cimport bool

cdef extern from "rsgd.h":
  cdef long int * rSGD (const Patterns & pattern,
                        const long int & K,
                        parameters & params,
                        const int & batch,
                        const long int & formula,
                        const long int & seed,
                        const long int & max_epochs,
                        const bool & init_equal,
                        const bool & waitcenter,
                        const bool & center
                        #string outfile=*,
                        #string weightsfile=*,
                        #bool bin=*
                        )

  cdef long int * predict (const long int * weights, const Patterns & p, const int & N);
