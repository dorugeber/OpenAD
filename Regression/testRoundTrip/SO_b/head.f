c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision,intent(in), dimension(2) :: x
	  double precision,intent(out) :: y
c$openad INDEPENDENT(x)
c	  y=sin(x(1))+sqrt(x(2))+x(1)*x(2)
	  y=x(1)**2*x(2)**3
c	  y=x(1)*x(2)
c$openad DEPENDENT(y)
	end subroutine
