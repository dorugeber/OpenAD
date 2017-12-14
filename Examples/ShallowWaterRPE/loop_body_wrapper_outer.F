c$openad XXX Template OADrts/ad_template.joint_split_oif.f
      subroutine loop_body_wrapper_outer(local_u,local_v,local_eta,cost,
     & calc_cost, time, time_index, nio, it, ntotal, ninner) 

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
      integer time_index, nio, it, ntotal, ninner 

      integer jt

      do jt = 1, ninner
         time_index = (it-1)*ninner+jt
         time = start_time+time_index*dt 
         if ( time_index .le. ntotal ) then
c            print *, 'inner index', jt
            call loop_body_wrapper_inner(local_u, local_v, local_eta,  
     & cost, calc_cost, time, time_index, nio) 
         end if
      end do
      end subroutine
