module a
	integer longName
end module

module b
	use a, only: s => longName
	interface bFoo
	module procedure foo
	end interface
	contains 
	subroutine foo()
	s=s+1
	end subroutine
end module 

program multRename
	use a
	use b
	longName=1
 	call bFoo()
	if (longName==2) then 
	print *, 'OK'
	else 
	print *, 'FAILED'
	end if
end program
