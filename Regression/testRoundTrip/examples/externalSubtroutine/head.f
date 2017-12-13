        module d
           public :: d_reed
           interface d_reed
              module procedure reed
           end interface
           private :: reed
           contains
             subroutine reed(x)
               double precision x
               x=x*2.0
             end subroutine
        end module d

c$openad XXX Template ad_template.f
	subroutine head(x,y) 
          use d
	  double precision, dimension(2) :: x
	  double precision, dimension(1) :: y
c$openad INDEPENDENT(x)
          call reed(x(1))
          y(1)=x(1)*x(2)
c$openad DEPENDENT(y)
	end subroutine
