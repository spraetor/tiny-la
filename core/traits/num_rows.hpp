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



/** \file num_rows.hpp */

#pragma once

#include "traits/concepts.hpp"

#include <boost/numeric/mtl/operation/num_rows.hpp>

namespace AMDiS 
{
  /// \cond HIDDEN_SYMBOLS
  namespace traits 
  {
  
    template <class T>
    struct num_rows 
      : ::mtl::traits::num_rows<T> {}; // import mtl-operation by default
      
    template <class T>
      requires ScalarExpr<T> || Arithmetic<T>
    struct num_rows<T>
    {
	typedef size_t type;
	type operator()(const T& v) const { return 1; }
    };
    
  } // end namespace traits
  /// \endcond
  
  /// Return the number of rows of a container \p t
  template <class T>
  inline auto num_rows(const T& t)
  {
      return traits::num_rows<T>()(t);
  }
    
} // end namespace AMDiS
