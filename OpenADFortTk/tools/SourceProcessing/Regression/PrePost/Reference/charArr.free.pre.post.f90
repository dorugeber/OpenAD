module OAD_intrinsics
end module
module m
 use OAD_active
 character*(6) :: ca
end module

subroutine foo()
  use OAD_active
  use OAD_intrinsics
  use m
  if (ca(1:3).eq.'bla') then
    print *,'OK'
  else
    print *,bla
  end if
end subroutine

program p
  use OAD_active
  use OAD_intrinsics
  use m
  ca='blabla'
  call foo()
end program
