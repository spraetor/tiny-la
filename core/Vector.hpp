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



/** \file Vector.hpp */

#pragma once

#include <algorithm>		// std::copy
#include <utility>		// std::swap
#include <initializer_list>	// std::initializer_list

#include "Log.h"
#include "Forward.h"
#include "MatrixVectorBase.hpp"

#include "traits/concepts.hpp"
#include "traits/size.hpp"
#include "traits/num_rows.hpp"
#include "traits/num_cols.hpp"

namespace AMDiS { 

  /// Base class for all vectors.
  /** Provide a MemoryPolicy \p MemoryPolicy and a \p SizePolicy for
   *  automatic size calculation. 
   **/
  template <concepts::Memory_policy  Mem, 
	    concepts::Size_policy    Size = DefaultSizePolicy>
  struct VectorBase 
      : public MatrixVectorBase< VectorBase<Mem, Size>, Mem>
  {
    typedef VectorBase                    self;
    typedef MatrixVectorBase<self, Mem>  super;
    
    typedef Value_type<super>       value_type;
    typedef Size_type<super>         size_type;
    
    typedef value_type*                pointer;
    typedef value_type const*    const_pointer;
    typedef pointer                   iterator;
    typedef const_pointer       const_iterator;
    
  protected:
    using super::_elements;
    using super::_size;
    using super::set;
      
  // ----- constructors / assignment -------------------------------------------
  public:
    /// \brief Default constructor.
    /// allocates memory for a vector of size \p s
    explicit VectorBase(size_type s = 0)
      : super(Size::eval(s))
    { }
    
    /// \brief Constructor with initializer.
    /// allocates memory for a vector of size \p s and sets all 
    /// entries to \p value0
    explicit VectorBase(size_type s, value_type value0)
      : super(Size::eval(s))
    {
      set(value0);
    }
    
    /// Copy constructor
    VectorBase(self const& other)
      : super(other._size)
    {
      std::copy(other._elements, other._elements + _size, _elements);
    }
    
    /// \brief Constructor based on an expression.
    /// Use the assignment operator for expressions to copy values elementwise
    template <Expression Expr>
    VectorBase(Expr const& expr)
      : super(size(expr))
    {
      this->operator=(expr);
    }

    /// constructor using initializer list
    VectorBase(std::initializer_list<value_type> l) 
      : super(l.size())
    {
      std::copy(l.begin(), l.end(), _elements);
    }

    /// destructor
    ~VectorBase() { }    
    
#ifndef _MSC_VER // bug in MSVC <= 2013
    /// copy assignment operator
    self& operator=(self const& other)
    {
      std::copy(other.begin(), other.end(), _elements);
      return *this;
    }
#endif
    
    using super::operator= ;
    using super::operator+= ;
    using super::operator-= ;
    using super::operator*= ;
    using super::operator/= ;
    
    // need non-templated arguments in order to eliminate a friend declaration 
    // warning in gcc
    friend void swap(VectorBase& first, VectorBase& second)
    {
      using std::swap; // enable ADL
      swap(first._size, second._size);
      swap(first._elements, second._elements);
    }
  
  // ----- element access functions  -------------------------------------------
  public:    
    // import operator() from super-class
    using super::operator() ;
    
    /// Access to the i-th vector element.
    inline value_type& operator[](size_type i) { return _elements[i]; }
    
    /// Access to the i-th vector element. (const variant)
    inline const value_type& operator[](size_type i) const { return _elements[i]; }
    
    /// Access to the i-th vector element with index checking.
    inline value_type& at(size_type i) 
    {
      TEST_EXIT_DBG(i < _size)("Index " << i << " out of range [0, " << _size << ")!\n");
      return _elements[i]; 
    }
    
    /// Access to the i-th vector element with index checking. (const variant)
    inline const value_type& at(size_type i) const
    { 
      TEST_EXIT_DBG(i < _size)("Index " << i << " out of range [0, " << _size << ")!\n");
      return _elements[i]; 
    }
  };
  
  
  /// Size of VectorBase
  template <class M, class S>
  size_t size(VectorBase<M,S> const& vec)
  {
    return vec.getSize();
  }
  
  /// number of rows of VectorBase
  template <class M, class S>
  size_t num_rows(VectorBase<M,S> const& vec)
  {
    return vec.getSize();
  }
  
  /// number of columns of VectorBase
  template <class M, class S>
  size_t num_cols(VectorBase<M,S> const& vec)
  {
    return 1;
  }

} // end namespace AMDiS

