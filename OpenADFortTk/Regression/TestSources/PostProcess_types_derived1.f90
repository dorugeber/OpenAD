!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
! A program with a user defined type.
!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

module coordrec

  type cart
     real :: x
     real :: y
  end type cart

end module coordrec


subroutine distanceFromOrigin(c,r)
  use coordrec
  
  type(cart), intent(in) :: c
  real :: r
  
  ! r^2 = x^2 + y^2
  r = sqrt((c%x)**2 + (c%y)**2)

end subroutine


program dosomething
  use coordrec

  type(cart) :: c
  integer :: n = 7
  real :: d
  
  c = cart(n, n)

  call distanceFromOrigin(c,d)
  print *,d
  
end program dosomething
