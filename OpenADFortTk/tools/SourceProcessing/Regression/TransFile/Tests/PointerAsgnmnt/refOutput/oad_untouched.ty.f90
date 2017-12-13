subroutine s(x,y)
  use OAD_active
  use m
  implicit none
  double precision :: x,y
  double precision, pointer :: aVarP, pVarP
  aVarP => aVar%v
  pVarP=>pVar
  y=aVarP*x*pVarP
end subroutine
