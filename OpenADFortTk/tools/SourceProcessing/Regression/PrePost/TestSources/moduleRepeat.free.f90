module m 
 public 
 real :: x(3)
end module

subroutine foo
use m
print *,x
end 

subroutine bar
use m 
print *,x
end 

program p 
use m
x=(/.1,.2,.3/)
call foo
call bar
end 
