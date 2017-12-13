subroutine foo(x)
  double precision :: x
  x=x*2.0
end subroutine 

program activeInterface
  double precision, dimension(2) :: x,y
  interface 
    subroutine foo(x)
      double precision :: x
    end subroutine
  end interface
!$openad INDEPENDENT(x)
!$openad DEPENDENT(y)
  x(1)=2.0
  x(2)=3.0
  call foo(x(1))
  y=x
  if (y(1).eq.4.0) then 
    print *, 'OK' 
  end if
end program
