c$openad XXX Template OADrts/ad_template.split.f
      subroutine ini_io

      use size
      use parms
      use pfields
      use weights

      implicit none

!un #include "size.inc"
!un #include "parms.inc"
!un #include "pfields.inc"
!un #include "weights.inc"

      integer ix, iy
      real xout(nx), yout(ny)
!nt
      character*(80) :: str1, str2, str3, str4
      integer :: mynx, myny
      real :: myearth
      mynx = nx ; myny = ny
      myearth = earth

      if ( fullio ) then
         print *, 'initializing I/O'
      end if

      call create_netcdf(foutname,runname,mynx,myny)
      if ( spherical ) then
         str1 = 'grid_type' ; str2 = 'spherical'
         str3 = 'earth_radius' ; str4 = 'Omega'
         call add_gatta_netcdf(foutname,str1,str2)
         call add_gattr_netcdf(foutname,str3,myearth)
         call add_gattr_netcdf(foutname,str4,om)
      else if ( cartesian ) then
         str1 = 'grid_type' ; str2 = 'cartesian'
         str3 = 'f0' ; str4 = 'beta'
         call add_gatta_netcdf(foutname,str1,str2)
         call add_gattr_netcdf(foutname,str3,f0)
         call add_gattr_netcdf(foutname,str4,beta)
      end if
      str1 = 'r_ini' ; str2 = 'time_step'
      call add_gattr_netcdf(foutname,str1,rini)
      call add_gattr_netcdf(foutname,str2,dt)
      if ( xperiodic ) then
         str1 = 'zonal_boundary_conditions' ; str2 = 'periodic'
         call add_gatta_netcdf(foutname, str1, str2)
      end if
      if ( yperiodic ) then
         str1 = 'meridional_boundary_conditions' ; str2 = 'periodic'
        call add_gatta_netcdf(foutname, str1, str2)
      end if
      str1 = 'data_files'
      str2 = ncdatafile//' '//depthfile//' '//forcingfile//' '//
     &     uinifile//' '//vinifile//' '//etainifile
      call add_gatta_netcdf(foutname,str1,str2)
      if ( start_time .ne. 0 ) then
         str1 = 'restart_file'
         call add_gatta_netcdf(foutname,str1,ncrestartfile)
      end if
      str1 = 'ntspinup' ; str2 = 'wf_depth'
      str3 = 'wf_eta' ; str4 = 'wf_u'
      call add_gatti_netcdf(foutname,str1,ntspinup)
c     
      call add_gattr_netcdf(foutname,str2,wf_depth)
      call add_gattr_netcdf(foutname,str3,wf_eta)
      call add_gattr_netcdf(foutname,str4,wf_u)
      str1 = 'wf_v' ; str2 = 'wf_lapldepth'
      str3 = 'wf_graddepth' ; str4 = 'wf_zonal_transport'
      call add_gattr_netcdf(foutname,str1,wf_v)
      call add_gattr_netcdf(foutname,str2,wf_lapldepth)
      call add_gattr_netcdf(foutname,str3,wf_graddepth)
      call add_gattr_netcdf(foutname,str4,wf_zonal_transport)
c     
      do ix = 1, nx
         xout(ix) = x(ix)
      end do
      do iy = 1, ny
         yout(iy) = y(iy)
      end do
      if ( spherical ) then
         str1 = 'deg'
         call add_coordinates_netcdf(foutname,mynx,xout,myny,yout,str1)
      else if ( cartesian ) then
         str1 = 'meters'
         call add_coordinates_netcdf(foutname,mynx,xout,myny,yout,str1)
      end if
c     
      str1 = 'U' ; str2 = 'zonal velocity' ; str3 = 'meters/seconds'
      call add_recvar_netcdf(foutname, str1, str2, str3 )
      str1 = 'V' ; str2 = 'meridional velocity'
      call add_recvar_netcdf(foutname, str1, str2, str3 )
      str1 = 'ETA' ; str2 = 'sea-surface elevation' ; str3 = 'meters'
      call add_recvar_netcdf(foutname, str1, str2, str3 )

      return
      end ! ini_io

