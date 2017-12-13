c$openad XXX Template ad_template.f
	subroutine foo(x,y) 
	  double precision, dimension(4):: x
	  double precision y
	  y=x(2)*x(4)
	end subroutine

c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision, dimension(2) :: x
	  double precision, dimension (2,2) :: t, p
	  double precision :: y, py
c$openad INDEPENDENT(x)
	  t(2,1)=x(1)
	  t(2,2)=x(2)
	  call foo(t,y)
	  call foo(p,py)
c$openad DEPENDENT(y)
	end subroutine

