program driver
  implicit none 
  external head
  integer, parameter :: n=2
  double precision , dimension (n) :: x, b
  b=(/.5D0,12.0D0/)
  print *, 'driver running for b =',b
  call head(b,x)
  print *, '            yields x =',x
end program driver
