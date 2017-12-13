program sumVar
	double precision:: v(3)
        real :: s
	v(1)=1.0D0
	v(2)=2.0D0
	v(3)=3.0D0
	s=sum(real(v))
	if (s==6.0) then 
	print *,'OK'
	else 	
	print *,'FAILED'
	end if
end program
