c$openad XXX Template ad_template.f
	subroutine foo(a,x) 
	  character (*) a
	  double precision x
	  if (a=='two') then 
	  x=x*2
	  endif 
	  if (a=='three') then 
	  x=x*3
	  end if 
	end subroutine

c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision, dimension(1) :: x
	  double precision, dimension(1) :: y
	  character (10) a
c$openad INDEPENDENT(x)
	  a='two'
	  call foo(a,x)
	  a='three'
	  call foo(a,x)
	  a='four'
	  y(1)=x(1)
c$openad DEPENDENT(y)
	end subroutine
