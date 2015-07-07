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



/** \file basic.hpp */

#pragma once

#include <cfloat>
#include <type_traits>

#include "traits/concepts.hpp"
#include "operations/meta.hpp"

namespace AMDiS 
{
  // a variable template for break tolerances
  struct constants
  {
    template <class T> 
    static constexpr T tol = T(0);
  };
  
  template <> 
  constexpr float constants::tol<float> = (float)(FLT_EPSILON);
    
  template <> 
  constexpr double constants::tol<double> = (double)(DBL_EPSILON);
    
  template <> 
  constexpr long double constants::tol<long double> = (double)(LDBL_EPSILON);
  
  namespace traits 
  {
    
    /// traits class to get larger type (based on sizeof(...)) of T1 and T2
    template <class... Ts> struct Larger_type;
    
    template <class T1, class T2, class... Ts>
    struct Larger_type<T1, T2, Ts...>
    {
      typedef typename if_then_else< (sizeof(T1) > sizeof(T2)), 
				      Larger_type<T1, Ts...>, 
				      Larger_type<T2, Ts...> >::type type;
    };
    
    template <class T1, class T2>
    struct Larger_type<T1, T2>
    {
      typedef if_then_else< (sizeof(T1) > sizeof(T2)), T1, T2 > type;
    };
    
    template <class T> struct Larger_type<T> { typedef T type; };
    
    template <class... Es>
    using max_size_type = typename Larger_type<Size_type<Es>...>::type;
    
  } // end namespace traits
    
}
