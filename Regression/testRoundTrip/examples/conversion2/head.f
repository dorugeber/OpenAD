c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision, dimension(2) :: x
	  double precision, dimension(2) :: y
	  double complex :: dc
c$openad INDEPENDENT(x)
	  dc=cmplx(x(1),x(2))
	  y(1)=real(dc)
	  y(2)=aimag(dc)
c$openad DEPENDENT(y)
	end subroutine
