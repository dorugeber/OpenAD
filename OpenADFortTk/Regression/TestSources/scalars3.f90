!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
! A simple program with scalar intrinsics
!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

subroutine foo(x1,x2,y)
  double precision x1,x2,y
  y=sin(x1)*cos(x2)
end subroutine foo


program scalars3
  double precision a,b,c
  a=3.5
  b=2.4
  call foo(a,b,c)
  print *,c
end program
