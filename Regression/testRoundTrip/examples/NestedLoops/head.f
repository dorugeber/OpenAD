c$openad XXX Template ad_template.f
	subroutine head(x,y)
	  double precision, dimension(2), intent(in) :: x
	  double precision, dimension(2), intent(out) :: y
          integer i,j
c$openad INDEPENDENT(x)
          do i=1,2
	    do j=1,2
C       this was in here originally: 
C            do while(x(1)<>x(2))
              y(i)=x(i)*x(j)
            end do
          end do
c$openad DEPENDENT(y)
	end subroutine
