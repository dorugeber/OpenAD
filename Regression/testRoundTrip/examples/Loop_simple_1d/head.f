	subroutine foo(x,y)
	double precision :: x,y
	  y=x
	end subroutine 

c$openad XXX Template ad_template.f
	subroutine head(x,y)
	  double precision, dimension(2), intent(in) :: x
	  double precision, dimension(2), intent(out) :: y
          integer i,j
c$openad INDEPENDENT(x)
c$openad xxx simple loop
	 do i=1,2
	    call foo(x(i),y(i)) 
	 end do
c$openad DEPENDENT(y)
	end subroutine
