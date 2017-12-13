subroutine foo(a)
  COMMON/CB/ CBvar
  double precision :: a, CBvar(2)
  CBvar(1)=CBvar(1)*a
  a=CBvar(1)
end subroutine

program cb1
  COMMON/CB/ CBvar
  double precision :: CBvar(2)
  double precision :: x,y
!$openad INDEPENDENT(x)
!$openad DEPENDENT(y)
  x=2.0D0
  CBvar(1)=x
  call foo(x)
  y=x*CBvar(1)
  call foo(y)
  print *, y
end program
