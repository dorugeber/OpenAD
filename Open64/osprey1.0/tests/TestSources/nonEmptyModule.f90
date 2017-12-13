module NEM
 integer i
end module

module aNEM
	integer ai,aj,ak
end module

subroutine foo()
  use aNEM
  aj=ai
end subroutine

program p
        use NEM
	use aNEM , only :  ai,ak
	integer aj
        aj=1
        ai=2
        call foo()
        if (aj ==1 ) then 
	  print *, 'OK'
        else 
          print *,'FAILED'
        end if
end program
