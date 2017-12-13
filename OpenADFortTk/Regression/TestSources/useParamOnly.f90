module parmMod
	integer, parameter :: c=3
end module

program useParm
	use parmMod, only: c
	integer i 
	i=2
	i=i*c
	if (i==6) then 
	print*,'OK'
	else 
	print*,'FAILED'
	end if 
end program
