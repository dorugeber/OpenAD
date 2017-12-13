! the function we will pass
function foo(xfoo) 
  real:: xfoo,foo
  foo=2*xfoo
end function 

! the function that takes another function as dummy
function bar(f,xbar)
  real::xbar,bar
  bar=f(xbar)
end function bar

program main 
  real:: x,y
  external bar, foo
  x=3.5
  y=bar(foo,x)
  print *,y
end program
