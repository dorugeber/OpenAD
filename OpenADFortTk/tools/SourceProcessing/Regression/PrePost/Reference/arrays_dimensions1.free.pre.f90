module OAD_intrinsics
end module
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
! A simple program to test array dimensions
!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

program array_dimensions
  use OAD_intrinsics

  implicit none
  real, dimension(10) :: a
  real, dimension(10,10) :: b
  real, dimension(10,10,10) :: c
  real, dimension(10,10,10,10) :: d
  real, dimension(10,10,10,10,10) :: e
  real, dimension(10,10,10,10,10,10) :: f
  real, dimension(10,10,10,10,10,10,10) :: g

  a = 1
  b = 2
  c = 3
  d = 4
  e = 5
  f = 6
  g = 7

end program

