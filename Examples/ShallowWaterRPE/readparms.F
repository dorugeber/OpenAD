c$openad XXX Template OADrts/ad_template.split.f
      subroutine readparms

      implicit none
c
c     general parameters and flags from file
c
      call read_data_file
c
c     read depth, friction, and forcing from a file
c     likewise the initial conditions
c
      call read_data_fields
c
      call prep_depth
c
      call check_cfl
c
      call make_masks
c
      call ini_scales
c
      call prep_coriolis

      return
      end ! subroutine readparms

c$openad XXX Template OADrts/ad_template.split.f
      subroutine read_data_file

      use size
      use parms
      use pfields
      use mini
      use weights

      implicit none

!un #include "size.inc"
!un #include "parms.inc"
!un #include "pfields.inc"
!un #include "mini.inc"
!un #include "weights.inc"

      integer ix, iy
      real delx, dely
c
c     general parameters and flags from file "data"
c
cph      open(20,file='data',form='formatted',status='old',action='read')
      open(20,file='data',form='formatted',status='old')

      read(20,*)
      read(20,*) nt, ntspinup, dt, start_time, dt_dump
      read(20,*)
c      read(20,*) rini, ah, f0, beta
      read(20,*) rini, f0, beta
      read(20,*)
      read(20,*) xstart, ystart
      read(20,*)
      read(20,*) delx, dely

c$$$      print*,'nt=',nt
c$$$      print*,'dt=',dt
c$$$      print*,'start_time=',start_time
c$$$      print*,'rini=',rini
c$$$      print*,'f0=',f0
c$$$      print*,'beta=',beta
c$$$      print*,'xstart=',xstart
c$$$      print*,'ystart=',ystart
c$$$      print*,'delx=',delx
c$$$      print*,'dely=',dely

      read(20,*)
      read(20,*) xperiodic, yperiodic, spherical, cartesian
      read(20,*)
      read(20,*) fullio, suppressio
      read(20,*)
      read(20,*) initial_grad, grad_check, optimize, calc_hess

c      print*,'xperiodic,yperiodic,spherical,cartesian',
c     & xperiodic,yperiodic,spherical,cartesian
c      print*,'fullio,suppressio', fullio, suppressio

      read(20,*)
      read(20,'(A80)') ncdatafile
      read(20,*)
      read(20,'(A80)') ncrestartfile
      read(20,*)
      read(20,'(A80)') foutname
      read(20,*)
      read(20,'(A80)') runname
      read(20,*)
      read(20,*) wf_depth, wf_eta, wf_u, wf_v, wf_zonal_transport,
     &     wf_lapldepth, wf_graddepth

c      print*, foutname, runname, depthfile, forcingfile, uinifile
c      print*, vinifile, etainifile, datafile

      read(20,*)
      read(20,*) nsim
      read(20,*)
      read(20,*) epsg, df1, dxmin, niter, impres, mode
      read(20,*)
      read(20,*) eps_grad, pgtol, factr, iprint
      
      close(20)
c
c     end parameter file
c
c     some first checks for parameter consistency
c      if ( mod(nt,ntcp1) .ne. 0. ) then
c         print *, 'nt must be a multiple of ntcp1 =', ntcp1
c         stop 'in readparms'
c      end if
      if ( spherical .and. cartesian ) then
         print *, 'grid specification is ambiguous'
cju         stop 'in read_data_file' 
      else if ( .not. spherical .and. .not. cartesian ) then
         print *, 'grid specification is ambiguous'
cju         stop 'in read_data_file'
      end if
c
      if ( spherical .and. yperiodic ) then
         print *, 'spherical grid and periodic boundary conditions'
         print *, 'in latitude do not make sense'
cju         stop 'in read_data_file'
      end if
c     with periodic meridional boundary condition only an f-plane makes sense
      if ( yperiodic .and. ( spherical .or. 
     &     ( cartesian .and. beta .ne. 0 ) ) ) then
         print *, 'yperiodic boundaries only make sense on an f-plane'
cju         stop 'in read_data_file'
      end if
c
c     grid steps
c
      if ( spherical ) then
         do iy = 0, ny+1
            y(iy) = dely/earth*(real(iy)-.5)/deg2rad + ystart 
         end do
         do ix = 0, nx+1
            x(ix) = delx/earth*(real(ix)-.5)/deg2rad + xstart
         end do
      else if ( cartesian ) then
         do ix = 0, nx+1
            x(ix) = delx*(real(ix)-0.5)
         end do
         do iy = 0, ny+1
            y(iy) = dely*(real(iy)-0.5)
         end do
      end if
