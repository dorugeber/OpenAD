module m 
contains 
elemental real pure function foo(x) 
  real, intent(in) :: x
  foo=x
end function
end module 

program p 
use m 
real x,y
x=3.0
y=foo(x)
print *,y
end program
