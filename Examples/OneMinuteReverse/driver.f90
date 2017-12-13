program driver
  use OAD_active
  use OAD_rev
  implicit none 
  external head
  type(active) :: x, y
  x%v=.5D0
  y%d=1.0D0
  our_rev_mode%tape=.TRUE.
  call head(x,y)
  print *, 'driver running for x =',x%v
  print *, '            yields y =',y%v,' dy/dx =',x%d
  print *, '    1+tan(x)^2-dy/dx =',1.0D0+tan(x%v)**2-x%d
end program driver
