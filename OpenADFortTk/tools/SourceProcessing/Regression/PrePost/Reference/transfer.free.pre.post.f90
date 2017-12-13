module OAD_intrinsics
end module
module m
use OAD_active
use OAD_intrinsics
  implicit none
contains
subroutine foo(i,x)
  integer :: i(:)
  real :: x(:)
  integer :: iSize
  iSize=size(transfer(i,x))
  x(:iSize)=transfer(i,x)
end subroutine
end module

program p
  use OAD_active
  use OAD_intrinsics
  use m
  implicit none
  integer ::i,pi(3)
  real :: px(3)
  pi=(/(i,i=1,3)/)
  call foo(pi,px)
  print *,pi,px
end program


