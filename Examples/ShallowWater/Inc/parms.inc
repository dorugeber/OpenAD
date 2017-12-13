c
c     basic parameters
c
      real g, earth, pi, deg2rad, rho0, invrho0
      parameter ( g = 9.81, earth = 6371000 )
      parameter ( pi = 3.14159265358979323844, deg2rad = pi/180. )
      parameter ( rho0 = 1028., invrho0 = 1./rho0 )
c
c     ``variable'' parameters
c
      common /timeparameter/ dt, start_time, dt_dump, nt, ntspinup
      real dt, start_time, dt_dump
      integer nt, ntspinup
c     
      common /iterpar/ iteration
      integer iteration
c
      common /parms/ om, f0, beta, rini, ah, xstart, ystart
      real om      
      real f0, beta
      real rini
      real ah
      real xstart, ystart
c
c     flags 
c
      common /flags/ xperiodic, yperiodic, spherical, cartesian,
     &     quadfric, suppressio, fullio,
     &     initial_grad, grad_check, optimize, calc_hess
      logical xperiodic, yperiodic, spherical, cartesian, quadfric
      logical suppressio, fullio
      logical initial_grad, grad_check, optimize, calc_hess
c
c     names
c
      common /names/ foutname, runname, depthfile, forcingfile,
     &     uinifile, vinifile, etainifile, ncdatafile, ncrestartfile
      character*(80) foutname
      character*(80) runname
      character*(80) depthfile, forcingfile
      character*(80) uinifile, vinifile, etainifile
      character*(80) ncdatafile, ncrestartfile
c
