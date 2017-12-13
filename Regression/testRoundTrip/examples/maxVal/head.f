c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision, dimension(2), intent(inout) :: x
	  double precision, dimension(1), intent(inout) :: y
c$openad INDEPENDENT(x)
	  y(1)=maxval(x)
c$openad DEPENDENT(y)
	end subroutine
