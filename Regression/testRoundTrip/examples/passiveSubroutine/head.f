c$openad XXX Template ad_template.f
      subroutine head(x,y)
      double precision x(1),y(1)
      double precision p,q
c$openad INDEPENDENT(x)
      call foo(x(1),y(1))
      p=1.0
      call bar(p,q)
c$openad DEPENDENT(y)
      end subroutine head

c$openad XXX Template ad_template.f
      subroutine foo(a,b) 
      double precision a,b
      b=a*a
      end subroutine

c$openad XXX Template ad_template.f
      subroutine bar(a,b)
      double precision a,b
      b=cos(a)
      end subroutine
