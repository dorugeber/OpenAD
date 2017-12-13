!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
! A simple program to test inlining
! (all params inout; local vars)
!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

subroutine inline_swap(a, b)
  double precision, intent(inout) :: a, b
  
  double precision :: t, t1, t2
  t = a
  t1 = a + b
  t2 = sin(a**b)
  
  a = b
  b = t
end subroutine inline_swap


program inlinetest
  double precision :: x, y
  double precision :: pi = 3.1415926
  double precision :: t

  x = pi
  y = pi / 2
  
  t = pi
  
  write(*,*) 'x =', x
  write(*,*) 'y =', y
  write(*,*) '--------------------'

  call inline_swap(x, y)
  
  write(*,*) 'x =', x
  write(*,*) 'y =', y

end program inlinetest

