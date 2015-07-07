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



/** \file meta.hpp */

#pragma once

#include <type_traits>
#include "traits/concepts.hpp"

namespace AMDiS 
{
  // introduce some shortcuts for integral constants
  // -----------------------------------------------
  template <int I> 
  using int_ = std::integral_constant<int, I>;
  
  template <bool B> 
  using bool_ = std::integral_constant<bool, B>;
  
  using true_ = bool_<true>;
  using false_ = bool_<false>;
  
  namespace concepts
  {
    /// Concept for an compile-time constant
    template <class I>
    concept bool CTC = requires() { /*constexpr*/ I::value; };
    
    /// Concept for an integral constant
    template <class I>
    concept bool Bool_ctc = CTC<I> && 
      requires() { 
	{ I::value } -> bool; 
      };
    
    /// Concept for an integral constant
    template <class I>
    concept bool Int_ctc = CTC<I> && 
      requires() { 
	{ I::value } -> int; 
      };
    
  } // end namespace concepts
    
  // --- some boolean operations -----------------------------------------------
  namespace aux
  {
    template <class T0, class... Ts>
      requires concepts::CTC<T0> && concepts::CTC<Ts...>
    struct or_ : bool_<T0::value || or_<Ts...>::value> {};
    
    template <class T0>
      requires concepts::CTC<T0>
    struct or_<T0> : bool_<T0::value> {};
      
    template <class T0, class... Ts>
      requires concepts::CTC<T0> && concepts::CTC<Ts...>
    struct and_ : bool_<T0::value && and_<Ts...>::value> {};
    
    template <class T0>
      requires concepts::CTC<T0>
    struct and_<T0> : bool_<T0::value> {};
    
  } // end namespace aux
  
  template <class... Ts>
  using and_ = aux::and_<Ts...>;
  
  template <class... Ts>
  static constexpr bool and_v = and_<Ts...>::value;
  
  template <class... Ts>
  using or_ = aux::or_<Ts...>;
  
  template <class... Ts>
  static constexpr bool or_v = or_<Ts...>::value;
  
  template <concepts::CTC A>
  using not_ = bool_<!(A::value)>;
  
  template <class T>
  static constexpr bool not_v = not_<T>::value;
  
  
  template <bool C, class T1, class T2>
  using if_then_else = typename std::conditional<C, T1, T2>::type;
  
  
  /// Namespace for compile-time functions
  /** The namespace \ref meta provides some meta-functions mainly for
    *  compile-time integer arithmetics
    **/
  namespace meta { }
  
  // _________________________________________________________________________ 
  /// max(x,y)  
  template <concepts::Totally_ordered T> 
  constexpr T max(T x, T y) { return x > y ? x : y; }
    
  // _________________________________________________________________________
  /// min(x,y)
  template <concepts::Totally_ordered T> 
  constexpr T min(T x, T y) { return x < y ? x : y; }
    
  namespace meta
  {
    // _________________________________________________________________________
    /// abs<X> == |X|
    template <int X>
    struct abs : int_< (X >= 0 ? X : -X) > {};

    // _________________________________________________________________________
    /// pow<X, p> == X^p
    template <int X, int P>
    struct pow : int_< (X * pow<X, P-1>::value) > {};

    /// \cond HIDDEN_SYMBOLS
    template <int X>
    struct pow<X, 0> : int_< 1 > {};
    /// \endcond

    /// sqr<X> = X^2
    template <int X>
    struct sqr : pow<X, 2> {};
  
    // _________________________________________________________________________
    /// root<x, p> == p-th-root(x)
    template <int X, int P, int I = 1>
    struct root : if_then_else
      < 
        (pow<I, P>::value < X), 
        root<X, P, I+1>, 
        int_< I > 
      > {};

    /// \cond HIDDEN_SYMBOLS
    template <int X, int P>
    struct root<X, P, X> : int_< X > {};
    /// \endcond

    /// sqrt<X> = integer sqrt of X
    template <int X>
    struct sqrt : root<X, 2> {};

    // _________________________________________________________________________
    /// log<x, p> == log_p(x)
    template <int X, int P, int I = 1>
    struct log : if_then_else
      < (pow<P, I>::value < X), log<X, P, I+1>, int_< I > > {};

    /// \cond HIDDEN_SYMBOLS
    template <int X, int P>
    struct log<X, P, X> : int_< 0 > {};
    /// \endcond

    /// evaluates to true if \p X is power of \p Base
    template <int X, int Base>
    struct is_power_of : bool_< (pow<Base, log<X, Base>::value>::value == X) > {};

    // _________________________________________________________________________
    /// evalutes to true, if \p U is divisible by \p V
    template <int U, int V>
    struct is_divisible_by : bool_< (U % V == 0) > {};

    /// \cond HIDDEN_SYMBOLS
    template <int U>
    struct is_divisible_by<U, 0> : false_ {};
    /// \endcond

    /// evalutes to true, if \p U is divisible by 2
    template <int U>
    struct is_even : is_divisible_by<U, 2> {};
    
    /// evalutes to true, if \p U is not divisible by 2
    template <int U>
    struct is_odd : not_< is_even<U> > {};

    
  } // end namespace meta

} // end namespace AMDiS