c$$$      subroutine adstate_io( time, nio )
c$$$
c$$$      implicit none
c$$$#include "size.inc"
c$$$#include "parms.inc"
c$$$#include "pfields.inc"
c$$$C==============================================
c$$$C define adjoint common blocks
c$$$C==============================================
c$$$      real adeta(0:nx+1,0:ny+1)
c$$$      real adu(0:nx+1,0:ny+1)
c$$$      real adv(0:nx+1,0:ny+1)
c$$$      common /advars/ adu, adv, adeta
c$$$
c$$$c     begin interface
c$$$      real time
c$$$      integer nio
c$$$c     end interface
c$$$      integer ix, iy
c$$$      real aduout(nx,ny), advout(nx,ny), adetaout(nx,ny)
c$$$
c$$$c
c$$$c     copy u, v, and eta to properly sized array, 
c$$$c     so that size(u) = [nx,ny]
c$$$c
c$$$      do ix = 1, nx
c$$$         do iy = 1, ny
c$$$            aduout(ix,iy) = adu(ix,iy)+real(int(umask(ix,iy))-1)*99.
c$$$            advout(ix,iy) = adv(ix,iy)+real(int(vmask(ix,iy))-1)*99.
c$$$            adetaout(ix,iy) =
c$$$     &           adeta(ix,iy)+real(int(etamask(ix,iy))-1)*99.
c$$$         end do
c$$$      end do
c$$$      call write_adstate_netcdf(foutname, nx, ny, nio, 'TIME', time)
c$$$      call write_adstate_netcdf(foutname, nx, ny, nio, 'ADU', aduout)
c$$$      call write_adstate_netcdf(foutname, nx, ny, nio, 'ADV', advout)
c$$$      call write_adstate_netcdf(foutname, nx, ny, nio, 'ADETA', adetaout)
c$$$c
c$$$c     stream function as an additional diagnostic, set the integration
c$$$c     to zero at the southern boundary, compute values at u-points
c$$$c
c$$$      return
c$$$      end                       ! adstate_io

c$openad XXX Template OADrts/ad_template.split.f
      subroutine state_io( time, nio )

      use size
      use parms
      use pfields
      use vars

      implicit none
!un #include "size.inc"
!un #include "parms.inc"
!un #include "pfields.inc"
!un #include "vars.inc"

c     begin interface
      real time
      integer nio
c     end interface
      integer ix, iy
      real uout(nx,ny), vout(nx,ny), etaout(nx,ny)
!nt
      character*(80) :: str1, str2
      integer mynx, myny
      mynx = nx ; myny = ny

c
c     copy u, v, and eta to properly sized array, 
c     so that size(u) = [nx,ny]
c
      do ix = 1, nx
         do iy = 1, ny
            uout(ix,iy) = u(ix,iy)+real(int(umask(ix,iy))-1)*99.
            vout(ix,iy) = v(ix,iy)+real(int(vmask(ix,iy))-1)*99.
            etaout(ix,iy) = eta(ix,iy)+real(int(etamask(ix,iy))-1)*99.
         end do
      end do
      str1 = 'TIME' ; str2 = 'U'
      call write_time_netcdf(foutname, nio, str1, time)
      call write_state_netcdf(foutname, mynx, myny, nio, str2, uout)
      str1 = 'V' ; str2 = 'ETA'
      call write_state_netcdf(foutname, mynx, myny, nio, str1, vout)
      call write_state_netcdf(foutname, mynx, myny, nio, str2, etaout)
c
c     stream function as an additional diagnostic, set the integration
c     to zero at the southern boundary, compute values at u-points
c
      return
      end                       ! state_io

c$openad XXX Template OADrts/ad_template.split.f
      subroutine pfields_io

      use size      
      use parms
      use pfields
      use force

      implicit none
!un #include "size.inc"      
!un #include "parms.inc"
!un #include "pfields.inc"
!un #include "force.inc"

      integer ix, iy
      real aux(nx,ny)
