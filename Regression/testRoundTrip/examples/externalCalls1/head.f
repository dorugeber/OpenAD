c$openad XXX Template ad_template.f
	subroutine foo(x,y) 
	  double precision, dimension(2) :: x
	  double precision y
	  double precision t
	  t=x(1)
	  call barExt(t)
	  y=t*x(2)
	end subroutine

c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision, dimension(2) :: x
	  double precision, dimension(1) :: y
	  double precision t
c$openad INDEPENDENT(x)
	  call barExt(t)
	  call foo(x,y)
c$openad DEPENDENT(y)
	end subroutine
