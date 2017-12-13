module OAD_intrinsics
end module
module m
use OAD_active
use OAD_intrinsics

  contains

  recursive subroutine foo(x,s)
    integer :: x,s,t
    integer :: oad_ctmp0
    if (x>0) then
      oad_ctmp0 = x-1
      call foo(oad_ctmp0,t)
      s = t+x
    else
      s=0
    end if
  end subroutine
end module

program p
  use OAD_active
  use OAD_intrinsics
  use m
  integer :: s
  call foo(6,s)
  print *,s
end program
    
