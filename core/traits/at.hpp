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



/** \file at.hpp */

#pragma once

#include "traits/concepts.hpp"

namespace AMDiS 
{  
  /// function to access scalars (const version)
  template <class S>
    requires ScalarExpr<S> || Arithmetic<S>
  auto const& at(const S& c, size_t = 0, size_t = 0)
  {
    return c;
  }
  
  /// function to access scalars
  template <class S>
    requires ScalarExpr<S> || Arithmetic<S>
  auto& at(S& c, size_t = 0, size_t = 0)
  {  
    return c;
  }
  
  /// function to access vectors components (const version)
  template <VectorExpr V>
  auto const& at(const V& v, size_t rows)
  {  
    return v(rows);
  }
  
  /// function to access vectors components
  template <VectorExpr V>
  auto& at(V& v, size_t rows)
  {  
    return v(rows);
  }
  
  
  /// function to access matrix components (const version)
  template <MatrixExpr M>
  auto const& at(const M& m, size_t rows, size_t cols)
  {  
    return m(rows, cols);
  }
  
  /// function to access matrix components
  template <MatrixExpr M>
  auto& at(M& m, size_t rows, size_t cols)
  {  
    return m(rows, cols);
  }
    
    
  template <class T>
  T& at(T* vec, size_t i) { return vec[i]; }
   
  template <class T>
  T const& at(T const* vec, size_t i) { return vec[i]; }
    
    
} // end namespace AMDiS
