subroutine foo(x)
double precision x
print *,x
end subroutine

program p
implicit real (q)
implicit real*8 (p)
implicit double precision (i-m)
j=3
call foo(j+5)
end program
