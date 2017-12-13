c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision, dimension(2), intent(inout) :: x
	  double precision, dimension(2), intent(inout) :: y
          double precision t(1,2)
c$openad INDEPENDENT(x)
          t(1,:)=x
	  y=t(1,:)
c$openad DEPENDENT(y)
	end subroutine
