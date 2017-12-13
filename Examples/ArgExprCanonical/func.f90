subroutine foo (a)
  real a
  a=a*2
end

program p
  real x
  x=1.0;
  call foo(x*3.0)
end program