!nt
      character*(80) :: str1, str2, str3
      integer mynx, myny
      mynx = nx ; myny = ny

      do ix = 1, nx
         do iy = 1, ny
            aux(ix,iy) = depth(ix,iy)
         end do
      end do
      str1 = 'depth' ; str2 = 'water depth' ; str3 = 'meters'
      call add_pfield_netcdf(foutname, mynx, myny, aux,
     &     str1, str2, str3 )
      do ix = 1, nx
         do iy = 1, ny
            aux(ix,iy) = uforce(ix,iy)
         end do
      end do
      str1 = 'uforce' ; str2 = 'zonal forcing' ; str3 = 'forcing units'
      call add_pfield_netcdf(foutname, mynx, myny, aux, str1,
     &     str2, str3 )
      do ix = 1, nx
         do iy = 1, ny
            aux(ix,iy) = vforce(ix,iy)
         end do
      end do
      str1 = 'vforce' ; str2 = 'meridional forcing' 
      str3 = 'forcing units'
      call add_pfield_netcdf(foutname, mynx, myny, aux, str1,
     &     str2, str3 )
      do ix = 1, nx
         do iy = 1, ny
            aux(ix,iy) = frict(ix,iy)
         end do
      end do
      str1 = 'frict' ; str2 = 'linear bottom friction coefficient'
      str3 = '1/seconds'
      call add_pfield_netcdf(foutname, mynx, myny, aux, str1,
     &     str2, str3 )

      return
      end ! pfields_io

c$openad XXX Template OADrts/ad_template.split.f
      subroutine save_gradient_io( n, adxc, gname )
c
c     this routine maps the gradient vector to a 2D field, the map is
c     defined by map_from_control_vector and should always be compared
c     to that routine!
c

      use size
      use parms
      use pfields

      implicit none
!un #include "size.inc"
!un #include "parms.inc"
!un #include "pfields.inc"

c     interface
      integer n
      real adxc(n)
      character*(80) gname
c     end interface
c     local variables
      real grad(nx,ny)
!nt
      character*(80) :: str1, str2
      integer mynx, myny
      mynx = nx ; myny = ny

      call map_gradient( n, adxc, grad )
      str1 = 'gradient of cost function with respect to depth'
      str2 = 'cost function units/m'
      call add_pfield_netcdf(foutname, mynx, myny, grad,
     &     gname, str1, str2 )

c     
c     suppress any further io to netcdf file to avoid overwriting
c     
      suppressio = .true.

      return
      end ! subroutine save_gradient_io

c$openad XXX Template OADrts/ad_template.split.f
      subroutine save_depth_io( n, xc, dname )
      
      use size
      use parms
      use pfields

      implicit none
!un #include "size.inc"
!un #include "parms.inc"
!un #include "pfields.inc"

c     interface
      integer n
      real xc(n)
      character*(80) dname
c     end interface
c     local variables
      integer ix, iy
      real aux(nx,ny)
!nt
      character*(80) :: str1, str2
      integer mynx, myny
      mynx = nx ; myny = ny

      call map_from_control_vector( n, xc )
      do ix = 1, nx
         do iy = 1, ny
            aux(ix,iy) = depth(ix,iy)
         end do
      end do
      str1 = 'water depth after optimization' ; str2 = 'm'
      call add_pfield_netcdf(foutname, mynx, myny, aux,
     &     dname, str1, str2 )

c     
c     suppress any further io to netcdf file
c     
      suppressio = .true.

      return
      end ! subroutine save_depth_io

c$openad XXX Template OADrts/ad_template.split.f
      subroutine inimini_io

      implicit none

      end ! subroutine inimin_io

c$openad XXX Template OADrts/ad_template.split.f
      subroutine save_weights_io

      use size
      use parms
      use weights

      implicit none

!nt #include "size.inc"
!nt #include "parms.inc"
!nt #include "weights.inc"
!nt
      character*(80) :: str1

      str1 = 'wf_depth'
      call add_gattr_netcdf(foutname,str1,wf_depth)
      str1 = 'wf_eta'
      call add_gattr_netcdf(foutname,str1,wf_eta)
      str1 = 'wf_zonal_transport'
      call add_gattr_netcdf(foutname,str1, 
     &     wf_zonal_transport)

      end ! subroutine save_weight_io
