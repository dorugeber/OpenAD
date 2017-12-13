c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision, dimension(1) :: x
	  double precision, dimension(1) :: y
c$openad INDEPENDENT(x)
          call logIt('string ending with a double quote"')
          y(1)=x(1)
c$openad DEPENDENT(y)
	end subroutine
