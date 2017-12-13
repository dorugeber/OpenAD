c$openad XXX Template ad_template.f
	subroutine foo(x,y) 
	  double precision x
	  double precision y
	  y=x*2
	end subroutine

c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision, dimension(2) :: x
	  double precision y
	  integer k
c$openad INDEPENDENT(x)
	  k=1
	  call foo(x(k),x(k+1))
	  call foo(x(k),y)
c$openad DEPENDENT(y)
	end subroutine

