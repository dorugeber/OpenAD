module OAD_intrinsics
end module
program statementFunc1
  use OAD_active
  use OAD_intrinsics
  real :: a,b,c
  sf(x) = x+x*2
  a = 1.3
  b = 3.0
  c = (a+a*2)*b
  write(*,*) c

end program
