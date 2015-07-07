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

 

/** \file all_expr_fwd.hpp */

#pragma once

#include "traits/basic.hpp"
#include "traits/size.hpp"
#include "traits/num_rows.hpp"
#include "traits/num_cols.hpp"

namespace AMDiS {

  template <class E, class F> struct ElementwiseUnaryExpr;
  template <class E1, class E2, class F> struct ElementwiseBinaryExpr;
  template <class V> struct ScalarExpr;
  template <class V, class E, bool l, class F> struct ScaleExpr;
  template <class E1, class E2, class F> struct VectorBinaryExpr;
  template <class E, class F> struct ReductionUnaryExpr;
  template <class E1, class E2, class F> struct ReductionBinaryExpr;
  template <class E1, class E2, bool b> struct MatVecExpr;

  // forward declaration of size() functions
  template <class M, class F> size_t size(ElementwiseUnaryExpr<M,F> const&);
  template <class E1, class E2, class F> size_t size(ElementwiseBinaryExpr<E1,E2,F> const&);
  template <class V> size_t size(ScalarExpr<V> const&);
  template <class V, class E, bool l, class F> size_t size(ScaleExpr<V,E,l,F> const&);
  template <class E1, class E2, class F> size_t size(VectorBinaryExpr<E1,E2,F> const&);
  template <class E, class F> size_t size(ReductionUnaryExpr<E,F> const&);
  template <class E1, class E2, class F> size_t size(ReductionBinaryExpr<E1,E2,F> const&);
  template <class E1, class E2, bool b> size_t size(MatVecExpr<E1,E2,b> const&);
  
  // forward declaration of num_rows() functions
  template <class M, class F> size_t num_rows(ElementwiseUnaryExpr<M,F> const&);
  template <class E1, class E2, class F> size_t num_rows(ElementwiseBinaryExpr<E1,E2,F> const&);
  template <class V> size_t num_rows(ScalarExpr<V> const&);
  template <class V, class E, bool l, class F> size_t num_rows(ScaleExpr<V,E,l,F> const&);
  template <class E1, class E2, class F> size_t num_rows(VectorBinaryExpr<E1,E2,F> const&);
  template <class E, class F> size_t num_rows(ReductionUnaryExpr<E,F> const&);
  template <class E1, class E2, class F> size_t num_rows(ReductionBinaryExpr<E1,E2,F> const&);
  template <class E1, class E2, bool b> size_t num_rows(MatVecExpr<E1,E2,b> const&);
  
  // forward declaration of num_cols() functions
  template <class M, class F> size_t num_cols(ElementwiseUnaryExpr<M,F> const&);
  template <class E1, class E2, class F> size_t num_cols(ElementwiseBinaryExpr<E1,E2,F> const&);
  template <class V> size_t num_cols(ScalarExpr<V> const&);
  template <class V, class E, bool l, class F> size_t num_cols(ScaleExpr<V,E,l,F> const&);
  template <class E1, class E2, class F> size_t num_cols(VectorBinaryExpr<E1,E2,F> const&);
  template <class E, class F> size_t num_cols(ReductionUnaryExpr<E,F> const&);
  template <class E1, class E2, class F> size_t num_cols(ReductionBinaryExpr<E1,E2,F> const&);
  template <class E1, class E2, bool b> size_t num_cols(MatVecExpr<E1,E2,b> const&);
  
} // end namespace AMDiS

