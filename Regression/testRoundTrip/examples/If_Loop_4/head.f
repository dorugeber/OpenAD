c$openad XXX Template ad_template.f
	subroutine head(x,y)
	  double precision, dimension(10), intent(in) :: x
	  double precision, dimension(10), intent(out) :: y
	  double precision pi
          integer i
c$openad INDEPENDENT(x)
          pi=3.1415
          do i=1,9
            if (i>5) then
              y(i)=pi*sin(x(i))
c	      print *, 'x(',i,')=',x(i)
c	      print *, 'y(',i,')=',y(i)
            else
              y(i)=pi+cos(x(i))
            end if 
          end do
          y(10)=y(1)*y(9) 
c$openad DEPENDENT(y)
	end subroutine
