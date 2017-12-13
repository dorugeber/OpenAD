subroutine foo()
common /cb/ g
double precision g
g=g*2.0
end subroutine

subroutine foo1()
common /cb/ g
double precision g
g=g*2.0
end subroutine

subroutine bar()
call foo()
call foo1()
end subroutine

program p 
common /cb/ g
double precision g
g=1.0
call bar()
print *, g
end program
