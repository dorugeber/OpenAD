module m 

contains 

 function foo(x)
  real :: foo
  foo=2*x
 end function 

end module 

program main 
  use m, faa=>foo
  real :: x,y
  x=2.0
  y=faa(x)
end program
