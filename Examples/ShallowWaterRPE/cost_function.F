c$openad XXX Template OADrts/ad_template.split.f
      subroutine cost_function( time, cf, local_u, local_v, local_eta )
c
c     specify your cost function here
c     
      use size
      use vars
      use data
      use weights

      implicit none
cju #include "size.inc"
cju #include "vars.inc"
c#include "pfields.inc"
cju #include "weights.inc"
cju #include "data.inc"
      
c     interface
      real time, cf
c     end interface

      real, intent(in) :: local_u(0:nx+1,0:ny+1)
      real, intent(in) :: local_v(0:nx+1,0:ny+1)
      real, intent(in) :: local_eta(0:nx+1,0:ny+1)

c     local variables
      integer ix, iy
c     zonal volume transport
      real zonal_transport
c
cju      logical is_eta_data_time
cju      external is_eta_data_time

      logical result
c
c     load the data
c
      call is_eta_data_time( time ,result)
      if ( result ) then
         call read_data( time )
      end if
c     
c     calculate cost function, if there is only one data slab, or if
c     the time step matches a data time
c     
      call is_eta_data_time( time ,result)
      if ( nedt .eq. -1 .or. result ) then
c
c     calculate cost function terms
c

c     zonal transport 
cju in the code for calc_zonal_transport the v and eta 
cju references had always been commented out
cju         call calc_zonal_transport_split( zonal_transport, local_u, local_v,
cju     &local_eta )
         call calc_zonal_transport_split( zonal_transport, local_u )
c$$$         cf = cf + zonal_transport*wf_zonal_transport
         cf = cf + .5*(zonal_transport-zonal_transport_data)**2
     &        *weight_zonal_transport
c
         do iy = 1, ny
            do ix = 1, nx
c     sea surface height
               cf = cf + .5*(local_eta(ix,iy)-eta_data(ix,iy))**2
     &              *weight_eta(ix,iy)
c     velocity
               cf = cf + .5*(local_u(ix,iy)-u_data(ix,iy))**2
     &              *weight_u(ix,iy)
               cf = cf + .5*(local_v(ix,iy)-v_data(ix,iy))**2
     &              *weight_v(ix,iy)
            end do
         end do

      end if

      return
      end ! subroutine cost_function
