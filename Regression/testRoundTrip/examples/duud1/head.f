c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision, dimension(2) :: x
	  double precision, dimension(1) :: y
c$openad INDEPENDENT(x)
	  x(1)=2*x(1)
          y(1)=x(1)*x(2)
c$openad DEPENDENT(y)
	end subroutine
