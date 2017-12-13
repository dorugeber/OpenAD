c$openad XXX Template ad_template.f
	subroutine head(x,y)
	  double precision, dimension(3), intent(in) :: x
	  double precision, dimension(3), intent(out) :: y
          integer i,j,k
c$openad INDEPENDENT(x)
          do i=1,3
            y(i)=x(i) 
          end do
          do i=1,3
            do j=1,3
              do k=1,3
                y(i)=y(i)*x(j)
              end do
            end do
          end do
c$openad DEPENDENT(y)
	end subroutine
