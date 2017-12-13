c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision, dimension(1), intent(inout) :: x
	  double precision, dimension(1), intent(inout) :: y
	  double precision t1,t2,t3,t4,t5,t6,t7,y1
c$openad INDEPENDENT(x)
	  t1=x(1)
	  t2=t1+2
          t3=t2/2
          t4=t3*2
          t5=t4+4
          t6=t5+3
          t7=t6-9
          y1=exp(t7)
C  	  y1=t1
          y(1)=y1
c$openad DEPENDENT(y)
	end subroutine
