program maxShapes
  implicit none
  double precision x(3),a,b,y
  dimension :: y(3)
  a=2.0D0
  b=4.0D0
  x=(/1.0, 2.0, 3.0/)
  y = max(a,b*(a-x))
  write(*,*) 'y = ',y
end program maxShapes

