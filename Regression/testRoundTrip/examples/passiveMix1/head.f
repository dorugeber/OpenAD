c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision, dimension(2), intent(inout) :: x
	  double precision, dimension(2), intent(inout) :: y
          double precision t
	  integer i 
c$openad INDEPENDENT(x)
	  i=1
          t=x(i)
          i=2
          t=t*x(i)
	  i=3
	  y(1)=sin(t)
	  y(2)=cos(t)
c$openad DEPENDENT(y)
	end subroutine
