module m
 contains 
 integer function foo(a) 
   integer :: a(3)
   foo=0
   foo=sum(a)
 end function
end module 

program p 
 use m
 integer :: i,j,k
 i=2;j=3;k=4
 i=foo([i,j,k])
 print *,i
end program
