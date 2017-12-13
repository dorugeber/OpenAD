	program charArrSlice
	character(10), dimension(2) ::  sa
	sa(2)="blabla"
        sa(1)="grigri"
	if (sa(2)(3:3) .eq. "a") then 
	  print *,'OK'
	endif
 	end 
