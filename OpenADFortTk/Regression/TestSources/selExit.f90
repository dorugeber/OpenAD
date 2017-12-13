program se
  integer i,j
  i=1; j=0
  do i=1,4,1
  if (i.lt.3) then 
  exit 
  end if
  end do
  select case(i) 
  case (1)
    j=2
  case default
    j=1
  end select
1 print *,j
end program
