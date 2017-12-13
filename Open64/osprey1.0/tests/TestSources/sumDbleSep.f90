program sumVar
	real :: v(3)
	double precision :: d(3),s
	v(1)=1.0
	v(2)=2.0
	v(3)=3.0
        d=dble(v)
	s=sum(d)
	if (s==6.0D0) then 
	print *,'OK'
	else 	
	print *,'FAILED'
	end if
end program
