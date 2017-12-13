      module all_globals_mod

      end module

c$openad XXX Template ad_template.f
	subroutine foo(x,y,k) 
	  double precision x,y
	  integer k
	  y = x*k
	  k = k+2
	end subroutine

c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision, dimension(2) :: x
	  double precision y
	  integer k
c$openad INDEPENDENT(x)
	  k=1
	  call foo(x(k),y,k)
c$openad DEPENDENT(y)
	end subroutine

