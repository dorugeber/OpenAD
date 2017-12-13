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

module c
	use a, only: s => longName
        interface cBar
        module procedure bar
        end interface
        contains
        subroutine bar()
	use b
	s=s+1
        call bFoo()
        end subroutine
end module 

program multRename
	use a
	use c
	longName=1
 	call cBar()
	print *, longName
end program
