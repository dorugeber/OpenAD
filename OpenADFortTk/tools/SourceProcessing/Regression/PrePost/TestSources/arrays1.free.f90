!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
! A simple program with array operations.
!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

program arrays
  double precision, dimension(20) :: x
  double precision :: y
  integer, dimension(10) :: ix
  integer :: i
  
  x = 2.5
  y = 3.5
  ix = 5
  i = 10

  x(1) = x(1) * x(4) + 3.1415 - y
  ix(4) = ix(1) / ix(2) + i

  print *,'x(1) = ', x(1), 'ix(4) = ',ix(4)

end program
