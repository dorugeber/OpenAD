c$openad XXX Template ad_template.f
	subroutine foo(x,y) 
	  double precision x
	  double precision y
	  y=x*x
	end subroutine

c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision, dimension(2) :: x, px
	  double precision y, py
c$openad INDEPENDENT(x)
	  call foo(x(1),y)
	  call foo(px(1),py)
c$openad DEPENDENT(y)
	end subroutine

