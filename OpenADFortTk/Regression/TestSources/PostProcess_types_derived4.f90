!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
! A program with a user defined type.
!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

module coordrec
  implicit none
  
  type cart
     real :: x
     real :: y
  end type cart
  
  interface assignment (=)
     module procedure cart_assign_cart
  end interface

contains
  
  subroutine cart_assign_cart(a, b)
    type(cart), intent(out) :: a
    type(cart), intent(in):: b
    
    a%x = b%x
    a%y = b%y
  end subroutine cart_assign_cart
  
end module coordrec


subroutine distanceFromOrigin(c,r)
  use coordrec
  
  type(cart), intent(in) :: c
  
  ! r^2 = x^2 + y^2
  r = sqrt((c%x)**2 + (c%y)**2)

end 


program dosomething
  use coordrec

  type(cart) :: c1, c2
  integer :: n = 7
  real :: d
  
  c1 = cart(n, n)
  c2 = cart(4.0, 4.0)
  c1 = c2
  
  call distanceFromOrigin(c,d)
  print *,d
  
end program dosomething
