c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision, dimension(1) :: x
	  double precision, dimension(1) :: y
c$openad INDEPENDENT(x)
          y(1)=INT(x(1))*2.5
c$openad DEPENDENT(y)
	end subroutine
