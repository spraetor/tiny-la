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



/** \file Forward.h */

#pragma once

#include "Config.h"
#include "Forward.h"
#include "Log.h"
#include <boost/mpl/int.hpp>	// int_

namespace AMDiS {


  /// \brief internal used indices to represent the different geometrical objects.
  /// Used as parameter for getGeo() and as template parameter for FixVec. 
  enum GeoIndex
  {
    CENTER   = 0, /**< in 1d the center is at the edge, in 2d at the face, in 3d 
		    * at the interior of an element. So a Line has no edge but
		    * only a center, a Triangle has no face but only a center.
		    */
    VERTEX   = 1, /**< index for element vertices.
		    * number of vertices is equal to number of parts and 
		    * neighbours.
		    */
    EDGE     = 2, /**< index for element edges */
    FACE     = 3, /**< index for element faces */
    DIMEN    =-1, /**< index for problem dimension */
    PARTS    =-2, /**< index for parts of an element (vertices in 1d, edges in 2d
		    * , faces in 3d). Number of element parts is equal to number
		    * of vertices and neighbours. 
		    */
    NEIGH    =-3, /**< index for neighbours of an element.
		    * Number of element neighbours is equal to number of 
		    * vertices and parts.
		    */
    WORLD    =-4, /**< index for world dimension */
    BOUNDARY =-5, /**< index for boundary nodes of an element. This could be
		    * vertices, edges or faces.
		    */
    PROJECTION=-6, /**< index for element and boundary projections */
    
    NO_INDEX =-127
  };

  /// \cond HIDDEN_SYMBOLS
  // dummy-type for partial template specialization
  template <GeoIndex> struct _geo {};
  /// \endcond
  
  namespace Global {
  
    // TODO: remove this dummy variable
    static constexpr small_t dimOfWorld = 2;
    static constexpr GeoIndex MINPART = CENTER;
    static constexpr GeoIndex MAXPART = PROJECTION;
 
    inline small_t getGeo(GeoIndex p, small_t dim)
    {
      TEST_EXIT_DBG(p >= MINPART && p <= MAXPART)
	("Calling for invalid geometry value " << p << "\n");
      TEST_EXIT_DBG(dim < 4)("invalid dim: " << dim << "\n");
      TEST_EXIT_DBG((dim != 0) || (p == PARTS || p == VERTEX || p == EDGE || p == FACE))
	("dim = 0\n");
      
      // TODO: use full implementation of getGeo()
      return dim + 1;
    }
     
    inline small_t getGeo(GeoIndex p) 
    {
      TEST_EXIT_DBG(p == WORLD)("Illegal request for geometry data: part = " << p << "!\n");
#ifdef DOW
	return DOW;
#else
        return dimOfWorld;
#endif
    }
  }
  
  template <GeoIndex G>
  struct FixedSize
  { 
    /// return argument \param s
    static constexpr size_t eval(size_t s)
    {
      return aux(_geo<G>(), s);
    }

  private:
    /// calculate size for DimVector
    template <GeoIndex H>
    static constexpr size_t aux(_geo<H>, size_t dim)
    {
      return Global::getGeo(H, dim);
    }
    
    /// calculate size for WorldVector
    static constexpr size_t aux(_geo<WORLD>, size_t = 0)
    {
      return Global::getGeo(WORLD);
    }
  };
  
  
  // ----- Upper bounds for fixed-size containers ------------------------------
  
  /// \cond HIDDEN_SYMBOLS
  template <int P> struct MaxSize    : boost::mpl::int_<-1> {};
  template <> struct MaxSize<CENTER> : boost::mpl::int_< 1> {};
  
  #ifdef DOW
  template <> struct MaxSize<WORLD>  : boost::mpl::int_<DOW> {};
  #else
  // upper bound
  template <> struct MaxSize<WORLD>  : boost::mpl::int_< 3> {};
  #endif
    
