module OAD_intrinsics
end module
integer function foo ()
   use OAD_intrinsics
   integer, save :: fooi=1
   fooi = fooi+1
   foo=fooi
end function
subroutine oad_s_foo(foo)
   use OAD_intrinsics
   integer, save :: fooi=1
   integer,intent(out) :: foo
   fooi = fooi+1
   foo=fooi
end subroutine oad_s_foo


module m
use OAD_intrinsics

real :: r

contains

  subroutine bar()

    external oad_s_foo
    integer foo
    integer :: oad_ctmp0
    call oad_s_foo(oad_ctmp0)
    print *,r,oad_ctmp0

  end subroutine
end module

subroutine head ()
  use OAD_intrinsics
  use m
external oad_s_foo
integer foo
  integer :: oad_ctmp0
  r=3.0
  call bar()
  call oad_s_foo(oad_ctmp0)
  print *,oad_ctmp0
end subroutine

program p
use OAD_intrinsics

call head()
end program

