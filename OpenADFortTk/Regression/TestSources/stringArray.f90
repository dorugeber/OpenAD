module m 
 integer , parameter :: n=3
 character(3) :: sArray(n)
end module

subroutine foo(s,r)
  use m 
  character(*) s
  logical r
  do i=1,n
    if (sArray(i) .eq. s) then
      r=.true.
    end if
  end do
end subroutine    

program p 
 use m
 logical :: l1, l2  
 sArray(1)='bla'
 sArray(2)='blo'
 sArray(3)='blu'
 call foo('ble',l1)
 call foo('blo',l2)
 if (.not. l1 .and. l2) then
  print *, 'OK'
 else 
  print *, 'BAD'
 end if
end program
