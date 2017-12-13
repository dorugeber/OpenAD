	module globals

	  double precision aGlobal
	  double precision aPassiveGlobal

	end module

c$openad XXX Template ad_template.f
	subroutine head(x,y) 

          use globals

	  double precision, dimension(2) :: x
	  double precision, dimension(1) :: y
c$openad INDEPENDENT(x)
	  aPassiveGlobal=2.0
	  aGlobal=x(1)*x(2)
          y(1)=aGlobal*aPassiveGlobal
c$openad DEPENDENT(y)
	end subroutine
