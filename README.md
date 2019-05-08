| **Authors**  | **Project** | **Documentation**                                                                   | **Build Status**              |
|:------------:|:-----------:|:-----------------------------------------------------------------------------------:|:-----------------------------:|
| [**N. Curti**](https://github.com/Nico-Curti) <br/> [**D. Dall'Olio**](https://github.com/DanieleDallOlio)   |  **rSGD**  | [![docs](https://img.shields.io/readthedocs/:packageName/:version.svg?style=plastic)](https://nico-curti.github.io/rSGD/) | **Linux/MacOS** : [![Travis](https://travis-ci.com/Nico-Curti/rSGD.svg?token=7QqsqaQiuDHSyGDT3xek&branch=master)](https://travis-ci.com/Nico-Curti/rSGD) <br/> **Windows** : *miss* |

[![GitHub pull-requests](https://img.shields.io/github/issues-pr/Nico-Curti/rSGD.svg?style=plastic)](https://github.com/Nico-Curti/rSGD/pulls)
[![GitHub issues](https://img.shields.io/github/issues/Nico-Curti/rSGD.svg?style=plastic)](https://github.com/Nico-Curti/rSGD/issues)

[![GitHub stars](https://img.shields.io/github/stars/Nico-Curti/rSGD.svg?label=Stars&style=social)](https://github.com/Nico-Curti/rSGD/stargazers)
[![GitHub watchers](https://img.shields.io/github/watchers/Nico-Curti/rSGD.svg?label=Watch&style=social)](https://github.com/Nico-Curti/rSGD/watchers)

<a href="https://github.com/UniboDIFABiophysics">
<div class="image">
<img src="https://cdn.rawgit.com/physycom/templates/697b327d/logo_unibo.png" width="90" height="90">
</div>
</a>

# Replicated Stochastic Gradient Descent

Optimization and extension of the [**Replicated Stochastic Gradient Descent**](https://github.com/carlobaldassi/BinaryCommitteeMachineRSGD.jl).

1. [Prerequisites](#prerequisites)
2. [Installation](#installation)
3. [Authors](#authors)
4. [License](#license)
5. [Acknowledgments](#acknowledgments)
6. [Citation](#citation)

## Prerequisites

The project is written in c++ language with the support of c++14 standard (*make sure that your c++ compiler support it!*).

## Installation

To build the executables, clone the repo and then type

```
make
```

for the complete list of available examples, or

```
make main
```

to compile the main example.

You can also build with CMake or easier with the [build.sh](https://github.com/Nico-Curti/rSGD/blob/master/build.sh) file in Linux/MacOS and [build.ps1](https://github.com/Nico-Curti/rSGD/blob/master/build.ps1) in Windows.

## Authors

* **Nico Curti** [git](https://github.com/Nico-Curti), [unibo](https://www.unibo.it/sitoweb/nico.curti2)
* **Daniele Dall'Olio** [git](https://github.com/DanieleDallOlio)

See also the list of [contributors](https://github.com/Nico-Curti/rSGD/contributors) [![GitHub contributors](https://img.shields.io/github/contributors/Nico-Curti/rSGD.svg?style=plastic)](https://github.com/Nico-Curti/rSGD/graphs/contributors/) who partecipated in this project.

## License

The `rSGD` package is licensed under the MIT "Expat" License. [![License](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/Nico-Curti/rSGD/blob/master/LICENSE.md)

### Acknowledgement

Thanks goes to all contributors of this project.

### Citation

Please cite `rSGD` if you use it in your research.

```tex
@misc{ReplicatedStochasticGradientDescent,
  author = {Nico Curti, Daniele Dall'Olio},
  title = {{R}eplicated {S}tochastic {G}radient {D}escent},
  year = {2019},
  publisher = {GitHub},
  howpublished = {\url{https://github.com/Nico-Curti/rSGD}},
}
```
