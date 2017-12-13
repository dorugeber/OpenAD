module OAD_intrinsics
end module
module m
use OAD_active
use OAD_intrinsics

interface oad_s_foo
module procedure oad_s_foo_s,oad_s_foo_d
end interface
interface foo
module procedure foo_s, foo_d
end interface

contains
function foo_s(a,b)
  real :: a,b,foo_s
  foo_s = a+b
end function

function foo_d(c,d)
  double precision:: c,d,foo_d
  foo_d = c-d
end function
subroutine oad_s_foo_s(a,b,foo_s)
  real :: a,b
  real,intent(out) :: foo_s
  foo_s = a+b
end subroutine oad_s_foo_s

subroutine oad_s_foo_d(c,d,foo_d)
  double precision :: c,d
  double precision,intent(out) :: foo_d
  foo_d = c-d
end subroutine oad_s_foo_d

end module

program main
  use OAD_active
  use OAD_intrinsics
  use m
  real(kind(1.0D0)) :: c,d,fd
  real(kind(0.0)) :: a,b,fs
  real :: oad_ctmp0
  double precision :: oad_ctmp1
  a=1.0
  b=2.0
  call oad_s_foo(a,b,oad_ctmp0)
  fs = oad_ctmp0
  c=1.0D0
  d=2.0D0
  call oad_s_foo(c,d,oad_ctmp1)
  fd = oad_ctmp1
  print *,fs,fd
end program
