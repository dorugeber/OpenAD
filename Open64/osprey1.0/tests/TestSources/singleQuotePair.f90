program singelQuotePair
	character(10) s
	integer i
	i =39
	write (s,'(A)')  '''bla'''
	if (s(5:5)==char(i)) then 
	print *, "OK"
	else 
	print *, "failed, s is ", s
	end if
end
