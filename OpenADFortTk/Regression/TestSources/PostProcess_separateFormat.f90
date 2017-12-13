      program writeFormat
      double precision :: x,y
      x = 0.0D0
      y = 1.0D0

      write (*,10600) x,y

10600 format(1x,' ***Outer Convergence of Thermal Flux (MG/PW) Was',      &
             t55,2es12.5)

      end program writeFormat

