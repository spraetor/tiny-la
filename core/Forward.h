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



/** \file Forward.h */

#pragma once

#include "Config.h"		// small_t
#include "traits/concepts.hpp"

namespace AMDiS {
  
  // some forwards declaration
  
  // memory-policies
  template <class T, small_t N, small_t M> requires (N*M > 0)  struct MemoryBaseStatic;
  template <class T, bool aligned>                             struct MemoryBaseDynamic;
  template <class T, small_t N, small_t M> requires (N*M > 0)  struct MemoryBaseHybrid;
    
  // size-policies
  struct DefaultSizePolicy;
  template <size_t S> struct StaticSizePolicy;
  
  // matrix-vector types
  template <concepts::Memory_policy M, concepts::Size_policy S>    struct VectorBase;
  template <concepts::Memory_policy M, concepts::Size_policy S>    struct MatrixBase;
  
} // end namespace AMDiS
