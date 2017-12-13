program theModuleNatureTest

  use, intrinsic :: iso_c_binding, only: c_associated, c_loc, c_ptr
  implicit none
  real, pointer :: a
  type(c_ptr) :: b
  if(c_associated(b, c_loc(a))) &
       stop 'b and a do not point to same target'

end program theModuleNatureTest

