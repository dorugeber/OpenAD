c$openad XXX Template ad_template.f
	subroutine head(x,y)
	  double precision, dimension(1), intent(in) :: x
	  double precision, dimension(1), intent(out) :: y
          integer i,j,k
c$openad INDEPENDENT(x)
          do i=1,1 
            y(1)=x(1) 
            if (y(1)>0.) then
              do j=1,3
                do k=1,3
                  if (x(1)>0) then
                    y(1)=y(1)*x(1)
                  end if 
                end do 
              end do
            else
              y(1)=y(1)/x(1)
            end if 
          end do
c$openad DEPENDENT(y)            
	end subroutine
