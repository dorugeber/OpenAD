c$openad XXX Template ad_template.f
	subroutine head(x,y)
	  double precision x(2),y(2)
          integer i
	  double precision s
c$openad INDEPENDENT(x)
          i=1
          y(1)=0.0
	  y(2)=0.0
c$openad xxx simple loop
	  do i=1,2
            s=x(i)
	    if (s.gt.0.D0) then 
	       s=0.D0
            end if
            y(i)=x(i)+s
          end do
c$openad DEPENDENT(y)
	end subroutine
