c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision, dimension(2), intent(inout) :: x
	  double precision, dimension(4), intent(inout) :: y
	  double precision t1,t2,y1,y2,y3,y4
c$openad INDEPENDENT(x)
	  t1=x(1)+x(2)
	  t2=-t1
          y1=t2
          y2=3.14-t2
	  y3=t2
          y4=t2+t1
          y(1)=y1
          y(2)=y2
	  y(3)=y3
          y(4)=y4
c$openad DEPENDENT(y)
	end subroutine
