subroutine foo(a)
  double precision :: d,cbv
  COMMON/CB/ d,cbv
  double precision :: a
  cbv=cbv*a
  a=cbv
end subroutine

program p
  double precision :: d,cbv
  COMMON/CB/ d,cbv
  equivalence(cbv,f)
  double precision :: x,y,f
!$openad INDEPENDENT(x)
!$openad DEPENDENT(y)
  x=2.0D0
  f=x
  cbv=cbv+1
  call foo(x)
  y=x*f
  call foo(y)
  print *, y
end program
