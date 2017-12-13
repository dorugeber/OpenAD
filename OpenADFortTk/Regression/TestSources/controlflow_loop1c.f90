!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
! A simple program to test control flow (loop).  (Cf. controlflow_loop2.f90)
!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

subroutine sub1()
  implicit none
  double precision :: id
  double precision :: y = 8
  do id = 1, 10
     y = id - 2
  end do
end subroutine

program controlflow
  call sub1()
end program

