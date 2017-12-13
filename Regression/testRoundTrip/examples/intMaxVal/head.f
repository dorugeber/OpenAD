c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision, dimension(2), intent(inout) :: x
	  double precision, dimension(2), intent(inout) :: y
          double precision t
	  integer :: i(3),maxI
c$openad INDEPENDENT(x)
	  i(1)=1
          i(2)=0
          i(3)=-1
	  maxI=maxval(i)
          t=x(maxI)*x(2)
	  y(maxI)=sin(t)
	  y(2)=cos(t)
c$openad DEPENDENT(y)
	end subroutine
