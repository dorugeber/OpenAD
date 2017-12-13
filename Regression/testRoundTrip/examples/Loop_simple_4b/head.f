c$openad XXX Template ad_template.f
	subroutine foo(x,y,k)
	  double precision, dimension(2), intent(in) :: x
	  double precision, dimension(2), intent(inout) :: y
          integer i,j,k
c$openad xxx simple loop
          do i=1,2
 	    j=k
            y(j)=y(j)+x(i)*x(i)
          end do
	end subroutine

c$openad XXX Template ad_template.f
	subroutine head(x,y)
	  double precision, dimension(2), intent(in) :: x
	  double precision, dimension(2), intent(out) :: y
c$openad INDEPENDENT(x)
	  y(1)=1.0
	  y(2)=1.0
          call foo(x,y,1)
	  call foo(x,y,2)
c$openad DEPENDENT(y)
	end subroutine
