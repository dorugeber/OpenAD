program doUp
  integer :: i
  integer :: j=1

  do i=3,1,-1
    j=j+i
  end do

  if (j==7) then 
    print *, 'OK'
  else 
    print *, 'failed'
  end if
end program
