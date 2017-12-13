!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
! A simple program with a nested recursive function
!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

program nestedrfunc
  implicit none

  integer :: n = 7

  n = factorial(n)

  ! call factorial(9) ! what happens to return?

contains

  recursive function factorial(n) result(res)
    integer res, n
    if (n .EQ. 1) then
       res = 1
    else
       res = n*factorial(n-1)
    end if
  end function factorial
  
end program nestedrfunc
