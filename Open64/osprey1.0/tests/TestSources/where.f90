program where
  integer i(3)
  i=(/1,2,3/)
  where (i>1) 
    i=-1
  elsewhere
    i=0
  end where
  if (i(1)==0 .and. i(2)==-1 .and. i(3)==-1) then
	print *,'OK'
  else 
	print *,'failed',i
  end if 
end program
