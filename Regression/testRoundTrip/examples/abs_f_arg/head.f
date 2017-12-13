c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision, dimension(2) :: x
	  double precision, dimension(2) :: y
c$openad INDEPENDENT(x)
	  x(1)=-1
          y=abs(x)
c$openad DEPENDENT(y)
	end subroutine
