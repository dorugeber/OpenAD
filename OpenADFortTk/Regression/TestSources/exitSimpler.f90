program exS
  integer i,j
  j=0
  do i=1,4,1
  j=j+1
  if (i.lt.3) exit 
  end do
  print *,j
end program
