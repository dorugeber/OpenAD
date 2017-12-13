	subroutine foo(a) 
	  double precision :: a
	   a=2*a
	end subroutine 

	subroutine head(x,y)
	  double precision, dimension(2), intent(in) :: x
	  double precision, dimension(2), intent(out) :: y
          integer :: i,k

c$openad INDEPENDENT(x)

c$openad xxx simple loop
          do i=1,2
	    k=i
            call foo(x(k))
	    k=-19
          end do
          y=x 
c$openad DEPENDENT(y)
	end subroutine
