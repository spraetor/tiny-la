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

 

/** \file MemoryBase.hpp */

#pragma once

#include "Log.h"			// TEST_EXIT_DBG, BOOST_STATIC_ASSERT_MSG
#include "operations/generic_loops.hpp"	// meta::FOR

#include "Config.h"
#include "utility/aligned_alloc.hpp"	// ALIGNED_ALLOC, ALIGNED_FREE, ...

namespace AMDiS {

  /// Memory base for vector types using static storage
  /** The template parameter \p T describes the value-type of the
   *  data elements. The maximal size (capacity) of the container
   *  ist set by the non-type template parameter \p N. In the case
   *  that the container is a matrix, a second dimension can be 
   *  specified by the non-type template parameter \p M. Then the 
   *  total size is N*M.
   **/
  template <class T, small_t N, small_t M = 1>
    requires (N*M > 0)
  struct MemoryBaseStatic
  {
//     STATIC_TEST_EXIT( N*M > 0 , "Container size must be > 0" );
    
    typedef MemoryBaseStatic            self;
    typedef T                     value_type;
    typedef small_t                size_type;
    typedef value_type*              pointer;
    typedef value_type const*  const_pointer;
    
    // static sizes
    static constexpr int _SIZE = N*M;
    static constexpr int _ROWS = N;
    static constexpr int _COLS = M;
    
  protected:
    static constexpr size_type _size = _SIZE;
    static constexpr size_type _capacity = _SIZE; // TODO: eventuell aufrunden
    
    ALIGNED(T, _elements, _capacity);   // T _elements[N];
  
  protected:
    /// default constructor
    explicit MemoryBaseStatic(size_type s = 0) 
    {
      TEST_EXIT_DBG(s == _SIZE)("Size must be equal to capacity!\n");
    }
    
  public:
    /// destructor
    ~MemoryBaseStatic() { }
    
  public:
    /// return the \ref _size of the vector.
    inline size_type getSize() const { return _size; }
    
    /// return the \ref _capacity of the vector.
    static constexpr size_type getCapacity() { return _capacity; }
    
    /// return the amount of memory in Bytes allocated by this vector.
    inline size_type getMemoryUsage() const 
    {
      return _capacity*sizeof(T) + sizeof(size_type);
    }
      
    /// return address of contiguous memory block \ref _elements
    inline pointer data() { return _elements; }
    
    /// return address of contiguous memory block \ref _elements (const version)
    inline const_pointer data() const { return _elements; }
    
    /// resize the vector. Only possible, if \p s <= _capacity
    void resize(size_type s) 
    {
      if (s != _size) {
	// Not supported
	assert( false );
      }
    }
    
  protected:
    
    template <class Target, class Source, class Assigner>
    void assign_aux(Target& target, Source const& src, Assigner assigner)
    {
      using meta::FOR;
      FOR<0,_SIZE>::assign(target, src, assigner);
//       for (size_type i = 0; i < _size; ++i)
// 	Assigner::apply(target(i), src(i));
    }
    
    template <class Functor>
    void for_each_aux(Functor f)
    {
      using meta::FOR;
      FOR<0,_SIZE>::for_each(_elements, f);
//       for (size_type i = 0; i < _size; ++i)
// 	f(_elements[i]);
    }
  };
  
  // ===========================================================================
  
  /// Memory base for vector types using dynamic storage
  /** The template parameter \p T describes the value-type of the
   *  data elements. The memory is allocated on the heap. When
   *  \p aligned is set to true an 16-Byte alignement of the data 
   *  is enforced, in order to use vectorization methods.
   **/
  template <class T, bool aligned>
  struct MemoryBaseDynamic
  {
    typedef MemoryBaseDynamic           self;
    
    typedef T                     value_type;
    typedef small_t               size_type;
    typedef value_type*              pointer;
    typedef value_type const*  const_pointer;
    
    // static sizes (by default -1 := dynamic size)
    static constexpr int _SIZE = -1;
    static constexpr int _ROWS = -1;
    static constexpr int _COLS = -1;
    
  protected:
    size_type  _size;
    size_type  _capacity;
    T*         _elements;
  
  protected:
    /// default constructor
    explicit MemoryBaseDynamic(size_type s = 0)
      : _size(s),
	_capacity(s),
	_elements(_size ? (aligned ? ALIGNED_ALLOC(T, s) : new T[s]) : NULL)
    { }
    
  public:
    /// destructor
    ~MemoryBaseDynamic()
    {
      if (_elements) {
	if (aligned) { ALIGNED_FREE(_elements); } 
	else { delete [] _elements; }
	_elements = NULL;
      }
    }
    
  public:
    /// return the \ref _size of the vector.
    inline size_type getSize() const { return _size; }
    
    /// return the \ref _capacity of the vector.
    inline size_type getCapacity() const { return _capacity; }
    
