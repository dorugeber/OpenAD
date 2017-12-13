c$openad XXX Template ad_template.f
	subroutine head(x,y)
	  double precision, dimension(2), intent(in) :: x
	  double precision, dimension(2), intent(out) :: y
          integer i,j
c$openad INDEPENDENT(x)
c$openad xxx simple loop
	 do i=1,2
	    k=i
c put control flow between definition and use of k
	    if (i==1) then
	      y(i)=2*x(i)
            end if
	    y(i)=x(k) 
	 end do
c$openad DEPENDENT(y)
	end subroutine
