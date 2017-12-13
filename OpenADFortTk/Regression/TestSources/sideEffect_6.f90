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

	subroutine head(x,y) 
	  double precision, dimension(1) :: x
	  double precision, dimension(1) :: y
	  character (10) a
	  a='two'
	  call foo(a,x)
	  a='three'
	  call foo(a,x)
	  a='four'
	  y(1)=x(1)
	end subroutine

	program sideEffect_6
	 double precision, dimension(1) :: x
         double precision, dimension(1) :: y
	 x(1)=2.4
	 print *,y
	end program

