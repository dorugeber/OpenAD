!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
! A simple program to test control flow (loop).  (Cf. controlflow_loop2.f90)
!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

subroutine sub1()
  implicit none
  double precision :: y = 8
  integer :: i
  do i = 1, 10, 2
     y = i - 2
  end do
end subroutine

subroutine sub2()
  implicit none
  double precision :: y = 8
  integer :: i
  do while (i < 10)
     y = i - 2
     i = i + 2
  end do
end subroutine

program controlflow
  call sub1()
  call sub2()
end program