  #ifdef DIM
  template <> struct MaxSize<DIMEN>  : boost::mpl::int_<DIM> {};
  template <> struct MaxSize<VERTEX> : boost::mpl::int_<DIM+1> {};
  template <> struct MaxSize<PARTS>  : boost::mpl::int_<DIM+1> {};
  template <> struct MaxSize<NEIGH>  : boost::mpl::int_<DIM+1> {};
  template <> struct MaxSize<EDGE>   : boost::mpl::int_<(DIM==1?1:(DIM==2?3:6))> {};
  template <> struct MaxSize<FACE>   : boost::mpl::int_<(DIM==1?0:(DIM==2?1:4))> {};
  #else
  // upper bound
  template <> struct MaxSize<DIMEN>  : boost::mpl::int_< 3> {};
  template <> struct MaxSize<VERTEX> : boost::mpl::int_< 4> {};
  template <> struct MaxSize<PARTS>  : boost::mpl::int_< 4> {};
  template <> struct MaxSize<NEIGH>  : boost::mpl::int_< 4> {};
  template <> struct MaxSize<EDGE>   : boost::mpl::int_< 6> {};
  template <> struct MaxSize<FACE>   : boost::mpl::int_< 4> {};
  #endif
  /// \endcond
  
#if HAS_ALIAS_TEMPLATES
  // ----- Vector types --------------------------------------------------------
  
  /// define a FixVec as a specialized static-vector
  #if FIXED_SIZE
  template <class T, GeoIndex G> using FixVec 
    = VectorBase<MemoryBaseHybrid<T, MaxSize<G>::value, 1>, FixedSize<G> >;
  #else
  template <class T, GeoIndex G> using FixVec 
    = VectorBase<MemoryBaseDynamic<T, false>, FixedSize<G>>;
  #endif
  
  /// define a WorldVector as a specialized FixVec
  template <class T> using WorldVector = FixVec<T, WORLD>;
  
  /// define a DimVec as a specialized FixVec
  template <class T> using DimVec = FixVec<T, PARTS>;
  
  template <class T, small_t N> 
  using StaticVector = VectorBase<MemoryBaseStatic<T, N, 1>, StaticSizePolicy<N> >;
  
  /// define a Vector as a specialized dynamic-vector
  template <class T> using Vector 
    = VectorBase<MemoryBaseDynamic<T, false>, DefaultSizePolicy >;
  
  // ----- Matrix types --------------------------------------------------------
  
  /// define a FixMat as a specialized static-matrix
  #if FIXED_SIZE
  template <class T, GeoIndex G> using FixMat 
    = MatrixBase<MemoryBaseHybrid<T, MaxSize<G>::value, MaxSize<G>::value>, FixedSize<G> >;
  #else
  template <class T, GeoIndex G> using FixMat 
    = MatrixBase<MemoryBaseDynamic<T, false>, FixedSize<G> >;
  #endif
    
  /// define a WorldMatrix as a specialized FixMat
  template <class T> using WorldMatrix = FixMat<T, WORLD>;
  
  /// define a DimMat as a specialized FixMat
  template <class T> using DimMat = FixMat<T, PARTS>;
  
  template <class T, small_t N, small_t M> 
  using StaticMatrix = MatrixBase<MemoryBaseStatic<T, N, M>, DefaultSizePolicy >;
  
  /// define a Matrix as a specialized dynamic-matrix
  template <class T> using Matrix 
    = MatrixBase<MemoryBaseDynamic<T, false>, DefaultSizePolicy >;
    
#else 
  // Instead of alias template add forward declarations here and 
  // definitions in Matrix_impl.hpp and Vector_impl.hpp
    
  // ----- Vector types --------------------------------------------------------
  template <class T, GeoIndex G> struct FixVec; 
  template <class T> struct WorldVector;
  template <class T> struct DimVec;
  template <class T, small_t N> struct StaticVector;
  template <class T> struct Vector;
  
  // ----- Matrix types --------------------------------------------------------
  template <class T, GeoIndex G> struct FixMat;
  template <class T> struct WorldMatrix;
  template <class T> struct DimMat;
  template <class T, small_t N, small_t M> struct StaticMatrix;
  template <class T> struct Matrix;
#endif
    
} // end namespace AMDiS
