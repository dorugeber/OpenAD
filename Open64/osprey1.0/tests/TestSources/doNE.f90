program doUp
  integer :: i
  integer :: j=1
  i=3
  do while(i.ne.0) 
    j=j+i
    i=i-1
  end do

  if (j==7) then 
    print *, 'OK'
  else 
    print *, 'failed j is', j
  end if
end program
