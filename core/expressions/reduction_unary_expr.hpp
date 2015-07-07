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

 

/** \file ReductionUnaryExpr.hpp */

#pragma once


#include "traits/concepts.hpp"
#include "traits/base_expr.hpp" // for base_expr
#include "operations/reduction_functors.hpp"

namespace AMDiS {

  /// Expression with one argument, that reduces to a scalar
  template <Expression E, class F>
      requires Reduction_sfunctor<F, Value_type<E>>
  struct ReductionUnaryExpr
  {
    typedef ReductionUnaryExpr        self;
    
    typedef Result_type<F>      value_type;
    typedef Size_type<E>         size_type;
    typedef E                    expr_type;
    
    static constexpr int _SIZE = 1;
    static constexpr int _ROWS = 1;
    static constexpr int _COLS = 1;
    
  private:
    static constexpr int ARG_SIZE = E::_SIZE;
    
  public:
    /// constructor takes on expression \p A.
    ReductionUnaryExpr(expr_type const& A) 
      : expr(A)
    { }
    
    /// access the elements of an expr.
    inline value_type operator()(size_type = 0, size_type = 0) const
    {
      return reduce(int_<ARG_SIZE>()) ;
    }
    
    /// cast operator for assignment to scalar.
    operator value_type() const
    {
      return reduce(int_<ARG_SIZE>());
    }
    
    expr_type const& get_first() const { return expr; }
    
  protected:
    template <int N>
    inline value_type reduce(int_<N>) const
    {
      using meta::FOR;
      value_type erg; F::init(erg);
      
      for (size_type i = 0; i < size(expr); ++i)
	F::update(erg, expr(i));
      
//       FOR<0,N>::accumulate(expr, erg, F());
      return F::post_reduction(erg);
    }
    
    inline value_type reduce(int_<-1>) const
    {
      value_type erg; F::init(erg);
      for (size_type i = 0; i < size(expr); ++i)
	F::update(erg, expr(i));
      return F::post_reduction(erg);
    }
    
  private:
    expr_type const& expr;
  };
  
  /// Size of ReductionUnaryExpr
  template <class E, class F>
  size_t size(ReductionUnaryExpr<E,F> const&) { return 1; }
  
  /// Size of ReductionUnaryExpr
  template <class E, class F>
  size_t num_rows(ReductionUnaryExpr<E,F> const&) { return 1; }
  
  /// Size of ReductionUnaryExpr
  template <class E, class F>
  size_t num_cols(ReductionUnaryExpr<E,F> const&) { return 1; }
  
  
  // norm |V|_1
  template <Expression E>
  using OneNormExpr =
    ReductionUnaryExpr<E, functors::one_norm_functor<Value_type<E> > >;
    
  // norm |V|_2
  template <Expression E>
  using TwoNormExpr =
    ReductionUnaryExpr<E, functors::two_norm_functor<Value_type<E> > >;
    
  // V*V
  template <Expression E>
  using UnaryDotExpr =
    ReductionUnaryExpr<E, functors::unary_dot_functor<Value_type<E> > >;
    
  // max(V)
  template <Expression E>
  using MaxExpr =
    ReductionUnaryExpr<E, functors::max_reduction_functor<Value_type<E> > >;
    
  // abs_max(V)
  template <Expression E>
  using AbsMaxExpr =
    ReductionUnaryExpr<E, functors::abs_max_reduction_functor<Value_type<E> > >;
    
  // min(V)
  template <Expression E>
  using MinExpr =
    ReductionUnaryExpr<E, functors::min_reduction_functor<Value_type<E> > >;
    
  // max(V)
  template <Expression E>
  using AbsMinExpr =
    ReductionUnaryExpr<E, functors::abs_min_reduction_functor<Value_type<E> > >;
    
  // sum(V)
  template <Expression E>
  using SumExpr =
    ReductionUnaryExpr<E, functors::sum_reduction_functor<Value_type<E> > >;
    
  // prod(V)
  template <Expression E>
  using ProdExpr =
    ReductionUnaryExpr<E, functors::prod_reduction_functor<Value_type<E> > >;
  
} // end namespace AMDiS
