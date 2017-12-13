module aEMP
	integer ai,aj,ak
end module

subroutine foo()
  use aEMP 
  aj=ai
end subroutine

module EMP

end module

program p
	use aEMP , only :  ai,ak
        use EMP
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
