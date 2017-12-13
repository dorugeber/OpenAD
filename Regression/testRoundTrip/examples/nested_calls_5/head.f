c$openad XXX Template ad_template.f
      subroutine head(x,y)
      double precision x(2),y(1)
      integer i
      i=1
c$openad INDEPENDENT(x)
      call foo(x(i),x(i+1),y(1))
c$openad DEPENDENT(y)
      end subroutine head

c$openad XXX Template ad_template.f
      subroutine foo(a,b,c) 
      double precision a,b,c
      c=a*a+b
      end subroutine

