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
	 j=1
	 do i=1,2
	    call foo(x(j),y(j)) 
	 end do
	 j=2
	 call foo(x(j),y(j))
c$openad DEPENDENT(y)
	end subroutine
