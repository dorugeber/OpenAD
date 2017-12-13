subroutine foo(a)
  COMMON/CB/ CBvar
  double precision :: a, CBvar
  CBvar=CBvar*a
  a=CBvar
end subroutine

program cb1
  COMMON/CB/ CBvar
  double precision :: CBvar
  double precision :: x,y
!$openad INDEPENDENT(x)
!$openad DEPENDENT(y)
  x=2.0D0
  CBvar=x
  call foo(x)
  y=x*CBvar
  call foo(y)
  print *, y
end program
