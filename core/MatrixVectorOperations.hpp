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



/** \file MatrixVectorOperations.hpp */

#pragma once

#include "expressions/all_expr.hpp"

#include "traits/concepts.hpp"
#include "operations/functors.hpp"
#include "operations/reduction_functors.hpp"
#include <boost/numeric/mtl/operation/sfunctor.hpp>

namespace AMDiS 
{

  // ---------------------------------------------------------------------------
  // Operations with Vector and Matrix (elementwise)
  
  /// expression for V + W
  template <Expression E1, Expression E2>
    requires concepts::Addable<Value_type<E1>, Value_type<E2>>
  auto operator+(E1 const& expr1, E2 const& expr2)
  {
    return PlusExpr<E1, E2>(expr1, expr2);
  }
  
  
  /// expression for V - W
  template <Expression E1, Expression E2>
    requires concepts::Subtractable<Value_type<E1>, Value_type<E2>>
  auto operator-(E1 const& expr1, E2 const& expr2)
  {
    return MinusExpr<E1, E2>(expr1, expr2);
  }
  
  
  /// expression for -V
  template <Expression E>
    requires concepts::Negatable<Value_type<E>>
  auto operator-(E const& expr)
  {
    using negate_op = ElementwiseUnaryExpr<E, mtl::sfunctor::negate<Value_type<E>> >;
    return negate_op(expr);
  }
  
  
  /// expression for V * scalar
  template <Arithmetic Value, Expression E>
    requires concepts::Multiplicable<Value_type<E>, Value>
  auto operator*(E const& expr, Value scal)
  {
    return RightScaleExpr<Value, E>(scal, expr);
  }
  
  
  /// expression for scalar * V
  template <Arithmetic Value, Expression E>
    requires concepts::Multiplicable<Value, Value_type<E>>
  auto operator*(Value scal, E const& expr)
  {
    return LeftScaleExpr<Value, E>(scal, expr);
  }
  
  
  /// expression for V / scalar
  template <Arithmetic Value, Expression E>
    requires concepts::Multiplicable<Value_type<E>, Value>
  auto operator/(E const& expr, Value scal)
  {
    return RightDivideExpr<Value, E>(scal, expr);
  }
  
  // ---------------------------------------------------------------------------
  // scalar product
  
  /// scalar product V*V
  template <VectorExpr E1, VectorExpr E2>
    requires concepts::Multiplicable<Value_type<E1>, Value_type<E2>>
  auto dot(E1 const& expr1, E2 const& expr2)
  {
    return DotExpr<E1, E2>(expr1, expr2)();
  }
  
  /// expression for V * W (dot product)
  template <VectorExpr E1, VectorExpr E2>
  auto operator*(E1 const& expr1, E2 const& expr2)
  {
    return dot(expr1, expr2);
  }

  // E1 x E2
  template <VectorExpr E1, VectorExpr E2> // assume vector shape for E1 and E2
  using CrossExpr = VectorBinaryExpr<E1, E2, 
      functors::MyCross<Value_type<E1>, Value_type<E2>> >;
    
  /// expression for V x W (cross product / outer product / tensor product)
  template <VectorExpr E1, VectorExpr E2>
    requires concepts::Multiplicable<Value_type<E1>, Value_type<E2>>
  auto cross(E1 const& expr1, E2 const& expr2)
  {
    return CrossExpr<E1, E2>(expr1, expr2);
  }
  
  // ---------------------------------------------------------------------------
  // reduction operations
  
  /// expression for one_norm(V)
  template <VectorExpr E>
  auto one_norm(E const& expr)
  {
    return OneNormExpr<E>(expr)();
  }
  
  /// expression for two_norm(V)
  template <VectorExpr E>
  auto two_norm(E const& expr)
  {
    return TwoNormExpr<E>(expr)();
  }
  
  /// expression for two_norm(M)
  template <MatrixExpr E>
  auto frobenius_norm(E const& expr)
  {
    return TwoNormExpr<E>(expr)();
  }
  
