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



/** \file Config.h */

#pragma once

/** \brief current AMDiS version */
#ifndef AMDIS_VERSION
  #define AMDIS_VERSION  "AMDiS: Version 0.9.1"
#endif

#include <boost/config.hpp>

#ifndef CACHE_LINE
  #define CACHE_LINE 16
#endif

// if FIXED_SIZE == 1 use static arrays
#ifndef FIXED_SIZE
  #define FIXED_SIZE 1
//  #define DOW 2
//  #define DIM 2
#endif

#if defined(__clang__)					// Clang/LLVM.
  #include "config/Config_clang.h"
#elif defined(__ICC) || defined(__INTEL_COMPILER)	// Intel ICC/ICPC. 
  #include "config/Config_intel.h"
#elif defined(__GNUC__) || defined(__GNUG__)		// GNU GCC/G++.
  #include "config/Config_gcc.h"
#elif defined(_MSC_VER)					// Microsoft Visual Studio. 
  #include "config/Config_msc.h"
#else
  error: not supported compiler
  // simply not configured and not tested
#endif

#include "config/Config_defaults.h"

#if !HAS_CONSTEXPR && !defined(constexpr)
  #define constexpr const
#endif

#if !HAS_OVERRIDE && !defined(override)
  #define override
#endif

typedef unsigned char small_t;   // only allow small matrices

#define DBL_TOL 1.e-9

// some workarounds for mtl (since the namespace has changed)
#define MTL_VEC mtl::vector
#define MTL_MAT mtl::matrix

