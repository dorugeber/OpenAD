c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision, dimension(2), intent(inout) :: x
	  double precision, dimension(4), intent(inout) :: y
	  double precision, dimension(4) :: b
	  double precision, dimension(2,2) :: A
c$openad INDEPENDENT(x)
          A=0.0
	  A(1,1) = x(1)
	  A(1,2) = x(2)
	  b = reshape(A,shape(b))
	  y = b
c$openad DEPENDENT(y)
	end subroutine

