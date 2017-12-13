c$openad XXX Template ad_template.f
	subroutine head(x,y)
	  double precision, dimension(1), intent(in) :: x
	  double precision, dimension(1), intent(out) :: y
          integer i
c$openad INDEPENDENT(x)
          i=2
	  if (x(1).lt.1.0) then 
          select case (i) 
          case (2)
            y(1)=i*x(1)
	    return
          case default
            y(1)=i*x(1)
	    return
          end select
	  end if 
	  y(1)=x(1)*3
c$openad DEPENDENT(y)
	end subroutine
