c$openad XXX Template ad_template.f
	subroutine foo(x,y,a) 
	  double precision, intent(inout) :: x
	  double precision, intent(inout) :: y
	  character (len=3) :: a
c$openad INDEPENDENT(x)
	  if (scan(a,'a')==3) then 
	     y=x
          else 
             y=2*x
          end if
c$openad DEPENDENT(y)
	end subroutine



c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision, dimension(1), intent(inout) :: x
	  double precision, dimension(1), intent(inout) :: y
c$openad INDEPENDENT(x)
	  call foo(x(1),y(1),'bla')
c$openad DEPENDENT(y)
	end subroutine
