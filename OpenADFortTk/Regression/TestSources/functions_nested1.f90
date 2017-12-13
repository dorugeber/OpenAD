!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
! A simple program with a nested procedure
!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

program nestedproc
  implicit none

  integer :: n = 7

  call foo(n)
  
contains

  subroutine foo(n)
    integer n, bogus
    if (n .EQ. 1) then
       bogus = 2
    else
       bogus = 3
    end if
  end subroutine foo
  
end program nestedproc
