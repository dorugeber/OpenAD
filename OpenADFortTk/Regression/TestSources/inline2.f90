!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
! A simple program to test inlining
! (not all not all params in-out; a local var)
!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

subroutine inline_useless(a, b)
  double precision, intent(inout) :: a
  double precision, intent(in) :: b
  double precision :: local

  a = cos(a)
  local = sin(b)
end subroutine inline_useless

program inlinetest
  double precision :: x, y

  x = 2
  y = 8
  
  call inline_useless(x, y)

end program inlinetest

