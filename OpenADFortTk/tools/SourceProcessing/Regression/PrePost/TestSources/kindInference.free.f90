module m 

 integer , parameter :: dp=kind(1.0D0)

end module 

module m1
 use m 
 integer, parameter :: rk=dp
end module 

subroutine foo(x)
 double precision :: x
 print *,x
end subroutine 

program p 
 use m 
 use m1
 real(dp) :: x1
 real (kind=rk) :: x2
 x1=1.0D0
 x2=2.0D0
 call foo(x1+x2) ! determine the kind of the temporary
end program
