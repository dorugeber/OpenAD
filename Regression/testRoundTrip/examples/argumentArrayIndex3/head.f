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
	  double precision, dimension(2) :: p,q
	  integer k,l
c$openad INDEPENDENT(x)
	  k=1
	  call foo(x(k),y)
c activate q, p stays passive
	  q(1)=y
 	  y=q(1)
	  p(1)=1.0
	  l=1
	  call foo(p(k),q(l))
c$openad DEPENDENT(y)
	end subroutine
