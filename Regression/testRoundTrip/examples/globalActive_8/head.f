	module globals

	  double precision aGlobal

	end module

c$openad XXX Template ad_template.f
	subroutine foo(x,y) 

          use globals 

	  double precision, dimension(2) :: x
	  double precision y
	  aGlobal=x(1)*x(2)
	  y=aGlobal
	end subroutine

c$openad XXX Template ad_template.f
	subroutine head(x,y) 

          use globals

	  double precision, dimension(2) :: x
	  double precision, dimension(2) :: y
	  double precision :: p 
c$openad INDEPENDENT(x)
	  call foo(x,y(1))
          call bar(p)
	  y(2)=p
c$openad DEPENDENT(y)
	end subroutine

c$openad XXX Template ad_template.f
	subroutine bar(aP) 
          use globals
	  double precision :: aP
	  aP=aGlobal**3
	end subroutine
