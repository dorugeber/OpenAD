	module aGlobalModule
	  public :: globalString
	  character(3) globalString
	end module

c$openad XXX Template ad_template.f
	subroutine foo(x,y)
	  use aGlobalModule
	  double precision, dimension(2), intent(in) :: x
	  double precision, dimension(2), intent(inout) :: y
	  character(3) localString
          integer i
c$openad xxx simple loop
          do i=1,2
	    if (globalString .eq. 'yes') then 
              y(i)=y(i)+x(i)*x(i)
            end if
	    if (globalString .eq. 'no') then 
              y(i)=y(i)+x(i)*x(i)*2
            end if
          end do
	  globalString='either'
	  localString=globalString
	end subroutine

c$openad XXX Template ad_template.f
	subroutine head(x,y)
	  use aGlobalModule
	  double precision, dimension(2), intent(in) :: x
	  double precision, dimension(2), intent(out) :: y
c$openad INDEPENDENT(x)
	  y(1)=1.0
	  y(2)=1.0
	  globalString='yes'
          call foo(x,y)
	  globalString='both'
c$openad DEPENDENT(y)
	end subroutine