c     
      do ix = 0, nx
         dx(ix) = x(ix+1)-x(ix)
      end do
c
      do iy = 0, ny
         dy(iy) = y(iy+1)-y(iy)
      end do
c
c     once, x and y are set, these fields follow
c
      do iy = 1, ny
         if ( spherical ) then
            rx(iy) = earth*cos(.5*(y(iy)+y(iy+1))*deg2rad)*deg2rad
         else if ( cartesian ) then
            rx(iy) = 1.
         end if
      end do
c
      if ( spherical ) then
         ry = earth*deg2rad
      else if ( cartesian ) then
         ry = 1.
      end if
c
      do iy = 1, ny+1
         if ( spherical ) then
            hy(iy) = cos(y(iy)*deg2rad)
         else if ( cartesian ) then
            hy(iy) = 1.
         end if
      end do
c
c     
c     find out about data
c
      call determine_data_time( ncdatafile )
c
      return
      end !subroutine read_data_file

c$openad XXX Template OADrts/ad_template.split.f
      subroutine boundary_conditions( nx, ny, field, 
     &     xperiodic, yperiodic )
c     
c     implement boundary conditions, only makes sense for fields that are
c     defined like this: 0:nx+1, 0:ny+1
c     
      implicit none
c     interface
      integer nx, ny
      real field(0:nx+1,0:ny+1)
      logical xperiodic, yperiodic
c     end interface

c     locals
      integer ix, iy
      
      if ( xperiodic ) then
         do iy = 0, ny+1
            field(0,iy)    = field(nx,iy)
            field(nx+1,iy) = field(1,iy)
         end do
      else
         do iy = 0, ny+1
            field(0,iy)    = 0.
            field(nx+1,iy) = 0.
         end do
      end if
      if ( yperiodic ) then
         do ix = 0, nx+1
            field(ix,0)    = field(ix,ny)
            field(ix,ny+1) = field(ix,1)
         end do
      else
         do ix = 0, nx+1
            field(ix,0)    = 0.
            field(ix,ny+1) = 0.
         end do
      end if
      if ( xperiodic .and. yperiodic ) then
         print *, 'boundary_conditions: ',
     &        'make sure that the corners are handled correctly'
      end if

      return
      end !subroutine boundary_conditions

c$openad XXX Template OADrts/ad_template.split.f
      subroutine read_data_fields

      use size
      use parms
      use pfields
      use force

c
c     read depth, friction, and forcing from a file
c     likewise the initial conditions
c
      implicit none
!un #include "size.inc"
!un #include "parms.inc"
!un #include "pfields.inc"
!un #include "force.inc"
c     locals
      integer ix, iy
      real x_in(1:nx), y_in(1:ny)
      logical exists
!nt
      real :: mytime
      character*(80) :: str1, str2, str3
      integer mynx, myny
      mynx = nx ; myny = ny
      mytime = -1.

      do ix = 1, nx
         x_in(ix) = 0.
      end do
      do iy = 1, ny
         y_in(iy) = 0.
      end do

      inquire(file=ncdatafile,exist=exists)
      if ( .not. exists ) then
         print *, ncdatafile, ' not found, cannot continue'
cju         stop 'in read_data_fields'
      else
c$$$         call read_vector_netcdf( ncdatafile, 'X', nx, x_in )
c$$$         do ix = 1, nx
c$$$            x(ix) = x_in(ix)
c$$$         end do
c$$$         call read_vector_netcdf( ncdatafile, 'Y', ny, y_in )
c$$$         do iy = 1, ny
c$$$            y(iy) = y_in(i)
c$$$         end do
         str1 = 'depth' ; str2 = 'frict'
         call read_field( ncdatafile, mytime, str1, inidepth )
         call read_extended_field( ncdatafile, str2, frict )
         if ( rini .ne. 0 ) then
            print *, 'rini = ', rini
            print *, 'will overwrite frict with rini'
            do ix = 1, nx
               do iy = 1, ny
                  frict(ix,iy) = rini
               end do
            end do
         end if
         call boundary_conditions(mynx,myny,frict,xperiodic,yperiodic) 
c     forcing
         str1 = 'uforce' ; str2 = 'vforce'
         call read_field( ncdatafile, mytime, str1, uforce )
         call read_field( ncdatafile, mytime, str2, vforce )
