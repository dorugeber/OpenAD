subroutine head(b,x) 
  integer, parameter :: n=2
  double precision , dimension(n) :: b,x
  double precision , dimension(n,n) :: A 
  data A/2.0D0,3.0D0,4.0D0,2.0D0/
!$openad INDEPENDENT(b)
  call solve(n,A,x,b,.false.)
  call test(n,A,x,b)
!$openad DEPENDENT(x)  
end subroutine
