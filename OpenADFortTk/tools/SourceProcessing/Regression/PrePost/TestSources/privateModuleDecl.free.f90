module theModule
  implicit none
  integer,parameter,private :: a=1
end module theModule

program privateModuleDecl
  use theModule
  implicit none
  integer :: a
  
  a = 7
  print *,'a = ', a
end program privateModuleDecl

