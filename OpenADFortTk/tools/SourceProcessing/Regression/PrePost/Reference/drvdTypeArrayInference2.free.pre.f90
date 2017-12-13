module OAD_intrinsics
end module
function foo(i)
  use OAD_intrinsics
  real :: i(2),foo
  foo = 2*i(1)
end function
subroutine oad_s_foo(i,foo)
  use OAD_intrinsics
  real :: i(2)
  real,intent(out) :: foo
  foo = 2*i(1)
end subroutine oad_s_foo

program p
  use OAD_intrinsics
  type t
   real, dimension(2) :: i
  end type
  type tt
   type (t) :: aT
  end type
  real y
  type(tt) :: aTT
  real :: oad_ctmp0
  real,dimension(2) :: oad_ctmp1
  aTT%aT%i(1)=2.0
  oad_ctmp1 = aTT%aT%i(:)+2.0
  call oad_s_foo(oad_ctmp1,oad_ctmp0)
  y = oad_ctmp0
  print *,y
end program
