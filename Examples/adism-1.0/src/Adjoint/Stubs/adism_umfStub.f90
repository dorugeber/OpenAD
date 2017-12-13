subroutine adism_umf(totpts,matcount,matrow,matcol,matval,b,x)
  !$openad XXX Template umfpack_template.split.f90
  implicit none

  ! variables/parameters for the UMFPACK routines
  ! UMD21I  Initialise user-controlled variables
  ! UMD2FA: Primary analysis+factorization routine
  ! UMD2RF: Numerical factorization routine
  ! UMD2SO: Routine for solving a linear system
  integer,parameter      ::     dp = kind(1.d0)
  integer                          ::   n,m       ! number of grids, coordinates
  integer, intent(in)              ::   totpts    ! order of matrix,n
  integer, intent(in)              ::   matcount
  integer, dimension(totpts*5)            ::   matrow
  integer, dimension(totpts*5)            ::   matcol
  real(dp),dimension(totpts*5)            ::   matval
  real(dp),dimension(totpts)            ::   b
  real(dp),dimension(totpts)            ::   x

  ! Dependency
  matval(1) = b(1)
  x(1) = matval(1)

end subroutine adism_umf
