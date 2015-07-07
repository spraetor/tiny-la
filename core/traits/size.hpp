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



/** \file size.hpp */

#pragma once

#include "traits/concepts.hpp"

namespace AMDiS 
{
  /// \cond HIDDEN_SYMBOLS
  namespace traits 
  {
    template <class T>
    struct size {
      typedef size_t type;
      inline type operator()(const T& v) const { return 0; }  
    };
      
    template <class T>
      requires ScalarExpr<T> || Arithmetic<T>
    struct size<T>
    {
      typedef size_t type;
      inline type operator()(const T& v) const { return 1; }
    };
    
  } // end namespace traits
  /// \endcond
  
  
  /// Returns the size of the container \p t
  template <class T>
  size_t inline size(const T& t)
  {
      return traits::size<T>()(t);
  }
    
} // end namespace AMDiS
