module m 
 character*(6) :: ca
end module 

subroutine foo()
  use m 
  if (ca(1:3) .eq. 'bla') then 
    print *, 'OK'
  else 
    print *, bla
  end if
end subroutine 

program p 
  use m 
  ca='blabla'
  call foo()
end program
