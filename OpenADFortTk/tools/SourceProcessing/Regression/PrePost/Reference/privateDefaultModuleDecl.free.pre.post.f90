module OAD_intrinsics
end module
module theModule
  use OAD_active
  implicit none
  integer,parameter :: a=1
  private
  integer, parameter, public :: b=2
end module theModule

program privateModuleDecl
  use OAD_active
  use OAD_intrinsics
  use theModule
  implicit none
  integer :: a
  
  a = 7+b
  print *,'a = ',a
end program privateModuleDecl

