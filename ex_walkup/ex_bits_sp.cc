//-----------------------------------------------
// example of bit twiddling operations
//-----------------------------------------------

#include "v4_port_sp.hh"

#include <iostream>

void test_float_clear_bits(void)
{
  v4int   a(1<<31,1<<31,-1,0);
  v4float b(-1,-2,-3,-4), c(5,6,7,8);

  c = b.clear_bits(a,b);

  std::cout << "a[0] = " << a[0]
	    << " a[1] = " << a[1]
	    << " a[2] = " << a[2]
	    << " a[3] = " << a[3] << std::flush << std::endl;
  std::cout << "b[0] = " << b[0]
	    << " b[1] = " << b[1]
	    << " b[2] = " << b[2]
	    << " b[3] = " << b[3] << std::flush << std::endl;
  std::cout << "c[0] = " << c[0]
	    << " c[1] = " << c[1]
	    << " c[2] = " << c[2]
	    << " c[3] = " << c[3] << std::flush << std::endl;

  if( a.any( a != v4int( 1<<31, 1<<31, -1, 0 ) ) ||
      a.any( b != v4float( -1,-2,-3,-4 ) )       ||
      a.any( c != v4float(  1, 2, 0,-4 ) ) )
  {
    std::cout << "float_clear_bits: FAIL" << std::flush << std::endl;
  }
  else
  {
    std::cout << "float_clear_bits: pass" << std::flush << std::endl;
  }
}

void test_float_set_bits(void)
{
  v4int   a(1<<31,1<<31,0,0);
  v4float b(1,2,3,4), c(5,6,7,8);

  c = b.set_bits(a,b);

  std::cout << "a[0] = " << a[0]
	    << " a[1] = " << a[1]
	    << " a[2] = " << a[2]
	    << " a[3] = " << a[3] << std::flush << std::endl;
  std::cout << "b[0] = " << b[0]
	    << " b[1] = " << b[1]
	    << " b[2] = " << b[2]
	    << " b[3] = " << b[3] << std::flush << std::endl;
  std::cout << "c[0] = " << c[0]
	    << " c[1] = " << c[1]
	    << " c[2] = " << c[2]
	    << " c[3] = " << c[3] << std::flush << std::endl;

  if( a.any( a != v4int( 1<<31, 1<<31, 0, 0 ) ) ||
      a.any( b != v4float(  1, 2, 3, 4 ) )      ||
      a.any( c != v4float( -1,-2, 3, 4 ) ) )
  {
    std::cout << "float_set_bits: FAIL" << std::flush << std::endl;
  }
  else
  {
    std::cout << "float_set_bits: pass" << std::flush << std::endl;
  }
}

void test_float_toggle_bits(void)
{
  v4int   a(1<<31,1<<31,0,0);
  v4float b(1,-2,3,-4), c(5,6,7,8);

  c = b.toggle_bits(a,b);

  std::cout << "a[0] = " << a[0]
	    << " a[1] = " << a[1]
	    << " a[2] = " << a[2]
	    << " a[3] = " << a[3] << std::flush << std::endl;
  std::cout << "b[0] = " << b[0]
	    << " b[1] = " << b[1]
	    << " b[2] = " << b[2]
	    << " b[3] = " << b[3] << std::flush << std::endl;
  std::cout << "c[0] = " << c[0]
	    << " c[1] = " << c[1]
	    << " c[2] = " << c[2]
	    << " c[3] = " << c[3] << std::flush << std::endl;

  if( a.any( a != v4int( 1<<31, 1<<31, 0, 0 ) ) ||
      a.any( b != v4float(  1,-2, 3,-4 ) )      ||
      a.any( c != v4float( -1, 2, 3,-4 ) ) )
  {
    std::cout << "float_toggle_bits: FAIL" << std::flush << std::endl;
  }
  else
  {
    std::cout << "float_toggle_bits: pass" << std::flush << std::endl;
  }
}

int main(int argc, char * argv[])
{
  int i0 = 1;
  float f0 = 1.0;

  std::cout << "sizeof(i0) = " << sizeof(i0) << std::endl;
  std::cout << "sizeof(f0) = " << sizeof(f0) << std::endl;

//   test_float_clear_bits();
//   test_float_set_bits();
  test_float_toggle_bits();

  return 0;
}
