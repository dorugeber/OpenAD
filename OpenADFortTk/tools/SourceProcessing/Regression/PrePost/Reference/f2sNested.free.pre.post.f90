module OAD_intrinsics
end module
module m
use OAD_active
use OAD_intrinsics
implicit none

type t
  real, dimension(2) :: a
end type

contains

function bar(r)
  real :: r,bar
  bar = 2*r
end function

function foo(aT)
  type(t) :: aT
  real :: foo
  foo = bar(aT%a(2))
end function

subroutine oad_s_bar(r,bar)
  real :: r
  real,intent(out) :: bar
  bar = 2*r
end subroutine oad_s_bar

subroutine oad_s_foo(aT,foo)
  type(t) :: aT
  real :: oad_ctmp0
  real,intent(out) :: foo
  call oad_s_bar(aT%a(2),oad_ctmp0)
  foo = oad_ctmp0
end subroutine oad_s_foo

end module

program p
  use OAD_active
  use OAD_intrinsics
  use m
  type(t) :: aT
  real y
  real :: oad_ctmp0
  aT%a(1)=3.2
  aT%a(2)=4.1
  call oad_s_foo(aT,oad_ctmp0)
  y = oad_ctmp0
  print *,y
end program
