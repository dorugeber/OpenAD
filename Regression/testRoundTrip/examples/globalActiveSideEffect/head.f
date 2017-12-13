	module globals
	  double precision :: aGlobalActive
	end module globals


	subroutine foo(p)
	  use globals
	  double precision, intent(inout) :: p
	  aGlobalActive = aGlobalActive*p
	end subroutine foo


c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  use globals
	  double precision, dimension(1), intent(in) :: x
	  double precision, dimension(1), intent(out) :: y
	  double precision :: anInactive
c$openad INDEPENDENT(x)
	  anInactive = 44.456
	  aGlobalActive = 1.61803399
	  call foo(x(1))
	  call foo(anInactive)
	  y(1) = sin(aGlobalActive)
c$openad DEPENDENT(y)
	end subroutine head

