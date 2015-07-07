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



/** \file resize.hpp */

#pragma once

#include <vector>
#include "traits/concepts.hpp"

namespace AMDiS 
{
  /// \cond HIDDEN_SYMBOLS
  namespace traits 
  {
  
    /// General declaration, used to disable unsupported types
    template <typename Collection>
    struct resize {};
    
    /// change_dim implementation for STL vectors
    template <typename Value>
    struct resize< std::vector<Value> > 
    {
      void operator()(std::vector<Value>& v, size_t r) { 
	v.resize(r);
      }
    };
    
  } // end namespace traits
  /// \endcond
    
  /// Resize function for vectors
  template <VectorExpr Vec_t>
  void resize(Vec_t& v, size_t rows)
  {  
    traits::resize<Vec_t>()(v, rows);
  }
  
  /// Resize function for matrices
  template <MatrixExpr Mat_t>
  void resize(Mat_t& m, size_t rows, size_t cols)
  {  
    traits::resize<Mat_t>()(m, rows, cols);
  }
    
} // end namespace AMDiS
