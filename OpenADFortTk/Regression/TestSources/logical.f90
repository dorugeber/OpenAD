program logical 

	logical a,b,c
	a=.true.
	b=.false.
        c=.false.
	if ( (a.and.b) .or. (b.and.c)) then 
	  print *,a
	else 
	  print *,b
        end if
end program 
