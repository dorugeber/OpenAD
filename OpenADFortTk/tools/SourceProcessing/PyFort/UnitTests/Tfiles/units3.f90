module tt
   real x(10),y(15)
 contains
   real function foo(x)
     foo = sin(x) * 3.3
   end

   double precision function bar(a,b)
     double precision a,b
     a = x(3)
     b = y(10)
     bar = a * b
   end

end

subroutine foo(x)
   real,dimension(10) :: x

   x = x ** 2
end

!
! trailing comment block
!
