//============================================================================//
//
// LPI 3D deck - Linearly polarized (in y) plane wave incident from left
//               boundary
//
// Adapted from Albright's Lightning 3D LPI deck.
// B. Albright, X-1-PTA; 28 Jan. 2007
// Lin Yin, X-1-PTA, 23 Feb 2009, for Cerrillos test
//
// Executable creates its own directory structure.  Remove the old with:
//
// rm -rf rundata ehydro Hhydro Hehydro restart poynting velocity particle
//        field
//============================================================================//

// Employ turnstiles to partially serialize the high-volume file writes.
// In this case, the restart dumps.  Set NUM_TURNSTILES to be the desired
// number of simultaneous writes.

#define NUM_TURNSTILES REPLACE_num_turnstiles

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//

begin_globals
{
  double e0;                   // peak amplitude of oscillating electric field
  double omega;                // angular freq. of the beam

  int fields_interval;         // how often to dump field and hydro
  int poynting_interval;       // how often to compute poynting flux on boundary
  int restart_interval;        // how often to write restart data
  int quota_check_interval;    // how often to check if quota exceeded
  int energies_interval;       // how often to dump energy history data
  int ehydro_interval;
  int Hhydro_interval;
  int eparticle_interval;
  int Hparticle_interval;

  double quota_sec;            // run quota in sec
  int rtoggle;                 // enable save of last 2 restart files for safety
  int load_particles;          // were particles loaded?
  double topology_x;           // domain topology needed to normalize Poynting diagnostic
  double topology_y;
  double topology_z;
  int mobile_ions;
  int H_present;
  int He_present;

  // Parameters for 3d Gaussian wave launch
  double lambda;
  double waist;                // how wide the focused beam is
  double width;
  double zcenter;              // center of beam at boundary in z
  double ycenter;              // center of beam at boundary in y
  double xfocus;               // how far from boundary to focus
  double mask;                 // # gaussian widths from beam center where I is nonzero

  // Ponyting diagnostic flags - which output to turn on

  // write_backscatter_only flag:  when this flag is nonzero, it means to only compute
  // poynting data for the lower-x surface.  This flag affects both the summed poynting
  // data as well as the surface data.

  int write_poynting_data;     // Whether to write poynting data to file (or just stdout)

  int write_backscatter_only;  // nonzero means we only write backscatter diagnostic for fields

  // write_poynting_sum is nonzero if you wish to write a file containing the integrated
  // poynting data on one or more surfaces.  If write_backscatter_only is nonzero, then
  // the output file will be a time series of double precision numbers containing the
  // integrated poynting flux at that point in time.  If write_backscatter_only is zero,
  // then for each time step, six double precision numbers are written, containing the
  // poynting flux through the lower x, upper x, lower y, upper y, lower z, and upper z
  // surfaces.

  int write_poynting_sum;      // nonzero if we wish to write integrated Poynting data

  // write_poynting_faces is probably useless, but I put it here anyway in case it's not.
  // When this flag is nonzero, it will print out the poynting flux at each of a 2D array
  // of points on the surface.  When this flag is turned on and write_backscatter_only is
  // nonzero, then only the 2D array of points on the lower-x boundary surface are written
  // for each time step.  When this flag is turned on and write_bacscatter_only is
  // zero, then it will write 2D array data for the lower x, upper x, lower y, upper y,
  // lower z, upper z surfaces for each time step.

  int write_poynting_faces;    // nonzero if we wish to write Poynting data on sim boundaries

  // write_eb_faces is nonzero when we wish to get the raw e and b data at the boundary
  // (e.g., to be used with a filter to distinguish between SRS and SBS backscatter).
  // When this flag is on and write_backscatter_only is nonzero, then only the 2D array
  // of points on the lower-x boundary surface are written for each time step.  When
  // this flag is turned on and write_backscatteR_only is zero, then it will write 2D
  // array data for the lower x, upper x, lower y, upper y, lower z, upper z surfaces for
  // each time step.  When turned on, four files are produced: e1, e2, cb1, cb2.  The
  // values of the quantities printed depend on the face one is considering:  for the
  // x faces, e1 = ey, e2 = ez, cb1 = cby, cb2 = cbz.  Similarly for y and z surfaces,
  // but where 1 and 2 stand for (z,x) and (x,y) coordinates, respectively.

  int write_eb_faces;          // nonzero if we wish to write E and B data on sim boundaries

  // Needed for movie files
  float vthe;                   // vthe/c
  float vthi_He;                // vthi_He/c
  float vthi_H;                 // vthi_H/c
  int   velocity_interval;      // how frequently to dump binned velocity space data

  // Dump parameters for standard VPIC output formats
  DumpParameters fdParams;
  DumpParameters hedParams;
  DumpParameters hHdParams;
  DumpParameters hHedParams;
  std::vector<DumpParameters *> outputParams;

  // Vadim:  modified restart machinery

  int write_restart;       // global flag for all to write restart files
  int write_end_restart;   // global flag for all to write restart files
};

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//

