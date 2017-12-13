MODULE mymod
  INTEGER myint
CONTAINS
  LOGICAL FUNCTION funand (arg1, arg2)
	LOGICAL, INTENT(IN) :: arg1, arg2
	funand=arg1 .AND. arg2
  END FUNCTION
END MODULE

PROGRAM MAIN
USE mymod
LOGICAL x1
LOGICAL x2
x1= .TRUE.
x2= funand(x1, x1)
if(x2) then
  print *, "OK"
else
  print *, "failed"
end if
END


