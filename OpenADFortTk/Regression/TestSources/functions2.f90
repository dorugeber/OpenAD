!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
! A simple program with a recursive function
!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

recursive function factorial(n) result(res)
  integer res, n
  if (n .EQ. 1) then
     res = 1
  else
     res = n*factorial(n-1)
  end if
end function factorial


program recfunc
  implicit none
  integer :: n = 7
  external factorial
  integer factorial

  n = factorial(n)
  
end program recfunc
