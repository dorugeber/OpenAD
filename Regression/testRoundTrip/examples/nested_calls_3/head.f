c$openad XXX Template ad_template.f
      subroutine head(x,y)
      double precision x(4),y(4)
      double precision c,d
c$openad INDEPENDENT(x)
      y(1)=x(3)
      Y(2)=x(4)
      call foo(x(1),x(2),c,d)
      y(3)=c*d
      y(4)=c+d
c$openad DEPENDENT(y)
      end subroutine head

c$openad XXX Template ad_template.f
      subroutine foo(a,b,c,d) 
      double precision a,b,c,d
      c=sin(a*b)
      d=cos(a+b) 
      end subroutine
