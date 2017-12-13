program ifNonThen
  implicit none
  double precision x,y

  x = 3.1415D0
  if (x .le. max(x,8.90D0)) y = max(x,8.90D0)
  write(*,*) 'y = ',y
end program ifNonThen

