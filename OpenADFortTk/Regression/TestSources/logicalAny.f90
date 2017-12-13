program logicalAny 
	logical a(3),b(2)
	a(1)=.true.
	a(2)=.false.
        a(3)=.false.
        b(1)=.false.
        b(2)=.false.
	if ( any(a).and. .not.any(b)) then 
	  print *,a,b
	else 
	  print *,"not ok"
        end if
end program 
