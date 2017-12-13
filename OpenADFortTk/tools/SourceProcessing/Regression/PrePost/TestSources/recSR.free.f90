module m 

  contains 

  recursive subroutine foo(x,s)
    integer :: x,s,t
    if (x>0) then 
      call foo(x-1,t)
      s=t+x
    else 
      s=0
    end if
  end subroutine 
end module 

program p 
  use m 
  integer :: s
  call foo(6,s)
  print *,s
end program
    
