module m 
contains 
subroutine foo(x,y)
  real , dimension(:) :: x,y
  y=min(x,y)
end subroutine 
end module

program main
 use m 
 real :: x(3), y(3)
 x=[1.0,2.0,3.0]
 y=[1.0,1.5,3.5]
 call foo(x,y)
 print *,y
end program
