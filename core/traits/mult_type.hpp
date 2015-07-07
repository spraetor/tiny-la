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



/** \file mult_type.hpp */

#pragma once

#include <utility>

#include "traits/basic.hpp"

namespace AMDiS 
{
  namespace traits
  {
    /// dummy type
    struct no_valid_type {};
    
    // multiplication types
    // _________________________________________________________________________
    
    template <class T1, class T2>
    struct mult_type_aux
    {
      typedef no_valid_type type;
    };
    
    /// determines the type of the product T1*T2
    template <class T1, class T2>
    using mult_type = typename mult_type_aux<T1, T2>::type;
    
    /// \cond HIDDEN_SYMBOLS
    // Scalar*Scalar => Scalar
    template <Arithmetic T1, Arithmetic T2>
    struct mult_type_aux<T1, T2>
    {
      typedef decltype( std::declval<T1>() * std::declval<T2>() ) type;
    };
    
    // Vec*Vec => Scalar (dot-product)
    template <VectorExpr T1, VectorExpr T2>
    struct mult_type_aux<T1, T2>
    {
      typedef mult_type<Value_type<T1>, Value_type<T2>> type;
    };
    
    // Mat*Mat => Mat
    template <MatrixExpr T>
    struct mult_type_aux<T, T>
    {
      typedef T type;
    };
    
    
    // Vec*Scalar => Vector
    template <template<class> class Vec_t, class T1, Arithmetic T2>
      requires VectorExpr<Vec_t<T1>>
    struct mult_type_aux<Vec_t<T1>, T2>
    {
      typedef Vec_t<mult_type<T1, T2> > type;
    };
    
    // Scalar*Vector => Vector
    template <template<class> class Vec_t, Arithmetic T1, class T2>
      requires VectorExpr<Vec_t<T2>>
    struct mult_type_aux<T1, Vec_t<T2>>
    {
      typedef Vec_t<mult_type<T1, T2> > type;
    };
    
    
    // Matrix*Scalar => Matrix
    template <template<class> class Mat_t, class T1, Arithmetic T2>
      requires MatrixExpr<Mat_t<T1>>
    struct mult_type_aux<Mat_t<T1>, T2>
    {
      typedef Mat_t<mult_type<T1, T2>> type;
    };
    
    // Scalar*Matrix => Matrix
    template <template<class> class Mat_t, Arithmetic T1, class T2>
      requires MatrixExpr<Mat_t<T1>>
    struct mult_type_aux<T1, Mat_t<T2> >
    {
      typedef Mat_t<mult_type<T1, T2>> type;
    };
    /// \endcond
    
    
    // addition types
    // _________________________________________________________________________
    
    template <class T1, class T2>
    struct add_type_aux
    {
      typedef no_valid_type type;
    };
    
    /// determines the type of the sum T1+T2
    template <class T1, class T2>
    using add_type = add_type_aux<T1, T2>;
    
    
    /// \cond HIDDEN_SYMBOLS
    // Scalar+Scalar => Scalar
    template <Arithmetic T1, Arithmetic T2>
    struct add_type_aux<T1, T2>
    {
      typedef decltype( std::declval<T1>() + std::declval<T2>() ) type;
    };
    
    // Vec+Vec => Vec
    template <template<class> class Vec_t, class T1, class T2>
      requires VectorExpr<Vec_t<T1>> && VectorExpr<Vec_t<T2>>
    struct add_type_aux<Vec_t<T1>, Vec_t<T2> >
    {
      typedef Vec_t<add_type<T1, T2>> type;
    };
    
    // Mat+Mat => Mat
    template <template<class> class Mat_t, class T1, class T2>
      requires MatrixExpr<Mat_t<T1>> && MatrixExpr<Mat_t<T2>>
    struct add_type_aux<Mat_t<T1>, Mat_t<T2>>
    {
      typedef Mat_t<add_type<T1, T2>> type;
    };
    
   
//     // Vec+Scalar => Vector
//     template <template<class> class Vec_t, class T1, ScalarExpr T2>
//       requires VectorExpr<Vec_t<T1>>
//     struct add_type_aux<Vec_t<T1>, T2>
//     {
//       typedef Vec_t<add_type<T1, T2>> type;
//     };    
//     
//     // Mat+Scalar => Mat
//     template <template<class> class Mat_t, class T1, ScalarExpr T2>
//       requires MatrixExpr<Mat_t<T1>>
//     struct add_type_aux<Mat_t<T1>, T2>
//     {
//       typedef Mat_t<add_type<T1, T2>> type;
//     };
    /// \endcond
      
  } // end namespace traits

} // end namespace AMDiS

