c$openad XXX Template ad_template.f
	subroutine foo() 
	end subroutine

c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision, dimension(2) :: x
	  double precision y
	  double precision t, t1
c$openad INDEPENDENT(x)
	  t=x(1)
	  call foo()
	  y=2*t
c$openad DEPENDENT(y)
	end subroutine

