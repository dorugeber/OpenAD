c$openad XXX Template ad_template.f
	subroutine foo(a) 
	  double precision a
	  a = a*2
	end subroutine

c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision, dimension(2) :: x
	  double precision y
	  integer i
c$openad INDEPENDENT(x)
	  i=1
	  call foo(x(i))
	  call foo(x(i))
	  y = x(1)*x(2)
c$openad DEPENDENT(y)
	end subroutine

