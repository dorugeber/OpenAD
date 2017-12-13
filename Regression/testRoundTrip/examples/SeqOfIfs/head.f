c$openad XXX Template ad_template.f
	subroutine head(x,y)
	  double precision, dimension(2), intent(in) :: x
	  double precision, dimension(2), intent(out) :: y
c$openad INDEPENDENT(x)
          if (x(1)<=x(2)) then 
            y(1)=x(2)-x(1)
          else
            y(1)=x(1)-x(2)
          end if
          if (y(1)==0) then 
            y(2)=x(1)
          else
            y(2)=y(1)
          end if
c$openad DEPENDENT(y)
	end subroutine
