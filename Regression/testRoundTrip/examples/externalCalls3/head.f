c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision :: x
	  double precision :: y
	  double precision :: t1, t2
c$openad INDEPENDENT(x)
	  t1=x
	  call barExt(x,t2)
          y=t2
c$openad DEPENDENT(y)
	end subroutine
