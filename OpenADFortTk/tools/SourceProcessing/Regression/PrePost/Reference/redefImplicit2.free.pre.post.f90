module OAD_intrinsics
end module
module m
use OAD_active
use OAD_intrinsics
contains
subroutine foo(x)
implicit double precision (x)
print *,x
end subroutine
subroutine bar(x)
implicit none
double precision x
print *,x
end subroutine
end module

program p
use OAD_active
use OAD_intrinsics
use m
implicit none
double precision j
  double precision :: oad_ctmp0
j=3
oad_ctmp0 = j+5
call foo(oad_ctmp0)
end program
