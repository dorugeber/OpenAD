module OAD_intrinsics
end module
program bracketing
  use OAD_active
  use OAD_intrinsics
  integer, dimension(4) :: a
  integer i
  a(1)=2
  a(2)=1
  a(3)=2
  a(4)=3
  i=a(a(a(a(a(a(a(a(a(a(4))))))))))
  print *,i
end program bracketing
