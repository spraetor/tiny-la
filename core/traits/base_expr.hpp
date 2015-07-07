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

 

/** \file BaseExpr.hpp */

#pragma once

#include "traits/basic.hpp"
#include "traits/size.hpp"
#include "traits/num_rows.hpp"
#include "traits/num_cols.hpp"
#include "traits/at.hpp"

namespace AMDiS {

  struct BaseExprSup {};
  
  template <class Model>
  struct BaseExpr : BaseExprSup
  {
    Model& sub() { return static_cast<Model&>(*this); }
    Model const& sub() const { return static_cast<Model const&>(*this); }
  };
  
  template <class Model>
  struct VectorExpr : BaseExpr<Model> {};
  
  template <class Model>
  struct MatrixExpr : BaseExpr<Model> {};
  
  
  // determine shape of expression
  template <class Sub, class Model>
  struct ShapedExpr_aux { typedef BaseExpr<Model> type; };
  
  template <VectorExpr Sub, class Model>
  struct ShapedExpr_aux<Sub, Model> { typedef VectorExpr<Model> type; };
  
  template <MatrixExpr Sub, class Model>
  struct ShapedExpr_aux<Sub, Model> { typedef MatrixExpr<Model> type; };
  
  template <class Sub, class Model>
  using ShapedExpr = typename ShapedExpr_aux<Sub, Model>::type;

  
} // end namespace AMDiS
