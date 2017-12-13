subroutine solve(n,A,x,b,trans)
!$openad xxx template Templates/solve.f90
  implicit none
  integer :: n
  double precision :: A(n,n), x(n), b(n)
  logical :: trans
  ! just establish a simple dependency here
  ! so the analysis gets the idea
  ! note that A remains passive 
  x(1)=b(1)/A(1,1)
end subroutine solve
