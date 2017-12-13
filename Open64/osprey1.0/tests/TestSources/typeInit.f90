      module m 
       type t
         integer :: c1,c2=0
       end type 
      end module 

      program p 
       use m 
       type(t) :: v = t(1,2)
       if (v%c2==2) then 
         print *,'OK'
       else 
         print *, v%c2
       end if 
      end program