begin_initialization
{
  // System of units
  double ec         = 4.8032e-10;          // stat coulomb
  double c_vac      = 2.99792458e10;       // cm/sec
  double m_e        = 9.1094e-28;          // g
  double k_b        = 1.6022e-12;          // erg/eV
  double mec2       = m_e*c_vac*c_vac/k_b;
  double mpc2       = mec2*1836.0;

  double cfl_req    = 0.98;                // How close to Courant should we try to run
  double damp       = 0;                   // How much radiation damping
  double iv_thick   = 2;                   // Thickness of impermeable vacuum (in cells)

  // Experimental parameters

  double t_e               = 2600;         // electron temperature, eV
  double t_i               = 1300;         // ion temperature, eV
  double n_e_over_n_crit   = 0.1415;       // n_e/n_crit
  double vacuum_wavelength = 527 * 1e-7;   // third micron light (cm)
  double laser_intensity   = 6.0e15 * 1e7; // in ergs/cm^2 (note: 1 W = 1e7 ergs)

  // Simulation parameters

  double Lx                = 17 * 0.02 * 12.0 * 1e-4 * REPLACE_scale_Lx; // In cm (note: 1 micron = 1e-4 cm)
  double Ly                =  3 * 0.02 * 12.0 * 1e-4 * REPLACE_scale_Ly;
  double Lz                =  3 * 0.02 * 12.0 * 1e-4 * REPLACE_scale_Lz;

  double nx                = 32*17*REPLACE_scale_nx;
  double ny                = 32* 3*REPLACE_scale_ny;
  double nz                = 32* 3*REPLACE_scale_nz;

  double topology_x        = 1*17*REPLACE_scale_topology_x;
  double topology_y        = 1*2*REPLACE_scale_topology_y;
  double topology_z        = 1*2*REPLACE_scale_topology_z;

  // single-processor mesh = 544 x 96 x 96

  double nppc               = REPLACE_nppc;        // Average number macro particles per cell per species
  int load_particles        = 1;                   // Flag to turn on/off particle load
  int mobile_ions           = REPLACE_mobile_ions; // Whether or not to push ions
  double f_He               = 0;                   // Ratio of number density of He to total ion density
  double f_H                = 1-f_He;              // Ratio of number density of H  to total ion density
  int H_present             = ( (f_H !=0) ? 1 : 0 );
  int He_present            = ( (f_He!=0) ? 1 : 0 );

  // Precompute some useful variables.
  double A_H                = 1;
  double A_He               = 4;
  double Z_H                = 1;
  double Z_He               = 2;
  double mic2_H             = mpc2*A_H;
  double mic2_He            = mpc2*A_He;
  double mime_H             = mic2_H /mec2;
  double mime_He            = mic2_He/mec2;

  double uthe               = sqrt(t_e/mec2);     // vthe/c
  double uthi_H             = sqrt(t_i/mic2_H);   // vthi/c for H
  double uthi_He            = sqrt(t_i/mic2_He);  // vthi/c for He

  // Plasma skin depth in cm
  double delta = (vacuum_wavelength / (2*M_PI) ) / sqrt( n_e_over_n_crit );

  double n_e   = c_vac*c_vac*m_e/(4*M_PI*ec*ec*delta*delta); // electron density in cm^-3
  double debye = uthe*delta;                      // electron Debye length (cm)
  double omega = sqrt( 1/n_e_over_n_crit );       // laser beam freq. in wpe

  // Peak instantaneous E field in "natural units"
  double e0    = sqrt( 2*laser_intensity / (m_e*c_vac*c_vac*c_vac*n_e) );

  // Set up local mesh resolution and time step
  Lx /= delta;                                    // Convert box size to skin depths
  Ly /= delta;
  Lz /= delta;

  double hx = Lx/nx;
  double hy = Ly/ny;
  double hz = Lz/nz;

  double cell_size_x       = hx*delta/debye;      // Cell size in Debye lengths
  double cell_size_y       = hy*delta/debye;
  double cell_size_z       = hz*delta/debye;

  double f_number          = 6;                   // f/# of beam
  double lambda            = vacuum_wavelength/delta; // vacuum wavelength in c/wpe
  double waist             = f_number*lambda;     // width of beam at focus in c/wpe
  double xfocus            = Lx/2;                // in c/wpe
  double ycenter           = 0;                   // center of spot in y on lhs boundary
  double zcenter           = 0;                   // center of spot in z on lhs boundary
  double mask              = 1.5;                 // set drive I=0 outside r>mask*width at lhs boundary
  double width = waist*sqrt( 1 + (lambda*xfocus/(M_PI*waist*waist))*(lambda*xfocus/(M_PI*waist*waist)));
  e0                       = e0*(waist/width);    // at entrance (3D Gaussian)

  double dt                = cfl_req*courant_length(Lx,Ly,Lz,nx,ny,nz); // in 1/wpe; n.b. c=1 in nat. units
  double nsteps_cycle      = trunc_granular(2*M_PI/(dt*omega),1)+1;
  dt                       = 2*M_PI/omega/nsteps_cycle; // nsteps_cycle time steps in one laser cycle

  double t_stop            = REPLACE_nstep*dt + 0.001*dt; // Runtime in 1/wpe

  int energies_interval    = REPLACE_energies_interval;
  int ehydro_interval      = REPLACE_field_interval;
  int Hhydro_interval      = REPLACE_field_interval;
  int eparticle_interval   = REPLACE_particle_interval;
  int Hparticle_interval   = REPLACE_particle_interval;
  int poynting_interval    = 0;                      // Num. steps between dumping poynting flux
  int fields_interval      = REPLACE_field_interval; // Num. steps between saving field data
  int velocity_interval    = int(100.0/dt);          // How frequently to dump velocity space data
  int restart_interval     = REPLACE_nrestart;       // Num. steps between restart dumps
  int quota_check_interval = 20;

  // Ben:  This quota thing gracefully terminates after writing a final restart after
  // 11.5 hours; if you want a longer time before shutdown, set this value larger.  If
  // you want the code to just run all weekend, then set it to very long (2400.*3500, e.g.)

  double quota_sec         = 11.6*3600;           // Run quota in sec.

  // Turn on integrated backscatter poynting diagnostic - right now there is a bug in this, so we
  // only write the integrated backscatter time history on the left face.

  int write_poynting_data = 1;                    // Whether to write poynting data to file (or just stdout)

  int write_backscatter_only = 0;                 // Nonzero means only write lower x face
  int write_poynting_sum   = 0;                   // Whether to write integrated Poynting data
  int write_poynting_faces = 0;                   // Whether to write poynting data on sim boundary faces
  int write_eb_faces       = 0;                   // Whether to write e and b field data on sim boundary faces

  double N_e               = nppc*nx*ny*nz;       // Number of macro electrons in box
  double Np_e              = Lx*Ly*Lz;            // "Number" of "physical" electrons in box (nat. units)
  double q_e               = -Np_e/N_e;           // Charge per macro electron
  double N_i               = N_e;                 // Number of macro ions of each species in box
  double Np_i              = Np_e/(Z_H*f_H+Z_He*f_He); // "Number" of "physical" ions of each sp. in box
  double qi_H              = Z_H *f_H *Np_i/N_i;  // Charge per H  macro ion
  double qi_He             = Z_He*f_He*Np_i/N_i;  // Charge per He macro ion

  // Print simulation parameters

  sim_log("***** Simulation parameters *****");
  sim_log("* Processors:                     "<<nproc());
  sim_log("* Topology:                       "<<topology_x<<" "<<topology_y<<" "<<topology_z);
  sim_log("* nsteps_cycle =                  "<<nsteps_cycle);
  sim_log("* Time step, max time, nsteps:    "<<dt<<" "<<t_stop<<" "<<int(t_stop/(dt)));
  sim_log("* Debye length, XYZ cell sizes:   "<<debye<<" "<<cell_size_x<<" "<<cell_size_y<<" "<<cell_size_z);
  sim_log("* Real cell sizes (in Debyes):    "<<hx/uthe<<" "<<hy/uthe<<" "<<hz/uthe);
  sim_log("* Lx, Ly, Lz =                    "<<Lx<<" "<<Ly<<" "<<Lz);
  sim_log("* nx, ny, nz =                    "<<nx<<" "<<ny<<" "<<nz);
  sim_log("* Charge/macro electron =         "<<q_e);
  sim_log("* Average particles/processor:    "<<N_e/nproc());
  sim_log("* Average particles/cell:         "<<nppc);
  sim_log("* Total # of particles =          "<< 2*N_e );
  sim_log("* Omega_0, Omega_pe:              "<<omega<<" "<<1);
  sim_log("* Plasma density, ne/nc:          "<<n_e<<" "<<n_e_over_n_crit);
  sim_log("* Vac wavelength (nm):            "<<vacuum_wavelength*1e7);
  sim_log("* I_laser (W/cm^2):               "<<laser_intensity/1e7);
  sim_log("* T_e, T_i (eV)                   "<<t_e<<" "<<t_i);
  sim_log("* m_e, m_H, m_He                  "<<"1 "<<mime_H<<" "<<mime_He);
  sim_log("* Radiation damping:              "<<damp);
  sim_log("* Fraction of courant limit:      "<<cfl_req);
  sim_log("* vthe/c:                         "<<uthe);
  sim_log("* vthi_H /c:                      "<<uthi_H);
  sim_log("* vthe_He/c:                      "<<uthi_He);
  sim_log("* emax at entrance:               "<<e0);
  sim_log("* emax at waist:                  "<<e0/(waist/width));

  sim_log("* Poynting interval:              "<<poynting_interval);
  sim_log("* fields interval:                "<<fields_interval);
  sim_log("* restart interval:               "<<restart_interval);
  sim_log("* velocity interval               "<<velocity_interval);
  sim_log("* quota check interval:           "<<quota_check_interval);

  sim_log("* num vacuum edge grids:          "<<iv_thick);
  sim_log("* width, waist, xfocus:           "<<width<<" "<<waist<<" "<<xfocus);
  sim_log("* ycenter, zcenter, mask:         "<<ycenter<<" "<<zcenter<<" "<<mask);
  sim_log("* write_poynting_sum:             "<<(write_poynting_sum ? "Yes" : "No"));
  sim_log("* write_poynting_faces:           "<<(write_poynting_faces? "Yes" : "No"));
  sim_log("* write_eb_faces:                 "<<(write_eb_faces ? "Yes" : "No"));
  sim_log("* write_backscatter_only:         "<<(write_backscatter_only ? "Yes" : "No"));
  sim_log("*********************************");

  // Set up high level simulation parameters

  sim_log("Setting up high-level simulation parameters.");
  num_step             = int(t_stop/(dt));

  status_interval      = REPLACE_status_interval;
  sync_shared_interval = REPLACE_sync_shared_interval;
  clean_div_e_interval = REPLACE_clean_div_e_interval;
  clean_div_b_interval = REPLACE_clean_div_b_interval;

  // Turn off some of the spam
  verbose = 1;

  // For maxwellian reinjection, we need more than the default number of
  // passes (3) through the boundary handler
  // Note:  We have to adjust sort intervals for maximum performance on Cell.
  num_comm_round = 16;

  global->e0                     = e0;
  global->omega                  = omega;

  global->fields_interval        = fields_interval;
  global->poynting_interval      = poynting_interval;
  global->restart_interval       = restart_interval;
  global->quota_check_interval   = quota_check_interval;
  global->energies_interval      = energies_interval;
  global->ehydro_interval        = ehydro_interval;
  global->Hhydro_interval        = Hhydro_interval;
  global->eparticle_interval     = eparticle_interval;
  global->Hparticle_interval     = Hparticle_interval;

  global->quota_sec              = quota_sec;
  global->rtoggle                = 0;
  global->load_particles         = load_particles;
  global->mobile_ions            = mobile_ions;
  global->H_present              = H_present;
  global->He_present             = He_present;
  global->topology_x             = topology_x;
  global->topology_y             = topology_y;
  global->topology_z             = topology_z;
  global->xfocus                 = xfocus;
  global->ycenter                = ycenter;
  global->zcenter                = zcenter;
  global->mask                   = mask;
  global->waist                  = waist;
  global->width                  = width;
  global->lambda                 = lambda;

  global->write_poynting_data    = write_poynting_data;

  global->write_poynting_sum     = write_poynting_sum;
  global->write_poynting_faces   = write_poynting_faces;
  global->write_eb_faces         = write_eb_faces;
  global->write_backscatter_only = write_backscatter_only;

  global->vthe                   = uthe;
  global->vthi_H                 = uthi_H;
  global->vthi_He                = uthi_He;
  global->velocity_interval      = velocity_interval;

  // Set up the species. Allow additional local particles in case of
  // non-uniformity.

  // Set up grid.
  sim_log( "Setting up computational grid." );

  grid->dx       = hx;
  grid->dy       = hy;
  grid->dz       = hz;
  grid->dt       = dt;
  grid->cvac     = 1;
  grid->eps0     = 1;

  sim_log( "Setting up absorbing mesh." );

  define_absorbing_grid( 0,         -0.5*Ly,    -0.5*Lz,        // Low corner
                         Lx,         0.5*Ly,     0.5*Lz,        // High corner
                         nx,         ny,         nz,            // Resolution
                         topology_x, topology_y, topology_z,    // Topology
                         reflect_particles );                   // Default particle BC

  sim_log( "Setting up species." );

  double max_local_np = 1.3*N_e/nproc();

  double max_local_nm = max_local_np/10.0;

  species_t * electron = define_species( "electron",
					 -1,
					 1,
					 max_local_np,
					 max_local_nm,
					 REPLACE_eon_sort_interval,
					 REPLACE_eon_sort_method );

  // Start with two ion species.  We have option to go to Xe and Kr gas fills if
  // we need a higher ion/electron macroparticle ratio.

  species_t *ion_H, *ion_He;

  if ( mobile_ions )
  {
    if ( H_present )
    {
      ion_H  = define_species( "H",
			       Z_H,
			       mime_H,
			       max_local_np,
			       max_local_nm,
			       REPLACE_ion_sort_interval,
			       REPLACE_ion_sort_method );
    }

    if ( He_present )
    {
      ion_He = define_species( "He",
			       Z_He,
			       mime_He,
			       max_local_np,
			       max_local_nm,
			       REPLACE_ion_sort_interval,
			       REPLACE_ion_sort_method );
    }
  }

  // From grid/partition.c: used to determine which domains are on edge
# define RANK_TO_INDEX(rank,ix,iy,iz) BEGIN_PRIMITIVE {                   \
    int _ix, _iy, _iz;                                                    \
    _ix  = (rank);                        /* ix = ix+gpx*( iy+gpy*iz ) */ \
    _iy  = _ix/int(global->topology_x);   /* iy = iy+gpy*iz */            \
    _ix -= _iy*int(global->topology_x);   /* ix = ix */                   \
    _iz  = _iy/int(global->topology_y);   /* iz = iz */                   \
    _iy -= _iz*int(global->topology_y);   /* iy = iy */                   \
    (ix) = _ix;                                                           \
    (iy) = _iy;                                                           \
    (iz) = _iz;                                                           \
  } END_PRIMITIVE

  sim_log( "Overriding x boundaries to absorb fields." );

  int ix, iy, iz;        // Domain location in mesh
  RANK_TO_INDEX( int(rank()), ix, iy, iz );

  // Set up Maxwellian reinjection B.C.

  sim_log( "Setting up Maxwellian reinjection boundary condition." );

  particle_bc_t * maxwellian_reinjection =
    define_particle_bc( maxwellian_reflux( species_list, entropy ) );

  set_reflux_temp( maxwellian_reinjection,
		   electron,
		   uthe,
		   uthe );

  if ( mobile_ions )
  {
    if ( H_present )
    {
      set_reflux_temp( maxwellian_reinjection,
		       ion_H,
		       uthi_H,
		       uthi_H );
    }

    if ( He_present )
    {
      set_reflux_temp( maxwellian_reinjection,
		       ion_He,
		       uthi_He,
		       uthi_He );
    }
  }

  // Set up materials

  sim_log( "Setting up materials." );

  define_material( "vacuum", 1 );

  define_field_array( NULL, damp );

  // Paint the simulation volume with materials and boundary conditions

