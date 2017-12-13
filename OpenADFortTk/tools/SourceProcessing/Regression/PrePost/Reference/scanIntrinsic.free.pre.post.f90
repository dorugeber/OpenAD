module OAD_intrinsics
end module
program p
  use OAD_active
  use OAD_intrinsics
  integer:: i
  i = scan('a+b','+')
  print *,i
end program
