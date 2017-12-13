c$openad XXX Template ad_template.f
      subroutine head(x,y)
      double precision x(1),y(1)
      integer i
c$openad INDEPENDENT(x)
	y(1)=x(1)
	i=1+INT(x(1)-1)
c$openad DEPENDENT(y)
      end subroutine head
