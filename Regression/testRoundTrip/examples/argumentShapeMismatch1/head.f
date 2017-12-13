	module m1                                                                   
        contains 
	subroutine foo(x,n)
	double precision:: x(n)
	integer :: n
	end subroutine  

	end module 

	module m2
	use m1  
	contains 
	subroutine bar(x,n)
        integer :: n
	double precision :: x(n)     
	integer :: i   
        do i=1,n  
          x(i)=x(i)*2  
        end do   
	i=2         
	call foo(x(i),size(x)-i)
	end subroutine          
	end module       

	subroutine head(x,y)
          use m2 
	  double precision, dimension(4) :: x, y
c$openad INDEPENDENT(x)
	  call bar(x,4)
	  y=x
c$openad DEPENDENT(y)
	end subroutine

