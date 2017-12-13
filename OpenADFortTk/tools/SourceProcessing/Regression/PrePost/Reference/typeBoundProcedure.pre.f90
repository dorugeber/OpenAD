module OAD_intrinsics
end module
module class_Circle
use OAD_intrinsics
  implicit none
  real :: pi = 3.1415926535897931d0

  type, public :: Circle
     real :: radius
   contains
     procedure :: print => circle_print
  end type Circle
  
contains
  subroutine circle_print(this)
    class(Circle), intent(in) :: this
    real :: area
    area = pi*this%radius**2
    print *, 'Circle: r = ', this%radius, ' area = ', area
  end subroutine circle_print
end module class_Circle


program circle_test
  use OAD_intrinsics
  use class_Circle
  implicit none

  type(Circle) :: c
! Declare a variable of type Circle.
  c = Circle(1.5)
! Use the implicit constructor, radius = 1.5.
  call c%print
! Call the type-bound subroutine
end program circle_test
