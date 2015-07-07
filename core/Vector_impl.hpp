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



/** \file Vector_impl.hpp */

#pragma once

#include "MemoryBase.hpp"

// define concrete specializations for VectorBase and MatrixBase, in the case that
// alias templates are not available

#if !HAS_ALIAS_TEMPLATES
namespace AMDiS {
    
  // ----- Vector types --------------------------------------------------------
  
  /// define a FixVec as a specialized static-vector
  template <class T, GeoIndex G> 
  struct FixVec 
#if FIXED_SIZE
      : public VectorBase<MemoryBaseHybrid<T, MaxSize<G>::value, 1>, FixedSize<G> >
#else
      : public VectorBase<MemoryBaseDynamic<T, false>, FixedSize<G> >
#endif
  {
    static constexpr int _SIZE = MaxSize<G>::value;
    static constexpr int _ROWS = MaxSize<G>::value;
    static constexpr int _COLS = 1;
    
    typedef FixVec                                  self;
#if FIXED_SIZE
    typedef MemoryBaseHybrid<T, _SIZE, 1>     MemoryBase;
#else
    typedef MemoryBaseDynamic<T, false>       MemoryBase;
#endif
    typedef VectorBase<MemoryBase, FixedSize<G> >  super;
    typedef typename super::size_type          size_type;
    typedef typename super::value_type        value_type;
    
    /// default constructor
    explicit FixVec(size_type s = 0) : super(s) { }
    /// constructor with initializer
    explicit FixVec(size_type s, value_type value0) : super(s, value0) {}
    /// copy constructor
    FixVec(self const& other) : super(static_cast<super const&>(other)) {}
    /// assignment of an expression    
    template <class Expr> FixVec(VectorExpr<Expr> const& expr) : super(expr) {}
    /// destructor
    ~FixVec() { }
    
    using super::operator= ;
  };
    
  /// define a WorldVector as a specialized FixVec
  template <class T> 
  struct WorldVector
      : public FixVec<T, WORLD>
  {
    typedef WorldVector                       self;
    typedef FixVec<T, WORLD>                 super;
    typedef typename super::size_type    size_type;
    typedef typename super::value_type  value_type;
    
    /// default constructor
    explicit WorldVector(size_type s = 0) : super(s) { }
    /// constructor with initializer
    explicit WorldVector(size_type s, value_type value0) : super(s, value0) {}
    /// copy constructor
    WorldVector(self const& other) : super(static_cast<super const&>(other)) {}
    /// assignment of an expression    
    template <class Expr> WorldVector(VectorExpr<Expr> const& expr) : super(expr) {}
    /// destructor
    ~WorldVector() { }
    
    using super::operator= ;
  };
  
  /// define a DimVec as a specialized FixVec
  template <class T> 
  struct DimVec
      : public FixVec<T, PARTS>
  {
    typedef DimVec                            self;
    typedef FixVec<T, PARTS>                 super;
    typedef typename super::size_type    size_type;
    typedef typename super::value_type  value_type;
    
    /// default constructor
    explicit DimVec(size_type s = 0) : super(s) { }
    /// constructor with initializer
    explicit DimVec(size_type s, value_type value0) : super(s, value0) {}
    /// copy constructor
    DimVec(self const& other) : super(static_cast<super const&>(other)) {}
    /// assignment of an expression    
    template <class Expr> DimVec(VectorExpr<Expr> const& expr) : super(expr) {}
    /// destructor
    ~DimVec() { }
    
    using super::operator= ;
  };
    
  /// define a static-vector
  template <class T, small_t N> 
  struct StaticVector
       : public VectorBase<MemoryBaseStatic<T, N, 1>, StaticSizePolicy<N> >
  {
    typedef StaticVector                                   self;
    typedef MemoryBaseStatic<T, N, 1>                MemoryBase;
    typedef VectorBase<MemoryBase, StaticSizePolicy<N> >  super;
    typedef typename super::size_type                 size_type;
    typedef typename super::value_type               value_type;
    
    /// default constructor
    explicit StaticVector(size_type s = 0) : super(s) { }
    /// constructor with initializer
    explicit StaticVector(size_type s, value_type value0) : super(s, value0) {}
    /// copy constructor
    StaticVector(self const& other) : super(static_cast<super const&>(other)) {}
    /// assignment of an expression    
    template <class Expr> StaticVector(VectorExpr<Expr> const& expr) : super(expr) {}
    /// destructor
    ~StaticVector() { }
    
    using super::operator= ;
  };
       
  /// define a Vector as a specialized dynamic-vector
  template <class T> 
  struct Vector
      : public VectorBase<MemoryBaseDynamic<T, false> >
  {
    typedef Vector                             self;
    typedef MemoryBaseDynamic<T, false>  MemoryBase;
    typedef VectorBase<MemoryBase >           super;
    typedef typename super::size_type     size_type;
    typedef typename super::value_type   value_type;
    
    /// default constructor
    explicit Vector(size_type s = 0) : super(s) { }
    /// constructor with initializer
    explicit Vector(size_type s, value_type value0) : super(s, value0) {}
    /// copy constructor
    Vector(self const& other) : super(static_cast<super const&>(other)) {}
    /// assignment of an expression    
    template <class Expr> Vector(VectorExpr<Expr> const& expr) : super(expr) {}
    /// destructor
    ~Vector() { }
    
    using super::operator= ;
  };
    
}
#endif // !HAS_ALIAS_TEMPLATES
