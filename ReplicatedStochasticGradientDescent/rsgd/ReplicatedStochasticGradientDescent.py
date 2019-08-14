#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
from __future__ import division

import pickle
import pandas as pd

import numpy as np
import warnings
from scipy.special import erf

from .Patterns import Pattern

from .misc import _check_string
from lib.ReplicatedStochasticGradientDescent.rSGD import _rsgd
from lib.ReplicatedStochasticGradientDescent.rSGD import _predict

import multiprocessing

__package__ = "ReplicatedStochasticGradientDescent"
__author__  = ["Nico Curti (nico.curit2@unibo.it)", "Daniele Dall'Olio (daniele.dallolio@studio.unibo.it)"]

NTH = multiprocessing.cpu_count()

class ReplicatedStochasticGradientDescent():

  def __init__(self, K=1, formula='simple', max_iter=1000, seed=135, init_equal=True, waitcenter=False, center=False):
    '''
    '''

    if formula not in ['simple', 'hard', 'continuous', 'corrected']:
      raise TypeError('Invalid iteration scheme. Allowed values are ["simple", "hard", "continuous", "corrected"]')

    self._K = K
    self._formula = formula
    self._max_iter = max_iter
    self._seed = seed
    self._init_equal = init_equal
    self._waitcenter = waitcenter
    self._center = center
    self._weights = None

    self._fit = False


  def predict(self, X):
    '''
    Predict the new labels computed by ReplicatedStochasticGradientDescent model

    Parameters
    ----------
    X : array of shape [n_samples, n_features]
        The input samples.

    Returns
    -------
    y : array of shape [n_samples]
        The predicted target values.
    '''

    if not self._fit:
      raise ValueError('ReplicatedStochasticGradientDescent classifier is not fitted yet. Please use the fit method before predict')

    if not self._weights:
      raise ValueError("Weights must be computed before predict function. Use 'fit' function or 'load_weights' to read them from file")

    if isinstance(X, Pattern):
      testset = X # use this with c++ version

    else:
      testset = Pattern(X, []) # use this with c++ version

    N, K = np.shape(X)

    # miss check dimensions

    predicted_labels = _predict(testset, self._weights.ravel().astype('int64'), N)
    return predicted_labels


  def fit(self, X, y=None, parameters={'y' : 1, 'eta': (2., 1.), 'lambda' : (.1, 1.), 'gamma' : (float('Inf'), .01) }, nth=NTH):
    '''
    Fit the ReplicatedStochasticGradientDescent model meta-transformer

    Parameters
    ----------
      X : array-like of shape (n_samples, n_features)
          The training input samples.

      y : array-like, shape (n_samples,)
          The target values (integers that correspond to classes in
          classification, real numbers in regression).

    Returns
    -------
      self : object
        Returns self.
    '''

    self._fit = False

    if isinstance(X, Pattern):
      pattern = X
    else:
      pattern = Pattern(X, y)

    self._weights = _rsgd(pattern=pattern.pattern,
                          K=self._K,
                          y=parameters['y'],
                          eta=parameters['eta'],
                          lamda=parameters['lambda'],
                          gamma=parameters['gamma'],
                          formula=self._formula,
                          seed=self._seed,
                          max_iter=self._max_iter,
                          init_equal=self._init_equal,
                          waitcenter=self._waitcenter,
                          center=self._center,
                          nth=nth
                          )

    self._fit = True

    return self


  def load_weights(self, weightfile, delimiter='\t', binary=False):
    '''
    Load weights from file

    Parameters
    ----------
      weightfile : string
        Filename of weights

      delimiter : char
        Separator for ascii loading

      binary : bool
        Switch between binary and ascii loading style

    Returns
    -------
      self
    '''

    if binary:
      with open(weightfile, 'rb') as fp:
        self._weights = pickle.load(fp)

    else:
      self._weights = pd.read_csv(weightfile, sep=delimiter, header=None).values.tolist()

    self._fit = True

    return self


  def save_weights(self, weightfile, delimiter='\t', binary=False):
    '''
    Load weights from file

    Parameters
    ----------
      weightfile : string
        Filename to dump the weights

      delimiter : char
        Separator for ascii dump

      binary : bool
        Switch between binary and ascii dumping style
    '''

    if binary:
      with open(weightfile, 'wb') as fp:
        pickle.dump(self._weights, fp)

    else:
      pd.DataFrame(self._weights).to_csv(weightfile, sep=delimiter, header=False, index=False)


  def __repr__(self):
    class_name = self.__class__.__name__
    return '<{} Class>'.format(class_name)

