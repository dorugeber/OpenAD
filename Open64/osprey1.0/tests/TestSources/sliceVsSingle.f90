subroutine foo(x)
  integer :: x
  x=x+1
end subroutine

subroutine bar(x)
  integer :: x(1)
  x(1)=x(1)+1
end subroutine 

program arrays3
  integer, dimension(2) :: x,y
  x(1)=1
  y(2)=2
! see if this is unparsed as 
! a slice rather than a scalar
  call foo(x(1))
  call bar(y(2:2))
  if (x(1).eq.2 .and. y(2).eq.3) then 
	print *, 'OK'
  else
	print *, 'failed'
  endif 
end program
