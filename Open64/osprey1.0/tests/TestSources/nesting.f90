module m 
contains 
 subroutine foo(x)
   implicit none
   integer x
   integer i
   i=2
   call bar()
   contains 
     subroutine bar()
      implicit none
       x=x*i
     end subroutine
   end subroutine 
end module 

program p 
  use m
  integer :: i=2
  call foo(i)
  if (i==4) then 
    print *,'OK'
  else 
    print *,i
 end if 
end program
