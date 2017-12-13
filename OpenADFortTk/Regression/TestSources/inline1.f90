!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
! A simple program to test inlining
! (all params inout; no local vars)
!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

subroutine inline_useless(a, b)
  double precision, intent(inout) :: a, b

  a = cos(a)
  b = sin(b)
end subroutine inline_useless

program inlinetest
  double precision :: x, y
  double precision :: pi = 3.1415926

  x = pi
  y = pi / 2
  
  call inline_useless(x, y)
  
end program inlinetest

