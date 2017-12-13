program maxSpec
  implicit none
  double precision x,y

  x = 3.1415D0
  y = DMAX1(x,8.90D0)
  write(*,*) 'y = ',y
end program 

