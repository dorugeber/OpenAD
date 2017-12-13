      module m 
        interface foo
           module procedure fooI
        end interface
        contains 
	subroutine fooI(x) 
c$openad XXX Template ad_template.f
	  double precision ::  x(:)
	end subroutine
        end module 

	subroutine head(x,y) 
        use m
c$openad XXX Template ad_template.f
	  double precision, dimension(:) :: x,y
c$openad INDEPENDENT(x)
	  y=x
          call foo(y)
c$openad DEPENDENT(y)
	end subroutine

