module OAD_intrinsics
end module
module m1
  use OAD_active
  integer :: i
  parameter (i=1+2)
end module

module m2
  use OAD_active
  use m1 , j=>i
end module

subroutine foo(x,n)
  use OAD_active
  use OAD_intrinsics
  real :: x(n)
  print *,x
end subroutine

subroutine head(x)
  use OAD_active
  use OAD_intrinsics
  use m2
  real x(j)
  integer :: oad_ctmp0
  oad_ctmp0 = j
  call foo(x,oad_ctmp0)
end subroutine

program p
 use OAD_active
 use OAD_intrinsics
 use m1
 real x(i)
 x=5
 call head(x)
end program
 
