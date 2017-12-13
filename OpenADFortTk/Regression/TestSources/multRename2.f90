module a
	integer longName
end module

module b
	integer longName
end module

module a1
	use a, only: s => longName
	private :: foo
	public :: a1Foo
	interface a1Foo
	module procedure foo
	end interface
	contains 
	subroutine foo()
	s=s+1
	end subroutine
end module 

module b1
	use b, only: s => longName
	private :: foo
	public :: b1Foo
        interface b1Foo
        module procedure foo
        end interface
        contains
        subroutine foo()
	s=s+1
        end subroutine
end module 

program multRename
	use a, only: aL=>longName
	use b, only: bL=>longName
	use a1
	use b1
	aL=1
	bL=2
 	call a1Foo()
	call b1Foo()
	print *, aL,bL
end program
