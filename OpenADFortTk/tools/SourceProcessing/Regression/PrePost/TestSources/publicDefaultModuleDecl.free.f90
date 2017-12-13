module theModule
  implicit none
  integer,parameter,private :: a=1
  public 
  integer, parameter :: b=2
end module theModule

program privateModuleDecl
  use theModule
  implicit none
  integer :: a
  
  a = 7+b
  print *,'a = ', a
end program privateModuleDecl

