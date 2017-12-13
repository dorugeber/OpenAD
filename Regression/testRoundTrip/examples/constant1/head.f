	module anotherModule
          use all_globals_mod
	  double precision, parameter :: pi2=PI
	end module


c$openad XXX Template ad_template.f
	subroutine head(x,y)
	  use anotherModule 
	  double precision, dimension(1) :: x
	  double precision, dimension(1) :: y
c$openad INDEPENDENT(x)
          y(1)=sin(x(1)*pi2*2)
c$openad DEPENDENT(y)
	end subroutine
