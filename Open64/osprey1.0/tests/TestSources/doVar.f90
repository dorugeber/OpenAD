program doUp
  integer :: i,l,u,s
  integer :: j=1, k=1

  l=1
  u=3
  s=1
  do i=l,u,s
    j=j+i
  end do
  l=3
  u=1
  s=-1
  do i=l,u,s
    k=k+i
  end do

  if (j==7 .and. k==7) then 
    print *, 'OK'
  else 
    print *, 'failed'
  end if
end program
