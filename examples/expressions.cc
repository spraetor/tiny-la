#include <iostream>
#include <cstdlib>
#include <vector>
#include <cassert>
#include <boost/numeric/conversion/cast.hpp> 

#define DOW  3

#include "AMDiS.h"

using namespace AMDiS;

// test constructors
template <class T>
void test1(size_t s)
{
   
  // size = DOW
#if HAS_INITIALIZER_LISTS
  Vector<T> vec{1,1,1};
#else
  Vector<T> vec(3, T(1));
#endif
  WorldVector<T> worldVec(DOW);
#if HAS_INITIALIZER_LISTS
  StaticVector<T, DOW> staticVec{ 1, 1, 1 };
#else
  StaticVector<T, DOW> staticVec(3, T(1));
#endif
  
  assert( size(vec) == size(worldVec) &&
	  size(vec) == size(staticVec) );
  
  // size = DOW+1
  FixVec<T, PARTS> fixVec(DOW);
  DimVec<T> dimVec(DOW);
  
  assert( size(fixVec) == size(dimVec) );
  
  // size = DOWxDOW
  Matrix<T> mat(DOW, DOW);
  WorldMatrix<T> worldMat(DOW, DOW);
  StaticMatrix<T,DOW,DOW> staticMat(DOW, DOW);
  
  assert( size(mat) == size(worldMat) &&
	  size(mat) == size(staticMat) );
  assert( num_rows(mat) == num_rows(worldMat) &&
	  num_rows(mat) == num_rows(staticMat) );
  assert( num_cols(mat) == num_cols(worldMat) &&
	  num_cols(mat) == num_cols(staticMat) );
  
  // size = (DOW+1)x(DOW+1)
  FixMat<T, PARTS> fixMat(DOW, DOW);
  DimMat<T> dimMat(DOW, DOW);
  
  assert( size(fixMat) == size(dimMat) );
  assert( num_rows(fixMat) == num_rows(dimMat) );
  assert( num_cols(fixMat) == num_cols(dimMat) );
  
}

// test expressions
template <class T>
void test2(size_t s)
{
  Vector<T> vec(DOW);
  StaticVector<T,DOW> staticVec(DOW);
  
  // size = DOWxDOW
  Matrix<T> mat(DOW, DOW);
  StaticMatrix<T,DOW,DOW> staticMat(DOW, DOW);
  
  T scalar = 1;
  
  // assignment of scalar to vector
  vec = scalar;
  staticVec = scalar;
  
  TEST_EXIT(sum(vec) == DOW)("[0] = " << sum(vec) << "\n");
  TEST_EXIT(sum(staticVec) == DOW)("[1] = " << sum(staticVec) << "\n");
  
  // assignment of scalar to matrix
  mat = scalar; 
  staticMat = scalar;
  
  TEST_EXIT(sum(mat) == DOW*DOW)("[2] = " << sum(mat) << "\n");
  TEST_EXIT(sum(staticMat) == DOW*DOW)("[3] = " << sum(staticMat) << "\n");
  
  // assignment of vectors to vectors
  staticVec = vec;
  vec = staticVec;
  
  TEST_EXIT(sum(vec) == DOW)("[4] = " << sum(vec) << "\n");
  TEST_EXIT(sum(staticVec) == DOW)("[5] = " << sum(staticVec) << "\n");
  
  // assignment of matrices to matrices
  staticMat = mat;
  mat = staticMat;
  
  TEST_EXIT(sum(mat) == DOW*DOW)("[6] = " << sum(mat) << "\n");
  TEST_EXIT(sum(staticMat) == DOW*DOW)("[7] = " << sum(staticMat) << "\n");
  
  // norms of vectors
  TEST_EXIT(one_norm(vec) == DOW)("[8] = " << one_norm(vec) << "\n");
  TEST_EXIT(one_norm(staticVec) == DOW)("[9] = " << one_norm(staticVec) << "\n");
  
  TEST_EXIT(two_norm(vec) == std::sqrt(double(DOW)))("[10] = " << two_norm(vec) << "\n");
  TEST_EXIT(two_norm(staticVec) == std::sqrt(double(DOW)))("[11] = " << two_norm(staticVec) << "\n");
  
  TEST_EXIT(unary_dot(vec) == DOW)("[12] = " << unary_dot(vec) << "\n");
  TEST_EXIT(unary_dot(staticVec) == DOW)("[13] = " << unary_dot(staticVec) << "\n");
  
  // scalar-products
  std::cout << "7) dot = " << (scalar = dot(vec, vec)) << "\n";
  std::cout << "8) dot = " << (scalar = dot(vec, staticVec)) << "\n";
  std::cout << "9) dot = " << (scalar = dot(staticVec, staticVec)) << "\n";
  
  dot((vec*scalar), (staticVec*scalar));
  
  scalar = vec * vec;
  scalar = dot(vec, staticVec);
  scalar = dot(staticVec, staticVec);
  
  // matrix-vector product
  vec = mat * vec;
  std::cout << "10) mat*vec = " << max(vec) << "\n";
  staticVec = staticMat * staticVec;
  std::cout << "11) mat*vec = " << max(staticVec) << "\n";
  
  vec = staticMat * vec;
  vec = staticMat * staticVec;
  vec = mat * staticVec;
  
  staticVec = staticMat * vec;
  staticVec = staticMat * staticVec;
  staticVec = mat * staticVec;
  
  // cross-product
  vec = cross(vec, vec);
  std::cout << "12) cross = " << max(vec) << "\n";
  vec = cross(vec, staticVec);
  staticVec = cross(vec, staticVec);

  // elementwise operations
  vec = vec + staticVec;
  std::cout << "13) " << vec << ", [" << max(vec) << "]\n";
  mat = mat + staticMat;
  std::cout << "14) " << mat << ", [" <<  max(mat) << "]\n";
  
  vec+= staticVec;
  mat+= staticMat;
  
  vec = vec - staticVec;
  mat = mat - staticMat;
  
  vec-= staticVec;
  mat-= staticMat;
  
  // mixed operations
  vec = mat*vec + staticVec * scalar - staticMat * (staticVec - scalar * vec);
  scalar = dot(staticMat * vec, scalar * (staticVec + vec));
  
  std::cout << "15) " << one_norm(vec) << "\n";
  std::cout << "16) " << two_norm(vec) << "\n";
  std::cout << "17) " << inf_norm(vec) << "\n";
  std::cout << "18) vec=(" << vec << ") => sum = " << sum(vec) << "\n";
  std::cout << "19) vec=(" << vec << ") => prod = " << prod(vec) << "\n";
  std::cout << "20) " << max(vec) << "\n";
  std::cout << "21) " << min(vec) << "\n";
  std::cout << "22) " << abs_max(vec) << "\n";
  std::cout << "23) " << abs_min(vec) << "\n";
}


int main(int argc, char** argv)
{
  test1<double>(10);
  test1<int>(10);
  test2<double>(10);
  //test2<int>(10);
  
  functors::root<8, double> F0;
  functors::root<9, double> F1;
  
  return 0;
}
