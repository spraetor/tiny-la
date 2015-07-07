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


 
/** \file binary_expr.hpp */

#pragma once

#include "traits/concepts.hpp"
#include "traits/size.hpp"
#include "traits/num_rows.hpp"
#include "traits/num_cols.hpp"

#include "traits/base_expr.hpp" // for shaped_expr

#include "operations/functors.hpp"
// #include "traits/mult_type.hpp"

namespace AMDiS {

  /// Expression with two arguments
  template <VectorExpr E1, VectorExpr E2, class Functor>
  struct VectorBinaryExpr
  {
    typedef VectorBinaryExpr                                  self;
    
    typedef Value_type<E1>                              value_type;
    typedef traits::max_size_type<E1,E2>                 size_type;
    typedef E1                                          expr1_type;
    typedef E2                                          expr2_type;
    
    static constexpr int _SIZE = max(E1::_SIZE, E2::_SIZE);
    static constexpr int _ROWS = max(E1::_ROWS, E2::_ROWS);
    static constexpr int _COLS = max(E1::_COLS, E2::_COLS);
    
  public:
    /// constructor takes two expressions
    VectorBinaryExpr(expr1_type const& A, expr2_type const& B) 
	: expr1(A), expr2(B) 
    { 
      assert( size(A) == size(B) );
    }
    
    /// access the elements of an expr.
    inline value_type operator()(size_type i) const
    { 
      return Functor::apply( i, expr1, expr2 );
    }
    
    /// access the elements of a matrix-expr.
//     inline value_type operator()(size_type i, size_type j) const
//     { 
//       return Functor::apply( i, j, expr1, expr2 );
//     }
    
    expr1_type const& get_first() const { return expr1; }
    expr2_type const& get_second() const { return expr2; }
    
  private:
    expr1_type const& expr1;
    expr2_type const& expr2;
  };
  
  /// Size of VectorBinaryExpr
  template <class E1, class E2, class F>
  size_t size(VectorBinaryExpr<E1, E2, F> const& expr)
  {
    return size(expr.get_first());
  }
  
  /// number of rows of VectorBinaryExpr
  template <class E1, class E2, class F>
  size_t num_rows(VectorBinaryExpr<E1, E2, F> const& expr)
  {
    return num_rows(expr.get_first());
  }
  
  /// number of columns of VectorBinaryExpr
  template <class E1, class E2, class F>
  size_t num_cols(VectorBinaryExpr<E1, E2, F> const& expr)
  {
    return num_cols(expr.get_first());
  }
  
} // end namespace AMDiS
