!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
! A simple program with a function (instead of subroutine)
!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

function foo(n) result(res)
  integer res, n
  if (n .EQ. 1) then
     res = 1
  else
     res = 5
  end if
end function foo

program functiontest

  integer :: n = 7

  n = foo(n)
  ! call foo(n) ! what happens to return?
  
end program functiontest