c     initial conditions
         if ( start_time .eq. 0. ) then 
            print *, 'cold start from initial fields'
            str1 = 'uini' ; str2 = 'vini' ; str3 = 'etaini'
            call read_field( ncdatafile, start_time, str1, uini )
            call read_field( ncdatafile, start_time, str2, vini )
            call read_field( ncdatafile, start_time, str3, etaini )
         else 
            print *, 'warm restart from time ', start_time
            print *, 'in restart file ', ncrestartfile
            inquire( file = ncrestartfile, exist = exists ) 
            if ( .not. exists ) then
               print *, ncrestartfile, ' not found'
cju               stop 'in read_data_fields'
            else 
               str1 = 'U' ; str2 = 'V' ; str3 = 'ETA'
               call read_field( ncrestartfile, start_time, str1, uini )
               call read_field( ncrestartfile, start_time, str2, vini )
               call read_field( ncrestartfile, start_time, str3, 
     &              etaini )
            end if
         end if
      end if ! ncdatafile exists
c
      return
      end !subroutine read_data_fields

c$openad XXX Template OADrts/ad_template.split.f
      subroutine prep_depth

      use size
      use parms
      use pfields
      
c
c     prepare control variable depth, boundary conditions, etc. from inidepth
c     also scaling for control variable
c
      implicit none

!nt #include "size.inc"
!nt #include "parms.inc"
!nt #include "pfields.inc"

      integer ix, iy
      real maxdepth
 !nt
      integer mynx, myny
      mynx = nx ; myny = ny

c
c     prepare depth
c
      maxdepth = 0.
      do iy = 1, ny
         do ix = 1, nx
            if ( inidepth(ix,iy) .gt. maxdepth ) then
               maxdepth = inidepth(ix,iy)
            end if
         end do
      end do
      do ix = 1, nx
         do iy = 1, ny
            depth(ix,iy) = inidepth(ix,iy)
            scaledepth(ix,iy) = inidepth(ix,iy)
c     perturb initial depth a little to test algorithm
            if ( depth(ix,iy) .lt. maxdepth ) then
c               depth(ix,iy) = .9*(depth(ix,iy)-maxdepth)+maxdepth
            end if
         end do
      end do
c
c     assign values to data_depth
c
      call read_depth_data
c
c     I guess this is completely superfluous, since boundaries are handled
c     in calc_depth_uv for hu and hv
c
      call boundary_conditions(mynx,myny,depth,xperiodic,yperiodic) 
c
      return
      end !subroutine prep_depth

c$openad XXX Template OADrts/ad_template.split.f
      subroutine ini_scales

      use size
      use pfields

c
c     compute scales for initial conditions
c
      implicit none

!un #include "size.inc"
!un #include "pfields.inc"

      integer ix, iy
      real varu, varv, vareta
 !nt
      integer mynx, myny
      mynx = nx ; myny = ny

      call variance( mynx, myny, uini, umask, varu )
      if ( varu .eq. 0. ) then
         varu = 1.
      end if
      call variance( mynx, myny, vini, vmask, varv )
      if ( varu .eq. 0. ) then
         varv = 1.
      end if
      call variance( mynx, myny, etaini, etamask, vareta )
      if ( varu .eq. 0. ) then
         vareta = 1.
      end if
      do iy = 1, ny
         do ix = 1, nx
            scaleu(ix,iy)   = sqrt(varu)*umask(ix,iy)
            scalev(ix,iy)   = sqrt(varv)*vmask(ix,iy)
            scaleeta(ix,iy) = sqrt(vareta)*etamask(ix,iy)
         end do
      end do
c
      return
      end ! subroutine ini_scales

c$openad XXX Template OADrts/ad_template.split.f
      subroutine prep_coriolis

      use size
      use parms
      use pfields

      implicit none

!un #include "size.inc"
!un #include "parms.inc"
!un #include "pfields.inc"
c     locals
      integer ix, iy, my
      real faux
      real fcori(0:nx+1,0:ny+1)
c
      my = nint(real(ny)/2.)
c
c     coriolis
c
      do iy = 0, ny+1
         if ( spherical ) then
            faux = 2*om*sin(y(iy)*deg2rad)
         else if ( cartesian ) then
            faux = f0 + beta*(y(iy)-y(my))
         end if
         do ix = 0, nx+1
            fcori(ix,iy) = faux
         end do
      end do
