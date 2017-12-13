c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	double precision, dimension(2) :: x
	double precision, dimension(2) :: y
        double precision, dimension(size(x)) :: z
	integer :: i,j, n=2
c$openad INDEPENDENT(x)
	z=x
        y=z
c$openad DEPENDENT(y)
	end subroutine

