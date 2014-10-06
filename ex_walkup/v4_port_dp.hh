#ifndef _v4_port_dp_hh_
#define _v4_port_dp_hh_

#include <iostream>
#include <bitset>

class v4
{
public:

  inline int any( const v4 &a )
  {
    std::cout << "Calling v4::any" << std::flush << std::endl;

    std::cout << "a.i[0] = " << a.i[0]
	      << " a.i[1] = " << a.i[1]
	      << " a.i[2] = " << a.i[2]
	      << " a.i[3] = " << a.i[3] << std::flush << std::endl;

    return a.i[0] || a.i[1] || a.i[2] || a.i[3];
  }

  union
  {
    long i[4];
    double f[4];
  };

  v4() {}                    // Default constructor

  v4(const v4 &a)            // Copy constructor
  {
    i[0] = a.i[0];
    i[1] = a.i[1];
    i[2] = a.i[2];
    i[3] = a.i[3];
  }

  ~v4() {}                   // Default destructor
};

class v4int : public v4
{
public:

  // v4int constructors / destructors

  v4int() {}                                // Default constructor

  v4int( const v4int &a )                   // Copy constructor
  {
    i[0] = a.i[0];
    i[1] = a.i[1];
    i[2] = a.i[2];
    i[3] = a.i[3];
  }

  v4int( const v4 &a )                      // Init from mixed
  {
    i[0] = a.i[0];
    i[1] = a.i[1];
    i[2] = a.i[2];
    i[3] = a.i[3];
  }

  v4int( int a )                            // Init from scalar
  {
    i[0] = a;
    i[1] = a;
    i[2] = a;
    i[3] = a;
  }

  v4int( int i0, int i1, int i2, int i3 )   // Init from scalars
  {
    std::cout << "Calling v4int::v4int ctor" << std::flush << std::endl;
    i[0] = i0;
    i[1] = i1;
    i[2] = i2;
    i[3] = i3;
    std::cout << "i[0] = " << i[0]
	      << " i[1] = " << i[1]
	      << " i[2] = " << i[2]
	      << " i[3] = " << i[3] << std::flush << std::endl;
  }

  ~v4int() {}                               // Destructor

  inline long &operator []( int n )
  {
    std::cout << "Calling v4int::operator []" << std::flush << std::endl;
    return i[n];
  }
};

inline v4int operator !=( const v4int &a, const v4int &b )
{
  v4int c;

  std::cout << "Calling v4int::operator !=" << std::flush << std::endl;

  c.i[0] = -(a.i[0] != b.i[0]);
  c.i[1] = -(a.i[1] != b.i[1]);
  c.i[2] = -(a.i[2] != b.i[2]);
  c.i[3] = -(a.i[3] != b.i[3]);

  return c;
}

class v4float : public v4
{
public:

  // v4float constructors / destructors

  v4float() {}                                        // Default constructor

  v4float( const v4float &a )                         // Copy constructor
  {
    f[0] = a.f[0];
    f[1] = a.f[1];
    f[2] = a.f[2];
    f[3] = a.f[3];
  }

  v4float( const v4 &a )                              // Init from mixed
  {
    f[0] = a.f[0];
    f[1] = a.f[1];
    f[2] = a.f[2];
    f[3] = a.f[3];
  }

  v4float( float a )                                  // Init from scalar
  {
    f[0] = a;
    f[1] = a;
    f[2] = a;
    f[3] = a;
  }

  v4float( float f0, float f1, float f2, float f3 )   // Init from scalars
  {
    std::cout << "Calling v4float::v4float ctor" << std::flush << std::endl;
    f[0] = f0;
    f[1] = f1;
    f[2] = f2;
    f[3] = f3;
    std::cout << "f[0] = " << (int)f[0]
	      << " f[1] = " << (int)f[1]
	      << " f[2] = " << (int)f[2]
	      << " f[3] = " << (int)f[3] << std::flush << std::endl;
  }

