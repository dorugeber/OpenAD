real function foo(a,b)
  real a,b
  foo = a*b
end

program p
  real x,y,a,b
  x=1.0; a=2.0; b=3.0
  y = x * foo(a,b)
end program
