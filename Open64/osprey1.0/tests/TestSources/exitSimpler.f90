subroutine foo (j)
  integer i,j
  j=0
  do i=1,4,1
  j=j+1
  if (i.lt.3) exit
  end do
end subroutine 

program exS
  integer i,j
  j=0
  call foo(j)
  if (j.eq.1) then 
     print *,'OK'
  else 
     print *,j
  end if
end program
