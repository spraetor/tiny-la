#include <iostream>
#include <cstdlib>
#include <vector>
#include <cassert>
#include <boost/numeric/conversion/cast.hpp> 

#define DOW  4

#include "AMDiS.h"

// test constructors
template <class T>
void test1(double value0, double value1)
{

ASM("# before constructor");
  AMDiS::StaticVector<T, DOW> vec1(DOW);
  AMDiS::StaticVector<T, DOW> vec2(DOW);
ASM("# after constructor");

ASM("# before assignment");
  for (size_t i = 0; i < DOW; ++i)
    vec1(i) = value0 + i;
  vec2 = vec1;
  vec2*= value1;
ASM("# after assignment");


ASM("# before norm");
  double result1 = two_norm(vec2);
ASM("# after norm");

ASM("# before add");
  vec1 = vec1 + vec2;
ASM("# after add");

ASM("# before norm");
  double result2 = two_norm(vec1);
ASM("# after norm");

  std::cout << result1 << result2;

}


int main(int argc, char** argv)
{
  double value0 = 7.3;
  double value1 = 1.4;

  if (argc > 3) {
    value0 = atoi(argv[1]);
    value1 = atoi(argv[2]);
  }
  test1<double>(value0, value1);
  return 0;
}
