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



/** \file meta_basic.hpp */

#pragma once

#include <type_traits>

#define STATIC_ASSERT(...) \
  static_assert(__VA_ARGS__, #__VA_ARGS__)

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
      requires concepts::Bool_ctc<T0>
    struct or_ : bool_<T0::value || or_<Ts...>::value> {};
    
    template <class T0>
      requires concepts::Bool_ctc<T0>
    struct or_<T0> : bool_<T0::value> {};
      
    template <class T0, class... Ts>
      requires concepts::Bool_ctc<T0>
    struct and_ : bool_<T0::value && and_<Ts...>::value> {};
    
    template <class T0>
      requires concepts::Bool_ctc<T0>
    struct and_<T0> : bool_<T0::value> {};
    
  } // end namespace aux
  
  template <class... Ts>
  using and_ = aux::and_<Ts...>;
  
  template <bool... Bs>
  using and_c = and_<bool_<Bs>...>;
  
  template <class... Ts>
  static constexpr bool and_v = and_<Ts...>::value;
  
  template <class... Ts>
  using or_ = aux::or_<Ts...>;
  
  template <bool... Bs>
  using or_c = or_<bool_<Bs>...>;
  
  template <class... Ts>
  static constexpr bool or_v = or_<Ts...>::value;
  
  template <concepts::CTC A>
  using not_ = bool_<!(A::value)>;
  
  template <class T>
  static constexpr bool not_v = not_<T>::value;
  
  
  template <bool C, class T1, class T2>
  using if_then_else = typename std::conditional<C, T1, T2>::type;
  
  
  // tuple of types
  namespace aux
  {
    struct nil {};
    
    template <class T0, class... Ts>
    struct cons {
      using head = T0;
      using tail = cons<Ts...>;
    };
    
    template <class T0>
    struct cons<T0> {
      using head = T0;
      using tail = nil;
    };
    
  } // end namespace aux
  
  template <class... Ts>
  using cons = aux::cons<Ts...>;
  
  // boolean operations on type tuples
  namespace aux
  {
    template <class... Ts>
    struct and_<cons<Ts...> > : and_<Ts...> {};
    
    template <class... Ts>
    struct or_<cons<Ts...> > : or_<Ts...> {};
  }
  
  template <template<class> class Container, class... Ts>
  using expand = cons<Container<Ts>...>;
  
  template <template<class, class> class U, class X>
  struct bind_1st {
    template <class T>
    using type = U<X,T>;
  };
  
  template <template<class, class> class U, class X>
  struct bind_2nd {
    template <class T>
    using type = U<T,X>;
  };
  
  
  template <template<class, class> class Container, class X, class... Ts>
  using expand_1st = expand<bind_2nd<Container, X>::template type, Ts...>;
  
  template <template<class, class> class Container, class X, class... Ts>
  using expand_2nd = expand<bind_1st<Container, X>::template type, Ts...>;  
  
  template <class T, class... Ts>
  using is_one_of = or_< std::is_same<T, Ts>... >;
  
} // end namespace AMDiS
