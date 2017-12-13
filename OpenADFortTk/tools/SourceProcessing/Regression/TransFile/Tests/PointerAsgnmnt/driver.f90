program p 
  use m 
  implicit none
  double precision :: x,y
  pVar=2.0D0
  aVar%v=3.0D0
  x=1.5D0
  call s(x,y)
  print *,y
end program
