c$openad XXX Template ad_template.f
	subroutine foo(a,n) 
	  use all_globals_mod
	  double precision :: a(n+g)
	  integer n
	  integer i
	  do i=1,n+1
	    a(i)=2*a(i)
	  end do
	end subroutine

c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  use all_globals_mod
	  double precision, dimension(3) :: x
	  double precision, dimension(1) :: y
	  integer i
c$openad INDEPENDENT(x)
	  g=1
	  i=2
	  call foo(x,2)
	  call foo(x,i)
	  y(1)=0
	  do i=1,3
	   y(1)=y(1)+x(i)
	  end do
c$openad DEPENDENT(y)
	end subroutine
