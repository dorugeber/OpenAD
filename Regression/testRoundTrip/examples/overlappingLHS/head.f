c$openad XXX Template ad_template.f
	subroutine head(x,y)
	  double precision, intent(in) :: x
	  double precision, dimension(5), intent(out) :: y
          integer i

c$openad INDEPENDENT(x)

          
c$openad xxx simple loop
          do i=1,5
            y(i)=x
            y(6-i)=x
          end do

c$openad DEPENDENT(y)
	end subroutine
