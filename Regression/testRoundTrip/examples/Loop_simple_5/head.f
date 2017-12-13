c$openad XXX Template ad_template.f
	subroutine head(x,y)
	  double precision, dimension(3), intent(in) :: x
	  double precision, dimension(3), intent(out) :: y
          integer i,j,k
c$openad INDEPENDENT(x)
c$openad xxx simple loop	 
          do i=1,3
	    if (i.eq.2) then 
	       y(i)=x(i) 
	    else
	       y(i)=2*x(i) 
	    end if
          end do
c$openad DEPENDENT(y)
	end subroutine
