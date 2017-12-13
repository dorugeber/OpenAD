c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision, dimension(2), intent(inout) :: x
	  double precision, dimension(2), intent(inout) :: y
	  double precision, dimension(2,2) :: A,B
c$openad INDEPENDENT(x)
          A=0.0
	  A(1,1) = x(1)
	  A(1,2) = x(2)
	  B = transpose(A)
	  y(1) = b(1,1)
	  y(2) = b(2,1)
c$openad DEPENDENT(y)
	end subroutine

