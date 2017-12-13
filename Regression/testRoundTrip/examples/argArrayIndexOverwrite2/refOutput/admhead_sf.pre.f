      module all_globals_mod

      end module

c$openad XXX Template ad_template.f
	subroutine foo(a,k) 
	  double precision a
	  integer k
	  k = k*2
	  a = a*k
	end subroutine

c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision, dimension(2) :: x
	  double precision y
	  integer k
c$openad INDEPENDENT(x)
	  k=1
	  call foo(x(k),k)
	  k=1
	  call foo(x(k),k)
	  y = x(1)*x(2)
c$openad DEPENDENT(y)
	end subroutine

