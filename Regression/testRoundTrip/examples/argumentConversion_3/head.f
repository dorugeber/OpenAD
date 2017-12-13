	module globals
	double precision gx
	double precision gy
	end module

	SUBROUTINE bar(barX,barY)
	double precision barx
	double precision bary
	double precision t
	t=barx
	barx=bary
	bary=t
	end subroutine

	SUBROUTINE foo( )
c foo is never called
c but gx and gy are active
	use globals
c here we need the conversion
	call bar(gx,gy)
	end subroutine
	
c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	use globals
	double precision, dimension(2) :: x
	double precision, dimension(1) :: y
c$openad INDEPENDENT(x)
	gx=x(1)
c make sure the globals are activated
	gy=gx
	y(1)=gy
c$openad DEPENDENT(y)
	end subroutine
