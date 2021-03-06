/******************************************************************************
 *
 * AMDiS - Adaptive multidimensional simulations
 *
 * Copyright (C) 2013 Dresden University of Technology. All Rights Reserved.
 * Web: https://fusionforge.zih.tu-dresden.de/projects/amdis
 *
 * Authors: 
 * Simon Vey, Thomas Witkowski, Andreas Naumann, Simon Praetorius, et al.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *
 * This file is part of AMDiS
 *
 * See also license.opensource.txt in the distribution.
 * 
 ******************************************************************************/



/** \file Config_gcc.h */

#pragma once

#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

#define COMPILER_NAME "gcc"
#define COMPILER_VERSION GCC_VERSION

// alignement specification
// ------------------------
#define ALIGNED(type,name,N)  type name[N] __attribute__ ((aligned(CACHE_LINE)))
#define ASSUME_ALIGNED(var)   __builtin_assume_aligned(var, CACHE_LINE)

typedef double aligned_double   __attribute__ ((aligned(CACHE_LINE)));
typedef float  aligned_float    __attribute__ ((aligned(CACHE_LINE)));
typedef int    aligned_int      __attribute__ ((aligned(CACHE_LINE)));
typedef size_t aligned_size_t   __attribute__ ((aligned(CACHE_LINE)));

// some compiler attributes
// ------------------------
#define NOINLINE                __attribute__ ((noinline))
#define ALWAYS_INLINE           __attribute__ ((always_inline))
#define OPENMODE                std::ios::openmode

#define ASM(text)								asm(text)

// C++11 features
// --------------
#if __cplusplus > 199711L

#if GCC_VERSION >= 40300
  #define HAS_VARIADIC_TEMPLATES 1
#endif

#if GCC_VERSION >= 40700
  #define HAS_ALIAS_TEMPLATES 1
#endif

#if GCC_VERSION >= 40300
  #define HAS_DECLTYPE 1
#endif

#if GCC_VERSION >= 40600
  #define HAS_CONSTEXPR 1
#endif

#if GCC_VERSION >= 40700
  #define HAS_DELEGATING_CONSTRUCTORS 1
#endif

#if GCC_VERSION >= 40600
  #define HAS_RANGE_BASED_FOR 1
#endif

#if GCC_VERSION >= 40400
  #define HAS_INITIALIZER_LISTS 1
#endif

#if GCC_VERSION >= 40700
  #define HAS_OVERRIDE 1
#endif

#if GCC_VERSION >= 40400
  #define HAS_TYPED_ENUMS 1
#endif

#if GCC_VERSION >= 40300
  #define HAS_RVALUE_REFERENCES 1
#endif

#endif
