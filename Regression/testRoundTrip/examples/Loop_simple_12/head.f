c$openad XXX Template ad_template.f
	subroutine head(x,y)
	  double precision x(2),y(2)
          integer i,j
c$openad INDEPENDENT(x)
          i=1
          y(1)=0.0
	  y(2)=0.0
c$openad xxx simple loop
	  do i=1,2
            j=1
            do while (j<3)
              y(i)=y(i)+x(i)*x(j)
              j=j+1
            end do
          end do
c$openad DEPENDENT(y)
	end subroutine
