module OAD_intrinsics
end module
module m
use OAD_intrinsics

interface oad_s_foo
module procedure oad_s_foo_s,oad_s_foo_a
end interface
interface foo
module procedure foo_s, foo_a
end interface

contains
function foo_s(la)
  real :: la,foo_s
  foo_s=la
end function

function foo_a(lb)
  real, dimension(:):: lb
  real :: foo_a
  foo_a=sum(lb)
end function
subroutine oad_s_foo_s(la,foo_s)
  real :: la
  real,intent(out) :: foo_s
  foo_s=la
end subroutine oad_s_foo_s

subroutine oad_s_foo_a(lb,foo_a)
  real, dimension(:):: lb
  real,intent(out) :: foo_a
  foo_a=sum(lb)
end subroutine oad_s_foo_a

end module

program main
  use OAD_intrinsics
  use m
  real :: a,b(3),fs,fa
  real :: oad_ctmp0
  real :: oad_ctmp1
  a=1.0
  b=[2.0,3.0,4.0]
  call oad_s_foo(a,oad_ctmp0)
  fs = oad_ctmp0
  call oad_s_foo(b,oad_ctmp1)
  fa = oad_ctmp1
  print *,fs,fa
end program
