module OAD_intrinsics
end module
subroutine s1()
use OAD_active
use OAD_intrinsics
  real :: x
  contains
  real function foo(x)
    integer:: x
    foo=x
  end function
  subroutine oad_s_foo(x,foo)
    integer:: x
    real,intent(out) :: foo
    foo=x
  end subroutine oad_s_foo
end subroutine

subroutine s2()
use OAD_active
use OAD_intrinsics
  real :: x
  contains
  real function foo(x)
   integer :: x
   foo=x
  end function
  subroutine oad_s_foo(x,foo)
   integer :: x
   real,intent(out) :: foo
   foo=x
  end subroutine oad_s_foo
end subroutine

program p
  use OAD_active
  use OAD_intrinsics
  call s1()
  call s2()
end program
