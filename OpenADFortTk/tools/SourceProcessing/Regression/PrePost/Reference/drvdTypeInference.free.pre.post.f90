module OAD_intrinsics
end module
function foo(i)
  use OAD_active
  use OAD_intrinsics
  real :: i,foo
  foo = 2*i
end function
subroutine oad_s_foo(i,foo)
  use OAD_active
  use OAD_intrinsics
  real :: i
  real,intent(out) :: foo
  foo = 2*i
end subroutine oad_s_foo

program p
  use OAD_active
  use OAD_intrinsics
  type t
   real :: i
  end type
  real y
  type(t) :: aT
  real :: oad_ctmp0
  real :: oad_ctmp1
  aT%i=2.0
  oad_ctmp1 = aT%i+2.0
  call oad_s_foo(oad_ctmp1,oad_ctmp0)
  y = oad_ctmp0
  print *,y
end program
