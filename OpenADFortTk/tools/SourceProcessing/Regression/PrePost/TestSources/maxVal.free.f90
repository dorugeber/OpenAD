program maxValTest
  implicit none
  double precision x(4),y
  x=(/1.0, 2.0, 3.0, 1.5/)
  y = maxval(x)
  write(*,*) 'y = ',y
end program maxValTest

