module m 

contains 

function foo(x) result(y)
  real :: x,y
  y=2*x
end function

end module

program p 
  use m
  implicit none
  real :: x,y
  x=2.0
  y=foo(x)
end program
