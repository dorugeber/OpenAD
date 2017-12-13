c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision :: x
	  integer :: y
c$openad INDEPENDENT(x)
          y=int(x)
c$openad DEPENDENT(y)
	end subroutine
