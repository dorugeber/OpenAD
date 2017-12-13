!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
! A simple program with a comparison of string slices
!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

subroutine foo(n,res)
  character(*) n
  integer res
  integer s
  s=1
  if (n(s+1:s+3) .EQ. "bla") then
     res = 1
  else
     res = 5
  end if
end 

program functiontest

  integer :: n 
  character(10) :: s
  s="ablab"
  n=2
  call foo(s,n)
  if (n.eq.1) then 
    print *,'OK'
  end if  
  
end program functiontest
