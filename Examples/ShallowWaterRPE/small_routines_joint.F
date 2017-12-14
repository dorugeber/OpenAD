c$openad XXX Template OADrts/ad_template.joint.f
      subroutine initial_values

      use size
      use parms
      use vars
      use pfields

      implicit none

cju #include "size.inc"
cju #include "parms.inc"
cju #include "vars.inc"
cju #include "pfields.inc"

      integer ix, iy

c$openad xxx simple loop
      do iy = 1, ny
         do ix = 1, nx
            u(ix,iy)   = uini(ix,iy)*umask(ix,iy)
            v(ix,iy)   = vini(ix,iy)*vmask(ix,iy)
            eta(ix,iy) = etaini(ix,iy)*etamask(ix,iy)
         end do
      end do
c     handle domain boundaries
      if ( xperiodic ) then
c$openad xxx simple loop
         do iy = 0, ny+1
            u(nx+1,iy) = u(1,iy)
            v(0,iy)    = v(nx,iy)
            eta(0,iy)  = eta(nx,iy)
         end do
      end if
      if ( yperiodic ) then
c$openad xxx simple loop
         do ix = 0, nx+1
            u(ix,0)    = u(ix,ny)
            v(ix,ny+1) = v(ix,1)
            eta(ix,0)  = eta(ix,0)
         end do
      end if

      return
      end !subroutine initial_values

c$openad XXX Template OADrts/ad_template.joint.f
      subroutine calc_depth_uv

      use size
      use pfields

      implicit none

cju #include "size.inc"
cju #include "pfields.inc"

      integer ix, iy

c     create depth at u and v points
c$openad xxx simple loop
      do iy = 1, ny+1
         do ix = 1, nx+1

            hu(ix,iy) = depth(ix,iy)*umask(ix,iy)
            hv(ix,iy) = depth(ix,iy)*vmask(ix,iy)

            if ( hu(ix,iy) .ne. 0. ) then
               invhu(ix,iy) = 1./hu(ix,iy)
            end if
            if ( hv(ix,iy) .ne. 0. ) then
               invhv(ix,iy) = 1./hv(ix,iy)
            end if
         end do
      end do

c$$$      open(20,file='huhv.dat',form='formatted',recl=102*20)      
c$$$      write(20,'(102E20.12)') ((hu(ix,iy),ix=0,nx+1),iy=ny+1,0,-1)
c$$$      write(20,'(102E20.12)') ((hu(ix,iy),ix=0,nx+1),iy=ny+1,0,-1)
c$$$      close(20)

      return
      end

c$openad XXX Template OADrts/ad_template.joint.f
      subroutine calc_zonal_transport_joint( zonal_transport,local_u )
c
c     returns the (linearized) zonal volume tranport through the 
c     western face of the ix'th grid box column
c
      use size
      use vars
      use pfields

      implicit none
c     interface
      real zonal_transport
c     end interface

      real, intent(in) :: local_u(0:nx+1,0:ny+1)

cju #include "size.inc"
cju #include "pfields.inc"
cju #include "vars.inc"
c     locals
      integer ix
      integer iy

      parameter ( ix = 6 )
cju      ix = 6
      zonal_transport = 0.
c$openad xxx simple loop
      do iy = 1, ny
         zonal_transport = zonal_transport 
     &        + local_u(ix,iy)*dy(iy)*hu(ix,iy)
c     &       + local_u(ix,iy)*dy(iy)*( eta(ix-1,iy)+eta(ix,iy) )
      end do

      return
      end !subroutine zonal_transport

c$openad XXX Template OADrts/ad_template.joint.f
      subroutine smoothness_lapldepth( cf )

      use size
      use parms
      use pfields
      use weights

      implicit none
c     interface
      real cf
c     end interface
cju #include "size.inc"
cju #include "parms.inc"
cju #include "pfields.inc"
cju #include "weights.inc"

