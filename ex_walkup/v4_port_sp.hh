#ifndef _v4_port_sp_hh_
#define _v4_port_sp_hh_

#include <iostream>

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
    int i[4];
    float f[4];
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

  inline int &operator []( int n )
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

  inline float &operator []( int n )
  {
    std::cout << "Calling v4float::operator []" << std::flush << std::endl;
    return f[n];
  }

  inline v4float clear_bits( const v4int &m, const v4float &a )
  {
    v4float b;

    b.i[0] = (~m.i[0]) & a.i[0];
    b.i[1] = (~m.i[1]) & a.i[1];
    b.i[2] = (~m.i[2]) & a.i[2];
    b.i[3] = (~m.i[3]) & a.i[3];

    return b;
  }

  inline v4float set_bits( const v4int &m, const v4float &a )
  {
    v4float b;

    b.i[0] = m.i[0] | a.i[0];
    b.i[1] = m.i[1] | a.i[1];
    b.i[2] = m.i[2] | a.i[2];
    b.i[3] = m.i[3] | a.i[3];

    return b;
  }

  inline v4float toggle_bits( const v4int &m, const v4float &a )
  {
    v4float b;

    std::cout << "Entering v4float::toggle_bits" << std::flush << std::endl;

    b.i[0] = m.i[0] ^ a.i[0];
    b.i[1] = m.i[1] ^ a.i[1];
    b.i[2] = m.i[2] ^ a.i[2];
    b.i[3] = m.i[3] ^ a.i[3];

    std::cout << "--------------------------------------------------------------"
	      << std::flush << std::endl;

    std::cout << "a.i[0] = " << m.i[0]
	      << " a.i[1] = " << m.i[1]
	      << " a.i[2] = " << m.i[2]
	      << " a.i[3] = " << m.i[3] << std::flush << std::endl;

    std::cout << "b.i[0] = " << a.i[0]
	      << " b.i[1] = " << a.i[1]
	      << " b.i[2] = " << a.i[2]
	      << " b.i[3] = " << a.i[3] << std::flush << std::endl;

    std::cout << "c.i[0] = " << b.i[0]
	      << " c.i[1] = " << b.i[1]
	      << " c.i[2] = " << b.i[2]
	      << " c.i[3] = " << b.i[3] << std::flush << std::endl;

    std::cout << "Leaving v4float::toggle_bits" << std::flush << std::endl;

    return b;
  }
};

#endif // _v4_port_sp_hh_
