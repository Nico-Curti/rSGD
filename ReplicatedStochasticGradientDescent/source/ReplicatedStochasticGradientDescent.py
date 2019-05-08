#!/usr/bin/env python

import pickle
import pandas as pd
import numpy as np
import warnings
from scipy.special import erf

from ReplicatedStochasticGradientDescent.source.Patterns import Pattern

from ReplicatedStochasticGradientDescent.source.misc import _check_string
from lib.ReplicatedStochasticGradientDescent.rSGD import _rsgd, _predict

__package__ = "ReplicatedStochasticGradientDescent"
__author__  = ["Nico Curti (nico.curit2@unibo.it)", "Daniele Dall'Olio (daniele.dallolio@studio.unibo.it)"]

import multiprocessing
NTH = multiprocessing.cpu_count()

class ReplicatedStochasticGradientDescent():

  def __init__(self,
                     K = 1,
                     formula = 'simple',
                     max_iter = 1000,
                     seed = 135,
                     init_equal = True,
                     waitcenter = False,
                     center = False):
    if formula not in ['simple', 'hard', 'continuous', 'corrected']:
      raise TypeError('Invalid iteration scheme. Allowed values are ["simple", "hard", "continuous", "corrected"]')

    self.K = K
    self.formula = formula
    self.max_iter = max_iter
    self.seed = seed
    self.init_equal = init_equal
    self.waitcenter = waitcenter
    self.center = center
    self.weights = None


  def predict(self, X):
    if not self.weights:
      raise ValueError("Weights must be computed before predict function. Use 'fit' function or 'load_weights' to read them from file")

    if isinstance(X, Pattern):
      testset = X # use this with c++ version
    else:
      testset = Pattern(X, []) # use this with c++ version

    predicted_labels = _predict(testset, weights)
    return predicted_labels


  def fit(self, X, y = None, parameters = {'y' : 1, 'eta': (2., 1.), 'lambda' : (.1, 1.), 'gamma' : (float('Inf'), .01) }, nth = NTH):
    if isinstance(X, Pattern):
      pattern = X
    else:
      pattern = Pattern(X, y)

    self.weights = _rsgd(pattern = pattern.pattern,
                         K = self.K,
                         y = parameters['y'],
                         eta = parameters['eta'],
                         lamda = parameters['lambda'],
                         gamma = parameters['gamma'],
                         formula = self.formula,
                         seed = self.seed,
                         max_iter = self.max_iter,
                         init_equal = self.init_equal,
                         waitcenter = self.waitcenter,
                         center = self.center,
                         nth = nth
                         )


  def load_weights(self, weightfile, delimiter='\t', binary=False):
    if binary:
      with open(weightfile, 'rb') as fp:
        self.weights = pickle.load(fp)
    else:
      self.weights = pd.read_csv(weightfile, sep=delimiter, header=None).values.tolist()

    self.hidden = len(self.weights)


  def save_weights(self, weightfile, delimiter='\t', binary=False):
    if binary:
      with open(weightfile, 'wb') as fp:
        pickle.dump(self.weights, fp)
    else:
      pd.DataFrame(self.weights).to_csv(weightfile, sep=delimiter, header=False, index=False)


  def __repr__(self):
    class_name = self.__class__.__name__
    return '<%s Class>'%(class_name)

