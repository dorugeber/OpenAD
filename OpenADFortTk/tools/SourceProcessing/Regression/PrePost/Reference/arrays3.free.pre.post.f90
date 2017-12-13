module OAD_intrinsics
end module
program arrays3
  use OAD_active
  use OAD_intrinsics
  integer :: x(2,3), y(2)
  DATA ((x (i,j), i=1,2), j=1,3) / 6 * 2 /
  y = x(1:2,2)
  print *,y
end program
