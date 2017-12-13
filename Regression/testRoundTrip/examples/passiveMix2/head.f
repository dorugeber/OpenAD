c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision, dimension(1), intent(inout) :: x
	  double precision, dimension(1), intent(inout) :: y
          double precision t,t1,t2
c$openad INDEPENDENT(x)
	  t=2.0D0
          t1=min(t,x(1))
c different signature here should trigger activation of both elements
          y(1)=min(t1,t)
          t2=min(t,2.5D0)
c$openad DEPENDENT(y)
	end subroutine
