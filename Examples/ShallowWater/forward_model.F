cadj subroutine ini_io
cadj subroutine ini_io input = 
cadj subroutine ini_io output = 
cadj subroutine pfields_io
cadj subroutine pfields_io input = 
cadj subroutine pfields_io output = 
cadj subroutine state_io
cadj subroutine state_io input = 1, 2
cadj subroutine state_io output =
cadj subroutine determine_data_time
cadj subroutine determine_data_time input = 1
cadj subroutine determine_data_time output =
cadj function is_eta_data_time
cadj function is_eta_data_time input = 1
cadj subroutine read_eta_data
cadj subroutine read_eta_data input = 1
cadj subroutine read_eta_data output = 1
!cadj subroutine read_eta_data adname = read_eta_data
!cadj subroutine read_eta_data ftlname = read_eta_data



c$openad XXX Template OADrts/ad_template_timing.joint.f
       subroutine forward_model( nctrl, xc, cost_final )

      use size
      use parms
      use vars
      use pfields
      use force
      use data
      use weights

      implicit none

cju #include "size.inc"
cju #include "parms.inc"
cju #include "vars.inc"
cph(
cju #include "pfields.inc"
cju #include "weights.inc"
cju #include "data.inc"
cju #include "force.inc"
cph)

c     interface
      integer nctrl
      real xc(nctrl)
      real cost_final
c     end interface

c     locals
      integer time_index, ntotal
      integer it, jt, nio 
      logical calc_cost
      real cost_d, cost_sd, cost_gd, cost, time
c     zonal volume transport
      real zonal_transport
c     checkpoint frequency for tamc code
      integer nouter, ninner
      real routin
cph new from inlining
      integer ix, iy, jx, jy
      integer k
      real frictu, frictv
      real gradetau, gradetav
      real fv, fu
cju      logical is_eta_data_time
cju      external is_eta_data_time
      real one
      parameter ( one = 1. )
c 
c     we will need to store the dynamic variables u, v, and eta, so they
c     do not have to be recomputed during the adjoint calculations, so far
c     use two level checkpointing, intialize the outer tape (first tape 
c     level) here to avoid unecessary recomputation of the forward model.
c 
cadj init tape_level_1 = 'state'
c
c     determine whether to calculate cost function ( can save some time )
c      

c$openad INDEPENDENT(xc)

      if ( initial_grad  .or. grad_check .or. optimize
     &     .or. calc_hess ) then
         calc_cost = .true.
      else
         calc_cost = .false.
      end if
c
c     initial local variables
c
      ninner = 500 
      cost_d = 0.
      cost_sd = 0.
      cost_gd = 0.
      cost = 0.
c
cph(
      call map_from_control_vector( nctrl, xc )
cph)
c     
c     calculate depth at the faces of the grid points ( u and v points )
c
cph(
      call calc_depth_uv
cph)
c
c     initialize dynamic variables
c
cph(
      call initial_values
cph)
c     
c     initialize I/O
      if ( .not. suppressio )  then
cph(
cph these contains netcdf stuff for diagnostics. Can be avoided for OpenAD.
cph         call ini_io
cph         call pfields_io
cph)
      end if
c     first cost function contribution is the time independend variable
c     depth
      if ( calc_cost ) then
cph(
         call cost_depth( cost_d )
cph)
c         call smoothness_lapldepth( cost_sd )
c         call smoothness_graddepth( cost_gd )
      end if
c
c     start time stepping
c
      nio = 0
      time_index = 0
      time = 0.
c
c     total number of time steps include spin up
c
      ntotal = nt+ntspinup
c$$$c
c$$$c     store the dynamic variables u, v, and eta, so they do not have 
c$$$c     to be recomputed during the adjoint calculations, so far use
c$$$c     two level checkpointing
c$$$c
c$$$cadj init tape_level_1 = 'state'
c
c     start integration over assimilation interval, determine the number
c     of cycles for the outer loop of check pointing, round to the nearest 
c     integer larger than routin (ratio of total to inner loop cycles)
c
      routin = real(ntotal)/real(ninner)
      if ( routin .ne. ntotal/ninner ) then
         nouter = int( routin ) + 1
      else
         nouter = int( routin ) 
      end if
      if ( fullio .and. .not. suppressio ) then
         print *, 'number of outer loops',
     &        ' = number of tape records = ', nouter
      end if
      time_index = 0
      time = start_time+time_index*dt
      if ( .not. suppressio ) then
         nio = nio + 1
         print *, 'Writing Time Step ', time_index
cph(
cph this contains netcdf stuff
cph         call state_io( time, nio )
cph)
      end if
      
c      ninner=10
c      nouter=6

      do it = 1, nouter
c
cadj store u,v,eta = tape_level_1, key=it
cadj init tape_level_2 = common, ninner
c
         print *, 'outer loop ', it
         call loop_body_wrapper_outer(u, v, eta,  
     & cost, calc_cost, time, time_index, nio, it, ntotal, ninner) 
c
      end do
c$$$c     overturning
c$$$      zonal_transport = 0.
c$$$      call calc_overturning( zonal_transport, u, v )
c$$$      print *, 'overturning transport = ', zonal_transport*1e-6, ' Sv'
c$$$      cost = zonal_transport
c     zonal transport
      zonal_transport = 0.
cph(
cju in the code for calc_zonal_transport the v and eta 
cju references had always been commented out
cju      call calc_zonal_transport_joint( zonal_transport, u, v, eta)
      call calc_zonal_transport_joint( zonal_transport, u)
cph)
      print *, 'zonal volume transport = ', zonal_transport*1e-6, ' Sv'
c
c     save cost function value (to trick TAMC)
c
cju changed according to Patrick's instructions:
cju      cost_final = cost + cost_d + cost_sd + cost_gd
      cost_final = zonal_transport

      if ( iteration .ge. 0 ) then
         open(10,file='cost.txt',form='formatted',position='append')
         write(10,'(I5,5E15.8)') iteration, cost_d, cost_sd, cost_gd,
     &        cost, cost_final
         close(10)
         iteration = iteration + 1
      end if

c$openad DEPENDENT(cost_final)

      return
      end !subroutine forward_model
