c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision,intent(in), dimension(2) :: x
	  double precision,intent(out) :: y
c$openad INDEPENDENT(x)
	  y=sin(x(1))+sqrt(x(2))+x(1)*x(2)
c$openad DEPENDENT(y)
	end subroutine
