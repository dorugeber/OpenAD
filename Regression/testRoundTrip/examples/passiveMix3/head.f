	subroutine foo(m1,m2)
	  implicit none
	  double precision,intent(in) :: m1
	  double precision,intent(inout) :: m2
	  m2 = min(m1,m2)
	end subroutine foo

c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  implicit none
	  double precision, dimension(1), intent(inout) :: x
	  double precision, dimension(1), intent(inout) :: y
	  double precision t,t1,p
c$openad INDEPENDENT(x)
	  t=2.0D0
	  p = sqrt(3.0D0)
	  t1=min(t,x(1))
c different signature here should trigger activation of both elements
	  y(1)=min(x(1),t1)
	  call foo(p,t1)
c$openad DEPENDENT(y)
	end subroutine
