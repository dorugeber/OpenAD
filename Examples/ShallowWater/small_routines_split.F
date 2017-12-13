c$openad XXX Template OADrts/ad_template.split.f
      subroutine time_step( it, local_u, local_v, local_eta )

      use size
      use vars

      implicit none

c     begin interface
      integer it
c     end interface

      real local_u(0:nx+1,0:ny+1)
      real local_v(0:nx+1,0:ny+1)
      real local_eta(0:nx+1,0:ny+1)

cju #include "size.inc"
cju #include "vars.inc"

!cadj init tape_level_aux = common, 1
c     alternate evaluation of momentum equations, because coriolis is
c     treated explicitly
      if ( mod(it,2) .ne. 0 ) then
!cadj store u = tape_level_aux
         call umomentum(local_u, local_v, local_eta)
         call vmomentum(local_u, local_v, local_eta)
      else 
!cadj store v = tape_level_aux
         call vmomentum(local_u, local_v, local_eta)
         call umomentum(local_u, local_v, local_eta)
      end if
c
c     continuity equation calculates eta
c
      call continuity(local_u, local_v, local_eta)

      return
      end ! subroutine time_step

c$openad XXX Template OADrts/ad_template.split.f
      subroutine umomentum(local_u,local_v, local_eta)

      use size
      use parms
      use vars
      use pfields
      use force

      implicit none

      real, intent(inout) :: local_u(0:nx+1,0:ny+1)
      real, intent(in) :: local_v(0:nx+1,0:ny+1)
      real, intent(in) :: local_eta(0:nx+1,0:ny+1)


cju #include "size.inc"
cju #include "parms.inc"
cju #include "vars.inc"
cju #include "pfields.inc"
cju #include "force.inc"

c     local variables
      integer ix, iy
      real frictu
      real gradetau
      real fv
c
c     evaluate u momentum equation
c
c$openad xxx simple loop
      do iy = 1, ny
         do ix = 1, nx
            frictu = .5*(frict(ix,iy)+frict(ix-1,iy))*invhu(ix,iy)
            gradetau = (local_eta(ix,iy)-local_eta(ix-1,iy))
     &           /(rx(iy)*.5*(dx(ix)+dx(ix-1)))
c     factor .25 is contained in fcoriu
            fv = fcoriu(ix,iy)*( local_v(ix,iy)  + local_v(ix-1,iy)
     &                         + local_v(ix,iy+1)+ local_v(ix-1,iy+1))
            local_u(ix,iy) = umask(ix,iy)/(1+0.5*frictu*dt)*(
     &           (1-.5*frictu*dt)*local_u(ix,iy) - g*dt*gradetau + dt*fv 
     &           + dt*uforce(ix,iy)*invhu(ix,iy)
     &           )
         end do
      end do

c     handle domain boundaries
      if ( xperiodic ) then
c$openad xxx simple loop
         do iy = 0, ny+1
            local_u(nx+1,iy) = local_u(1,iy)
         end do
      end if
      if ( yperiodic ) then
c     for coriolis term in v equation
c$openad xxx simple loop
         do ix = 0, nx+1
            local_u(ix,0) = local_u(ix,ny)
         end do
      end if
c
      end ! subroutine umomentum

c$openad XXX Template OADrts/ad_template.split.f
      subroutine vmomentum(local_u, local_v, local_eta)

      use size
      use parms
      use vars
      use pfields
      use force

      implicit none

      real, intent(in) :: local_u(0:nx+1,0:ny+1)
      real, intent(inout) :: local_v(0:nx+1,0:ny+1)
      real, intent(in) :: local_eta(0:nx+1,0:ny+1)

cju #include "size.inc"
cju #include "parms.inc"
cju #include "vars.inc"
cju #include "pfields.inc"
cju #include "force.inc"

      integer ix, iy
      real frictv
      real gradetav
      real fu
