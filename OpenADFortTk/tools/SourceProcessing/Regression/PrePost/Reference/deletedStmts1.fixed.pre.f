      module OAD_intrinsics
      end module
      program p
      use OAD_intrinsics
      integer i,j
      j=3
      if (j<3) then
         assign 20 to i
      else
         assign 30 to i
      end if
      goto i , (20,30)
      goto 50
 20   print *,'20'
      goto 50
 30   print *,'30'
 50   continue
      end program
