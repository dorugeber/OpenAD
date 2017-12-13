module OAD_intrinsics
end module
module m
use OAD_active
use OAD_intrinsics

contains

function foo(x) result(y)
  real :: x,y
  y = 2*x
end function

subroutine oad_s_foo(x,y)
  real :: x
  real,intent(out) :: y
  y = 2*x
end subroutine oad_s_foo

end module

program p
  use OAD_active
  use OAD_intrinsics
  use m
  implicit none
  real :: x,y
  real :: oad_ctmp0
  x=2.0
  call oad_s_foo(x,oad_ctmp0)
  y = oad_ctmp0
end program
