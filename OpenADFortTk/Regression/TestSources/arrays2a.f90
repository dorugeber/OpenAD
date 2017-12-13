subroutine compute2()
  implicit none
  real :: x
  real, dimension(20) :: a, b, c
  x = 5

  a = x
  b = 9
  c = 68
  a = (a/3.1) + (b * SQRT(c))
  write(*,*) 'a =', a
end subroutine


program f77_f90_arrays
  double precision :: x, y
  x = 2
  y = 8

  call compute2()

end program
