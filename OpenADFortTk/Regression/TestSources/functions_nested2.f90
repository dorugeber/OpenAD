!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
! A simple program with a nested function
!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

program nestedfunc
  implicit none

  integer :: n = 7

  n = foo(n)
  
contains

  function foo(n) result(res)
    integer res, n
    if (n .EQ. 1) then
       res = 1
    else
       res = 5
    end if
  end function foo

end program nestedfunc