c     now the two fields on u and v points
      do ix = 1, nx
         do iy = 1, ny
c     u point
            faux = vmask(ix,iy)   + vmask(ix-1,iy)
     &           + vmask(ix,iy+1) + vmask(ix-1,iy+1)
            if ( faux .eq. 0. ) then
               faux = 0.
            else
c               faux = 1./faux*umask(ix,iy)
               faux = .25*umask(ix,iy)
            end if
            fcoriu(ix,iy) = .5*( fcori(ix,iy) + fcori(ix-1,iy) )*faux
c     v point
            faux = umask(ix,iy)   + umask(ix+1,iy)
     &           + umask(ix,iy-1) + umask(ix+1,iy-1)
            if ( faux .eq. 0. ) then
               faux = 0.
            else
c               faux = 1./faux*vmask(ix,iy)
               faux = .25*vmask(ix,iy)
            end if
            fcoriv(ix,iy) = .5*( fcori(ix,iy) + fcori(ix,iy-1) )*faux
         end do
      end do
c
      return
      end !subroutine readparms

c$openad XXX Template OADrts/ad_template.split.f
      subroutine make_masks

      use size
      use parms
      use pfields

c     
c     land sea masking follows from depth
c
      implicit none
!un #include "size.inc"
!un #include "parms.inc"
!un #include "pfields.inc"

      integer ix, iy
      real mindepth
!nt
      integer mynx, myny
      mynx = nx ; myny = ny

      do ix = 1, nx
         do iy = 1, ny
!nt         if ( min(depth(ix,iy),depth(ix-1,iy)) .ne. 0 ) then
            if (depth(ix,iy) .lt. depth(ix-1,iy)) then 
               mindepth=depth(ix,iy)
            else 
               mindepth=depth(ix-1,iy)   
            endif
            if ( mindepth .ne. 0 ) then
               umask(ix,iy) = 1.
            else
               umask(ix,iy) = 0.
            end if
         end do
      end do
      do ix = 1, nx
         do iy = 1, ny
!nt         if ( min(depth(ix,iy),depth(ix,iy-1)) .ne. 0 ) then
            if ( depth(ix,iy) .lt. depth(ix,iy-1)) then 
               mindepth=depth(ix,iy)
            else 
               mindepth=depth(ix,iy-1)
            end if 
            if ( mindepth .ne. 0 ) then
               vmask(ix,iy) = 1.
            else
               vmask(ix,iy) = 0.
            end if
         end do
      end do
      do ix = 1, nx
         do iy = 1, ny
            if ( depth(ix,iy) .ne. 0 ) then
               etamask(ix,iy) = 1.
            else
               etamask(ix,iy) = 0.
            end if
         end do               
      end do
      call boundary_conditions(mynx,myny,umask,xperiodic,yperiodic)
      call boundary_conditions(mynx,myny,vmask,xperiodic,yperiodic)
c$$$      if ( xperiodic ) then
c$$$         do iy = 0, ny+1
c$$$            umask(0,iy)    = umask(nx,iy) ! not really necessary
c$$$            umask(nx+1,iy) = umask(1,iy)
c$$$c     for coriolis we need these
c$$$            vmask(0,iy)    = vmask(nx,iy)
c$$$            vmask(nx+1,iy) = vmask(1,iy)
c$$$         end do
c$$$      end if
c$$$      if ( yperiodic ) then
c$$$         do ix = 0, nx+1
c$$$            vmask(ix,0)    = vmask(ix,ny) ! not really necessary
c$$$            vmask(ix,ny+1) = vmask(ix,1)
c$$$c     for coriolis we need these
c$$$            umask(ix,0)    = umask(ix,ny)
c$$$            umask(ix,ny+1) = umask(ix,1)
c$$$         end do
c$$$      end if
c$$$      if ( xperiodic .and. yperiodic ) then
c$$$         print *, 'make sure the masks are set correctly'
c$$$      end if
c
      return
      end !subroutine make_masks

c$openad XXX Template OADrts/ad_template.split.f
      subroutine variance( nx, ny, f, fmask, varf )

      implicit none

c     interface
      integer nx, ny
      real f(nx,ny), fmask(0:nx+1,0:ny+1)
      real varf
