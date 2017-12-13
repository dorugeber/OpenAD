c$openad XXX Template ad_template.f
        subroutine foo(x,y,a)
          double precision, dimension(1), intent(in) :: x
          double precision, dimension(1), intent(out) :: y
	  integer a
          y(a)=x(a)
        end subroutine


c$openad XXX Template ad_template.f
	subroutine head(x,y)
	  double precision, dimension(1), intent(in) :: x
	  double precision, dimension(1), intent(out) :: y
          integer a
c$openad INDEPENDENT(x)
c set a
	  a=1
	  call foo(x,y,a)
c unset a to make sure it is restored
	  a=20000
c$openad DEPENDENT(y)
	end subroutine
