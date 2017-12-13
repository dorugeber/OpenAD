!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
! A simple program with a comparison of string slices
!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

subroutine foo(fsa,fn)
  character(*) fsa(2)
  integer fn
  integer s
  s=1
  if (fsa(2)(s+1:s+3) .EQ. "bla") then
     fn = 1
  else
     fn = 5
  end if
end 

program functiontest

  integer :: n 
  character(10) :: sa(2)
  sa(2)="ablab"
  n=2
  call foo(sa,n)
  if (n.eq.1) then 
    print *,'OK'
  end if  
  
end program functiontest