c     end interface
c     locals
      integer k, ix, iy
      real meanf

      varf = 0.
      meanf = 0.
      k = 0
      do iy = 1, ny
         do ix = 1, nx
            if ( fmask(ix,iy) .ne. 0. ) then
               meanf = meanf + f(ix,iy)
               k = k+1
            end if
         end do
      end do
      if ( k .ne. 0 ) then
         meanf = meanf/real(k)
      end if
      do iy = 1, ny
         do ix = 1, nx
            varf = varf+(f(ix,iy)-meanf)**2*fmask(ix,iy)
         end do
      end do
      if ( k .gt. 1 ) then
         varf = varf/real(k-1)
      end if

      return
      end ! subroutine variance

c$openad XXX Template OADrts/ad_template.split.f
      subroutine check_cfl

      use size
      use parms
      use pfields

      implicit none
!un #include "size.inc"
!un #include "parms.inc"
!un #include "pfields.inc"

c     local variables
      integer ix, iy
      real mdep, mdx, mdy
      real cflx, cfly, wavespeed
      real minimum, maximum

      mdep = 0.
      mdx = 1.e23
      mdy = 1.e23

      do ix = 1, nx
!nt      mdx = min(mdx,dx(ix))
         if ( mdx .lt. dx(ix)) then 
            minimum=mdx
         else
            minimum=dx(ix)
         end if
         mdx = minimum
      end do
      do iy = 1, ny
!nt: should be mdy
!nt      mdy = min(mdx,dy(iy))
         if ( mdy .lt. dy(iy)) then 
            minimum=mdy
         else
            minimum=dy(iy)
         end if 
         mdy = minimum
      end do
      do ix = 1, nx
         do iy = 1, ny
!nt         mdep = max(mdep,depth(ix,iy))
            if (mdep .gt. depth(ix,iy) ) then 
               maximum=mdep
            else
               maximum=depth(ix,iy)
            end if 
            mdep = maximum
         end do
      end do
      wavespeed = sqrt(g*mdep)
      cflx = wavespeed*dt/mdx
      cfly = wavespeed*dt/mdy

      print *, 'rough check of CLF criterion:'
      if ( ( cflx .ge. 1. ) .or. ( cfly .ge. 1. ) ) then
         print *, 'warning: CLF criterion not met'
         print *, 'sqrt(g*max(depth))*dt/min(dx) = ', cflx
         print *, 'sqrt(g*max(depth))*dt/min(dy) = ', cfly
      else
         print *, 'OK'
      end if
      
      return
      end ! subroutine check_cfl

c$openad XXX Template OADrts/ad_template.split.f
      subroutine read_extended_field( ncdatafile, fname, field )

      use size

c     
c     wrapper for importing a data field (nx,ny) from netcdf file
c     as opposed to read_field, this subroutine deals with fields whose
c     dimensions are (0:nx+1) X (0:ny+1),
c     also time parameter is omitted
c     
      implicit none
!un #include "size.inc"
c     interface
      character*(80) ncdatafile, fname
      real field(0:nx+1,0:ny+1)
c     end interface
c     locals
      integer ix, iy
      real f_in(1:nx,1:ny)
      logical exists
!nt
      integer mynx, myny
      mynx = nx ; myny = ny

      do ix = 1, nx
         do iy = 1, ny
            f_in(ix,iy) = 0.
         end do
      end do
      call read_field_netcdf( ncdatafile, fname, mynx, myny, f_in )
      do ix = 1, nx
         do iy = 1, ny
            field(ix,iy) = f_in(ix,iy)
         end do
      end do

      return
      end !subroutine read_extended_field

c$openad XXX Template OADrts/ad_template.split.f
      subroutine read_field( ncdatafile, start_time, fname, field )

      use size

c     
c     wrapper for importing a data field (nx,ny) from netcdf file
c     
      implicit none
!un #include "size.inc"
c     interface
      character*(80) ncdatafile, fname
      real start_time
      real field(1:nx,1:ny)
c     end interface
c     locals
      integer ix, iy
      real f_in(1:nx,1:ny)
      logical exists
!nt
      integer :: mynx = nx, myny = ny
      
      do ix = 1, nx
         do iy = 1, ny
            f_in(ix,iy) = 0.
         end do
      end do
      if ( start_time .le. 0. ) then
         call read_field_netcdf( ncdatafile, fname, mynx, myny, f_in )
      else
         call read_snap_netcdf( ncdatafile, 
     &        start_time, mynx, myny, fname, f_in )
      end if
      do ix = 1, nx
         do iy = 1, ny
            field(ix,iy) = f_in(ix,iy)
         end do
      end do

      return
      end !subroutine read_field
