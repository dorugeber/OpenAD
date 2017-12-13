subroutine foo(x)
   doubleprecision x
   print *,x
end subroutine

program p 
  integer i
  i=2
  call foo(dsin(dfloat(i)))
end 
