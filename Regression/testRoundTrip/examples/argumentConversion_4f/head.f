c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision, dimension(:) :: x
	  double precision, dimension(:) :: y
c$openad INDEPENDENT(x)
	  call barExt(x)
	  y(1)=x(1)+x(2)
          y(2)=-x(1)-x(2)
c$openad DEPENDENT(y)
	end subroutine
