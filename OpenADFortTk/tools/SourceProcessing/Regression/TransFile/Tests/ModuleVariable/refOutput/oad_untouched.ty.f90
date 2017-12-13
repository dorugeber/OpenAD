subroutine s(x,y)
  use OAD_active
  use m
  implicit none
  double precision :: x,y
  y = aVar%v*x*pVar
end subroutine