  /// expression for norm(V) := two_norm(V)
  template <VectorExpr E>
  auto norm(E const& expr)
  {
    return two_norm(expr);
  }
  
  /// expression for norm(M) := frobenius_norm(M)
  template <MatrixExpr E>
  auto norm(E const& expr)
  {
    return frobenius_norm(expr);
  }
  
  /// expression for unary_dot(V) = V*V
  template <VectorExpr E>
  auto unary_dot(E const& expr)
  {
    return UnaryDotExpr<E>(expr)();
  }
  
  /// expression for max(V)
  template <Expression E>
  auto max(E const& expr)
  {
    return MaxExpr<E>(expr)();
  }
  
  /// expression for abs_max(V)
  template <Expression E>
  auto abs_max(E const& expr)
  {
    return AbsMaxExpr<E>(expr)();
  }
  
  /// expression for inf_norm(V) = abs_max(V)
  template <VectorExpr E>
  auto inf_norm(E const& expr)
  {
    return abs_max(expr);
  }
  
  /// expression for inf_norm(M) = sqrt(rows*cols) * abs_max(M) = Gesamtnorm(M)
  template <MatrixExpr E>
  auto inf_norm(E const& expr)
  {
    return abs_max(expr) * std::sqrt(size(expr));
  }
  
  /// expression for min(V)
  template <Expression E>
  auto min(E const& expr)
  {
    return MinExpr<E>(expr)();
  }
  
  /// expression for abs_min(V)
  template <Expression E>
  auto abs_min(E const& expr)
  {
    return AbsMinExpr<E>(expr)();
  }
  
  /// expression for sum(V)
  template <Expression E>
    requires concepts::Addable<Value_type<E>, Value_type<E>>
  auto sum(E const& expr)
  {
    return SumExpr<E>(expr)();
  }
  
  /// expression for sum(V) = v0 + v1 + v2 + ...
  template <Expression E>
  auto mean(E const& expr)
  {
    return sum(expr) / size(expr);
  }
  
  /// expression for prod(V) = v0 * v1 * v2 * ...
  template <Expression E>
    requires concepts::Multiplicable<Value_type<E>, Value_type<E>>
  auto prod(E const& expr)
  {
    return ProdExpr<E>(expr)();
  }
  
  /// euklidean distance |V1 - V2|_2
  template <VectorExpr E1, VectorExpr E2>
  auto distance(E1 const& expr1, E2 const& expr2) // NOTE: in AMDiS::absteukl
  {
    return std::sqrt(unary_dot(expr1 - expr2));
  }
  
  
  // ---------------------------------------------------------------------------
  // matrix-vector multiplication
  
  /// expression for Mat * V
  template <MatrixExpr M, VectorExpr V>
  auto operator*(M const& mat, V const& vec)
  {
    return MatVecExpr<M, V, false>(mat, vec);
  }

  
  /// comparison of expressions
  template <Expression E1, Expression E2>
  inline bool operator==(E1 const& v1, E2 const& v2) 
  {    
    for (Size_type<E1> i = 0; i < size(v1); ++i)
      if (std::abs(v1(i) - v2(i)) > constants::tol<decltype(v1(i)-v2(i))>) 
	return false;
    
    return false;
  }

  
  /// comparison of expressions
  template <Expression E1, Expression E2>
  inline bool operator!=(E1 const& expr1, E2 const& expr2) 
  {
    return !(expr1 == expr2);
  }
  
  
  /// test for less-then (elementwise) up to DBL_TOL
  template <Expression E1, Expression E2>
  inline bool operator<(E1 const& v1, E2 const& v2) 
  {
    for (Size_type<E1> i = 0; i < size(v1); ++i) {
      if (std::abs(v1(i) - v2(i)) < constants::tol<decltype(v1(i)-v2(i))>) 
	continue;
      return v1(i) < v2(i);
    }
    return false;
  }
} // end namespace AMDiS
