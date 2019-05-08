#!/usr/bin/env python

def read_version(CMakeLists):
  version = []
  with open(CMakeLists, 'r') as fp:
    for row in fp:
      if len(version) == 3: break
      if 'RSGD_MAJOR' in row:
        version.append(row.split('RSGD_MAJOR')[-1])
      elif 'RSGD_MINOR' in row:
        version.append(row.split('RSGD_MINOR')[-1])
      elif 'RSGD_REVISION' in row:
        version.append(row.split('RSGD_REVISION')[-1])
  version = [v.strip().replace(')', '').replace('(', '') for v in version]
  version = map(int, version)
  return tuple(version)

__package__ = 'version'
__author__  = ['Nico Curti (nico.curit2@unibo.it)', "Daniele Dall'Olio (daniele.dallolio@studio.unibo.it)"]

VERSION = read_version('./CMakeLists.txt')

__version__ = '.'.join(map(str, VERSION))

