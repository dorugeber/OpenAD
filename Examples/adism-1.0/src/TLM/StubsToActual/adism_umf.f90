subroutine adism_umf(totpts,matcount,errstat,matrow,matcol,matval,b,x)
  use OAD_active
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
  integer                          ::   errstat
  integer, dimension(totpts*5)            ::   matrow
  integer, dimension(totpts*5)            ::   matcol
  type(active),dimension(totpts*5)        ::   matval
  type(active),dimension(totpts)          ::   b
  type(active),dimension(totpts)          ::   x

  ! locals
  integer                          ::   i,j
  real(dp),dimension(20)           ::   RINFO
  integer,dimension(40)            ::   INFO
  
  integer                          :: lvalue,lindex
  integer,dimension(2000000)        :: index
  real(dp),dimension(2000000)       :: value
  integer                          :: ii
  
  integer,dimension(20)            ::   ICNTL
  integer,dimension(20)            ::   KEEP
  real(dp),dimension(10)           ::   CNTL
  integer,parameter                ::   JOB=1
  real(dp),dimension(2*totpts)     ::   w
  real(dp),dimension(totpts)       :: sol 
  real(dp),dimension(totpts)       :: dotRHS
 
  lindex = 2000000
  lvalue = 2000000
  info = 0
  keep = 0
  cntl = 0
  keep = 0
  icntl = 0
  rinfo = 0
  sol=0.0_dp
  
  call UMD21I (KEEP,CNTL,ICNTL)

  do ii=1,matcount
     value(ii)=matval(ii)%v
     index(ii)=matrow(ii)
     index(ii+matcount)=matcol(ii)
  end do
    
  call UMD2FA(totpts, & 
       matcount, & 
       1, & 
       .false., & 
       lvalue, & 
       lindex, & 
       value, & 
       index, & 
       KEEP, & 
       CNTL, & 
       ICNTL, & 
       INFO, & 
       RINFO )
  if (INFO (1) .lt. 0) stop ! if info(1) is negative there is an error
  call UMD2SO (totpts, & 
       0, & 
       .false., & 
       lvalue, & 
       lindex, & 
       value, & 
       index, & 
       KEEP, & 
       b%v, & 
       sol, & 
       w, & 
       CNTL, & 
       ICNTL, & 
       INFO, & 
       RINFO)
  if (INFO (1).lt.0) stop ! info(1) is the error
  ! copy back the solution
  do ii=1,totpts
     x(ii)%v=sol(ii)
     dotRHS(ii)=0.0_dp
  end do
  ! build dot(A)*x
  do ii=1,matcount
     dotRHS(matrow(ii))=dotRHS(matrow(ii))+matval(ii)%d*sol(matcol(ii))
  end do
  ! new right hand side: 
  do ii=1,totpts
     sol(ii)=0.0_dp
     dotRHS(ii)=b(ii)%d-dotRHS(ii)
  end do
  ! solve with the same factors
  call UMD2SO (totpts, & 
       0, & 
       .false., & 
       lvalue, & 
       lindex, & 
       value, & 
       index, & 
       KEEP, & 
       dotRHS, & 
       sol, & 
       w, & 
       CNTL, & 
       ICNTL, & 
       INFO, & 
       RINFO)
  if (INFO (1).lt.0) stop ! info(1) is the error
  ! copy back the solution
  do ii=1,totpts
     x(ii)%d=sol(ii)
  end do
  end subroutine 
