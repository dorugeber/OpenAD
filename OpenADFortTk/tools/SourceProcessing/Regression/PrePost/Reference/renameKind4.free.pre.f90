module OAD_intrinsics
end module
module m1
  integer,parameter :: x=5
end module

module m2
use OAD_intrinsics
  use m1, only: z=>x
  contains

    function foo()
      real :: foo
      foo=2.0D0
    end function

    subroutine oad_s_foo(foo)
      real,intent(out) :: foo
      foo=2.0D0
    end subroutine oad_s_foo
end module

program p
  use OAD_intrinsics
  use m2
  use m1, only: y=>x
  real, dimension(y) :: x
  real :: oad_ctmp0
  call oad_s_foo(oad_ctmp0)
  x(1) = oad_ctmp0
  print *,x(1)
end program
  