# define iv_region (   x<      hx*iv_thick || x>Lx  -hx*iv_thick  \
                    || y<-Ly/2+hy*iv_thick || y>Ly/2-hy*iv_thick  \
		       || z<-Lz/2+hz*iv_thick || z>Lz/2-hz*iv_thick ) // all boundaries are i.v.

  set_region_bc( iv_region,
		 maxwellian_reinjection,
		 maxwellian_reinjection,
		 maxwellian_reinjection );

  // Load particles.

  if ( load_particles )
  {
    sim_log( "Loading particles." );

    // Fast load of particles--don't bother fixing artificial domain correlations

    double xmin=grid->x0, xmax=grid->x1;
    double ymin=grid->y0, ymax=grid->y1;
    double zmin=grid->z0, zmax=grid->z1;

    repeat( N_e/(topology_x*topology_y*topology_z) )
    {
      double x = uniform( rng(0), xmin, xmax );
      double y = uniform( rng(0), ymin, ymax );
      double z = uniform( rng(0), zmin, zmax );

      if ( iv_region ) continue;           // Particle fell in iv_region.  Don't load.

      // third to last arg is "weight," a positive number

      inject_particle( electron, x, y, z,
                       normal( rng(0), 0, uthe),
                       normal( rng(0), 0, uthe),
                       normal( rng(0), 0, uthe), -q_e, 0, 0 );

      if ( mobile_ions )
      {
        if ( H_present )  // Inject an H macroion on top of macroelectron
	{
          inject_particle( ion_H, x, y, z,
                           normal( rng(0), 0, uthi_H),
                           normal( rng(0), 0, uthi_H),
                           normal( rng(0), 0, uthi_H), qi_H, 0, 0 );
	}

        if ( He_present ) // Inject an H macroion on top of macroelectron
	{
          inject_particle( ion_He, x, y, z,
                           normal( rng(0), 0, uthi_He),
                           normal( rng(0), 0, uthi_He),
                           normal( rng(0), 0, uthi_He), qi_He, 0, 0 );
	}
      }
    }
  }

  //--------------------------------------------------------------------------//
  // New dump definition
  //--------------------------------------------------------------------------//

  //--------------------------------------------------------------------------//
  // Set data output format
  //
  // This option allows the user to specify the data format for an output
  // dump.  Legal settings are 'band' and 'band_interleave'.  Band-interleave
  // format is the native storage format for data in VPIC.  For field data,
  // this looks something like:
  //
  //   ex0 ey0 ez0 div_e_err0 cbx0 ... ex1 ey1 ez1 div_e_err1 cbx1 ...
  //
  // Banded data format stores all data of a particular state variable as a
  // contiguous array, and is easier for ParaView to process efficiently.
  // Banded data looks like:
  //
  //   ex0 ex1 ex2 ... exN ey0 ey1 ey2 ...
  //
  //--------------------------------------------------------------------------//

  sim_log( "Setting up hydro and field diagnostics." );

  global->fdParams.format = REPLACE_field_io_format;

  sim_log( "Field output format          : REPLACE_field_io_format" );

  global->hedParams.format = REPLACE_field_io_format;

  sim_log( "Electron hydro output format : REPLACE_field_io_format" );

  global->hHdParams.format = REPLACE_field_io_format;

  sim_log( "Hydrogen hydro output format : REPLACE_field_io_format" );

  global->hHedParams.format = REPLACE_field_io_format;

  sim_log( "Helium hydro output format   : REPLACE_field_io_format" );

  //--------------------------------------------------------------------------//
  // Set stride
  //
  // This option allows data down-sampling at output.  Data are down-sampled
  // in each dimension by the stride specified for that dimension.  For
  // example, to down-sample the x-dimension of the field data by a factor
  // of 2, i.e., half as many data will be output, select:
  //
  //   global->fdParams.stride_x = 2;
  //
  // The following 2-D example shows down-sampling of a 7x7 grid (nx = 7,
  // ny = 7.  With ghost-cell padding the actual extents of the grid are 9x9.
  // Setting the strides in x and y to equal 2 results in an output grid of
  // nx = 4, ny = 4, with actual extents 6x6.
  //
  // G G G G G G G G G
  // G X X X X X X X G
  // G X X X X X X X G         G G G G G G
  // G X X X X X X X G         G X X X X G
  // G X X X X X X X G   ==>   G X X X X G
  // G X X X X X X X G         G X X X X G
  // G X X X X X X X G         G X X X X G
  // G X X X X X X X G         G G G G G G
  // G G G G G G G G G
  //
  // Note that grid extents in each dimension must be evenly divisible by
  // the stride for that dimension:
  //
  //   nx = 150;
  //   global->fdParams.stride_x = 10; // legal -> 150/10 = 15
  //
  //   global->fdParams.stride_x = 8; // illegal!!! -> 150/8 = 18.75
  //--------------------------------------------------------------------------//

  // Strides for field and hydro arrays.  Note that here we have defined them
  // the same for fields and all hydro species; if desired, we could use
  // different strides for each.   Also note that strides must divide evenly
  // into the number of cells in a given domain.

  // Define strides and test that they evenly divide into grid->nx, ny, nz

  int stride_x = 1, stride_y = 1, stride_z = 1;

  if ( int( grid->nx )%stride_x )
    ERROR( ( "Stride doesn't evenly divide grid->nx." ) );

  if ( int( grid->ny )%stride_y )
    ERROR( ( "Stride doesn't evenly divide grid->ny." ) );

  if ( int( grid->nz )%stride_z )
    ERROR( ( "Stride doesn't evenly divide grid->nz." ) );

  //--------------------------------------------------------------------------//
  // Fields
  //--------------------------------------------------------------------------//

  // relative path to fields data from global header
  sprintf( global->fdParams.baseDir, "fields" );

  // base file name for fields output
  sprintf( global->fdParams.baseFileName, "fields" );

  // set field strides
  global->fdParams.stride_x = stride_x;
  global->fdParams.stride_y = stride_y;
  global->fdParams.stride_z = stride_z;

  sim_log( "Fields x-stride " << global->fdParams.stride_x );
  sim_log( "Fields y-stride " << global->fdParams.stride_y );
  sim_log( "Fields z-stride " << global->fdParams.stride_z );

  // add field parameters to list
  global->outputParams.push_back( &global->fdParams );

  //--------------------------------------------------------------------------//
  // Electron hydro
  //--------------------------------------------------------------------------//

  // relative path to electron species data from global header
  sprintf( global->hedParams.baseDir, "hydro" );

  // base file name for fields output
  sprintf( global->hedParams.baseFileName, "e_hydro" );

  // set electron hydro strides
  global->hedParams.stride_x = stride_x;
  global->hedParams.stride_y = stride_y;
  global->hedParams.stride_z = stride_z;

  sim_log( "Electron species x-stride " << global->hedParams.stride_x );
  sim_log( "Electron species y-stride " << global->hedParams.stride_y );
  sim_log( "Electron species z-stride " << global->hedParams.stride_z );

  // add electron hydro parameters to list
  global->outputParams.push_back( &global->hedParams );

  //--------------------------------------------------------------------------//
  // Hydrogen hydro
  //--------------------------------------------------------------------------//

  // relative path to electron species data from global header
  sprintf( global->hHdParams.baseDir, "hydro" );

  // base file name for fields output
  sprintf( global->hHdParams.baseFileName, "H_hydro" );

  // set hydrogen hydro strides
  global->hHdParams.stride_x = stride_x;
  global->hHdParams.stride_y = stride_y;
  global->hHdParams.stride_z = stride_z;

  sim_log( "Ion species x-stride " << global->hHdParams.stride_x );
  sim_log( "Ion species y-stride " << global->hHdParams.stride_y );
  sim_log( "Ion species z-stride " << global->hHdParams.stride_z );

  // add hydrogen hydro parameters to list
  global->outputParams.push_back( &global->hHdParams );

  //--------------------------------------------------------------------------//
  // Helium hydro
  //--------------------------------------------------------------------------//

  // relative path to electron species data from global header
  sprintf( global->hHedParams.baseDir, "hydro" );

  // base file name for fields output
  sprintf( global->hHedParams.baseFileName, "He_hydro" );

  // set helium hydro strides
  global->hHedParams.stride_x = stride_x;
  global->hHedParams.stride_y = stride_y;
  global->hHedParams.stride_z = stride_z;

  sim_log( "Ion species x-stride " << global->hHedParams.stride_x );
  sim_log( "Ion species y-stride " << global->hHedParams.stride_y );
  sim_log( "Ion species z-stride " << global->hHedParams.stride_z );

  // add helium hydro parameters to list
  global->outputParams.push_back( &global->hHedParams );

  //--------------------------------------------------------------------------//
  // Set output fields
  //
  // It is now possible to select which state-variables are output on a
  // per-dump basis.  Variables are selected by passing an or-list of
  // state-variables by name.  For example, to only output the x-component
  // of the electric field and the y-component of the magnetic field, the
  // user would call output_variables like:
  //
  //   global->fdParams.output_variables( ex | cby );
  //
  // NOTE: OUTPUT VARIABLES ARE ONLY USED FOR THE BANDED FORMAT.  IF THE
  // FORMAT IS BAND-INTERLEAVE, ALL VARIABLES ARE OUTPUT AND CALLS TO
  // 'output_variables' WILL HAVE NO EFFECT.
  //
  // ALSO: DEFAULT OUTPUT IS NONE.  THIS IS DUE TO THE WAY THAT VPIC
  // HANDLES GLOBAL VARIABLES IN THE INPUT DECK AND IS UNAVOIDABLE.
  //
  // For convenience, the output variable 'all' is defined:
  //
  //   global->fdParams.output_variables( all );
  //--------------------------------------------------------------------------//
  // CUT AND PASTE AS A STARTING POINT. REMEMBER TO ADD APPROPRIATE GLOBAL
  // DUMPPARAMETERS VARIABLE.
  //
  // output_variables( all );
  //
  // output_variables( electric | div_e_err | magnetic | div_b_err |
  //                   tca      | rhob      | current  | rhof |
  //                   emat     | nmat      | fmat     | cmat );
  //
  // output_variables( current_density  | charge_density |
  //                   momentum_density | ke_density     | stress_tensor );
  //--------------------------------------------------------------------------//

  //global->fdParams.output_variables( all );

  global->fdParams.output_variables( electric | magnetic );

  //global->hedParams.output_variables( all );

  //global->hedParams.output_variables( current_density | momentum_density );

  global->hedParams.output_variables(  current_density  | charge_density |
                                       momentum_density | ke_density |
                                       stress_tensor );

  global->hHdParams.output_variables(  current_density  | charge_density |
                                       momentum_density | ke_density |
                                       stress_tensor );

  global->hHedParams.output_variables( current_density  | charge_density |
                                       momentum_density | ke_density |
                                       stress_tensor );

  //--------------------------------------------------------------------------//
  // Convenience functions for simlog output.
  //--------------------------------------------------------------------------//

  char varlist[256];

  create_field_list( varlist, global->fdParams );

  sim_log( "Fields variable list: " << varlist );

  create_hydro_list( varlist, global->hedParams );

  sim_log( "Electron species variable list: " << varlist );

  create_hydro_list( varlist, global->hHdParams );

  sim_log( "Ion species variable list: " << varlist );

  //--------------------------------------------------------------------------//
  // Wrapup initialization.
  //--------------------------------------------------------------------------//

  sim_log( "***Finished with user-specified initialization ***" );

  //--------------------------------------------------------------------------//
  // Upon completion of the initialization, the following occurs:
  //
  // - The synchronization error (tang E, norm B) is computed between domains
  //   and tang E / norm B are synchronized by averaging where discrepancies
  //   are encountered.
  // - The initial divergence error of the magnetic field is computed and
  //   one pass of cleaning is done (for good measure)
  // - The bound charge density necessary to give the simulation an initially
  //   clean divergence e is computed.
  // - The particle momentum is uncentered from u_0 to u_{-1/2}
  // - The user diagnostics are called on the initial state
  // - The physics loop is started
  //
  // The physics loop consists of:
  //
  // - Advance particles from x_0,u_{-1/2} to x_1,u_{1/2}
  // - User particle injection at x_{1-age}, u_{1/2} (use inject_particles)
  // - User current injection (adjust field(x,y,z).jfx, jfy, jfz)
  // - Advance B from B_0 to B_{1/2}
  // - Advance E from E_0 to E_1
  // - User field injection to E_1 (adjust field(x,y,z).ex,ey,ez,cbx,cby,cbz)
  // - Advance B from B_{1/2} to B_1
  // - (periodically) Divergence clean electric field
  // - (periodically) Divergence clean magnetic field
  // - (periodically) Synchronize shared tang e and norm b
  // - Increment the time step
  // - Call user diagnostics
  // - (periodically) Print a status message
  //--------------------------------------------------------------------------//
}

