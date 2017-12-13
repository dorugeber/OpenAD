!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
! A program with a user defined type.
!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

module coord2d

  type cart
     double precision :: x
     double precision :: y
     character(len=20) :: msg
  end type cart

  type polar
     double precision :: r
     double precision :: theta ! radians
     character(len=20) :: msg
  end type polar

end module coord2d


subroutine cart2polar(c, p)
  use coord2d

  type(cart), intent(in) :: c
  type(polar), intent(out) :: p
  
  ! r^2 = x^2 + y^2
! p%r = sqrt((c%x)**2 + (c%y)**2)
  p%r = ((c%x)**2 + (c%y)**2)
  
  ! sin(theta) = y/r (more convenient than tan(theta) = y/x)
  if (p%r .eq. 0) then
     p%theta = 0
  else
!    p%theta = asin(c%y/p%r)
     p%theta = sin(c%y/p%r)
  end if
  
  p%msg = "because"
end subroutine cart2polar


program dosomething
  use coord2d
  implicit none

  type(cart) :: c
  type(polar) :: p
  double precision :: cx=0D0
  
  c = cart(cx, 9D0, 'why?')

  call cart2polar(c, p)
  
  write(*,'(A,2(F,A),2A)') 'cart c = (', c%x,',',c%y,',',c%msg,')'
  print *, 'polar p = (', p%r,',',p%theta,',',p%msg,')'
  
end program dosomething
