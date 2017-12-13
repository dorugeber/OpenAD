subroutine foo()
common /  / x
real x
print *,x
end subroutine

program p 
common // x
real x
x=.3
call foo()
end program
