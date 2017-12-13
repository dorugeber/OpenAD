c$openad XXX Template ad_template.f
	subroutine head(x,y)
	  double precision, dimension(5), intent(in) :: x
	  double precision, dimension(3), intent(out) :: y
	  double precision t

c$openad INDEPENDENT(x)

          t=x(1)*x(2)*x(3)
          y(1)=sin(t)
          t=t*x(4)*x(5)
          y(2)=sin(t)
          y(3)=cos(t)

c$openad DEPENDENT(y)
	end subroutine
