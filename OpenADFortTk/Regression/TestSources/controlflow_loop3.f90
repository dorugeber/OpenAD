!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
! A simple program to test control flow (if/gotos). (Cf. controlflow_loop1.f90)
!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

subroutine compute(x, y)
  double precision, intent(inout) :: x, y
  double precision :: z
  x = x / y
  y = ( y * .789 ) - 2
  z = y**5
end subroutine


subroutine myloop(x, y)
      double precision, intent(inout) :: x, y
  
 10   if ( x .gt. 0 ) then

        call compute(x, y)

        write(*,*) 'x =', x
        write(*,*) 'y =', y

        if (y < 0) return
        goto 10
     end if
  
end subroutine


program controlflow
  double precision :: x, y
  x = 2
  y = 8

  call myloop(x, y)

end program

