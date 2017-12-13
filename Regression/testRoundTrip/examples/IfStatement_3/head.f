c$openad XXX Template ad_template.f
	subroutine foo(x,l)
	  double precision:: x
	  logical :: l 
	  l=(x.ne.0.0D0)
	end subroutine

c$openad XXX Template ad_template.f
	subroutine head(x,y)
	  double precision, dimension(1) :: x
	  double precision, dimension(1) :: y
          logical :: l
c$openad INDEPENDENT(x)
          call foo(x(1),l)
          if (l) then
            y(1)=x(1)
          end if
c$openad DEPENDENT(y)
	end subroutine
