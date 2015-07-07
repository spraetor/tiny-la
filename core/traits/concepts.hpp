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



/** \file concepts.hpp */

#pragma once

#include <type_traits>
#include "operations/meta_basic.hpp"

namespace AMDiS 
{
  template <class T>
    requires requires() { typename T::value_type; }
  using Value_type = typename T::value_type;
  
  template <class T>
    requires requires() { typename T::size_type; }
  using Size_type = typename T::size_type;
  
  template <class T>
    requires requires() { typename T::result_type; }
  using Result_type = typename T::result_type;
    
  
  /// \brief concepts to test for expressions
  template <class T> concept bool ExpressionBase =
    requires() { 
      typename Size_type<T>; 
      typename Value_type<T>;
      { T::_SIZE } -> int; 
      { T::_ROWS } -> int; 
      { T::_COLS } -> int;
    };
    
    
  /// \brief all expressions need a simple operator()(int) method for traverse routines
  template <class T> concept bool Expression = ExpressionBase<T> && 
    requires(T expr, Size_type<T> i) { 
      expr(i); 
    };
  
    
  /// \brief Concepts to test for matrices
  template <class T>
  concept bool MatrixExpr = Expression<T> &&
    requires(T mat, Size_type<T> i, Size_type<T> j) { 
      mat(i, j); 
    };
    

  /// \brief Concept to test for vectors
  template <class T>
  concept bool VectorExpr = Expression<T> && !MatrixExpr<T>;
  
    
  /// \brief Concept to test for scalars
  template <class T>
  concept bool ScalarExpr = ExpressionBase<T> && !VectorExpr<T> &&
    requires(T scalar) { 
      scalar(); 
    };
  
    
  /// \brief Defines a concept for a size policy
  template <class S>
  concept bool Size_policy = 
    requires(std::size_t i) { 
      { S::eval(i) } -> std::size_t; 
    };
  
  
  /// \brief Defines a concept for a memory policy
  template <class M>
  concept bool Memory_policy = ExpressionBase<M> &&
    requires(M m) { 
      m.data(); 
      m.getSize(); 
    };

    

  /// \brief Concept to test for integer types: 
  /// (unsigned) {(short|long|long long) int | char}
  template <class T>
  concept bool Integral = std::is_integral<T>::value;
  

  /// \brief Concept to test for floating point types: 
  /// {float | double | long double}
  template <class T>
  concept bool Floating_point = std::is_floating_point::value;
  
    
  /// Concept to test for scalar types: integral / floating point types
  template <class T>
  concept bool Arithmetic = Integral<T> || Floating_point<T>;
 
  
  /// Concept for a static unary functor
  template <class F, class T>
  concept bool Unary_sfunctor = 
      requires(T arg) { 
	{ F::apply(arg) } -> Result_type<F>; 
      };
      
  
  /// Concept for a static binary functor
  template <class F, class T0, class T1>
  concept bool Binary_sfunctor = 
      requires(T0 arg0, T1 arg1) { 
	{ F::apply(arg0, arg1) } -> Result_type<F>; 
      };
      
  
  /// concept for a static binary reduction functor
  template <class F, class... Ts>
  concept bool Reduction_sfunctor = 
      requires() { typename Result_type<F>; } &&	// type requirement
      requires(Result_type<F> erg, Ts... args) { 
	F::init(erg);					// init the value erg
	F::update(erg, args...); 			// update the value erg
	{ F::post_reduction(erg) } -> Result_type<F>; 	// apply a post reduction
      };
    
    
  namespace concepts
  {
      
    /// \brief Concept to test whether two types can be conterted to each other 
    template <class A, class B>
    concept bool Convertible = std::is_convertible<A, B>::value;
    
    
    /// \brief Concept to test whether two types can be multiplied, 
    /// i.e. there is an operator*(A,B)
    template <class A, class B>
    concept bool Multiplicable = requires(A a, B b) { a * b; };
      
    
    /// \brief Concept to test whether two types can be divided, 
    /// i.e. there is an operator/(A,B)
    template <class A, class B>
    concept bool Divisible = requires(A a, B b) { a / b; };
      

    /// \brief Concept to test whether two types can be added, 
    /// i.e. there is an operator+(A,B)
    template <class A, class B>
    concept bool Addable = requires(A a, B b) { a + b; };
      

    /// \brief Concept to test whether two types can be subtracted, 
    /// i.e. there is an operator-(A,B)
    template <class A, class B>
    concept bool Subtractable = requires(A a, B b) { a - b; };
      

    /// \brief Concept to test whether the type can be negated, 
    /// i.e. there is an operator-(A)
    template <class A>
    concept bool Negatable = requires(A a) { -a; };
    
    template <class T>
    concept bool Equality_comparable =
	requires(T a, T b) { 
	    { a == b } -> bool; 
	    { a != b } -> bool;
	};
    
    template <class T>
    concept bool Totally_ordered =
	Equality_comparable<T> &&
	requires(T a, T b) {
	  { a < b } -> bool;
	  { a > b } -> bool;
	  { a <= b } -> bool;
	  { a >= b } -> bool;
	};
  
    
    /// \brief Concept to test whether std::memcpy can be used for T
    template <class T>
    concept bool Trivially_copyable = std::is_trivially_copyable<T>::value;
	
  } // end namespace concepts
}
