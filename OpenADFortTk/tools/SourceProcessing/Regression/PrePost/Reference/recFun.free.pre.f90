module OAD_intrinsics
end module
module m
use OAD_intrinsics

  contains

  recursive integer function foo(x)
    real :: x
    if (x>0) then
      foo = foo(x-1)+x
    else
      foo=0
    end if
  end function

  recursive subroutine oad_s_foo(x,foo)
    real :: x
    integer :: oad_ctmp0
    real :: oad_ctmp1
    integer,intent(out) :: foo
    if (x>0) then
      oad_ctmp1 = x-1
      call oad_s_foo(oad_ctmp1,oad_ctmp0)
      foo = oad_ctmp0+x
    else
      foo=0
    end if
  end subroutine oad_s_foo
end module

program p
  use OAD_intrinsics
  use m
  integer :: oad_ctmp0
  call oad_s_foo(6.0,oad_ctmp0)
  r = oad_ctmp0
  print *,r
end program
    
