module OAD_intrinsics
end module
function foo(i)
  use OAD_active
  use OAD_intrinsics
  real i
  real foo
  foo=i
end function
subroutine oad_s_foo(i,foo)
  use OAD_active
  use OAD_intrinsics
  real i
  real,intent(out) :: foo
  foo=i
end subroutine oad_s_foo

program p
use OAD_active
use OAD_intrinsics
interface
function foo(i)
  use OAD_active
  real i
  real foo
end function
subroutine oad_s_foo(i,foo)
  use OAD_active
  real i
  real,intent(out) :: foo
end subroutine
end interface
  real :: oad_ctmp0
call oad_s_foo(3.0,oad_ctmp0)
x = oad_ctmp0
print *,x
end program
