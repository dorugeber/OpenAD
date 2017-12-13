module m
contains
! the function we will pass
function foo(xfoo) 
  real:: xfoo,foo
  foo=2*xfoo
end function 
end module 

! the function that takes another function as dummy
function bar(f,xbar)
  real::xbar,bar
  interface 
    function f(xf)
      real:: xf
      real :: f
    end function
  end interface
  bar=f(xbar)
end function bar

program main 
  use m
  real:: x,y
  external bar
  x=3.5
  y=bar(foo,x)
  print *,y
end program
