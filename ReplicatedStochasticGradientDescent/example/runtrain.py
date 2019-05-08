#!usr/bin/env python

import argparse
from ReplicatedStochasticGradientDescent import ReplicatedStochasticGradientDescent as rSGD, NTH
from ReplicatedStochasticGradientDescent import Pattern

__package__ = "rSGD_train_example"
__author__  = ["Nico Curti (nico.curit2@unibo.it)", "Daniele Dall'Olio (daniele.dallolio@studio.unibo.it)"]


def parse_args():
  description = "rSGD train example"

  parser = argparse.ArgumentParser(description = description)
  parser.add_argument('--patterns',
                      dest='patterns',
                      required = True,
                      type=str,
                      action='store',
                      help = 'Pattern Filename' )
  parser.add_argument('--output',
                      dest='output',
                      required = False,
                      type=str,
                      action='store',
                      help = 'Output filename',
                      default='')
  parser.add_argument('--bin',
                      dest='bin',
                      required = False,
                      type=bool,
                      action='store',
                      help = 'File format: (0) Textfile(default), (1) Binary',
                      default=False,
                      choices=[0, 1])
  parser.add_argument('--delimiter',
                      dest='delimiter',
                      required = False,
                      type=str,
                      action='store',
                      help = 'Delimiter for text files(default: \"\\t\")',
                      default='\t')
  parser.add_argument('--replica',
                      dest='replica',
                      required = False,
                      type=int,
                      action='store',
                      help = 'Number of replicas(default: 1)',
                      default=1)
  parser.add_argument('--hidden',
                      dest='hidden',
                      required = False,
                      type=int,
                      action='store',
                      help = 'Number of Hidden Layers(default: 3)',
                      default=3)
  parser.add_argument('--iteration',
                      dest='iteration',
                      required = False,
                      type=int,
                      action='store',
                      help = 'Max Number of iterations(default: 1000)',
                      default=1000)
  parser.add_argument('--seed',
                      dest='seed',
                      required = False,
                      type=int,
                      action='store',
                      help = 'Seed random generator(default: 135)',
                      default=135)
  parser.add_argument('--eta',
                      dest='eta',
                      required = False,
                      type=float,
                      action='store',
                      help = 'Initial value of the step for the energy (loss) term gradient(default: 2.)',
                      default=2.)
  parser.add_argument('--lambda',
                      dest='lamda',
                      required = False,
                      type=float,
                      action='store',
                      help = 'Initial value of the step for the interaction gradient(default: .1)',
                      default=.1)
  parser.add_argument('--gamma',
                      dest='gamma',
                      required = False,
                      type=float,
                      action='store',
                      help = 'Initial value of the interaction strength(default: inf)',
                      default=float('Inf'))
  parser.add_argument('--factor',
                      dest='factor',
                      required = False,
                      type=float,
                      action='store',
                      nargs='+',
                      help = 'Factors used to update "eta", "lambda" and "gamma" respectively after each epoch',
                      default=[1., 1., .01])
  parser.add_argument('--formula',
                      dest='formula',
                      required = False,
                      type=str,
                      action='store',
                      help = 'Iteration update scheme',
                      default='simple',
                      choices=['simple', 'hard', 'corrected', 'continuous'])
  parser.add_argument('--waitcenter',
                      dest='waitcenter',
                      required = False,
                      type=bool,
                      action='store',
                      help = 'Whether to only exit successfully if the center replica has solved the problem',
                      default=False)
  parser.add_argument('--equal',
                      dest='equal',
                      required = False,
                      type=bool,
                      action='store',
                      help = 'Whether to initialize all replicated networks equally',
                      default=False)
  parser.add_argument('--center',
                      dest='center',
                      required = False,
                      type=bool,
                      action='store',
                      help = 'Whether to explicity use a central replica (if "false", it is traced out)',
                      default=False)
  parser.add_argument('--nth',
                      dest='nth',
                      required = False,
                      type=int,
                      action='store',
                      help = 'Number of thread to use',
                      default=NTH)

  args = parser.parse_args()

  return args


def train():
  args = parse_args()

  pattern = Pattern()
  pattern.load(args.patterns, args.bin, args.delimiter)

  rsgd = rSGD(args.K, args.formula, args.max_iter, args.seed, args.equal, args.waitcenter, args.center)

  rfbp.fit(pattern, parameters = {'y' : args.replica,
                                  'eta' : (args.eta, args.factor[0]),
                                  'lambda' : (args.lamda, args.factor[1]),
                                  'gamma' : (args.gamma, args.factor[2])
                                  })

  rfbp.save_weights(args.output, args.bin)

if __name__ == '__main__':

  train()
