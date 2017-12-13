module OAD_intrinsics
end module
module m1
use OAD_active
use OAD_intrinsics
 contains
  subroutine foo(x)
    real :: x
    x = x*2
    print *,"foo ",x
  end subroutine
  subroutine bar(y)
    double precision :: y
    y = y*2
    print *,"bar ",y
  end subroutine
end module

module m2
  use OAD_active
  use m1
  interface i
   module procedure foo
   module procedure bar
  end interface
end module

subroutine head(x,y)
  use OAD_active
  use OAD_intrinsics
  use m2
  real :: x
  double precision :: t,y
  call i(x)
  t=x
  call i(t)
  y=t
end subroutine

program p
  use OAD_active
  use OAD_intrinsics
  real :: x
  double precision :: y
  x=1.0
  call head(x,y)
  print *,y
end program

