module OAD_intrinsics
end module
module m
use OAD_active
use OAD_intrinsics
contains
elemental real pure function foo(x)
  real, intent(in) :: x
  foo=x
end function
elemental pure subroutine oad_s_foo(x,foo)
  real, intent(in) :: x
  real,intent(out) :: foo
  foo=x
end subroutine oad_s_foo
end module

program p
use OAD_active
use OAD_intrinsics
use m
real x,y
  real :: oad_ctmp0
x=3.0
call oad_s_foo(x,oad_ctmp0)
y = oad_ctmp0
print *,y
end program
