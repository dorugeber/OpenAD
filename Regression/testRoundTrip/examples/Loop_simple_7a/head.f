c$openad XXX Template ad_template.f
	subroutine bar(x,y, k )
	  double precision, dimension(3), intent(in) :: x
	  double precision, dimension(3), intent(out) :: y
          integer i,k
c$openad xxx simple loop	 
	  do i=1,min(k,3)
	    y(i)=x(i) 
	  end do
	end subroutine

c$openad XXX Template ad_template.f
	subroutine head(x,y)
	  double precision, dimension(3), intent(in) :: x
	  double precision, dimension(3), intent(out) :: y
          integer i,j,k
c$openad INDEPENDENT(x)
	  k=3
	  call bar(x,y,k)
c$openad DEPENDENT(y)
	end subroutine
