c$openad XXX Template ad_template.f
	subroutine foo(x,y) 
	  double precision x
	  double precision y
	  y=x
	end subroutine

c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision x
	  double precision y, py
c$openad INDEPENDENT(x)
	  call foo(x,y)
	  call foo(2.0,py)
c$openad DEPENDENT(y)
	end subroutine

