module OAD_intrinsics
end module
program p
  use OAD_active
  use OAD_intrinsics
  character(kind=kind('A'), len=10) :: name
  name='blah'
  print *,name
end program
