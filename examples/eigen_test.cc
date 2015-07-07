#include <Eigen/Core>


#include <cstdlib>
#include <vector>
#include <boost/numeric/conversion/cast.hpp> 
#include <boost/date_time/posix_time/posix_time.hpp>

#define DOW  2
using namespace boost::posix_time;
// -----------------------------------------------------------------------------

template <class Timer>
inline double measure(Timer& first_seq)
{
  time_duration td = microsec_clock::local_time() - first_seq;
  first_seq = microsec_clock::local_time();
  return boost::numeric_cast<double>(td.total_nanoseconds())*1.e-6;
}
  

template <class T>
T test(size_t size, size_t seed, double& time1, double& time2, double& time3, double& time4)
{
  typedef Eigen::Matrix<T, Eigen::Dynamic, 1> Vector;
  typedef Eigen::Matrix<T, Eigen::Dynamic, 1>     WorldVector;
//   typedef Eigen::Matrix<T, DOW, 1>     WorldVector;
  
  typedef Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> Matrix;
  typedef Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>         WorldMatrix;
//   typedef Eigen::Matrix<T, DOW, DOW>         WorldMatrix;
  
  std::vector<Vector> points1; points1.reserve(size);
  std::vector<WorldVector> points2; points2.reserve(size);
  
  // fill vectors with random values
  ptime first_seq = microsec_clock::local_time();
  
  std::srand(seed);
  for (size_t i = 0; i < size; ++i) {
    Vector x(DOW);
    WorldVector y(DOW);
    
    for (size_t j = 0; j < DOW; ++j)
      x[j] = boost::numeric_cast<T>(10*((std::rand() / double(RAND_MAX)) - 0.5));
    points1.push_back(x);
    
    y.noalias() = T(0.1) * x;
    for (size_t j = 0; j < DOW; ++j)
      y[j] += boost::numeric_cast<T>(10*((std::rand() / double(RAND_MAX)) - 0.5));
    points2.push_back(y);
  }
  time1 += measure(first_seq);
  
  std::vector<T> nrm1; nrm1.reserve(size);
  std::vector<T> nrm2; nrm2.reserve(size);
  
  // calculate norms of each vector
  for (size_t i = 0; i < size; ++i) {
    nrm1[i] = points1[i].norm();
    nrm2[i] = points2[i].squaredNorm();
  }
  time2 += measure(first_seq);
  
  // perform a sum-reduction on the scalar products of the vectors
  T result = 0;
  for (size_t i = 0; i < size; ++i) {
    result += points1[i].dot(points2[i]);
  }
  time3 += measure(first_seq);
  
  for (size_t i = 0; i < size; ++i) {
    Matrix mat1(DOW, DOW);
    
    T value = boost::numeric_cast<T>(5*((std::rand() / double(RAND_MAX)) - 0.5));
    T value2 = boost::numeric_cast<T>(2*((std::rand() / double(RAND_MAX)) - 0.5));
    
    mat1 = Matrix::Constant(DOW, DOW, value);
    mat1*= value2;
    
    WorldMatrix mat2(DOW, DOW);
    for (size_t j = 0; j < DOW; ++j)
      for (size_t k = 0; k < DOW; ++k)
	mat2(j,k) = boost::numeric_cast<T>(10*((std::rand() / double(RAND_MAX)) - 0.5));
    
    WorldVector tmp(DOW); 
    for (size_t k = 0; k < DOW; ++k)
      tmp[k] = boost::numeric_cast<T>(10*((std::rand() / double(RAND_MAX)) - 0.5));
    
		points1[i].noalias() = mat1 * points2[i];
		points2[i].noalias() = T(0.5) * (tmp + points1[i]);
		points1[i].noalias() += mat2 * points2[i];
  }
  time4 += measure(first_seq);
  
  for (size_t i = 0; i < size; ++i) {
    result += points1[i].dot(points2[i]);
  }
  
  return result;
}


int main(int argc, char** argv)
{
  size_t n_outer = 1000, n_inner = 1000;
  if (argc > 1)
    n_outer = atoi(argv[1]);
  if (argc > 2)
    n_inner = atoi(argv[2]);
    
  double time1 = 0.0, time2 = 0.0, time3 = 0.0, time4 = 0.0;
  ptime first_seq = microsec_clock::local_time();
  
  int sign = 1;
  double result_d = 0.0;
  for (size_t i = 0; i < n_outer; ++i)
    result_d += (sign *= -1) * test<double>(n_inner, i, time1, time2, time3, time4);
  std::cout << "double: " << result_d << "\n";
    
  int result_i = 0;
  for (size_t i = 0; i < n_outer; ++i)
    result_i += (sign *= -1) * test<int>(n_inner, i, time1, time2, time3, time4);
  std::cout << "int: " << result_i << "\n";

  time_duration td = microsec_clock::local_time()-first_seq;
  std::cout << "Time = " << boost::numeric_cast<double>(td.total_nanoseconds())*1.e-9 << " sec\n";
  std::cout << "Time1 = " << time1*1.e-3 << " sec\n";
  std::cout << "Time2 = " << time2*1.e-3 << " sec\n";
  std::cout << "Time3 = " << time3*1.e-3 << " sec\n";
  std::cout << "Time4 = " << time4*1.e-3 << " sec\n";
  
  return 0;
}
