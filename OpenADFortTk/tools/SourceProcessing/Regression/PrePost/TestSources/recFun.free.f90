module m 

  contains 

  recursive integer function foo(x)
    real :: x
    if (x>0) then 
      foo=foo(x-1)+x
    else 
      foo=0
    end if
  end function
end module 

program p 
  use m 
  r=foo(6.0)
  print *,r
end program
    
