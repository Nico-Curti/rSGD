#ifndef UTILS_H
#define UTILS_H
#include <iostream>
#include <limits>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <random>
#include <memory>
//#if __has_include(<filesystem>)
//#  include <filesystem>
//#  define have_filesystem 1
//#endif
#ifdef _OPENMP
#include <omp.h>
#else
#include <chrono>
#endif

#ifdef _MSC_VER
#ifdef LINKING_DLL
#define DLLSPEC __declspec(dllimport)
#else
#define DLLSPEC __declspec(dllexport)
#endif
#else
#define DLLSPEC
#endif

static constexpr double infinity = std :: numeric_limits < double > :: infinity();
static constexpr double epsilon  = std :: numeric_limits < double > :: epsilon();

enum {
  _simple = 0L,
  _hard,
  _corrected,
  _continuous
};

extern DLLSPEC std :: mt19937 global_engine;


static std :: unordered_map < std :: string, long int > _formulas_
{
  {"simple",     _simple},
  {"hard",       _hard},
  {"corrected",  _corrected},
  {"continuous", _continuous}
};

std :: vector < std :: string > split (const std :: string & txt, const std :: string & del);

#if !defined __clang__ && __GNUC__ == 4 && __GNUC_MINOR__ < 9
namespace std
{

template < typename T >
std :: unique_ptr < T > make_unique ( std :: size_t size )
{
  return std :: unique_ptr < T > ( new typename std :: remove_extent < T > :: type[size] () );
}

}
#endif


inline bool file_exists (const std :: string & filename)
{
//#if have_filesystem == 1
//  return std :: filesystem :: exists(std :: filesystem :: path(filename));
//#else
  if (FILE *file = fopen(filename.c_str(), "r"))
  {
    fclose(file);
    return true;
  }
  return false;
//#endif
}

inline const auto what_time_is_it_now()
{
#ifdef _OPENMP
  return omp_get_wtime();
#else
  return std :: chrono :: high_resolution_clock :: now();
#endif
}

template < typename Time > inline const auto duration (const Time & start)
{
#ifdef _OPENMP
  return omp_get_wtime() - start;
#else
  return std :: chrono :: duration_cast < std :: chrono :: seconds >(std :: chrono :: high_resolution_clock :: now() - start).count();
#endif
}

//static void menu_SGD()
//{
//    std :: cout << std :: endl;
//    std :: cout << " BinaryCommitteeMachineRSGD Algorithm" << std :: endl;
//    std :: cout << " ====================================" << std :: endl;
//    std :: cout << " *                                  *" << std :: endl;
//    std :: cout << " *       University of Bologna      *" << std :: endl;
//    std :: cout << " *                                  *" << std :: endl;
//    std :: cout << " *            Nico Curti            *" << std :: endl;
//    std :: cout << " *                &                 *" << std :: endl;
//    std :: cout << " *         Daniele Dall'Olio        *" << std :: endl;
//    std :: cout << " *                                  *" << std :: endl;
//    std :: cout << " *       nico.curti2@unibo.it       *" << std :: endl;
//    std :: cout << " * daniele.dallolio@studio.unibo.it *" << std :: endl;
//    std :: cout << " *                                  *" << std :: endl;
//    std :: cout << " ====================================" << std :: endl;
//    std :: cout << "          Optimized Library          " << std :: endl << std :: endl;
//}

#endif // __utils__
