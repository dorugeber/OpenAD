module a
	integer ai,aj,ak
end module

subroutine foo()
  use a 
  aj=ai
end subroutine

program multRename
	use a , only :  ai,ak
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
