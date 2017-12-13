	module globals

	  double precision aGlobal

	end module


c$openad XXX Template ad_template.f
	subroutine foo(x,y) 

	  double precision, dimension(2) :: x
	  double precision y
	  y=x(1)*x(2)
	end subroutine

c$openad XXX Template ad_template.f
	subroutine head(x,y) 

          use globals

	  double precision, dimension(2) :: x
	  double precision, dimension(1) :: y
c$openad INDEPENDENT(x)
	  call foo(x,aGlobal)
          y(1)=aGlobal
c$openad DEPENDENT(y)
	end subroutine