    /// return the amount of memory in Bytes allocated by this vector.
    inline size_type getMemoryUsage() const 
    {
      return (aligned ? ALIGNED_SIZE(_capacity*sizeof(T), CACHE_LINE) 
		      : _capacity*sizeof(T))
	    + 2*sizeof(size_type);
    }
      
    /// return address of contiguous memory block \ref _elements
    inline pointer data() { return _elements; }
    
    /// return address of contiguous memory block \ref _elements (const version)
    inline const_pointer data() const { return _elements; }
    
    /// resize the vector. If \p s <= \ref _capacity simply set the \ref _size
    /// attribute the s, otherwise realloc memory
    void resize(size_type s) 
    {
      if (s <= _capacity) {
	_size = s;
	// TODO: (maybe) set unused entries to NULL
      } else {
	if (_elements) {
	  if (aligned) { ALIGNED_FREE(_elements); } 
	  else { delete [] _elements; }
	}
	_elements = aligned ? ALIGNED_ALLOC(T, s) : new T[s];
	_size = s;
      }
    }
    
  protected:
    template <class Target, class Source, class Assigner>
    void assign_aux(Target& target, Source const& src, Assigner assigner)
    {
      assign_aux(target, src, assigner, bool_<aligned>());
    }
    
    template <class Target, class Source, class Assigner> // not assume aligned
    void assign_aux(Target& target, Source const& src, Assigner assigner, false_)
    {
      for (size_type i = 0; i < _size; ++i)
	Assigner::apply(target(i), src(i));
    }
    
    template <class Target, class Source, class Assigner> // assume aligned
    void assign_aux(Target& target, Source const& src, Assigner assigner, true_)
    {
      value_type* var = (value_type*)ASSUME_ALIGNED(_elements);
      for (size_type i = 0; i < _size; ++i)
	Assigner::apply(var[i], src(i));
    }
    
    template <class Functor>
    void for_each_aux(Functor f)
    {
      for (size_type i = 0; i < _size; ++i)
	f(_elements[i]);
    }
  };
  
  // ===========================================================================
  
  /// Memory base for vector types using static storage, with dynamic size
  /** The template parameter \p T describes the value-type of the
   *  data elements. The maximal size (capacity) of the container
   *  ist set by the non-type template parameter \p N. In the case
   *  that the container is a matrix, a second dimension can be 
   *  specified by the non-type template parameter \p M. Then the 
   *  total size allocated is N*M. The internal size used is set in the 
   *  constructor or the \ref resize function.
   **/
  template <class T, small_t N, small_t M=1>
    requires (N*M > 0)
  struct MemoryBaseHybrid
  {    
    typedef MemoryBaseHybrid            self;
    typedef T                     value_type;
    typedef small_t                size_type;
    typedef value_type*              pointer;
    typedef value_type const*  const_pointer;
    
    // static sizes
    static constexpr int _SIZE = -1;
    static constexpr int _ROWS = -1;
    static constexpr int _COLS = -1;
    
  protected:
    size_type _size;
    static constexpr size_type _capacity = N*M;
    
    ALIGNED(T, _elements, _capacity);   // T _elements[N];
  
  protected:
    /// default constructor
    explicit MemoryBaseHybrid(size_type s = 0) 
      : _size(s)
    {
      TEST_EXIT_DBG(s <= _capacity)("Size must be <= capacity!\n");
    }
    
  public:
    /// destructor
    ~MemoryBaseHybrid() { }
    
  public:
    /// return the \ref _size of the vector.
    inline size_type getSize() const { return _size; }
    
    /// return the \ref _capacity of the vector.
    inline size_type getCapacity() const { return _capacity; }
    
    /// return the amount of memory in Bytes allocated by this vector.
    inline size_type getMemoryUsage() const 
    {
      return _capacity*sizeof(T) + sizeof(size_type);
    }
      
    /// return address of contiguous memory block \ref _elements
    inline pointer data() { return _elements; }
    
    /// return address of contiguous memory block \ref _elements (const version)
    inline const_pointer data() const { return _elements; }
    
    /// resize the vector. If \p s <= \ref _capacity simply set the \ref _size
    /// attribute the s, otherwise realloc memory
    void resize(size_type s) 
    {
      if (s <= _capacity) {
	_size = s;
      } else {
	// not supported
	assert( false );
      }
    }
    
  protected:
    template <class Target, class Source, class Assigner> 
    void assign_aux(Target& target, Source const& src, Assigner assigner)
    {
      for (size_type i = 0; i < _size; ++i)
	Assigner::apply(target(i), src(i));
    }
    
    template <class Functor>
    void for_each_aux(Functor f)
    {
      for (size_type i = 0; i < _size; ++i)
	f(_elements[i]);
    }
  };

} // end namespace AMDiS

