c$openad XXX Template ad_template.f
	subroutine foo(a,k) 
	  double precision a
	  integer k
	  a = a*k
	  k = 2*k
	end subroutine

c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision, dimension(2) :: x
	  double precision y
	  integer i,j
c$openad INDEPENDENT(x)
	  i=1
	  j=1
	  call foo(x(i+j),i)
	  call foo(x(i-j),i)
	  y = x(1)*x(2)
c$openad DEPENDENT(y)
	end subroutine