  ~v4float() {}                                       // Destructor

  inline double &operator []( int n )
  {
    std::cout << "Calling v4float::operator []" << std::flush << std::endl;
    return f[n];
  }

  inline v4float clear_bits( const v4int &m, const v4float &a )
  {
    v4float b;
    union { int i[4]; float f[4]; } t;
    union { int i[4]; float f[4]; } u;
    union { int i[4]; float f[4]; } w;

    t.f[0] = a.f[0];
    t.f[1] = a.f[1];
    t.f[2] = a.f[2];
    t.f[3] = a.f[3];

    u.i[0] = m.i[0];
    u.i[1] = m.i[1];
    u.i[2] = m.i[2];
    u.i[3] = m.i[3];

    w.i[0] = (~u.i[0]) & t.i[0];
    w.i[1] = (~u.i[1]) & t.i[1];
    w.i[2] = (~u.i[2]) & t.i[2];
    w.i[3] = (~u.i[3]) & t.i[3];

    b.f[0] = w.f[0];
    b.f[1] = w.f[1];
    b.f[2] = w.f[2];
    b.f[3] = w.f[3];

//     b.i[0] = (~m.i[0]) & a.i[0];
//     b.i[1] = (~m.i[1]) & a.i[1];
//     b.i[2] = (~m.i[2]) & a.i[2];
//     b.i[3] = (~m.i[3]) & a.i[3];

    return b;
  }

  inline v4float set_bits( const v4int &m, const v4float &a )
  {
    v4float b;
    union { int i[4]; float f[4]; } t;
    union { int i[4]; float f[4]; } u;
    union { int i[4]; float f[4]; } w;

    t.f[0] = a.f[0];
    t.f[1] = a.f[1];
    t.f[2] = a.f[2];
    t.f[3] = a.f[3];

    u.i[0] = m.i[0];
    u.i[1] = m.i[1];
    u.i[2] = m.i[2];
    u.i[3] = m.i[3];

    w.i[0] = u.i[0] | t.i[0];
    w.i[1] = u.i[1] | t.i[1];
    w.i[2] = u.i[2] | t.i[2];
    w.i[3] = u.i[3] | t.i[3];

    b.f[0] = w.f[0];
    b.f[1] = w.f[1];
    b.f[2] = w.f[2];
    b.f[3] = w.f[3];

//     b.i[0] = m.i[0] | a.i[0];
//     b.i[1] = m.i[1] | a.i[1];
//     b.i[2] = m.i[2] | a.i[2];
//     b.i[3] = m.i[3] | a.i[3];

    return b;
  }

