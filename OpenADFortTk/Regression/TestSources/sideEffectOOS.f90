! side effect out of scope
module m
  real :: r
end module

subroutine foo(x)
  use m  ! here we know r 
  implicit none
  real::x 
  r=x
end subroutine

subroutine bar(x)
  implicit none 
  real :: x
  call foo(x) ! here we don't know r
end subroutine 

program p 
  use m ! here we know r
  implicit none
  real :: p_r
  p_r=3.0 
  r=2.0
  call bar(p_r)
  print *,r
end program
