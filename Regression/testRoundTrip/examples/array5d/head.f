c$openad XXX Template ad_template.f
      subroutine foo(a)
      double precision, dimension(1,1,1,1,1) :: a
      a(1,1,1,1,1)=a(1,1,1,1,1)*2
      end subroutine 

c$openad XXX Template ad_template.f
      subroutine head(x,y) 
      double precision, dimension(1) :: x
      double precision, dimension(1) :: y
      double precision, dimension(1,1,1,1,1) :: z
c$openad INDEPENDENT(x)
      z(1,1,1,1,1)=x(1)
      call foo(z)
      y(1)=z(1,1,1,1,1)
c$openad DEPENDENT(y)
      end subroutine

