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



/** \file Matrix_impl.hpp */

#pragma once

#include "MemoryBase.hpp"

// define concrete specializations for VectorBase and MatrixBase, in the case that
// alias templates are not available

#if !HAS_ALIAS_TEMPLATES
namespace AMDiS {
    
  // ----- Matrix types --------------------------------------------------------
  
  /// define a FixMat as a specialized static-matrix
  template <class T, GeoIndex G> 
  struct FixMat 
#if FIXED_SIZE
      : public MatrixBase<MemoryBaseHybrid<T, MaxSize<G>::value, MaxSize<G>::value>, FixedSize<G> >
#else
      : public MatrixBase<MemoryBaseDynamic<T, false>, FixedSize<G> >
#endif
  {
    static constexpr int _ROWS = MaxSize<G>::value;
    static constexpr int _COLS = MaxSize<G>::value;
    static constexpr int _SIZE = _ROWS * _COLS;
    
    typedef FixMat                                   self;
#if FIXED_SIZE
    typedef MemoryBaseHybrid<T, _ROWS, _COLS>  MemoryBase;
#else
    typedef MemoryBaseDynamic<T, false>        MemoryBase;
#endif
    typedef MatrixBase<MemoryBase, FixedSize<G> >   super;
    typedef typename super::size_type           size_type;
    typedef typename super::value_type         value_type;
    
    /// default constructor
    explicit FixMat(size_type r = 0, size_type c = 0) : super(r, c) {}
    /// constructor with initializer
    explicit FixMat(size_type r, size_type c, value_type value0) : super(r, c, value0) {}
    /// copy constructor
    FixMat(self const& other) : super(static_cast<super const&>(other)) {}
    /// constructor based on an expression    
    template <class Expr>
    FixMat(MatrixExpr<Expr> const& expr) : super(expr) {}
    /// destructor
    ~FixMat() { }
    
    using super::operator= ;
  };
    
  /// define a WorldMatrix as a specialized FixMat
  template <class T> 
  struct WorldMatrix
      : public FixMat<T, WORLD>
  {
    typedef WorldMatrix                       self;
    typedef FixMat<T, WORLD>                 super;
    typedef typename super::size_type    size_type;
    typedef typename super::value_type  value_type;
    
    /// default constructor
    explicit WorldMatrix(size_type r = 0, size_type c = 0) : super(r, c) {}
    /// constructor with initializer
    explicit WorldMatrix(size_type r, size_type c, value_type value0) : super(r, c, value0) {}
    /// copy constructor
    WorldMatrix(self const& other) : super(static_cast<super const&>(other)) {}
    /// constructor based on an expression    
    template <class Expr>
    WorldMatrix(MatrixExpr<Expr> const& expr) : super(expr) {}
    /// destructor
    ~WorldMatrix() { }
    
    using super::operator= ;
  };
  
  /// define a DimMat as a specialized FixMat
  template <class T> 
  struct DimMat
      : public FixMat<T, PARTS>
  {
    typedef DimMat                            self;
    typedef FixMat<T, PARTS>                 super;
    typedef typename super::size_type    size_type;
    typedef typename super::value_type  value_type;
    
    /// default constructor
    explicit DimMat(size_type r = 0, size_type c = 0) : super(r, c) {}
    /// constructor with initializer
    explicit DimMat(size_type r, size_type c, value_type value0) : super(r, c, value0) {}
    /// copy constructor
    DimMat(self const& other) : super(static_cast<super const&>(other)) {}
    /// constructor based on an expression    
    template <class Expr>
    DimMat(MatrixExpr<Expr> const& expr) : super(expr) {}
    /// destructor
    ~DimMat() { }
    
    using super::operator= ;
  };
  
  /// define a static-matrix
  template <class T, small_t N, small_t M> 
  struct StaticMatrix
      : public MatrixBase<MemoryBaseStatic<T, N, M> >
  {
    typedef StaticMatrix                       self;
    typedef MemoryBaseStatic<T, N, M>    MemoryBase;
    typedef MatrixBase<MemoryBase>            super;
    typedef typename super::size_type     size_type;
    typedef typename super::value_type   value_type;
    
    /// default constructor
    explicit StaticMatrix(size_type r = 0, size_type c = 0) : super(r, c) {}
    /// constructor with initializer
    explicit StaticMatrix(size_type r, size_type c, value_type value0) : super(r, c, value0) {}
    /// copy constructor
    StaticMatrix(self const& other) : super(static_cast<super const&>(other)) {}
    /// constructor based on an expression    
    template <class Expr>
    StaticMatrix(MatrixExpr<Expr> const& expr) : super(expr) {}
    /// destructor
    ~StaticMatrix() { }
    
    using super::operator= ;
  };
  
  /// define a Matrix as a specialized dynamic-matrix
  template <class T> 
  struct Matrix 
      : public MatrixBase<MemoryBaseDynamic<T, false> >
  {
    typedef Matrix                             self;
    typedef MemoryBaseDynamic<T, false>  MemoryBase;
    typedef MatrixBase<MemoryBase>            super;
    typedef typename super::size_type     size_type;
    typedef typename super::value_type   value_type;
    
    /// default constructor
    explicit Matrix(size_type r = 0, size_type c = 0) : super(r, c) {}
    /// constructor with initializer
    explicit Matrix(size_type r, size_type c, value_type value0) : super(r, c, value0) {}
    /// copy constructor
    Matrix(self const& other) : super(static_cast<super const&>(other)) {}
    /// constructor based on an expression    
    template <class Expr>
    Matrix(MatrixExpr<Expr> const& expr) : super(expr) {}
    /// destructor
    ~Matrix() { }
    
    using super::operator= ;
  };
    
}
#endif
