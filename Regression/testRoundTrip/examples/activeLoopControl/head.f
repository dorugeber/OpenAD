c$openad XXX Template ad_template.f
	subroutine head(x,y)
	  double precision, dimension(2), intent(in) :: x
	  double precision, dimension(1), intent(out) :: y
          integer i
c$openad INDEPENDENT(x)
          y(1)=x(1) 
          do i=int(x(1)),int(x(2))+2
            y(1)=y(1)*x(2)
          end do
c$openad DEPENDENT(y)
	end subroutine
