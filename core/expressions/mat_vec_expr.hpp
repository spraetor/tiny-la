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


 
/** \file MatVecExpr.hpp */

#pragma once

#include <boost/numeric/linear_algebra/identity.hpp>	// mtl::math::zero

#include "traits/concepts.hpp"
#include "traits/num_rows.hpp"
#include "traits/num_cols.hpp"
#include "traits/base_expr.hpp"
#include "operations/meta.hpp"

#include "Vector.hpp"

namespace AMDiS {  
  
  // if necessary assign vector_expr to buffer-vector, otherwise store an expr.
  
  template <Expression E, int S, bool positive>
  struct BufferTypeAux
  {
    typedef VectorBase<
	MemoryBaseStatic<Value_type<E>, S, 1>, 
	StaticSizePolicy<S> > type;
  };
  
  template <Expression E, int S>
  struct BufferTypeAux<E, S, false>
  {
    typedef VectorBase< MemoryBaseDynamic<Value_type<E>, false> > type;
  };
  
  template <class E, bool use_buffer>
    requires requires() { E::_SIZE; }
  using BufferType = 
      if_then_else< use_buffer, 
		    typename BufferTypeAux<E, E::_SIZE, (E::_SIZE > 0)>::type,
		    E const& >;

      
  /// \brief Expression with two arguments, that multiplies a matrix_expr with a vector_expr
  // TODO: be more flexible, i.e. vec*mat, mat^T*vec, ...
  // TODO: add more optimizations for simple matrix*vector etc.
  template <MatrixExpr M, VectorExpr V, bool use_buffer>
  struct MatVecExpr
  {
    typedef MatVecExpr                           self;
    
    typedef Value_type<V>                  value_type; // TODO: use mult_type
    typedef traits::max_size_type<M, V>     size_type;
	
    typedef M                             matrix_type;
    typedef BufferType<V, use_buffer>     vector_type;
    
    // sizes of the resulting expr.
    static constexpr int _SIZE = M::_ROWS;
    static constexpr int _ROWS = M::_ROWS;
    static constexpr int _COLS = max(V::_COLS, 1);
    
  private:
    static constexpr int ARG_COLS = max(V::_ROWS, M::_COLS);
    
  public:
    /// constructor takes a matrix expression \p mat and a 
    /// vector expression \p vec for the matrix-vector product.
    MatVecExpr(matrix_type const& mat, vector_type const& vec) 
	: matrix(mat), vector(vec)
    { 
      TEST_EXIT_DBG( num_cols(mat) == num_rows(vec) )("Sizes do not match!\n");
    }
    
    /// access the elements of an expr.
    inline value_type operator()(size_type i) const
    {
      return reduce(i, int_<ARG_COLS>());
    }
    
    matrix_type const& get_matrix() const { return matrix; }
    vector_type const& get_vector() const { return vector; }
    
  protected:  
    template <int N> requires (N > 0)
    inline value_type reduce(size_type row, int_<N>) const
    {
      using meta::FOR;
      value_type erg = math::zero(value_type());
      FOR<0,N>::inner_product([row, this](size_type col) { return this->matrix(row, col); }, 
			      vector, erg, functors::dot_functor<value_type, value_type>());
      return erg;
    }
    
    inline value_type reduce(size_type r, int_<-1>) const
    {
      value_type erg = math::zero(value_type());	  
      for (size_type c = 0; c < num_cols(matrix); ++c)
	erg += matrix(r,c) * vector(c);
      return erg;
    }
    
  private:
    matrix_type const&  matrix;
    vector_type vector;
  };
  
  
  /// Size of MatVecExpr
  template <class E1, class E2, bool b>
  size_t size(MatVecExpr<E1,E2,b> const& expr)
  {
    return num_rows(expr.get_matrix());
  }
  
  /// number of rows of MatVecExpr
  template <class E1, class E2, bool b>
  size_t num_rows(MatVecExpr<E1,E2,b> const& expr)
  {
    return num_rows(expr.get_matrix());
  }
  
  /// number of columns of MatVecExpr
  template <class E1, class E2, bool b>
  size_t num_cols(MatVecExpr<E1,E2,b> const& expr)
  {
    return 1;
  }
  
} // end namespace AMDiS
