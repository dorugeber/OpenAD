module OAD_intrinsics
end module
subroutine foo(x)
   use OAD_active
   use OAD_intrinsics
   doubleprecision x
   print *,x
end subroutine

program p
  use OAD_active
  use OAD_intrinsics
  integer i
  double precision :: oad_ctmp0
  i=2
  oad_ctmp0 = sin(dble(i))
  call foo(oad_ctmp0)
end
