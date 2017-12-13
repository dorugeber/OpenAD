c$openad XXX Template ad_template.f
      subroutine driver_g(x,g)

      implicit none 
      
      double precision, dimension(2) :: x,g 
      
      double precision , dimension(2,2) :: active_x
      double precision , dimension(2) :: active_y
      integer i,j

c$openad INDEPENDENT(x)
c forward approach for each direction
      do i=1,2
c initialize
        do j=1,2
          active_y(2)=0
          active_x(j,1)=x(j)
          if (i==j) then
	    active_x(j,2)=1.0
          else
	    active_x(j,2)=.0
          end if
        end do
        call head(active_x,active_y)
        g(i)=active_y(2)
      end do
c$openad DEPENDENT(g)
      end 
