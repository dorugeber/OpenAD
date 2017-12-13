module OAD_intrinsics
end module
module m1
  use OAD_active
  logical(kind=kind(.true.)) :: l1=.false.
end module

program p
  use OAD_active
  use OAD_intrinsics
  use m1
  logical(kind=kind(.true.)) :: aLogical=.true.
  print *,aLogical,l1
end program
