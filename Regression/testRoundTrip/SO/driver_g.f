      subroutine driver_g(x,g)

      use active_module
      implicit none 
      
      double precision, dimension(2) :: x,g 
      
      type(active), dimension(2) :: active_x
      type(active) :: active_y
      integer i,j


c forward approach for each direction
      do i=1,2
c initialize
        do j=1,2
          active_x(j)%v=x(j)
          if (i==j) then
	    active_x(j)%d=1.0
          else
	    active_x(j)%d=.0
          end if
        end do
        call head(active_x,active_y)
        g(i)=active_y%d
      end do

      end 