c     locals
      integer ix, iy
      real dfdx(nx,ny), ddfddx(nx,ny)
      real dfdy(nx,ny), ddfddy(nx,ny)
      real lapldepth

c     first derivative: forward differences
c$openad xxx simple loop
      do iy = 1, ny
         do ix = 1, nx-1
            dfdx(ix,iy) = (depth(ix+1,iy)-depth(ix,iy))/dx(ix+1)
         end do
         if ( xperiodic ) then
            dfdx(nx,iy) = (depth(1,iy)-depth(nx,iy))/dx(1)
         else
            dfdx(nx,iy) = 0.
         end if
      end do
c
c$openad xxx simple loop
      do ix = 1, nx
         do iy = 1, ny-1
            dfdy(ix,iy) = (depth(ix,iy+1)-depth(ix,iy))/dy(iy+1)
         end do
         if ( yperiodic ) then
            dfdy(ix,ny) = (depth(ix,1)-depth(ix,ny))/dy(1)
         else
            dfdy(ix,ny) = 0.
         end if
      end do
c     second derivatives: backward differences
c$openad xxx simple loop
      do iy = 1, ny
         if ( xperiodic ) then
            ddfddx(1,iy) = (dfdx(1,iy)-dfdx(nx,iy))/dx(1)
         else
            ddfddx(1,iy) = 0.
         end if
         do ix = 2, nx
            ddfddx(ix,iy) = (dfdx(ix,iy)-dfdx(ix-1,iy))/dx(ix)
         end do
      end do
c
c$openad xxx simple loop
      do ix = 1, nx
         if ( yperiodic ) then
            ddfddy(ix,1) = (dfdy(ix,1)-dfdy(ix,ny))/dy(1)
         else
            ddfddy(ix,1) = 0.
         end if
         do iy = 2, ny
            ddfddy(ix,iy) = (dfdy(ix,iy)-dfdy(ix,iy-1))/dy(iy)
         end do
      end do
c$openad xxx simple loop
      do iy = 1, ny
         do ix = 1, nx
            lapldepth = ddfddx(ix,iy) + ddfddy(ix,iy)
            cf = cf + .5*lapldepth*lapldepth*weight_lapldepth(ix,iy)
         end do
      end do

      return
      end ! subroutine smoothness_lapldepth
c
c$openad XXX Template OADrts/ad_template.joint.f
      subroutine smoothness_graddepth( cf )

      use size
      use parms
      use pfields
      use weights

      implicit none
c     interface
      real cf
c     end interface
cju #include "size.inc"
cju #include "parms.inc"
cju #include "pfields.inc"
cju #include "weights.inc"

c     locals
      integer ix, iy
      real dfdx(nx,ny)
      real dfdy(nx,ny)
      real graddepth

c     first derivative: backward differences
c$openad xxx simple loop
      do iy = 1, ny
         if ( xperiodic ) then
            dfdx(1,iy) = (depth(1,iy)-depth(nx,iy))/dx(1)
         else
            dfdx(1,iy) = 0.
         end if
         do ix = 2, nx
            dfdx(ix,iy) = (depth(ix,iy)-depth(ix-1,iy))/dx(ix)
         end do
      end do
c
c$openad xxx simple loop
      do ix = 1, nx
         if ( yperiodic ) then
            dfdy(ix,1) = (depth(ix,1)-depth(ix,ny))/dy(1)
         else
            dfdy(ix,1) = 0.
         end if
         do iy = 2, ny
            dfdy(ix,iy) = (depth(ix,iy)-depth(ix,iy-1))/dy(iy)
         end do
      end do
c
c$openad xxx simple loop
      do iy = 1, ny
         do ix = 1, nx
            cf = cf + .5*weight_graddepth(ix,iy)*
     &           ( .5*dfdx(ix,iy)*dfdx(ix,iy)*umask(ix,iy)
     &           + .5*dfdy(ix,iy)*dfdy(ix,iy)*vmask(ix,iy) )
         end do
      end do

      return
      end ! subroutine smoothness_graddepth
