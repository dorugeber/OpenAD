!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
! A simple program to test control flow (loop).  (Cf. controlflow_loop2.f90)
!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

subroutine compute(x, y)
  double precision, intent(inout) :: x, y
  x = x / y
  y = ( y * .789 ) - 2
end subroutine


subroutine myloop(x, y)
  double precision, intent(inout) :: x, y
  
  do while (x > 0)

     call compute(x, y)

     write(*,*) 'x =', x
     write(*,*) 'y =', y

     if (y < 0) exit

  end do
  
end subroutine


program controlflow
  double precision :: x, y
  x = 2
  y = 8

  call myloop(x, y)

end program

