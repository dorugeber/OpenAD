program p 
  use OAD_active
  implicit none
  common /cb/ g
  type(active) :: g 
  double precision :: x,y
  g%v=2.0D0
  x=1.5D0
  call faa(x,y)
  print *,y
end program
