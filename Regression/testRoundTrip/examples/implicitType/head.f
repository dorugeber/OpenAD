c$openad XXX Template ad_template.f
	subroutine head(x,y) 
          implicit double precision (s,t)
	  double precision, dimension(1), intent(inout) :: x
	  double precision, dimension(1), intent(inout) :: y
c$openad INDEPENDENT(x)
          s=x(1)
          t=s
	  y(1)=sin(t)
c$openad DEPENDENT(y)
	end subroutine
