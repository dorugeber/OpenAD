program maxDouble ! c0
  implicit none ! c1
  double precision x,y ! c2
  ! c3
  x = 3.1415D0  ! c4
  y = max(x,8.90D0) !! c5 
  write(*,*) 'y = ',y ! c6 
end program maxDouble !c7

