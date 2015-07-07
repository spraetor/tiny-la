#include <iostream>

#define ASM(text) asm(text)
#define ALIGNED(type,name,N)  type name[N] __attribute__ ((aligned(16)))
#define USE_ALIGNMENT
#define N 16

int main() {
  using std::size_t;
	
#ifdef USE_ALIGNMENT
  ALIGNED(double, a, N);
  ALIGNED(double, b, N);
  ALIGNED(double, c, N);
#else
  double a[N];
  double b[N];
  double c[N];
#endif
	
  ASM("# begin fill");
  for (size_t i = 0; i < N; ++i) {
	  a[i] = double(i);
	  b[i] = double((i*17+3) % 5);
	  c[i] = double((i*1627+8461) % 7);
  }
  ASM("# end fill");
  
  ASM("# begin add");
  for (size_t i = 0; i < N; ++i) {
	  a[i] = b[i] + c[i];
  }
  ASM("# end add");
  
  ASM("# begin add2");
  for (size_t i = 0; i < N; ++i) {
	  b[i] += c[i];
  }
  ASM("# end add2");

  double result = 0.0;
  ASM("# begin norm");
  for (size_t i = 0; i < N; ++i) {
	  result += a[i] * c[i];
  }
  ASM("# end norm");
  
  double result1 = 0.0;
  ASM("# begin norm1");
  for (size_t i = 0; i < N; ++i) {
	  result1 += b[i] * b[i];
  }
  ASM("# end norm1");

  double result2 = 0.0;
  double tmp0 = 0.0, tmp1 = 0.0, tmp2 = 0.0, tmp3 = 0.0;
  ASM("# begin norm2");
  for (size_t i = 0; i < N; i+=4) {
    tmp0 = b[i] * b[i];
    tmp1 = b[i+1] * b[i+1];
    tmp2 = b[i+2] * b[i+2];
    tmp3 = b[i+3] * b[i+3];
    result2 += (tmp0 + tmp1) + (tmp2 + tmp3);
  }
  ASM("# end norm2");


  std::cout << result << result1 << result2;
}
