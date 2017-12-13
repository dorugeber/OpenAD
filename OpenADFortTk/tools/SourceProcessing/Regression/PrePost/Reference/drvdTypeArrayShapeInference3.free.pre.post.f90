module OAD_intrinsics
end module
function foo(i)
  use OAD_active
  use OAD_intrinsics
  real, dimension(2) :: i,foo
  foo = 2*i
end function
subroutine oad_s_foo(i,foo)
  use OAD_active
  use OAD_intrinsics
  real,dimension(2) :: i
  real,dimension(2),intent(out) :: foo
  foo = 2*i
end subroutine oad_s_foo

program p
  use OAD_active
  use OAD_intrinsics
  interface
     function foo(i)
       use OAD_active
       real, dimension(2) :: i,foo
     end function foo
     subroutine oad_s_foo(i,foo)
       use OAD_active
       real,dimension(2),intent(out) :: foo
       real,dimension(2) :: i
     end subroutine
  end interface
  type t
   real, dimension(2,2) :: i
   integer :: idx=2
  end type
  real y(2)
  type(t) :: aT
  real,dimension(2) :: oad_ctmp0
  real,dimension(2) :: oad_ctmp1
  aT%i=2.0
  oad_ctmp1 = aT%i(aT%idx,:)*2
  call oad_s_foo(oad_ctmp1,oad_ctmp0)
  y = oad_ctmp0
  print *,y
end program
