c$openad XXX Template ad_template.f
      subroutine head(x,y) 
        implicit none
        double precision, dimension(2) :: x
        double precision, dimension(1) :: y
c$openad INDEPENDENT(x)
          call foo()
          call bar()
c$openad DEPENDENT(y)
        contains
          subroutine foo()
            implicit none
            y(1)=x(1)*x(2)
          end subroutine
          subroutine bar()
            implicit none
	    y(1)=y(1)*sin(x(1))*cos(x(2))
          end subroutine
      end subroutine
