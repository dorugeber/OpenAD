c$openad XXX Template ad_template.f
	subroutine head(x,y)
	  double precision, dimension(1), intent(inout) :: x
	  double precision, dimension(1), intent(inout) :: y
          integer i
c$openad INDEPENDENT(x)
          
          y(1)=x(1) 
          if (y(1)>0.) then
            do i=1,3
              y(1)=y(1)*x(1)
            end do
          else
            y(1)=y(1)/x(1)
          end if 
c$openad DEPENDENT(y)            
	end subroutine
