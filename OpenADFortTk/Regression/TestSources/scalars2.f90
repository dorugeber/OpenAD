!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
! A simple program with scalar addition, subtraction, multiplication, division
!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

program scalartest
  double precision :: x
  double precision :: y
  integer :: ix
  integer :: jjj
  x=6.0
  y=2.3
  ix=2
  jjj=3
  x = x * x + 3.1415 - y
  ix = ix / ix + jjj

  print *,'x(1) = ', x, 'ix(4) = ',ix

end program
