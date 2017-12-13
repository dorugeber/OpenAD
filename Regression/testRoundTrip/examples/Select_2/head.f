
C -------------------------------------------------------
C NOTE: THIS EXAMPLE SEGVS IN REVERSE BECAUSE OF BUG #78
C -------------------------------------------------------

c$openad XXX Template ad_template.f
	subroutine head(x,y)
	  double precision, dimension(3), intent(in) :: x
	  double precision, dimension(3), intent(out) :: y
          integer i
c$openad INDEPENDENT(x)
          do i=1,3
          select case (i) 
          case (1)
            y(i)=sin(x(i))
          case (2)
            y(i)=cos(x(i))
          case default
            y(i)=2*x(i)
          end select
	  end do
c$openad DEPENDENT(y)
	end subroutine
