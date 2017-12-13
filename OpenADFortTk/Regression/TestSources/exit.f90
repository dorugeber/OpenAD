program ex
  integer i,j
  i=1; j=0
  do i=1,4,1
  if (i.lt.3) then 
  exit 
  end if
  j=j+1
  end do
  print *,j
end program
