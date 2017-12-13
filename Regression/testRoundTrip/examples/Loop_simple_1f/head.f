c$openad XXX Template ad_template.f
	subroutine head(x,y)
	  double precision, dimension(4), intent(in) :: x
	  double precision, dimension(4), intent(out) :: y
          integer i,j
c$openad INDEPENDENT(x)
c$openad xxx simple loop
	 do i=0,3
	    y(i+1)=x(i+1) 
	 end do
c$openad DEPENDENT(y)
	end subroutine
