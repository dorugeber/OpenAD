module OAD_intrinsics
end module
subroutine foo ()
   use OAD_active
   use OAD_intrinsics
   integer, save :: fooi=1
   fooi = fooi+1
   print *,fooi
end subroutine


subroutine bar(f)
  use OAD_active
  use OAD_intrinsics
  external f
  call f()
end subroutine

subroutine head ()
  use OAD_active
  use OAD_intrinsics
  external foo
  call bar(foo)
end subroutine

program p
use OAD_active
use OAD_intrinsics
call head()
end program
