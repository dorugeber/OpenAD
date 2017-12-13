module m 
contains
subroutine foo(x)
implicit double precision (x)
print *,x
end subroutine
subroutine bar(x)
implicit none
double precision x
print *,x
end subroutine
end module

program p
use m
implicit none
double precision j
j=3
call foo(j+5)
end program
