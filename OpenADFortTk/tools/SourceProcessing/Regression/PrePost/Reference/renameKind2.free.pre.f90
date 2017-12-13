module OAD_intrinsics
end module
module m1
  integer , parameter :: double=selected_real_kind(14)
end module

module m2
use OAD_intrinsics
  contains

    function foo()
      use m1, dp=>double
      real(dp) :: foo
      foo=2.0D0
    end function

    subroutine oad_s_foo(foo)
      use m1, dp=>double
      real(dp),intent(out) :: foo
      foo=2.0D0
    end subroutine oad_s_foo
end module

program p
  use OAD_intrinsics
  use m1, dpp=>double
  use m2
  real(dpp)::x
  double precision :: oad_ctmp0
  call oad_s_foo(oad_ctmp0)
  x = oad_ctmp0
  print *,x
end program

