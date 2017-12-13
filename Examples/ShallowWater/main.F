      program shallow_water

      use adsize

      implicit none

      real cost
      real t0
      integer nc
      real xc(ncmax)

      print *, 'inifields:'
      call inifields
      print *, 'readparms:'
      call readparms
      cost = 0.
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
c
c #############
      call forward_model( nc, xc, cost )
      print *, 'cost function = ', cost
c #############

      stop 'normal in shallow_water'

      end                       !program shallow_water
