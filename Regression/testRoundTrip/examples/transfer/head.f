c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision, dimension(1), intent(inout) :: x
	  double precision, dimension(1), intent(inout) :: y
          integer(8) :: i
c$openad INDEPENDENT(x)
C transfer is a passivating operation !
          i=transfer(x(1),i)
	  y(1) = x(1)+transfer(i,x(1))
c$openad DEPENDENT(y)
	end subroutine
