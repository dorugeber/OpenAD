c$openad XXX Template ad_template.f
	subroutine foo(a) 
	  double precision a
	  a = a*2
	end subroutine

c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision, dimension(2) :: x
	  double precision y
	  integer, dimension(4) :: a
	  integer i
c$openad INDEPENDENT(x)
	  a(1)=2
	  a(2)=1
	  a(3)=2
	  a(4)=3
	  i=1
	  call foo(x(a(a(a(a(a(a(a(a(a(a(a(4)))))))))))))
	  y = x(1)*x(2)
c$openad DEPENDENT(y)
	end subroutine

