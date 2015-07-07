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

 

/** \file ElementwiseBinaryExpr.hpp */

#pragma once

#include <boost/numeric/mtl/operation/sfunctor.hpp>

#include "traits/concepts.hpp"
#include "traits/size.hpp"
#include "traits/num_rows.hpp"
#include "traits/num_cols.hpp"

#include "traits/base_expr.hpp" // for ShapedExpr

namespace AMDiS {

  /// Expression with two arguments
  template <Expression E1, Expression E2, class F>
    requires Binary_sfunctor<F, Value_type<E1>, Value_type<E2>>
  struct ElementwiseBinaryExprBase
  {
    typedef ElementwiseBinaryExprBase           self;
    
    typedef Result_type<F>                value_type;
    typedef traits::max_size_type<E1,E2>   size_type;
    typedef E1                            expr1_type;
    typedef E2                            expr2_type;
    
    static constexpr int _SIZE = max(E1::_SIZE, E2::_SIZE);
    static constexpr int _ROWS = max(E1::_ROWS, E2::_ROWS);
    static constexpr int _COLS = max(E1::_COLS, E2::_COLS);
    
    /// constructor takes two expressions
    ElementwiseBinaryExprBase(expr1_type const& A, expr2_type const& B) 
	: expr1(A), expr2(B) 
    { 
      TEST_EXIT_DBG( size(A) == size(B) )("Sizes do not match!\n");
    }
    
    /// access the elements of an expr.
    inline value_type operator()(size_type i) const
    { 
      return F::apply( expr1(i), expr2(i) );
    }
    
    expr1_type const& get_first() const { return expr1; }
    expr2_type const& get_second() const { return expr2; }
    
  private:
    expr1_type const& expr1;
    expr2_type const& expr2;
  };
  
  
  template <Expression E1, Expression E2, class F>
  struct ElementwiseBinaryExpr {};
  
  
  // elementwise vector expressions
  template <VectorExpr E1, VectorExpr E2, class F>
  struct ElementwiseBinaryExpr<E1, E2, F>
    : public ElementwiseBinaryExprBase<E1, E2, F>
  {
    typedef ElementwiseBinaryExprBase<E1, E2, F>  super;
    ElementwiseBinaryExpr(E1 const& e1, E2 const& e2) : super(e1, e2) { }
  };
    
  
  // elementwise matrix expression
  template <MatrixExpr E1, MatrixExpr E2, class F>
  struct ElementwiseBinaryExpr<E1, E2, F>
    : public ElementwiseBinaryExprBase<E1, E2, F>
  {
    typedef ElementwiseBinaryExprBase<E1, E2, F>  super;
    ElementwiseBinaryExpr(E1 const& e1, E2 const& e2) : super(e1, e2) { }
    
    /// access the elements of a matrix-expr.
    inline typename super::value_type 
    operator()(typename super::size_type i, typename super::size_type j) const
    { 
      return F::apply( super::expr1(i, j), super::expr2(i, j) );
    }
    using super::operator();
  };
  
  
  /// Size of ElementwiseBinaryExpr
  template <class E1, class E2, class F>
  size_t size(ElementwiseBinaryExpr<E1, E2, F> const& expr)
  {
    return size(expr.get_first());
  }
  
  /// number of rows of ElementwiseBinaryExpr
  template <class E1, class E2, class F>
  size_t num_rows(ElementwiseBinaryExpr<E1, E2, F> const& expr)
  {
    return num_rows(expr.get_first());
  }
  
  /// number of columns of ElementwiseBinaryExpr
  template <class E1, class E2, class F>
  size_t num_cols(ElementwiseBinaryExpr<E1, E2, F> const& expr)
  {
    return num_cols(expr.get_first());
  }

  // E1 + E2
  template <class E1, class E2>
  using PlusExpr =
    ElementwiseBinaryExpr<E1, E2, 
      mtl::sfunctor::plus<Value_type<E1>, Value_type<E2> > >;
      
  // E1 - E2
  template <class E1, class E2>
  using MinusExpr =
    ElementwiseBinaryExpr<E1, E2, 
      mtl::sfunctor::minus<Value_type<E1>, Value_type<E2> > >;
      
  
} // end namespace AMDiS