  inline v4float toggle_bits( const v4int &m, const v4float &a )
  {
    v4float b;
    union { int i[4]; float f[4]; } t;
    union { int i[4]; float f[4]; } u;
    union { int i[4]; float f[4]; } w;
    long n, s;

    std::cout << "Entering v4float::toggle_bits" << std::flush << std::endl;

    t.f[0] = a.f[0];
    t.f[1] = a.f[1];
    t.f[2] = a.f[2];
    t.f[3] = a.f[3];

    u.i[0] = m.i[0];
    u.i[1] = m.i[1];
    u.i[2] = m.i[2];
    u.i[3] = m.i[3];

    w.i[0] = u.i[0] ^ t.i[0];
    w.i[1] = u.i[1] ^ t.i[1];
    w.i[2] = u.i[2] ^ t.i[2];
    w.i[3] = u.i[3] ^ t.i[3];

    b.f[0] = w.f[0];
    b.f[1] = w.f[1];
    b.f[2] = w.f[2];
    b.f[3] = w.f[3];

    std::cout << "--------------------------------------------------------------"
	      << "--------------------------------------------------------------"
	      << std::flush << std::endl;

    std::cout << "a.i[0] = " << std::bitset<32>( u.i[0] ) << std::flush << std::endl;
    std::cout << "b.i[0] = " << std::bitset<32>( t.i[0] ) << std::flush << std::endl;
    std::cout << "b.f[0] = " << std::bitset<32>( t.f[0] ) << std::flush << std::endl;
    std::cout << "c.i[0] = " << std::bitset<64>( b.i[0] ) << std::flush << std::endl;

    std::cout << "--------------------------------------------------------------"
	      << "--------------------------------------------------------------"
	      << std::flush << std::endl;

    std::cout << "a.i[1] = " << std::bitset<32>( u.i[1] ) << std::flush << std::endl;
    std::cout << "b.i[1] = " << std::bitset<32>( t.i[1] ) << std::flush << std::endl;
    std::cout << "b.f[1] = " << std::bitset<32>( t.f[1] ) << std::flush << std::endl;
    std::cout << "c.i[1] = " << std::bitset<64>( b.i[1] ) << std::flush << std::endl;

    std::cout << "--------------------------------------------------------------"
	      << "--------------------------------------------------------------"
	      << std::flush << std::endl;

    std::cout << "a.i[2] = " << std::bitset<32>( u.i[2] ) << std::flush << std::endl;
    std::cout << "b.i[2] = " << std::bitset<32>( t.i[2] ) << std::flush << std::endl;
    std::cout << "b.f[2] = " << std::bitset<32>( t.f[2] ) << std::flush << std::endl;
    std::cout << "c.i[2] = " << std::bitset<64>( b.i[2] ) << std::flush << std::endl;

    std::cout << "--------------------------------------------------------------"
	      << "--------------------------------------------------------------"
	      << std::flush << std::endl;

    std::cout << "a.i[3] = " << std::bitset<32>( u.i[3] ) << std::flush << std::endl;
    std::cout << "b.i[3] = " << std::bitset<32>( t.i[3] ) << std::flush << std::endl;
    std::cout << "b.f[3] = " << std::bitset<32>( t.f[3] ) << std::flush << std::endl;
    std::cout << "c.i[3] = " << std::bitset<64>( b.i[3] ) << std::flush << std::endl;

    std::cout << "--------------------------------------------------------------"
	      << "--------------------------------------------------------------"
	      << std::flush << std::endl;

    std::cout << "a.i[0] = " << m.i[0]
	      << " a.i[1] = " << m.i[1]
	      << " a.i[2] = " << m.i[2]
	      << " a.i[3] = " << m.i[3] << std::flush << std::endl;

    std::cout << "--------------------------------------------------------------"
	      << "--------------------------------------------------------------"
	      << std::flush << std::endl;

    std::cout << "b.i[0] = " << a.i[0]
	      << " b.i[1] = " << a.i[1]
	      << " b.i[2] = " << a.i[2]
	      << " b.i[3] = " << a.i[3] << std::flush << std::endl;

    std::cout << "--------------------------------------------------------------"
	      << "--------------------------------------------------------------"
	      << std::flush << std::endl;

    std::cout << "b.f[0] = " << a.f[0]
	      << " b.f[1] = " << a.f[1]
	      << " b.f[2] = " << a.f[2]
	      << " b.f[3] = " << a.f[3] << std::flush << std::endl;

    std::cout << "--------------------------------------------------------------"
	      << "--------------------------------------------------------------"
	      << std::flush << std::endl;

    std::cout << "c.i[0] = " << b.i[0]
	      << " c.i[1] = " << b.i[1]
	      << " c.i[2] = " << b.i[2]
	      << " c.i[3] = " << b.i[3] << std::flush << std::endl;

    std::cout << "--------------------------------------------------------------"
	      << "--------------------------------------------------------------"
	      << std::flush << std::endl;

    std::cout << "c.f[0] = " << b.f[0]
	      << " c.f[1] = " << b.f[1]
	      << " c.f[2] = " << b.f[2]
	      << " c.f[3] = " << b.f[3] << std::flush << std::endl;

    std::cout << "--------------------------------------------------------------"
	      << "--------------------------------------------------------------"
	      << std::flush << std::endl;

    n = 8;
    std::cout << "      8 = " << std::bitset<64>( n ) << std::flush << std::endl;

    n = 7;
    std::cout << "      7 = " << std::bitset<64>( n ) << std::flush << std::endl;

    n = 6;
    std::cout << "      6 = " << std::bitset<64>( n ) << std::flush << std::endl;

    n = 5;
    std::cout << "      5 = " << std::bitset<64>( n ) << std::flush << std::endl;

    n = 4;
    std::cout << "      4 = " << std::bitset<64>( n ) << std::flush << std::endl;

    n = -4;
    std::cout << "    - 4 = " << std::bitset<64>( n ) << std::flush << std::endl;

    n = 3;
    std::cout << "      3 = " << std::bitset<64>( n ) << std::flush << std::endl;

    n = 2;
    std::cout << "      2 = " << std::bitset<64>( n ) << std::flush << std::endl;

    n = -2;
    std::cout << "    - 2 = " << std::bitset<64>( n ) << std::flush << std::endl;

    n = 1;
    std::cout << "      1 = " << std::bitset<64>( n ) << std::flush << std::endl;

    s = n<<31;
    std::cout << "1 << 31 = " << std::bitset<64>( s ) << std::flush << std::endl;

    s = n<<16;
    std::cout << "1 << 16 = " << std::bitset<64>( s ) << std::flush << std::endl;

    s = n<<2;
    std::cout << "1 <<  2 = " << std::bitset<64>( s ) << std::flush << std::endl;

    s = n<<1;
    std::cout << "1 <<  1 = " << std::bitset<64>( s ) << std::flush << std::endl;

    n = 0;
    std::cout << "      0 = " << std::bitset<64>( n ) << std::flush << std::endl;

    std::cout << "--------------------------------------------------------------"
	      << "--------------------------------------------------------------"
	      << std::flush << std::endl;

    std::cout << "Leaving v4float::toggle_bits" << std::flush << std::endl;

    return b;
  }

