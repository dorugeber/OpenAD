program selGoto
  integer i,j
  i=1; j=0
  if (i.lt.3) then 
  goto 1
  end if
  select case(i) 
  case (1)
    j=2
  case default
    j=1
  end select
1 print *,j
end program
