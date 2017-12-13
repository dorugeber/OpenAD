	subroutine foo(x,t)
	  double precision, intent(in) :: x
	  double precision, intent(inout) :: t
	  t = t*x
	end subroutine foo


c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision, dimension(1), intent(in) :: x
	  double precision, dimension(1), intent(out) :: y
	  double precision :: t
c$openad INDEPENDENT(x)
	  i = 1
	  t = x(i)
	  call foo(x(i),t)
	  y(i) = t
c$openad DEPENDENT(y)
	end subroutine head