//----------------------------------------------------------------------------//
// Definition of user_diagnostics function.
//----------------------------------------------------------------------------//

#define should_dump( x ) \
  ( global->x##_interval > 0 && remainder( step(), global->x##_interval ) == 0 )

begin_diagnostics
{
  //--------------------------------------------------------------------------//
  // NOTE: YOU CANNOT DIRECTLY USE C FILE DESCRIPTORS OR SYSTEM CALLS ANYMORE.
  //
  // To create a new directory, use:
  //
  //   dump_mkdir( "full-path-to-directory/directoryname" )
  //
  // To open a file, use: FileIO class
  //
  // Example for file creation and use:
  //
  //   // declare file and open for writing
  //   // possible modes are: io_write, io_read, io_append,
  //   // io_read_write, io_write_read, io_append_read
  //   FileIO fileIO;
  //   FileIOStatus status;
  //   status= fileIO.open( "full-path-to-file/filename", io_write );
  //
  //   // formatted ASCII  output
  //   fileIO.print( "format string", varg1, varg2, ... );
  //
  //   // binary output
  //   // Write n elements from array data to file.
  //   // T is the type, e.g., if T=double
  //   // fileIO.write( double *data, size_t n );
  //   // All basic types are supported.
  //   fileIO.write( T *data, size_t n );
  //
  //   // close file
  //   fileIO.close();
  //--------------------------------------------------------------------------//

  //--------------------------------------------------------------------------//
  // Data output directories.
  //--------------------------------------------------------------------------//
  // WARNING: The directory list passed to "global_header" must be consistent
  // with the actual directories where fields and species are output using
  // "field_dump" and "hydro_dump".
  //
  // DIRECTORY PATHES SHOULD BE RELATIVE TO THE LOCATION OF THE GLOBAL HEADER.
  //--------------------------------------------------------------------------//

  //--------------------------------------------------------------------------//
  // Normal rundata dump.
  //--------------------------------------------------------------------------//

  if ( step() == 0 )
  {
    dump_mkdir( "fields" );
    dump_mkdir( "hydro" );
    dump_mkdir( "rundata" );
    dump_mkdir( "restart0" );
    dump_mkdir( "restart1" );  // 1st backup
    dump_mkdir( "restart2" );  // 2nd backup
    dump_mkdir( "particle" );

    double dumpstart = uptime();

    REPLACE_begin_turnstile
    dump_grid( "rundata/grid" );
    REPLACE_end_turnstile

    sim_log( "Grid dump completed, REPLACE_io_type." );

    double dumpelapsed = uptime() - dumpstart;

    sim_log( "Grid dump duration, REPLACE_io_type " << dumpelapsed );

    dump_materials( "rundata/materials" );
    dump_species( "rundata/species" );

    global_header( "global", global->outputParams );
  }

  //--------------------------------------------------------------------------//
  // Normal rundata energies dump.
  //--------------------------------------------------------------------------//

  if ( should_dump( energies ) )
  {
    dump_energies( "rundata/energies", step() == 0 ? 0 : 1 );
  }

  //--------------------------------------------------------------------------//
  // Field data output.
  //--------------------------------------------------------------------------//

  // if ( step() == 1 || should_dump( fields ) )
  if ( should_dump( fields ) )
  {
    double dumpstart = uptime();

    REPLACE_begin_turnstile
    field_dump( global->fdParams );
    REPLACE_end_turnstile

    sim_log( "Field dump completed, REPLACE_io_type." );

    double dumpelapsed = uptime() - dumpstart;

    sim_log( "Field dump duration, REPLACE_io_type " << dumpelapsed );
  }

  //--------------------------------------------------------------------------//
  // Electron species output.
  //--------------------------------------------------------------------------//

  if ( should_dump( ehydro ) )
  {
    double dumpstart = uptime();

    hydro_dump( "electron", global->hedParams );

    sim_log( "Electron hydro dump completed, REPLACE_io_type." );

    double dumpelapsed = uptime() - dumpstart;

    sim_log( "Electron hydro dump duration, REPLACE_io_type "
	     << dumpelapsed );
  }

  //--------------------------------------------------------------------------//
  // Hydrogen ion species output.
  //--------------------------------------------------------------------------//

  if ( should_dump( Hhydro ) )
  {
    double dumpstart = uptime();

    hydro_dump( "H", global->hHdParams );

    sim_log( "Hydrogen ion hydro dump completed, REPLACE_io_type." );

    double dumpelapsed = uptime() - dumpstart;

    sim_log( "Hydrogen ion hydro dump duration, REPLACE_io_type "
	     << dumpelapsed );
  }

  //--------------------------------------------------------------------------//
  // Energy Spectrum Output.
  //--------------------------------------------------------------------------//

  // #include "energy.cxx" // Subroutine to compute energy spectrum diagnostic.

  // Vadim:

  // #include "dissipation.cxx"
  // #include "Ohms_exp_all_v2.cxx"

  //--------------------------------------------------------------------------//
  // Restart dump.
  //--------------------------------------------------------------------------//

  if ( step() && !( step()%global->restart_interval ) )
  {
    // Set restart flag. The actual restart files are written during the
    // next step.
    global->write_restart = 1;
  }

  else
  {
    if ( global->write_restart )
    {
      global->write_restart = 0; // reset restart flag

      double dumpstart = uptime();

      if ( !global->rtoggle )
      {
	global->rtoggle = 1;

        REPLACE_begin_turnstile
	checkpt( "restart1/restart", 0 );
        REPLACE_end_turnstile
      }

      else
      {
	global->rtoggle = 0;

        REPLACE_begin_turnstile
	checkpt( "restart2/restart", 0 );
        REPLACE_end_turnstile
      }

      mp_barrier(); // Just to be safe

      sim_log( "Restart dump completed, REPLACE_io_type." );

      double dumpelapsed = uptime() - dumpstart;

      sim_log("Restart duration, REPLACE_io_type "<<dumpelapsed);

      if ( rank() == 0 )
      {
        FileIO fp_restart_info;

        if ( !( fp_restart_info.open( "latest_restart", io_write ) == ok ) )
	{
	  ERROR( ( "Cannot open file." ) );
	}

        if ( !global->rtoggle )
	{
          fp_restart_info.print( "restart restart2/restart" );
        }

	else
	{
          fp_restart_info.print( "restart restart1/restart" );
	}

        fp_restart_info.close();
      }
    }
  }

  //--------------------------------------------------------------------------//
  // Dump particle data.
  //--------------------------------------------------------------------------//

  char subdir[36];

  if ( should_dump( eparticle ) &&
       ( step() != 0 ) &&
       step() > 0*( global->fields_interval ) )
  {
    sprintf( subdir, "particle/T.%d", step() );

    dump_mkdir( subdir );

    sprintf( subdir, "particle/T.%d/eparticle", step() );

    double dumpstart = uptime();

    REPLACE_begin_turnstile
    dump_particles( "electron", subdir );
    REPLACE_end_turnstile

    sim_log( "Electron particle dump completed, REPLACE_io_type." );

    double dumpelapsed = uptime() - dumpstart;

    sim_log( "Electron particle dump duration, REPLACE_io_type " << dumpelapsed );
  }

  //--------------------------------------------------------------------------//
  // Shut down simulation when wall clock time exceeds global->quota_sec.
  // Note that the mp_elapsed() is guaranteed to return the same value for all
  // processors (i.e., elapsed time on proc #0), and therefore the abort will
  // be synchronized across processors. Note that this is only checked every
  // few timesteps to eliminate the expensive mp_elapsed call from every
  // timestep. mp_elapsed has an ALL_REDUCE in it.
  //--------------------------------------------------------------------------//

  if ( ( step() > 0 &&
	 global->quota_check_interval > 0 &&
	 ( step() & global->quota_check_interval ) == 0 ) ||
       ( global->write_end_restart ) )
  {
    if ( global->write_end_restart )
    {
      global->write_end_restart = 0; // reset restart flag

      sim_log( "Allowed runtime exceeded for this job.  Terminating....\n" );

      double dumpstart = uptime();

      REPLACE_begin_turnstile
      checkpt( "restart0/restart", 0 );
      REPLACE_end_turnstile

      mp_barrier(); // Just to be safe

      sim_log( "Restart dump completed, REPLACE_io_type." );

      double dumpelapsed = uptime() - dumpstart;

      sim_log( "Restart duration, REPLACE_io_type " << dumpelapsed );

      if ( rank() == 0 )
      {
	FileIO fp_restart_info;

        if ( !( fp_restart_info.open( "latest_restart", io_write ) == ok ) )
	{
	  ERROR( ( "Cannot open file." ) );

	  fp_restart_info.print( "restart restart0/restart" );

	  fp_restart_info.close();
	}

	exit(0); // Exit or abort?
      }

      if ( uptime() > global->quota_sec )
      {
	global->write_end_restart = 1;
      }
    }
  }

  //--------------------------------------------------------------------------//
  // Done with diagnostics.
  //--------------------------------------------------------------------------//
}

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//

begin_field_injection
{
  // Inject a light wave from lhs boundary with E aligned along y. Use scalar
  // diffraction theory for the Gaussian beam source. (This is approximate).
  //
  // For quiet startup (i.e., so that we don't propagate a delta-function
  // noise pulse at time t=0) we multiply by a constant phase term exp(i phi)
  // where:
  //   phi = k*global->xfocus+atan(h)    (3d)
  //
  // Inject from the left a field of the form ey = e0 sin( omega t )

# define DY    ( grid->y0 + (iy-0.5)*grid->dy - global->ycenter )
# define DZ    ( grid->z0 + (iz-1  )*grid->dz - global->zcenter )
# define R2    ( DY*DY + DZ*DZ )
# define PHASE ( global->omega*t + h*R2/(global->width*global->width) )
# define MASK  ( R2<=pow(global->mask*global->width,2) ? 1 : 0 )

  if ( grid->x0 == 0 )               // Node is on left boundary
  {
    double alpha      = grid->cvac*grid->dt/grid->dx;
    double emax_coeff = (4/(1+alpha))*global->omega*grid->dt*global->e0;
    double prefactor  = emax_coeff*sqrt(2/M_PI);
    double t          = grid->dt*step();

    // Compute Rayleigh length in c/wpe
    double rl         = M_PI*global->waist*global->waist/global->lambda;

    double pulse_shape_factor = 1;
    float pulse_length        = 70;                // units of 1/wpe
    float sin_t_tau           = sin( 0.5 * t * M_PI / pulse_length );
    pulse_shape_factor        = ( t < pulse_length ? sin_t_tau : 1 );
    double h                  = global->xfocus/rl; // Distance / Rayleigh length

    // Loop over all Ey values on left edge of this node
    for( int iz = 1; iz <= grid->nz + 1; ++iz )
    {
      for( int iy = 1; iy <= grid->ny; ++iy )
      {
        field( 1, iy, iz ).ey += prefactor
                                 * cos(PHASE)
                                 * exp( -R2 / ( global->width*global->width ) )
                                 * MASK * pulse_shape_factor;
      }
    }
  }
}

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//

begin_particle_injection
{
  // No particle injection for this simulation
}

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//

begin_current_injection
{
  // No current injection for this simulation
}

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//

begin_particle_collisions
{
  // No particle collisions for this simulation
}
