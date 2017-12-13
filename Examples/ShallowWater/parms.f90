      module parms
!c
!c     basic parameters
!c
        SAVE
      real g, earth, pi, deg2rad, rho0, invrho0
      parameter ( g = 9.81, earth = 6371000 )
      parameter ( pi = 3.14159265358979323844, deg2rad = pi/180. )
      parameter ( rho0 = 1028., invrho0 = 1./rho0 )
!c
!c     ``variable'' parameters
!c
!un      common /timeparameter/ dt, start_time, dt_dump, nt, ntspinup
!ju   all constant after init/read from netcdf
      real dt, start_time, dt_dump
      integer nt, ntspinup
!c     
!un      common /iterpar/ iteration
!ju   don't see how this is used properly 
      integer iteration
!c
!un      common /parms/ om, f0, beta, rini, ah, xstart, ystart
!ju   all constant after init/read from netcdf
      real om      
      real f0, beta
      real rini
      real ah
      real xstart, ystart
!c
!c     flags 
!c
!un      common /flags/ xperiodic, yperiodic, spherical, cartesian,
!un     &     quadfric, suppressio, fullio,
!un     &     initial_grad, grad_check, optimize, calc_hess
      logical xperiodic, yperiodic, spherical, cartesian, quadfric
      logical suppressio, fullio
      logical initial_grad, grad_check, optimize, calc_hess
!c
!c     names
!c
!un      common /names/ foutname, runname, depthfile, forcingfile,
!un     &     uinifile, vinifile, etainifile, ncdatafile, ncrestartfile
      character*(80) foutname
      character*(80) runname
      character*(80) depthfile, forcingfile
      character*(80) uinifile, vinifile, etainifile
      character*(80) ncdatafile, ncrestartfile
!c
      end module
