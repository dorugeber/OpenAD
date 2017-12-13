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
	  double precision, dimension(2) :: t
	  integer i
c$openad INDEPENDENT(x)
	  i=2
	  call barExt(t(i))
	  i=1
	  call foo(x,y(i))
c$openad DEPENDENT(y)
	end subroutine
