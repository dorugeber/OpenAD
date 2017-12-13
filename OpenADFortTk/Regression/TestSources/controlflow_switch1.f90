!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
! A simple program to test control flow (loop).  (Cf. controlflow_loop2.f90)
!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

subroutine sub1(i, bogus)
  implicit none
  integer, intent(in) :: i
  real, intent(inout) :: bogus
  
  select case(i)
  case(1)
     bogus = 1.0
  case(3)
     bogus = 2.0
  case(5)
     bogus = 3.0
  case(8)
     bogus = 4.0
  case default
     bogus = 0.0
  end select
  
end subroutine sub1


subroutine sub2(dayOfMonth, bogus)
  implicit none
  integer, intent(in) :: dayOfMonth
  real, intent(inout) :: bogus
  
  select case(dayOfMonth)
  case(1, 21, 31)
     bogus = 1.0 ! 'st'
  case(2, 22)
     bogus = 2.0 ! 'nd'
  case(3, 23)
     bogus = 3.0 ! 'rd'
  case(4:20, 24:30)
     bogus = 4.0 ! 'th'
  case default
     bogus = 0.0 ! '??'
  end select
  
end subroutine sub2

program controlflow
  real :: bogus 
  integer :: one = 1
  call sub1(one, bogus)
  call sub2(one, bogus)
end program