  inline void load_4x4_tr( const void * ALIGNED(16) pa,
                           const void * ALIGNED(16) pb,
                           const void * ALIGNED(16) pc,
                           const void * ALIGNED(16) pd,
                           v4float &a, v4float &b,
			   v4float &c, v4int &d )
  {
//     std::cout << "a0[0] = " << ((const float * ALIGNED(16))pa)[0]
// 	      << " a0[1] = " << ((const float * ALIGNED(16))pa)[1]
// 	      << " a0[2] = " << ((const float * ALIGNED(16))pa)[2]
// 	      << " a0[3] = " << ((const int   * ALIGNED(16))pa)[3] << std::flush << std::endl;
//     std::cout << "a1[0] = " << ((const float * ALIGNED(16))pb)[0]
// 	      << " a1[1] = " << ((const float * ALIGNED(16))pb)[1]
// 	      << " a1[2] = " << ((const float * ALIGNED(16))pb)[2]
// 	      << " a1[3] = " << ((const int   * ALIGNED(16))pb)[3] << std::flush << std::endl;
//     std::cout << "a2[0] = " << ((const float * ALIGNED(16))pc)[0]
// 	      << " a2[1] = " << ((const float * ALIGNED(16))pc)[1]
// 	      << " a2[2] = " << ((const float * ALIGNED(16))pc)[2]
// 	      << " a2[3] = " << ((const int   * ALIGNED(16))pc)[3] << std::flush << std::endl;
//     std::cout << "a3[0] = " << ((const float * ALIGNED(16))pd)[0]
// 	      << " a3[1] = " << ((const float * ALIGNED(16))pd)[1]
// 	      << " a3[2] = " << ((const float * ALIGNED(16))pd)[2]
// 	      << " a3[3] = " << ((const int   * ALIGNED(16))pd)[3] << std::flush << std::endl;

    a.f[0] = ((const float * ALIGNED(16))pa)[0];
    b.f[0] = ((const float * ALIGNED(16))pa)[1];
    c.f[0] = ((const float * ALIGNED(16))pa)[2];
    d.i[0] = ((const int   * ALIGNED(16))pa)[3];

    a.f[1] = ((const float * ALIGNED(16))pb)[0];
    b.f[1] = ((const float * ALIGNED(16))pb)[1];
    c.f[1] = ((const float * ALIGNED(16))pb)[2];
    d.i[1] = ((const int   * ALIGNED(16))pb)[3];

    a.f[2] = ((const float * ALIGNED(16))pc)[0];
    b.f[2] = ((const float * ALIGNED(16))pc)[1];
    c.f[2] = ((const float * ALIGNED(16))pc)[2];
    d.i[2] = ((const int   * ALIGNED(16))pc)[3];

    a.f[3] = ((const float * ALIGNED(16))pd)[0];
    b.f[3] = ((const float * ALIGNED(16))pd)[1];
    c.f[3] = ((const float * ALIGNED(16))pd)[2];
    d.i[3] = ((const int   * ALIGNED(16))pd)[3];

//     std::cout << "&a.v[0] = " << &a.v[0] << std::flush << std::endl;
//     std::cout << "&a.v[1] = " << &a.v[1] << std::flush << std::endl;
//     std::cout << "&a.v[2] = " << &a.v[2] << std::flush << std::endl;
//     std::cout << "&a.v[3] = " << &a.v[3] << std::flush << std::endl;

//     std::cout << "&a.f[0] = " << &a.f[0] << std::flush << std::endl;
//     std::cout << "&a.f[1] = " << &a.f[1] << std::flush << std::endl;
//     std::cout << "&a.f[2] = " << &a.f[2] << std::flush << std::endl;
//     std::cout << "&a.f[3] = " << &a.f[3] << std::flush << std::endl;

//     std::cout << "&a.i[0] = " << &a.i[0] << std::flush << std::endl;
//     std::cout << "&a.i[1] = " << &a.i[1] << std::flush << std::endl;
//     std::cout << "&a.i[2] = " << &a.i[2] << std::flush << std::endl;
//     std::cout << "&a.i[3] = " << &a.i[3] << std::flush << std::endl;

//     vector4double a0 = vec_ld( 0, ((float *)pa) );          // a0 =  0  1  2  3
//     vector4double b0 = vec_ld( 0, ((float *)pb) );          // b0 =  4  5  6  7
//     vector4double c1 = vec_ld( 0, ((float *)pc) );          // c1 =  8  9 10 11
//     vector4double d1 = vec_ld( 0, ((float *)pd) );          // d1 = 12 13 14 15

//     // Step 1: Interleave top and bottom half

//     vector4double a1 = vec_perm( a0, c1, vec_gpci(00415) ); // a1 =  0  8  1  9
//     vector4double b1 = vec_perm( b0, d1, vec_gpci(00415) ); // b1 =  4 12  5 13
//     c1               = vec_perm( a0, c1, vec_gpci(02637) ); // c1 =  2 10  3 11
//     d1               = vec_perm( b0, d1, vec_gpci(02637) ); // d1 =  6 14  7 15

//     // Step 2: Interleave even and odd rows

//     a.v              = vec_perm( a1, b1, vec_gpci(00415) ); // a  =  0  4  8 12
//     b.v              = vec_perm( a1, b1, vec_gpci(02637) ); // b  =  1  5  9 13
//     c.v              = vec_perm( c1, d1, vec_gpci(00415) ); // c  =  2  6 10 14
// //  d.v              = vec_perm( c1, d1, vec_gpci(02637) ); // d  =  3  7 11 15

//     d.i[0]           = ((const int * ALIGNED(16))pa)[3];
//     d.i[1]           = ((const int * ALIGNED(16))pb)[3];
//     d.i[2]           = ((const int * ALIGNED(16))pc)[3];
//     d.i[3]           = ((const int * ALIGNED(16))pd)[3];
  }
};

#endif // _v4_port_dp_hh_
