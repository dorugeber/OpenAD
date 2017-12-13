subroutine test(n,A,x,b)
  implicit none 
  integer :: n
  double precision :: A(n,n), x(n), b(n)
  print *, '  test: max(|Ax-b|) = ',maxval(abs(matmul(A,x)-b))
end subroutine test
