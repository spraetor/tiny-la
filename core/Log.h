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



/** \file Log.h */

#pragma once

#include <iostream>			// std::cout
#include <sstream>			// std::stringstream
#include <cassert>			// assert
#include <boost/static_assert.hpp>	// BOOST_STATIC_ASSERT[_MSG]

#ifndef NO_THROW_ERROR
#include <stdexcept>			// std::runtime_error
#endif

// ===========================================================================
// ===== message macros ======================================================
// ===========================================================================

/// Should be the first call in every functions. It defines the current 
/// function name nn for message output via MSG, WARNING, ...
#define FUNCNAME(nn) const char *funcName; funcName = nn;
  
#ifdef NDEBUG
  #define FUNCNAME_DBG(nn)
#else
  #define FUNCNAME_DBG(nn) const char *funcName; funcName = nn;
#endif

/// prints a message
#define MSG(text) { std::cout<<text; }

#ifdef NO_THROW_ERROR
  #define ERROR_EXIT(text) { MSG("ERROR: " << text); assert( false ); }
#else
  #define ERROR_EXIT(text) { std::stringstream ss; ss << "ERROR: " << text; \
			     throw std::runtime_error(ss.str()); }
#endif

/// if test is false, an error message is printed and the program exits
#define TEST_EXIT(test) if (!(test)) ERROR_EXIT

#define STATIC_TEST_EXIT BOOST_STATIC_ASSERT_MSG

/// In debug mode, it corresponds to TEST_EXIT
#ifdef NDEBUG
  #define MSG_DBG(text)
  #define TEST_EXIT_DBG(test) MSG_DBG
  #define DBG_VAR(var)
#else
  #define MSG_DBG MSG
  #define TEST_EXIT_DBG(test) if (!(test)) ERROR_EXIT
  #define DBG_VAR(var) var
#endif

/// prints a message, if min(Msg::msgInfo, info) >= noinfo
#define INFO(info,noinfo) if (info >= noino) MSG

/// If the value of Msg::wait is not zero the macro will produce the message 
/// 'wait for <enter> ...' and will continue after pressing the return or enter
/// key. Otherwise the program continues without a message.
#define WAIT { std::string line; MSG("wait for <enter> ..."); std::cin >> line; }

/// produces the message 'wait for <enter> ...' and will continue after
/// pressing the return or enter key.
#define WAIT_REALLY { std::string line; MSG("wait for <enter> ..."); std::cin >> line; }
