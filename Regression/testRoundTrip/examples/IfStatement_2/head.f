c$openad XXX Template ad_template.f
	subroutine head(x,y)
	  double precision, dimension(1), intent(in) :: x
	  double precision, dimension(1), intent(out) :: y
c$openad INDEPENDENT(x)
          y(1)=2. 
          if (x(1)>0.) then
            y(1)=x(1)
            if (x(1)>1.) then
              y(1)=y(1)
            else
              if (x(1)>0.) then
                y(1)=x(1)
              end if
            end if
          end if
          y(1)=y(1)*y(1)
c$openad DEPENDENT(y)
	end subroutine
