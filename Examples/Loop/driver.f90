program driver
  use OAD_active
  use OAD_tape
  implicit none 
  external head
  type(active) :: x
  integer :: n
  call oad_tape_init()
  x%v=.5D0
  x%d=1.0D0
  write (*,fmt='(A)',advance='no') 'number of iterations = '
  read (*,*) n 
  call loopWrapper(x,n)
  print *, 'driver running for x =',x%v
  print *, '        yields dy/dx =',x%d
end program driver
