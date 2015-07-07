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

 

/** \file ReductionBinaryExpr.hpp */

#pragma once

#include "traits/concepts.hpp"
#include "traits/base_expr.hpp" // for base_expr

namespace AMDiS {

  /// Expression with two arguments, that reduces to a scalar
  template <Expression E1, Expression E2, class F>
      requires Reduction_sfunctor<F, Value_type<E1>, Value_type<E2>>
  struct ReductionBinaryExpr
  {
    typedef ReductionBinaryExpr                self;
    
    typedef Result_type<F>               value_type;
    typedef traits::max_size_type<E1,E2>  size_type;
    typedef E1                           expr1_type;
    typedef E2                           expr2_type;
    
    static constexpr int _SIZE = 1;
    static constexpr int _ROWS = 1;
    static constexpr int _COLS = 1;
    
  private:
    static constexpr int ARG_SIZE = max(E1::_SIZE, E2::_SIZE);
    
  public:
    /// constructor takes two expression \p A and \p B.
    ReductionBinaryExpr(expr1_type const& A, expr2_type const& B) 
	: expr1(A), expr2(B)
    { 
      TEST_EXIT_DBG( size(A) == size(B) )("Sizes do not match!\n");
    }
    
    /// access the elements of an expr.
    inline value_type operator()(size_type = 0, size_type = 0) const
    {
      return reduce(int_<ARG_SIZE>());
    }
    
    /// cast operator for assignment to scalar.
    operator value_type() const
    {
      return reduce(int_<ARG_SIZE>());
    }
    
    expr1_type const& get_first() const { return expr1; }
    expr2_type const& get_second() const { return expr2; }
    
  protected:
    template <int N>
    inline value_type reduce(int_<N>) const
    {
      using meta::FOR;
      value_type erg; F::init(erg);
      for (size_type i = 0; i < N; ++i)
	F::update(erg, expr1(i), expr2(i));
//       FOR<0,N>::inner_product(expr1, expr2, erg, F());
      return F::post_reduction(erg);
    }
    
    inline value_type reduce(int_<-1>) const
    {
      value_type erg; F::init(erg);
      for (size_type i = 0; i < size(expr1); ++i)
	F::update(erg, expr1(i), expr2(i));
      return F::post_reduction(erg);
    }
    
  private:
    expr1_type const& expr1;
    expr2_type const& expr2;
  };
  
  /// Size of ReductionBinaryExpr
  template <class E1, class E2, class F>
  size_t size(ReductionBinaryExpr<E1,E2,F> const&) { return 1; }
  
  /// Size of ReductionBinaryExpr
  template <class E1, class E2, class F>
  size_t num_rows(ReductionBinaryExpr<E1,E2,F> const&) { return 1; }
  
  /// Size of ReductionBinaryExpr
  template <class E1, class E2, class F>
  size_t num_cols(ReductionBinaryExpr<E1,E2,F> const&) { return 1; }
  
  // standard inner product
  template <Expression E1, Expression E2>
  using DotExpr =
    ReductionBinaryExpr<E1, E2, 
	      functors::dot_functor<Value_type<E1>, Value_type<E2> > >;
  
} // end namespace AMDiS
