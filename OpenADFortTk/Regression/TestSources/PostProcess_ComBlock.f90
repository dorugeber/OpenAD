subroutine foo(a)
  COMMON/CB/ CBvar
  double precision :: a, CBvar
  CBvar=CBvar*a
  a=CBvar
end subroutine

program ProgramCB
  COMMON/CB/ CBvar
  double precision :: CBvar
  double precision :: x,y
!$openad INDEPENDENT(x)
!$openad DEPENDENT(y)
  CBvar=1.4D0
  x=2.0D0
  call foo(x)
  y=x
  call foo(y)
  print *, y
end program
