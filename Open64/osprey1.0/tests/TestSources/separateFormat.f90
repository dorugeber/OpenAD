      program writeFormat
      double precision :: x,y
      x = 0.0D0
      y = 1.0D0

      open(80,file='writeFormat.out')
      write (80,10600) x,y
      close(80)

10600 format(1x,' ***Outer Convergence of Thermal Flux (MG/PW) Was',      &
             t55,2es12.5)

      print *, 'OK'

      end program writeFormat

