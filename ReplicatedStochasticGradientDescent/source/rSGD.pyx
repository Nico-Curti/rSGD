# distutils: language = c++
# cython: language_level=2

from libcpp.memory cimport unique_ptr
from libcpp.vector cimport vector
from cython.operator cimport dereference as deref
cimport numpy as np

from rsgd cimport rSGD
from rsgd cimport predict
from pattern cimport Patterns
from params cimport parameters

from ReplicatedStochasticGradientDescent.source.misc import _check_string
import numpy as np
import numbers
from sklearn.utils import check_X_y
from sklearn.utils import check_array
import warnings


cdef class _Parameters:
  cdef unique_ptr[parameters] thisptr

  def __init__(self, y, betas, lamdas, gammas):
    beta, beta_f = betas
    lamda, lamda_f = lamdas
    gamma, gamma_s = gammas

    self.thisptr.reset(new parameters(y, beta, lamda, gamma, beta_f, lamda_f, gamma_s))

  def __repr__(self):
    class_name = self.__class__.__name__
    return '<{} Class>'.format(class_name)

cdef class _Pattern:

  cdef unique_ptr[Patterns] thisptr

  cdef public:
    long int Nrow, Ncol

  def __init__(self, other=None, binary=False, delimiter='\t'):

    # TODO : insert c++ constructor from matrix and add case here

    try:

      N, M = other

      if not isinstance(N, int) or not isinstance(M, int):
        raise TypeError('N and M must be an integer')

      self.thisptr.reset(new Patterns(N, M))

    except ValueError:

      filename = other
      filename = _check_string(filename, exist=True)
      delimiter = _check_string(delimiter, exist=False)
      self.thisptr.reset(new Patterns(filename, binary, delimiter))

    except:

      self.thisptr.reset(new Patterns())

    self.Nrow = deref(self.thisptr).Nrow
    self.Ncol = deref(self.thisptr).Ncol


  @property
  def labels(self):
    labels = [deref(self.thisptr).output[i] for i in range(self.Nrow)]
    return labels

  @property
  def data(self):
    data = [deref(self.thisptr).input[i] for i in range(self.Nrow * self.Ncol)]
    return np.asarray(data).reshape(self.Ncol, self.Nrow)

  def __repr__(self):
    class_name = self.__class__.__name__
    return '<{} Class (Ninput: {} Nfeatures: {} >)'.format(class_name, self.Nrow, self.Ncol)




def _rsgd(_Pattern pattern, K, y, eta, lamda, gamma, batch, formula, seed, max_iter, init_equal, waitcenter, center, nth=1) :

  params = _Parameters(y, eta, lamda, gamma)
  cdef long int * weights = rSGD(deref(pattern.thisptr.get()), K, deref(params.thisptr.get()), batch, formula, seed, max_iter, init_equal, waitcenter, center)

  return [int(weights[i]) for i in range(pattern.Nrow * K)]

def _predict(_Pattern pattern, long int [::1] weights, long int N):

  cdef long int * pred = predict(&weights[0], deref(pattern.thisptr.get()), N)

  return [int(pred[i]) for i in range(pattern.Nrow)]
