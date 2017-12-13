subroutine foo
common /c/ x(3)
real :: x
print *,x
end 

program p 
common /c/ x(3)
real :: x
x=(/.1,.2,.3/)
call foo
end 
