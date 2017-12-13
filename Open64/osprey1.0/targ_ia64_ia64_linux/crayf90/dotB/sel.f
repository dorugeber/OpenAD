      program silly
      integer i
      i=7
      select case (i)
       case (0)
           print *,"case <0>"
       case (1:9)
           print *,"case <1:9>"
       case (10:20)
           print *,"case <10:20>"
      end select
      end
