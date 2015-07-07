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

 

/** \file ScalarExpr.hpp */

#pragma once

#include "traits/concepts.hpp"
#include "traits/size.hpp"
#include "traits/num_rows.hpp"
#include "traits/num_cols.hpp"

#include "traits/base_expr.hpp" // for BaseExpr

namespace AMDiS {

  /// Expression to encapsulate scalars
  template <Arithmetic Value>
  struct ScalarExpr
  {
    typedef ScalarExpr          self;
    typedef small_t        size_type;
    typedef Value         value_type;
    
    static constexpr int _SIZE = 1;
    static constexpr int _ROWS = 1;
    static constexpr int _COLS = 1;
    
  public:
    /// construcor takes the factor \p factor_
    ScalarExpr(value_type factor_) : factor(factor_) {}
    
    /// access the elements of an expr.
    value_type operator()(size_type = 0, size_type = 0) const
    { 
      return factor;
    }
    
    /// cast operator for assignment
    operator value_type() const
    {
      return factor;
    }
  
  private:
    value_type factor;
  };
  
  /// Size of ScalarExpr
  template <class Value>
  inline size_t size(ScalarExpr<Value> const&) {  return 1; }
  
  /// Size of ScalarExpr
  template <class Value>
  inline size_t num_rows(ScalarExpr<Value> const&) { return 1; }
  
  /// Size of ScalarExpr
  template <class Value>
  inline size_t num_cols(ScalarExpr<Value> const&) { return 1; }
  
} // end namespace AMDiS
