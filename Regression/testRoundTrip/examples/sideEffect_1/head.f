	module globals

	  double precision aGlobal

	end module


c$openad XXX Template ad_template.f
	subroutine foo(x,y) 

	  double precision, dimension(1) :: x
	  double precision y
	  y=x(1)*x(1)
	end subroutine

c$openad XXX Template ad_template.f
	subroutine bar(x) 
          use globals
	  double precision :: x
	  x=x*aGlobal
	end subroutine

c$openad XXX Template ad_template.f
	subroutine head(x,y) 
          use globals
	  double precision, dimension(1) :: x
	  double precision, dimension(1) :: y
c$openad INDEPENDENT(x)
	  aGlobal=2.0
	  call bar(x(1))
	  call foo(x,aGlobal)
          y(1)=aGlobal
c$openad DEPENDENT(y)
	end subroutine
