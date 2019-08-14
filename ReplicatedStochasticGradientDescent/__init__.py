#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function

import os
import warnings

try:

  from .rsgd.ReplicatedStochasticGradientDescent import ReplicatedStochasticGradientDescent
  from .rsgd.ReplicatedStochasticGradientDescent import NTH
  from .rsgd.Patterns import Pattern

except ImportError:
  pass

__all__ = ["ReplicatedStochasticGradientDescent"]

__package__ = "rSGD"
__author__  = ["Nico Curti", "Daniele Dall'Olio"]
__email__   = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']
