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
#include "traits/concepts.hpp"

namespace AMDiS 
{
  namespace map
  {
    template <class T>
    concept bool AccessBase = 
      requires() { 
	typename Value_type<T>; 
	typename Size_type<T>;
      };
    
    template <class T>
    concept bool ArrayAccess = AccessBase<T> &&
      requires(T a, Size_type<T> i) { 
	{ a[i] } -> Value_type<T>;
      };
      
    template <class T>
    concept bool VectorAccess = AccessBase<T> &&
      requires(T a, Size_type<T> i) { 
	{ a(i) } -> Value_type<T>;
      };
    
    template <class T>
    struct Accessor;
    
    template <ArrayAccess A>
      requires !VectorAccess<A>
    struct Accessor<A>
    {
      Accessor(A& a) : a(a) {}
      
      Value_type<A>& operator()(Size_type<A> i) { return a[i]; }
      Value_type<A> const& operator()(Size_type<A> i) const { return a[i]; }
      Value_type<A>& operator[](Size_type<A> i) { return a[i]; }
      Value_type<A> const& operator[](Size_type<A> i) const { return a[i]; }
      
    private:
      A& a;
    };
    
    template <VectorAccess V>
      requires !ArrayAccess<V>
    struct Accessor<V>
    {
      Accessor(V& v) : v(v) {}
      
      Value_type<V>& operator()(Size_type<V> i) { return v(i); }
      Value_type<V> const& operator()(Size_type<V> i) const { return v(i); }
      Value_type<V>& operator[](Size_type<V> i) { return v(i); }
      Value_type<V> const& operator[](Size_type<V> i) const { return v(i); }
      
    private:
      V& v;
    };
    
  } // end namespace map
} // end namespace AMDiS
