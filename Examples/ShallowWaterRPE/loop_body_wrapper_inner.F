c$openad XXX Template OADrts/ad_template.joint_split_iif.f
      subroutine loop_body_wrapper_inner(local_u,local_v,local_eta,cost,
     & calc_cost, time, time_index, nio) 

      use size
      use parms
      use vars
      use pfields
      use force
      use data
      use weights
      
      implicit none

      real local_u(0:nx+1,0:ny+1)
      real local_v(0:nx+1,0:ny+1)
      real local_eta(0:nx+1,0:ny+1)
      real cost
      logical calc_cost
      real time
      integer time_index, nio 
c
cadj store u,v,eta = tape_level_2

cph(
               call time_step( time_index, local_u, local_v, local_eta )
cph)

               if ( ( time_index .gt. ntspinup ) .and. calc_cost ) then
cph(
                  call cost_function( time , cost , local_u, local_v, 
     &local_eta)
cph)
               end if
c
c     after stepping the model forward, store the dynamic variables
c
               if ( fullio .and. .not. suppressio .and.
     &              ( mod(real(time_index)*dt,dt_dump) .eq. 0. ) ) then
                  nio = nio + 1
                  print *, 'Writing Time Step ', time_index
cph(
cph this contains netcdf stuff
cph                  call state_io( time, nio )
cph)
               end if
      end subroutine
