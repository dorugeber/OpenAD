!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
! A simple program to test control flow (loop).  (Cf. controlflow_loop2.f90)
!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

subroutine sub1()
  implicit none
  double precision :: y = 8
  integer :: i, j

  do i = 1, 10
    do j = 1, 10, 2
       y = i + j
    end do
    y = y - 2.0
  end do

end subroutine

program controlflow
  call sub1()
end program

