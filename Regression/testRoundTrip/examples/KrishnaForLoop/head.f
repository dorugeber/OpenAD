c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision, dimension(2) :: x
	  double precision, dimension(1) :: y
	  integer :: i,a,b
	  double precision :: j
	  a = 1
	  b = 3
	  do i = a,b
	    j = 3.0D0
	  end do
c$openad INDEPENDENT(x)
	  y(1)=x(1)*x(2)
c$openad DEPENDENT(y)
	end subroutine

