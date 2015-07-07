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

 

/** \file ScaleExpr.hpp */

#pragma once

#include <boost/numeric/mtl/operation/sfunctor.hpp>

#include "traits/concepts.hpp"
#include "traits/size.hpp"
#include "traits/num_rows.hpp"
#include "traits/num_cols.hpp"

namespace AMDiS {

  /// Expression with one argument
  template <Arithmetic Value, ExpressionBase E, bool from_left, class F>
    requires (from_left==true  && Binary_sfunctor<F, Value, Value_type<E>>) ||
             (from_left==false && Binary_sfunctor<F, Value_type<E>, Value>)
  struct ScaleExprBase
  {
    typedef ScaleExprBase            self;
    
    typedef Value_type<E>      value_type;
    typedef Size_type<E>        size_type;
    typedef E                   expr_type;
    
    static constexpr int _SIZE = E::_SIZE;
    static constexpr int _ROWS = E::_ROWS;
    static constexpr int _COLS = E::_COLS;
    
  public:
    /// constructor takes the factor \p v and and expression \p A
    ScaleExprBase(Value v, expr_type const& A) 
	: value(v), expr(A) 
    { }
    
    expr_type const& get_first() const { return expr; }
    
    /// access the elements of an expr.
    inline value_type operator()(size_type i) const
    { 
      return apply(i, bool_<from_left>());
    }
    
  private:
    // scale from left
    inline value_type apply(size_type i, true_) const 
    {
      return F::apply( value, expr(i) );
    }
    
    // scale from right
    inline value_type apply(size_type i, false_) const 
    {
      return F::apply( expr(i), value );
    }
    
  protected:
    Value value;
    expr_type const& expr;
  };
  
  
  template <Arithmetic Value, ExpressionBase E, bool from_left, class F>
  struct ScaleExpr {};
  
  // scaling of vectors
  template <Arithmetic Value, VectorExpr E, bool from_left, class F>
  struct ScaleExpr<Value, E, from_left, F>
      : public ScaleExprBase<Value, E, from_left, F>
  {
    typedef ScaleExprBase<Value, E, from_left, F>  super;
    ScaleExpr(Value v, E const& expr) : super(v, expr) { }
  };
  
  
  // scaling of matrices
  template <Arithmetic Value, MatrixExpr E, bool from_left, class F>
  struct ScaleExpr<Value, E, from_left, F>
      : public ScaleExprBase<Value, E, from_left, F>
  {
    typedef ScaleExprBase<Value, E, from_left, F>  super;
    ScaleExpr(Value v, E const& expr) : super(v, expr) { }
    
    /// access the elements of a matrix-expr.
    inline typename super::value_type 
    operator()(typename super::size_type i, typename super::size_type j) const
    { 
      return apply(i, j, bool_<from_left>());
    }
    using super::operator();
    
  private:
    // scale from left
    inline typename super::value_type 
    apply(typename super::size_type i, typename super::size_type j, true_) const 
    {
      return F::apply( super::value, super::expr(i,j) );
    }
    
    // scale from right
    inline typename super::value_type 
    apply(typename super::size_type i, typename super::size_type j, false_) const 
    {
      return F::apply( super::expr(i,j), super::value );
    }
  };
  
  
  /// Size of ScaleExpr
  template <class V, class E, bool l, class F>
  size_t size(ScaleExpr<V, E, l, F> const& expr)
  {
    return size(expr.get_first());
  }
  
  /// Size of ScaleExpr
  template <class V, class E, bool l, class F>
  size_t num_rows(ScaleExpr<V, E, l, F> const& expr)
  {
    return num_rows(expr.get_first());
  }
  
  /// Size of ScaleExpr
  template <class V, class E, bool l, class F>
  size_t num_cols(ScaleExpr<V, E, l, F> const& expr)
  {
    return num_cols(expr.get_first());
  }

  // s * V
  template <Arithmetic Value, Expression E>
    requires concepts::Multiplicable<Value, Value_type<E>>
  using LeftScaleExpr =
    ScaleExpr<Value, E, true, mtl::sfunctor::times<Value_type<E>, Value>>;
    
  // V * s
  template <Arithmetic Value, Expression E>
    requires concepts::Multiplicable<Value_type<E>, Value>
  using RightScaleExpr =
    ScaleExpr<Value, E, false, mtl::sfunctor::times<Value_type<E>, Value>>;
  
  // V / s
  template <Arithmetic Value, Expression E>
    requires concepts::Divisible<Value_type<E>, Value>
  using RightDivideExpr =
    ScaleExpr<Value, E, false, mtl::sfunctor::divide<Value_type<E>, Value>>;
  
} // end namespace AMDiS