c
c     evaluate v momentum equation
c
c$openad xxx simple loop
      do iy = 1, ny
         do ix = 1, nx
            frictv = .5*(frict(ix,iy)+frict(ix,iy-1))*invhv(ix,iy)
            gradetav = (local_eta(ix,iy)-local_eta(ix,iy-1))
     &           /(ry*.5*(dy(iy)+dy(iy-1)))
c     factor .25 is contained in fcoriv
            fu = fcoriv(ix,iy)*( local_u(ix,iy)  + local_u(ix+1,iy)
     &                         + local_u(ix,iy-1)+ local_u(ix+1,iy-1))
            local_v(ix,iy) = vmask(ix,iy)/(1+0.5*frictv*dt)*(
     &           (1-.5*frictv*dt)*local_v(ix,iy) - g*dt*gradetav - dt*fu
     &           + dt*vforce(ix,iy)*invhv(ix,iy)
     &           )
         end do
      end do
c     handle domain boundaries
      if ( xperiodic ) then
c$openad xxx simple loop
         do iy = 0, ny+1
c     for coriolis term in u equation
            local_v(0,iy) = local_v(nx,iy)
         end do
      end if
      if ( yperiodic ) then
c$openad xxx simple loop
         do ix = 0, nx+1
            local_v(ix,ny+1) = local_v(ix,1)
         end do
      end if
c
      end ! subroutine vmomentum

c$openad XXX Template OADrts/ad_template.split.f
      subroutine continuity(local_u, local_v, local_eta)
      
      use size
      use parms
      use vars
      use pfields

      implicit none

      real, intent(in) :: local_u(0:nx+1,0:ny+1)
      real, intent(in) :: local_v(0:nx+1,0:ny+1)
      real, intent(inout) :: local_eta(0:nx+1,0:ny+1)

cju #include "size.inc"
cju #include "parms.inc"
cju #include "pfields.inc"
cju #include "vars.inc"
      
      integer ix, iy

c     eta equation
c$openad xxx simple loop
      do iy = 1, ny
         do ix = 1, nx
            local_eta(ix,iy) = local_eta(ix,iy) - etamask(ix,iy)*dt*(
     &        (hu(ix+1,iy)*local_u(ix+1,iy) - hu(ix,iy)*local_u(ix,iy))
     &           /(rx(iy)*dx(ix))
     &           + ( hy(iy+1)* hv(ix,iy+1)*local_v(ix,iy+1) 
     &             - hy(iy)  * hv(ix,iy)  *local_v(ix,iy)  )
     &           /(ry*dy(iy))
     &           )
         end do
      end do
c     handle domain boundaries
      if ( xperiodic ) then
c$openad xxx simple loop
         do iy = 0, ny+1
            local_eta(0,iy) = local_eta(nx,iy)
         end do
      end if
      if ( yperiodic ) then
c$openad xxx simple loop
         do ix = 0, nx+1
            local_eta(ix,0) = local_eta(ix,ny)
         end do
      end if

      return
      end ! subroutine continuity

c$openad XXX Template OADrts/ad_template.split.f
      subroutine calc_overturning( overturning, local_u)
c
c     returns the (linearized) overturning tranport 
c
      use size
      use vars
      use pfields

      implicit none
c     interface
      real overturning
c     end interface

      real, intent(in) :: local_u(0:nx+1,0:ny+1)

cju #include "size.inc"
cju #include "pfields.inc"
cju #include "vars.inc"
c     locals
      integer ix
      integer iy
      parameter ( ix = 7 ) 
cju      ix = 7
      overturning = 0.
c$openad xxx simple loop
      do iy = 1, ny/2
         overturning = overturning
     &        + local_u(ix,iy)*dy(iy)*hu(ix,iy)
      end do

      return
      end !subroutine calc_overturning

c$openad XXX Template OADrts/ad_template.split.f
      subroutine calc_zonal_transport_split( zonal_transport,local_u )
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
