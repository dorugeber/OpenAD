	module globals
	  integer i
	end module globals


	subroutine foo(p,q)
	  use globals
	  double precision, intent(in) :: p
	  double precision, intent(inout) :: q
	  q = p*q
	  i = i+1
	end subroutine foo


c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  use globals
	  double precision, dimension(2), intent(in) :: x
	  double precision, dimension(1), intent(out) :: y
	  double precision, dimension(2) :: v
c$openad INDEPENDENT(x)
	  i = 1
	  y(1)=1.0D0
	  v(1) = 0.00908583
	  v(2) = -0.0002984642
	  call foo(x(1),y(1))
	  call foo(x(i),v(i))
	  y(1) = y(1)*v(2)
c$openad DEPENDENT(y)
	end subroutine head

