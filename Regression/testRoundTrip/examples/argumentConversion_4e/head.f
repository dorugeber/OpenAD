c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision, dimension(2) :: x
	  double precision, dimension(1) :: y
	  double precision, dimension(2,3) :: t
c$openad INDEPENDENT(x)
	  do i=1,2
	    t(i,2)=x(i)
          end do
	  call barExt(t(1,2))
	  y(1)=t(1,2)
c$openad DEPENDENT(y)
	end subroutine
