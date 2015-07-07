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



/** \file category.hpp */

#pragma once

#include "traits/basic.hpp"

namespace AMDiS 
{
  namespace traits 
  {
    // categories
    // _________________________________________________________________________
    /// \cond HIDDEN_SYMBOLS
    template <class T>
    struct category {};
    
    // scalars
    template <Arithmetic T>
    struct category<T>
    {
      typedef tag::scalar tag;
      typedef T           value_type;
      typedef size_t      size_type;
    };
    
    // specialization for pointer types
    template <class T>
    struct category<T*>
    {
      typedef tag::vector tag;
      typedef T           value_type;
      typedef size_t      size_type;
    };
  
    template <class T>
    struct category<const T> : category<T> {};
    /// \endcond
    
  } // end namespace traits

} // end namespace AMDiS
