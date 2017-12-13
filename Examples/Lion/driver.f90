program driver
  use OAD_active
  implicit none 
  external head
  type(active) :: x(2), y(4)
  x%v=.5D0
  x%d=1.0D0
  call head(x,y)
end program driver
