module OAD_intrinsics
end module
module m
use OAD_active
use OAD_intrinsics

contains

 function foo(x)
  real :: foo
  foo = 2*x
 end function

 subroutine oad_s_foo(x,foo)
  real,intent(out) :: foo
  foo = 2*x
 end subroutine oad_s_foo

end module

program main
  use OAD_active
  use OAD_intrinsics
  use m, oad_s_faa => oad_s_foo,faa => foo
  real :: x,y
  real :: oad_ctmp0
  x=2.0
  call oad_s_faa(x,oad_ctmp0)
  y = oad_ctmp0
end program
