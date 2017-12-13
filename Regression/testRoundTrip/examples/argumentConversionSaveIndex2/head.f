	module globals
	  integer i
	end module globals


	subroutine foo(p)
	  use globals
	  double precision, intent(inout) :: p
	  p = p*2
	  i = i+1
	end subroutine foo


c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  use globals
	  double precision, dimension(1), intent(in) :: x
	  double precision, dimension(1), intent(out) :: y
	  double precision, dimension(1) :: a
	  integer :: j
c$openad INDEPENDENT(x)
	  i = 1
	  j = 0
	  a(1) = 0.00908583
	  call foo(a(i+j))
	  call foo(x(1))
	  y(1) = x(1)
c$openad DEPENDENT(y)
	end subroutine head

