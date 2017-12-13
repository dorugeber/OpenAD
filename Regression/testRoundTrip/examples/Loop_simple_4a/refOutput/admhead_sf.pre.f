c$openad XXX Template ad_template.f
	subroutine head(j,x,y)
	  double precision, dimension(1), intent(in) :: x
	  double precision, dimension(1), intent(out) :: y
	  integer i,j
c$openad INDEPENDENT(x)
c$openad xxx simple loop
          do i=1,2
            y(j)=x(j)
          end do
c$openad DEPENDENT(y)
	end subroutine
