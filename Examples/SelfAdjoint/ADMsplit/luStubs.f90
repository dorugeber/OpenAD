! stub for module LU

module luStubs

  public :: solve

  interface solve
     module procedure solveImplStub
  end interface

  interface test
     module procedure testImplStub
  end interface

contains 

  subroutine solveImplStub(A,x,b)
    !$openad xxx template oad_template_solve.f90
    implicit none 
    double precision, intent(in) :: A(:,:), b(:)
    double precision, intent(out) :: x(:)
    double precision, dimension(size(A,1),size(A,2)) ::  L,U
    double precision, dimension(size(A,1)) ::  d
    double precision coeff
    ! just establish a simple dependency here
    ! so the analysis gets the idea
    x(1)=b(1)/A(1,1)
  end subroutine

  subroutine testImplStub(A,x,b)
    !$openad xxx template oad_template_test.f90
    implicit none 
    double precision, intent(in) :: A(:,:), b(:), x(:)
  end subroutine

end module
