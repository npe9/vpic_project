#ifndef _v4_qpx_hxx_
#define _v4_qpx_hxx_

#ifndef ALIGNED
#define ALIGNED(n)
#endif

class v4
{

public:
  vector4double v;

  v4() {}
  v4(const v4 &a)
  {
    v = a.v;
  }
  ~v4() {}

  void transpose( v4 &a, v4 &b, v4 &c, v4 &d )
  {
    vector4double p0 = vec_perm(a.v, b.v, vec_gpci(00415));
    vector4double p1 = vec_perm(a.v, b.v, vec_gpci(02637));
    vector4double p2 = vec_perm(c.v, d.v, vec_gpci(00415));
    vector4double p3 = vec_perm(c.v, d.v, vec_gpci(02637));

    a.v = vec_perm(p0, p2, vec_gpci(00145));
    b.v = vec_perm(p0, p2, vec_gpci(02367));
    c.v = vec_perm(p1, p3, vec_gpci(00145));
    d.v = vec_perm(p1, p3, vec_gpci(02367));
  }

  void load_4x4_tr( const void * ALIGNED(16) pa,
		    const void * ALIGNED(16) pb,
		    const void * ALIGNED(16) pc,
		    const void * ALIGNED(16) pd,
		    v4 &a, v4 &b, v4 &c, v4 &d )
  {
    vector4double v0 = vec_ld( 0, (float *)pa );
    vector4double v1 = vec_ld( 0, (float *)pb );
    vector4double v2 = vec_ld( 0, (float *)pc );
    vector4double v3 = vec_ld( 0, (float *)pd );

    vector4double p0 = vec_perm(v0, v1, vec_gpci(00415));
    vector4double p1 = vec_perm(v0, v1, vec_gpci(02637));
    vector4double p2 = vec_perm(v2, v3, vec_gpci(00415));
    vector4double p3 = vec_perm(v2, v3, vec_gpci(02637));

    a.v = vec_perm(p0, p2, vec_gpci(00145));
    b.v = vec_perm(p0, p2, vec_gpci(02367));
    c.v = vec_perm(p1, p3, vec_gpci(00145));
    d.v = vec_perm(p1, p3, vec_gpci(02367));
  }
};

#endif // _v4_qpx_hxx_
