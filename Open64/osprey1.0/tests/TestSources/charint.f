        program main
		character (len=10) :: str1="undefined", str2
		integer pos
		
		pos=scan("fortran", 'tr')+scan('fortran', 'tr', back=.TRUE.)
		if(pos.EQ.8) then
		  pos=verify('aabb','ab')+verify('abc','ab')
		  if(pos.EQ.3) then
		    if(lle("undefined", str1)) then
			   str2="abc"
			   if(lgt(str1,str2)) then
			     pos=len(str2)-len_trim(str2)
				  if(pos.EQ.7) then
				    print *, "OK"
				  else
				    print *, "failed"
				  end if
			   else
			     print *, "failed"
			   end if
			
			else
			  print *, "failed"
			end if
		  else
		    print *, "failed"
		  end if
		
		else
		  print *, "failed"
		end if
		end program main