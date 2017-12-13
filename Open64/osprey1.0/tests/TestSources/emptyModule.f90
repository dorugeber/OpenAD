module EMP

end module

module aEMP
	integer ai,aj,ak
end module

subroutine foo()
  use aEMP 
  aj=ai
end subroutine

program p
        use EMP
	use aEMP , only :  ai,ak
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
