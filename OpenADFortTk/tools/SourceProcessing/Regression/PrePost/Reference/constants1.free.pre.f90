module OAD_intrinsics
end module
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
! A simple program to test handling of constants
!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


subroutine test

  character :: char_test = 'T'
  real :: num1_test = 6.0
  integer :: num2_test = 11

end subroutine

program myprog
  use OAD_intrinsics

  implicit none

  character :: char
  real :: num1
  integer :: num2

  char = 'S'
  num1 = 5.0
  num2 = 10
  
  call test()

end program

