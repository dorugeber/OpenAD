      subroutine foo(a,b,c,d)
      double precision :: a,c
      double precision, optional :: b,d
      c=2*a
      if (present(b).and.present(d)) then 
      d=3*b
      end if
      end subroutine

      subroutine head(x,y)
      interface 
       subroutine foo(a,b,c,d)
       double precision :: a,c
       double precision, optional :: b,d
       end subroutine
      end interface 
      double precision :: x(3),y(3),u,v
!$openad INDEPENDENT(x)
!$openad DEPENDENT(y)
      call foo(a=x(1),c=y(1))
      call foo(x(2),x(3),y(2),y(3))
      end subroutine
