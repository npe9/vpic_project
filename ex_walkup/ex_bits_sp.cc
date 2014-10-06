//-----------------------------------------------
// example of bit twiddling operations
//-----------------------------------------------

#include "v4_port_sp.hh"

#include <iostream>
#include <bitset>

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

typedef struct particle_wdn
{
  float dx, dy, dz;
  int32_t ii;
} particle_wdn_t;

void test_load_4x4_tr_mixed1(void)
{
  DECLARE_ALIGNED_ARRAY( particle_wdn_t, 64, mem, 4 );

  v4float a0, a1, a2;
  v4int   a3;
  int i, j;

  for( i=0; i < 4; i++ )
  {
    mem[i].dx = i*4 + 0;
    mem[i].dy = i*4 + 1;
    mem[i].dz = i*4 + 2;
    mem[i].ii = i*4 + 3;
  }

  load_4x4_tr( &mem[0].dx,
	       &mem[1].dx,
	       &mem[2].dx,
	       &mem[3].dx,
	       a0, a1, a2, a3 );

  for( i=0; i < 4; i++ )
  {
    j = i*4 + 0;
    if( mem[i].dx != j ) break;
    j = i*4 + 1;
    if( mem[i].dy != j ) break;
    j = i*4 + 2;
    if( mem[i].dz != j ) break;
    j = i*4 + 3;
    if( mem[i].ii != j ) break;
  }
  j++;

  std::cout << "a0[0] = " << a0[0]
	    << " a0[1] = " << a0[1]
	    << " a0[2] = " << a0[2]
	    << " a0[3] = " << a0[3] << std::flush << std::endl;
  std::cout << "a1[0] = " << a1[0]
	    << " a1[1] = " << a1[1]
	    << " a1[2] = " << a1[2]
	    << " a1[3] = " << a1[3] << std::flush << std::endl;
  std::cout << "a2[0] = " << a2[0]
	    << " a2[1] = " << a2[1]
	    << " a2[2] = " << a2[2]
	    << " a2[3] = " << a2[3] << std::flush << std::endl;
  std::cout << "a3[0] = " << a3[0]
	    << " a3[1] = " << a3[1]
	    << " a3[2] = " << a3[2]
	    << " a3[3] = " << a3[3] << std::flush << std::endl;

  std::cout << "j     = " << j      << std::flush << std::endl;

  if( any( a0 != v4float( 0, 4, 8,12) ) ||
      any( a1 != v4float( 1, 5, 9,13) ) ||
      any( a2 != v4float( 2, 6,10,14) ) ||
      any( a3 != v4int  ( 3, 7,11,15) ) ||
      j       != 16 )
  {
    std::cout << "load_4x4_tr: mixed1: FAIL" << std::flush << std::endl;
  }
  else
  {
    std::cout << "load_4x4_tr: mixed1: pass" << std::flush << std::endl;
  }
}

void test_store_4x4_tr_mixed1(void)
{
  DECLARE_ALIGNED_ARRAY( particle_wdn_t, 64, mem, 4 );

  v4float a0( 0, 4, 8,12);
  v4float a1( 1, 5, 9,13);
  v4float a2( 2, 6,10,14);
  v4int   a3( 3, 7,11,15);

  int i, j;

  for( i=0; i < 4; i++ )
  {
    mem[i].dx = 0;
    mem[i].dy = 0;
    mem[i].dz = 0;
    mem[i].ii = 0;
  }

  store_4x4_tr( a0, a1, a2, a3,
		&mem[0].dx,
		&mem[1].dx,
		&mem[2].dx,
		&mem[3].dx );

  for( i=0; i < 4; i++ )
  {
    std::cout << "mem[" << i << "].dx = " << mem[i].dx
	      << " mem[" << i << "].dy = " << mem[i].dy
	      << " mem[" << i << "].dz = " << mem[i].dz
	      << " mem[" << i << "].ii = " << mem[i].ii
	      << std::flush << std::endl;
    j = i*4 + 0;
    if( mem[i].dx != j ) break;
    j = i*4 + 1;
    if( mem[i].dy != j ) break;
    j = i*4 + 2;
    if( mem[i].dz != j ) break;
    j = i*4 + 3;
    if( mem[i].ii != j ) break;
  }
  j++;

  std::cout << "a0[0] = " << a0[0]
	    << " a0[1] = " << a0[1]
	    << " a0[2] = " << a0[2]
	    << " a0[3] = " << a0[3] << std::flush << std::endl;
  std::cout << "a1[0] = " << a1[0]
	    << " a1[1] = " << a1[1]
	    << " a1[2] = " << a1[2]
	    << " a1[3] = " << a1[3] << std::flush << std::endl;
  std::cout << "a2[0] = " << a2[0]
	    << " a2[1] = " << a2[1]
	    << " a2[2] = " << a2[2]
	    << " a2[3] = " << a2[3] << std::flush << std::endl;
  std::cout << "a3[0] = " << a3[0]
	    << " a3[1] = " << a3[1]
	    << " a3[2] = " << a3[2]
	    << " a3[3] = " << a3[3] << std::flush << std::endl;

  std::cout << "j     = " << j      << std::flush << std::endl;

  if( any( a0 != v4float( 0, 4, 8,12) ) ||
      any( a1 != v4float( 1, 5, 9,13) ) ||
      any( a2 != v4float( 2, 6,10,14) ) ||
      any( a3 != v4int  ( 3, 7,11,15) ) ||
      j       != 16 )
  {
    std::cout << "store_4x4_tr: mixed1: FAIL" << std::flush << std::endl;
  }
  else
  {
    std::cout << "store_4x4_tr: mixed1: pass" << std::flush << std::endl;
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
//   test_float_toggle_bits();

  test_load_4x4_tr_mixed1();
  test_store_4x4_tr_mixed1();

  return 0;
}
