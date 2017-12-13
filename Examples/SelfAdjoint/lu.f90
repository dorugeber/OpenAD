! no pivoting braindead LU 
! written just to illustrate 
! the solve call handling

module lu

  public :: solve

  interface solve
     module procedure solveImpl
  end interface

  interface test
     module procedure testImpl
  end interface

contains 

  subroutine solveImpl(A,x,b)
    implicit none 
    double precision, intent(in) :: A(:,:), b(:)
    double precision, intent(out) :: x(:)
    double precision, dimension(size(A,1),size(A,2)) ::  L,U
    double precision, dimension(size(A,1)) ::  d
    double precision coeff
    integer i, j, k, n
    n=size(A,1)
    L=0.0
    U=A
    x=0.0
    do k=1, n-1
       do i=k+1,n
          coeff=U(i,k)/U(k,k)
          L(i,k) = coeff
          do j=k+1,n
             U(i,j) = U(i,j)-coeff*U(k,j)
          end do
       end do
    end do
    do i=1,n
       L(i,i) = 1.0
    end do
    do k=1,n
       d(1) = b(1)
       do i=2,n
          d(i)=b(i)
          do j=1,i-1
             d(i) = d(i) - L(i,j)*d(j)
          end do
       end do
       x(n)=d(n)/U(n,n)
       do i = n-1,1,-1
          x(i) = d(i)
          do j=n,i+1,-1
             x(i)=x(i)-U(i,j)*x(j)
          end do
          x(i) = x(i)/U(i,i)
       end do
    end do
  end subroutine

  subroutine testImpl(A,x,b)
    implicit none 
    double precision, intent(in) :: A(:,:), x(:), b(:)
    print *, '  test: max of |Ax-b|: ',maxval(abs(matmul(A,x)-b))
  end subroutine

end module
