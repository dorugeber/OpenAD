module m1 
  integer :: i 
  parameter (i=1+2)
end module

module m2
  use m1 , j=>i
end module 

subroutine foo(x,n)
  real :: x(n)
  print *,x
end subroutine 

subroutine head(x) 
  use m2
  real x(j)
  call foo(x,j)
end subroutine 

program p 
 use m1
 real x(i)
 x=5
 call head(x)
end program 
 
