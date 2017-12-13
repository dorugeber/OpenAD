!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
! A program with simple control flow and duplicate symbol
! names (requires correct scoping). 
!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

module globals
  double precision, save :: x, y
  integer, save :: a, b
end module globals


subroutine compute(x, y)
  double precision, intent(inout) :: x, y

  x = x / y
  y = ( y * .789 ) - 2
end subroutine


subroutine myloop(x, y)
  double precision, intent(inout) :: x, y
  
  do while (x > 0)

     write(*,*) 'x =', x
     write(*,*) 'y =', y

     call compute(x, y)

     write(*,*) 'x =', x
     write(*,*) 'y =', y

     if (y < 0) exit

  end do
  
end subroutine


program scopingtest
  use globals

  x = 2
  y = 8

  call myloop(x, y)

end program scopingtest

