c$openad XXX Template ad_template.f
	subroutine head(x,y)
	  double precision, dimension(4), intent(in) :: x
	  double precision, dimension(4), intent(out) :: y
          integer i

c$openad INDEPENDENT(x)
          
c$openad xxx simple loop
          do i=lbound(x,1),ubound(x,1)
            y(i)=x(i)
          end do
c$openad DEPENDENT(y)
	end subroutine
