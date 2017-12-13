	subroutine bar(x,y)
	  double precision, dimension(4) :: x,y
	  integer :: i
	  do i=1,4
	     if (x(i)>1.2) then 
		y(i)=2.0*y(i)
             end if
	  end do
	end subroutine

	subroutine foo(x,y)
	  double precision, dimension(:) :: x,y
	  call bar(x,y)
	end subroutine

	subroutine head(x,y) 
	interface
	subroutine foo(x,y)
	  double precision, dimension(:) :: x,y
	end subroutine
	end interface
	  double precision, dimension(4) :: x,y
c$openad INDEPENDENT(x)
	  y=x
	  call foo(x,y)  
c$openad DEPENDENT(y)
	end subroutine

