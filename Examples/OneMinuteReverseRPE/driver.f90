program driver
  use OAD_active
  use OAD_rev
  use rp_emulator
  implicit none 
  external head
  type(active) :: x, y

! INITIALIZATION EMULATOR
  RPE_ACTIVE = .TRUE.
  RPE_DEFAULT_SBITS = 5

  x%v=.5D0
  y%d=1.0D0
  call zero_deriv(x)

  our_rev_mode%tape=.TRUE.
  call head(x,y)
  print *, 'driver running for x =',x%v
  print *, '            yields y =',y%v,' dy/dx =',x%d
  print *, '    1+tan(x)^2-dy/dx =',1.0D0+tan(x%v)**2-x%d
end program driver
