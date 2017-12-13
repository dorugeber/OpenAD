subroutine s(x,y) 
  use m
  implicit none
  double precision :: x,y
  double precision, pointer :: aVarP, pVarP
  aVarP=>aVar
  pVarP=>pVar
  y=aVarP*x*pVarP
end subroutine
