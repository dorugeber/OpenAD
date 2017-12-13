module OAD_intrinsics
end module
module m
use OAD_active
use OAD_intrinsics
contains
subroutine foo(x,y)
double precision:: x,y
y=x
end subroutine
end module

program main
use OAD_active
use OAD_intrinsics
use m
integer :: dp
parameter (dp = kind(1.0d0))
real(dp) :: y
integer :: i,j
  double precision :: oad_ctmp0
i=2
j=3
oad_ctmp0 = real(i+1,kind=dp)/real(j-1,kind=dp)
call foo(oad_ctmp0,y)
print *,y
end program
