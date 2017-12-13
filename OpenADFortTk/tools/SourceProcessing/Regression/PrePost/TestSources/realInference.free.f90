module m 
contains
subroutine foo(x,y)
double precision:: x,y
y=x
end subroutine 
end module 

program main
use m  
integer, parameter :: dp=kind(1.0d0)
real(dp) :: y
integer :: i,j
i=2
j=3
call foo(real(i+1,kind=dp)/real(j-1,kind=dp),y)
print *,y
end program
