c$openad XXX Template ad_template.f
	subroutine head(x,y)
	  double precision, dimension(1), intent(in) :: x
	  double precision, dimension(1), intent(out) :: y
          integer i
	  character(10) :: aString

c$openad INDEPENDENT(x)
          
          y(1)=x(1) 
	  aString='blah'

c$openad xxx simple loop
          do i=1,3
            if (aString .eq. 'blah') then
              y(1)=y(1)*x(1)
            end if
            if (aString .eq. 'bloh') then
              y(1)=y(1)-x(1)
            end if
          end do
          y(1)=y(1) 
c$openad DEPENDENT(y)
	end subroutine
