c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision, dimension(1) :: x
	  double precision, dimension(1) :: y
           Real*8 PI
           PARAMETER ( PI    = 3.14159265358979323844d0   )
           Real*8 deg2rad
           PARAMETER ( deg2rad = 2.d0*PI/360.d0           )
c$openad INDEPENDENT(x)
          y(1)=cos(deg2rad*x(1))
c$openad DEPENDENT(y)
	end subroutine
