      program shallow_water

      use OAD_active
      use OAD_rev
      use OAD_tape
      use OAD_cp

      use adsize

      implicit none

      type(active) cost
      real t0
      integer nc
      integer i
      type(active) xc(ncmax)

      print *, 'inifields:'

c set the execution mode
      our_rev_mode%arg_store=.FALSE.
      our_rev_mode%arg_restore=.FALSE.
      our_rev_mode%res_store=.FALSE.
      our_rev_mode%res_restore=.FALSE.
      our_rev_mode%plain=.TRUE.
      our_rev_mode%tape=.FALSE.
      our_rev_mode%adjoint=.FALSE.

c initialize the tape
      call tape_init()

c initialize the checkpoint areas
      call cp_init()

      call inifields
      print *, 'readparms:'
      call readparms
      cost%v = 0.
      print *, 'forward_model:'
      call make_weights
c
c     for time dependent data this should be done in time stepping!
c     
      t0=0.
      call read_eta_data( t0 )
c
      call length_of_control_vector( nc )
c
      print *, 'calculate intial cost function'
      call map_to_control_vector( nc, xc )

      cost%d=1.0

      do i=1,nc
         xc(i)%d=0.0
      end do

c
c #############

      our_rev_mode%arg_store=.FALSE.
      our_rev_mode%arg_restore=.FALSE.
      our_rev_mode%res_store=.FALSE.
      our_rev_mode%res_restore=.FALSE.
      our_rev_mode%plain=.FALSE.
      our_rev_mode%tape=.TRUE.
      our_rev_mode%adjoint=.TRUE.
      call forward_model( nc, xc, cost )

      print *, 'cost function = ', cost%v

      do i=1,nc
         write(*,'(A,I3,A,E25.17E3)') 'grad(', i, ')= ', xc(i)%d
      end do

c #############

      stop 'normal in shallow_water'

      end                       !program shallow_water
