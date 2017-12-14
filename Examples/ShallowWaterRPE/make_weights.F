c$openad XXX Template OADrts/ad_template.split.f
      subroutine make_weights

      implicit none

ctest      call make_weights_depth
      call make_weights_eta
      call make_weights_uv
      call make_weights_zonal_transport
      call make_weights_lapldepth
      call make_weights_graddepth

      end !subroutine make_weights

c$openad XXX Template OADrts/ad_template.split.f
      subroutine make_weights_depth

      use size
      use pfields
      use weights

      implicit none

!un #include "size.inc"
!un #include "pfields.inc"
!un #include "weights.inc"

c     locals
      integer ix, iy, jx, jy, nflag
      real offdiag
      real error_depth
!nt
      integer :: mynx = nx, myny = ny
      character*(80) :: strwd 
      character*(80) :: strW
      strwd = 'weights_depth.nc'
      strW = 'W'

      nflag = 0
ctest      call read_weight_depth_netcdf( strwd, strW, 
ctest     &     weight_depth, mynx, myny, nflag )
c     this is the backup matrix
#ifdef CTEST 
      if ( nflag .lt. 0 ) then
         offdiag = 0.0
         do iy = 1, ny
            do ix = 1, nx
               weight_depth(ix,ix,iy,iy) = 1.
               if ( ix .gt. 1 ) then
                  weight_depth(ix,ix-1,iy,iy) = offdiag
               end if
               if ( ix .lt. nx ) then
                  weight_depth(ix,ix+1,iy,iy) = offdiag
               end if
               if ( iy .gt. 1 ) then
                  weight_depth(ix,ix,iy,iy-1) = offdiag
               end if
               if ( iy .lt. ny ) then
                  weight_depth(ix,ix,iy,iy+1) = offdiag
               end if
            end do
         end do
      end if
c     scale the matrix by error_depth and weight_factor
      error_depth = 1.
      do iy = 1, ny
         do jy = 1, ny
            do ix = 1, nx
               do jx = 1, nx
                  weight_depth(ix,jx,iy,jy) = weight_depth(ix,jx,iy,jy)
     &                 *wf_depth*(1./error_depth)**2
     &                 *etamask(ix,iy)*etamask(jx,jy)
               end do
            end do
         end do
      end do
#endif /* CTEST */
c$$$c     print out the matrix
c$$$      open(10,file='weights_depth.asc');
c$$$      do iy = 1, ny
c$$$         do ix = 1, nx
c$$$            write(10,*) '(nx,ny) = (',ix,',',iy,')'
c$$$            write(10,'(20ES12.4)') 
c$$$     &           ((weight_depth(ix,jx,iy,jy),jx=1,nx),jy=1,ny)
c$$$         end do
c$$$      end do
c$$$      close(10)

      end !subroutine make_weights_depth

c$openad XXX Template OADrts/ad_template.split.f
      subroutine make_weights_eta

      use size
      use pfields
      use weights

      implicit none

!un #include "size.inc"
!un #include "pfields.inc"
!un #include "weights.inc"

c     locals
      integer ix, iy
      real error_eta

      error_eta = 1.
c$openad xxx simple loop
      do iy = 1, ny
         do ix = 1, nx
            weight_eta(ix,iy) = 
     &           wf_eta*(1./error_eta)**2*etamask(ix,iy)
         end do
      end do

      end !subroutine make_weights_eta

c$openad XXX Template OADrts/ad_template.split.f
      subroutine make_weights_uv

      use size
      use pfields
      use weights

      implicit none

!un #include "size.inc"
!un #include "pfields.inc"
!un #include "weights.inc"

c     locals
      integer ix, iy
      real error_u, error_v

      error_u = 1.
      error_v = 1.
c$openad xxx simple loop
      do iy = 1, ny
         do ix = 1, nx
            weight_u(ix,iy) = 
     &           wf_u*(1./error_u)**2*umask(ix,iy)
            weight_v(ix,iy) = 
     &           wf_v*(1./error_v)**2*vmask(ix,iy)
         end do
      end do

      end !subroutine make_weights_eta

c$openad XXX Template OADrts/ad_template.split.f
      subroutine make_weights_zonal_transport

      use size
      use weights

      implicit none
!un #include "size.inc"
!un #include "weights.inc"

c     locals
      real error_zonal_transport
      
      error_zonal_transport = 1.e+06
      weight_zonal_transport = 
     &     wf_zonal_transport*(1./error_zonal_transport)**2

      end !subroutine make_weights_zonal_transport

c$openad XXX Template OADrts/ad_template.split.f
      subroutine make_bounds_for_x( nc, lower_bound, upper_bound )

      use size
      use pfields

      implicit none

!un #include "size.inc"
!un #include "pfields.inc"

c     interface
      integer nc
      real lower_bound(nc), upper_bound(nc)
c     end interface

      integer ix, iy, k
      real ub, lb

      ub =  50. ! meter
      lb = -50. ! meter

c$openad xxx simple loop
      do iy = 1, ny
         do ix = 1, nx
            if ( etamask(ix,iy) .ne. 0 ) then
               k = k+1
               upper_bound(k) = ub/inidepth(ix,iy)
               lower_bound(k) = lb/inidepth(ix,iy)
            end if
         end do
      end do

      end ! subroutine make_bounds_x

c$openad XXX Template OADrts/ad_template.split.f
      subroutine make_weights_lapldepth

      use size
      use pfields
      use weights

      implicit none

!un #include "size.inc"
!un #include "pfields.inc"
!un #include "weights.inc"

c     locals
      integer ix, iy
      real error

      error = 1.
c$openad xxx simple loop
      do iy = 1, ny
         do ix = 1, nx
            weight_lapldepth(ix,iy) = 
     &           wf_lapldepth*(1./error)**2*etamask(ix,iy)
         end do
      end do

      end ! subroutine make_weights_lapldepth

c$openad XXX Template OADrts/ad_template.split.f
      subroutine make_weights_graddepth

      use size
      use pfields
      use weights

      implicit none

!un #include "size.inc"
!un #include "pfields.inc"
!un #include "weights.inc"

c     locals
      integer ix, iy
      real error

      error = 1.
c$openad xxx simple loop
      do iy = 1, ny
         do ix = 1, nx
            weight_graddepth(ix,iy) = 
     &           wf_graddepth*(1./error)**2*etamask(ix,iy)
         end do
      end do

      end ! subroutine make_weights_graddepth
