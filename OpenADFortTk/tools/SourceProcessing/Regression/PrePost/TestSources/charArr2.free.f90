module m1 
 integer :: len
 parameter (len=6)
end module 

module m2 
 use m1
 character*(len) :: ca
end module 

subroutine foo()
  use m2 
  if (ca(1:3) .eq. 'bla') then 
    print *, 'OK'
  else 
    print *, bla
  end if
end subroutine 

program p 
  use m2 
  ca='blabla'
  call foo()
end program
