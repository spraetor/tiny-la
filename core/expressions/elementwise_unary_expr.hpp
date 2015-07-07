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

 

/** \file ElementwiseUnaryExpr.hpp */

#pragma once

#include "traits/concepts.hpp"
#include "traits/size.hpp"
#include "traits/num_rows.hpp"
#include "traits/num_cols.hpp"

#include "traits/base_expr.hpp" // for ShapedExpr

namespace AMDiS {

  /// Expression with one argument
  template <ExpressionBase E, class F>
    requires Unary_sfunctor<F, Value_type<E>>
  struct ElementwiseUnaryExprBase
  {
    typedef ElementwiseUnaryExprBase    self;
    
    typedef Value_type<F>     value_type;
    typedef Size_type<E>       size_type;
    typedef E                  expr_type;
    
    static constexpr int _SIZE = E::_SIZE;
    static constexpr int _ROWS = E::_ROWS;
    static constexpr int _COLS = E::_COLS;
    
    /// constructor takes an expression
    ElementwiseUnaryExprBase(expr_type const& A) 
	: expr(A) 
    { }
    
    /// access the elements of an expr.
    inline value_type operator()(size_type i) const
    { 
      return F::apply( expr(i) );
    }
    
    expr_type const& get_first() const { return expr; }
    
  private:
    expr_type const& expr;
  };
  
  
  template <ExpressionBase E, class F>
  struct ElementwiseUnaryExpr {};
  
  
  template <VectorExpr E, class F>
  struct ElementwiseUnaryExpr<E, F>
    : public ElementwiseUnaryExprBase<E, F>
  {
    typedef ElementwiseUnaryExprBase<E, F>  super;
    ElementwiseUnaryExpr(E const& expr) : super(expr) { }
  };
  
  
  template <MatrixExpr E, class F>
  struct ElementwiseUnaryExpr<E, F>
    : public ElementwiseUnaryExprBase<E, F>
  {
    typedef ElementwiseUnaryExprBase<E, F>  super;
    ElementwiseUnaryExpr(E const& expr) : super(expr) { }
    
    /// access the elements of a matrix-expr.
    inline typename super::value_type 
    operator()(typename super::size_type i, typename super::size_type j) const
    { 
      return F::apply( super::expr(i, j) );
    }
    using super::operator();
  };
  
  
  /// Size of ElementwiseUnaryExpr
  template <class E, class F>
  size_t size(ElementwiseUnaryExpr<E, F> const& expr)
  {
    return size(expr.get_first());
  }
  
  /// number of rows of ElementwiseUnaryExpr
  template <class E, class F>
  size_t num_rows(ElementwiseUnaryExpr<E, F> const& expr)
  {
    return num_rows(expr.get_first());
  }
  
  /// number of columns of ElementwiseUnaryExpr
  template <class E, class F>
  size_t num_cols(ElementwiseUnaryExpr<E, F> const& expr)
  {
    return num_cols(expr.get_first());
  }
} // end namespace AMDiS
