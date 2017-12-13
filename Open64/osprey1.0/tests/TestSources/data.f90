program sumVar
	double precision:: v(3),w(3)
	data w/1,2,3/
	v(1)=1.0D0
	v(2)=2.0D0
	v(3)=3.0D0
	if (w(3)==v(3)) then 
	print *,'OK'
	else 	
	print *,'FAILED'
	end if
end program
