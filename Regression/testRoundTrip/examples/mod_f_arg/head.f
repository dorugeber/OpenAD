c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision, dimension(1) :: x
	  double precision, dimension(1) :: y
c$openad INDEPENDENT(x)
	  x(1)=x(1)*10
          y(1)=mod(x(1),2.0D0)
c$openad DEPENDENT(y)
	end subroutine
