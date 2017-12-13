program exU
  integer i,ii,j
  j=0
  do i=1,4,1
  j=j+1
  do ii=1,4,1
     exit 
  end do
  end do
  print *,j
end program
