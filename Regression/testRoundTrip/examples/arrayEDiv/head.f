c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision, dimension(2) :: x
	  double precision, dimension(2) :: y
c$openad INDEPENDENT(x)
          y=x/2.0
c$openad DEPENDENT(y)
	end subroutine
