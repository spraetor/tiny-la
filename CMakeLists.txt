project("tiny-la")
cmake_minimum_required(VERSION 2.8)

unset(LIBRARIES)
unset(DEFINITIONS)

set(SRC_DIR examples)

# find package MTL4
find_package(MTL REQUIRED)
if (MTL_FOUND)
  message ("-- Found MTL version ${MTL_VERSION}: ${MTL_INCLUDE_DIRS}")
  list(APPEND LIBRARIES ${MTL_LIBRARIES})
  list(APPEND DEFINITIONS ${MTL_CXX_DEFINITIONS})
  include_directories(${MTL_INCLUDE_DIRS})
endif (MTL_FOUND)

# find package Boost
set(BOOST_LIBS_REQUIRED date_time)
find_package(Boost 1.48 REQUIRED ${BOOST_LIBS_REQUIRED})
if(Boost_FOUND)
  list(APPEND LIBRARIES ${Boost_LIBRARIES})
  include_directories(${Boost_INCLUDE_DIR})
else()
  message(ERROR "Boost libraries not found")
endif(Boost_FOUND)

# add include path to the core library
include_directories(./core)
add_definitions(${DEFINITIONS})

add_executable("amdis_test" ${SRC_DIR}/amdis_test.cc)
target_link_libraries("amdis_test" ${LIBRARIES})

add_executable("expressions" ${SRC_DIR}/expressions.cc)
target_link_libraries("expressions" ${LIBRARIES})

add_executable("expressions_simple" ${SRC_DIR}/expressions_simple.cc)
target_link_libraries("expressions_simple" ${LIBRARIES})

add_executable("test2" ${SRC_DIR}/test2.cc)
target_link_libraries("test2" ${LIBRARIES})

add_executable("test3" ${SRC_DIR}/test3.cc)
target_link_libraries("test3" ${LIBRARIES})
