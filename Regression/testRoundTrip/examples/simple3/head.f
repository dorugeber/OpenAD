c$openad XXX Template ad_template.f
	subroutine head(x1,x2,y1,y2) 
	  double precision v3,v4,x1,x2,y1,y2
c$openad INDEPENDENT(x1)
c$openad INDEPENDENT(x2)
	  v3=x1*x2
	  v4=x1*v3
          y1=v3*x2*v4
          y2=sin(v4)
c$openad DEPENDENT(y1)
c$openad DEPENDENT(y2)
	end subroutine
