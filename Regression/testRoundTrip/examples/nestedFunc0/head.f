c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision, dimension(2) :: x
	  double precision, dimension(1) :: y
c$openad INDEPENDENT(x)
          call foo()
c$openad DEPENDENT(y)
	  contains
	    subroutine foo()
	      y(1)=x(1)*x(2)
            end subroutine
	end subroutine
