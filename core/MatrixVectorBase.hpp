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



/** \file MatrixVectorBase.hpp */

#pragma once

#include <algorithm> // std::copy, std::fill
#include <ostream>   // std::basic_ostream

#include <boost/numeric/mtl/operation/assign_mode.hpp>

#include "Log.h"			// TEST_EXIT_DBG

#include "traits/concepts.hpp"
#include "traits/size.hpp"
#include "traits/base_expr.hpp"

#include "operations/meta.hpp" 
#include "operations/assign.hpp"

namespace AMDiS 
{
  
  /// Base class for matrix and vector types.
  /**
    * This class manages the assignment operators and element access operators.
    */
  template <class Model, concepts::Memory_policy Super>
  struct MatrixVectorBase : public Super
  {
    typedef MatrixVectorBase            self;
    typedef Super                      super;
    typedef Model                      model;
    
    typedef Value_type<super>     value_type;
    typedef Size_type<super>       size_type;
    
    typedef value_type*              pointer;
    typedef value_type const*  const_pointer;
    typedef pointer                 iterator;
    typedef const_pointer     const_iterator;
    
    using super::_elements;
    using super::_size;
    
  // ---------------------------------------------------------------------------
  protected:
    /// default constructor
    explicit MatrixVectorBase(size_type s = 0)
      : super(s)
    { }
    
  public:  
    /// assignment of an expression    
    template <Expression Expr>
    explicit MatrixVectorBase(Expr const& expr)
      : super(size(expr))
    {
      this->operator=(expr);
    }
  
    /// destructor
    ~MatrixVectorBase() { }
    
    /// fill vector with scalar value
    template <class S>
      requires concepts::Convertible<S, value_type>
    void set(S const& value) 
    {
      std::fill(_elements, _elements + _size, value);
    }
    
    /// fill vector with values from pointer. No check of length is performed!
    inline void setValues(value_type* values)
    {
      std::copy(values, values + _size, _elements);
    }

    /// Access to the i-th data element.
    inline value_type& operator()(size_type i) { return _elements[i]; }
    
    /// Access to the i-th data element. (const variant)
    inline const value_type& operator()(size_type i) const { return _elements[i]; }
    
    /// Returns pointer to the first vector element.
    inline iterator begin() { return _elements; }
    
    /// Returns pointer to the first vector element. (const variant)
    inline const_iterator begin() const { return _elements; }

    /// Returns pointer after the last vector element.
    inline iterator end() { return _elements + _size; }
    
    /// Returns pointer after the last vector element. (const variant)
    inline const_iterator end() const { return _elements + _size; }
    
  // ---------------------------------------------------------------------------
  public:
    /// assignment of an expression    
    template <Expression Expr>
    model& operator=(Expr const& expr)
    {
      assign(expr, mtl::assign::assign_sum());
      return static_cast<model&>(*this);
    }
    
    /// compound plus-assignment of an expression   
    template <Expression Expr>
    model& operator+=(Expr const& expr)
    {
      assign(expr, mtl::assign::plus_sum());
      return static_cast<model&>(*this);
    }
    
    /// compound minus-assignment of an expression   
    template <Expression Expr>
    model& operator-=(Expr const& expr)
    {
      assign(expr, mtl::assign::minus_sum());
      return static_cast<model&>(*this);
    }
    
    /// compound times-assignment of an expression   
    template <Expression Expr>
    model& operator*=(Expr const& expr)
    {
      assign(expr, mtl::assign::times_sum());
      return static_cast<model&>(*this);
    }
    
    /// compound divides-assignment of an expression   
    template <Expression Expr>
    model& operator/=(Expr const& expr)
    {
      assign(expr, mtl::assign::divide_sum());
      return static_cast<model&>(*this);
    }
    
  // ---------------------------------------------------------------------------
  public:
    /// Assignment operator for scalars
    template <Arithmetic S>
      requires concepts::Convertible<S, value_type>
    model& operator=(S value) 
    {
      for_each(assign::value<value_type, S>(value));
      return static_cast<model&>(*this);
    }

    /// compound assignment *= of a scalar
    template <Arithmetic S>
      requires concepts::Convertible<S, value_type>
    model& operator*=(S value)
    {
      for_each(assign::mult_value<value_type, S>(value));
      return static_cast<model&>(*this);
    }
    
    /// compound assignment /= of a scalar
    template <Arithmetic S>
      requires concepts::Convertible<S, value_type>
    model& operator/=(S value)
    {
      for_each(assign::div_value<value_type, S>(value));
      return static_cast<model&>(*this);
    }
    
  // ---------------------------------------------------------------------------
  private:
    
    /// basic assignment for compound operators given by the Assigner
    template <class Expr, class Assigner>
    void assign(Expr const& expr, Assigner assigner)
    {
      TEST_EXIT_DBG( _size == size(expr) )("Sizes do not match!\n");
      super::assign_aux(static_cast<Model&>(*this), expr, assigner);
    }
    
    /// basic assignment for compound operators given by the Assigner
    template <class Functor>
    inline void for_each(Functor f)
    {
      super::for_each_aux(f);
    }
  };
  
  
  // ===========================================================================
  
  struct DefaultSizePolicy
  { 
    /// return argument \param s
    static constexpr size_t eval(size_t s)
    {
      return s;
    }
  };
  
  template <size_t S>
  struct StaticSizePolicy
  {
    static constexpr size_t value = S;
    
    /// return static size parameter \p S
    static constexpr size_t eval(size_t)
    {
      return S;
    }
  };
  
  // ===========================================================================
  
  // OUTPUT << MatrixVectorBase
  template <class charT, class traits, class Model, class Memory>
  std::basic_ostream<charT, traits>& 
  operator<<(std::basic_ostream<charT,traits>& out, 
	     const MatrixVectorBase<Model, Memory>& vector)
  {
    typename MatrixVectorBase<Model, Memory>::const_iterator it = vector.begin();
    out << *it; ++it;
    for (; it != vector.end(); ++it)
      out << ' ' << *it;
    return out;
  }

} // end namespace AMDiS
