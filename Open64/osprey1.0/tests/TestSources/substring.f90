	program charArrSlice
	character(10) ::  s
	s="blabla"
	if ((s(2:2) .eq. "l") .and. s(2:4).eq."lab") then 
	  print *,'OK'
	endif
 	end 
