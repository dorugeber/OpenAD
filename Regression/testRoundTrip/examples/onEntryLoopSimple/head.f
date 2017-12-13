	subroutine foo(k,a) 
	  integer :: k,i
	  double precision :: a(k)
	  do i=1,k
	     a(i)=2*a(i)
	  end do
	end subroutine 

	subroutine head(x,y)
	  double precision, dimension(2), intent(in) :: x
	  double precision, dimension(2), intent(out) :: y
          integer :: i,k

c$openad INDEPENDENT(x)

c$openad xxx simple loop
          do i=1,2
	    k=2
            call foo(k,x)
	    k=-19
          end do
          y=x 
c$openad DEPENDENT(y)
	end subroutine
