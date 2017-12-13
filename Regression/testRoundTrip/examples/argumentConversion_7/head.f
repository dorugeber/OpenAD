	subroutine foo(t1,t2)
c$openad XXX Template ad_template.f
	  double precision :: t1(2),t2(2)
          t1=2*t1
          print *,t2
	end subroutine

	subroutine bar(t1,t2)
c$openad XXX Template ad_template.f
	  double precision :: t1(2),t2(2)
          t1=2*t1
          print *,t2
	end subroutine

	subroutine head(x,y) 
	interface 
	subroutine bar(t1,t2)
	double precision :: t1(2),t2(2)
	end subroutine
	end interface
c$openad XXX Template ad_template.f
	  double precision, dimension(4) :: x,y
c$openad INDEPENDENT(x)
c f77 style call
	  call foo(x(1),x(3))  
c f90 style call
	  call bar(x(1:2),x(3:4))
	  y=x
c$openad DEPENDENT(y)
	end subroutine

