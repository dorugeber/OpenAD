      subroutine foo(x)
	character(*) x
	print *,x
      end subroutine 
      program p
        call foo("")
      end program p
