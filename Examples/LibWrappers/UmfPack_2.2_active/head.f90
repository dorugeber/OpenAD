subroutine head(b,x) 
  integer, parameter :: n=2
  double precision , dimension(n) :: b,x
  double precision , dimension(n,n) :: A 
  A(1,1)=2.0D0+b(1)
  A(1,2)=4.0D0
  A(2,2)=2.0D0+b(2)
  A(2,1)=3.0D0
!$openad INDEPENDENT(b)
  call solve(n,A,x,b,.false.)
  call test(n,A,x,b)
!$openad DEPENDENT(x)  
end subroutine
