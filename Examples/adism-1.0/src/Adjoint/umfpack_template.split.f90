subroutine template()
  use OAD_tape
  use OAD_rev
  use OAD_cp
  use w2f__types

  !$TEMPLATE_PRAGMA_DECLARATIONS
  integer                          ::   i,j
  real(w2f__8),dimension(20)           ::   RINFO
  integer,dimension(40)            ::   INFO

  integer                          :: lvalue,lindex
  integer,dimension(2000000)         :: index
  real(w2f__8),dimension(2000000)        :: value
  integer                          :: ii

  integer,dimension(20)            ::   ICNTL
  integer,dimension(20)            ::   KEEP
  real(w2f__8),dimension(10)           ::   CNTL
  integer,parameter                ::   JOB=1
  character(len=50)                ::   msg
  real(w2f__8),dimension(2*totpts)     ::   w

  real(w2f__8),dimension(totpts)  ::   sol,forwardSol
  type(modeType) :: our_orig_mode

  if (our_rev_mode%plain) then
     print *, "UMF plain"
     lindex = 2000000
     lvalue = 2000000
     info = 0
     keep = 0
     cntl = 0
     keep = 0
     icntl = 0
     rinfo = 0

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
     do ii=1,totpts
        x(ii)%v=sol(ii)
     end do
  end if
  if (our_rev_mode%tape) then
     print *, "UMF tape"
     lindex = 2000000
     lvalue = 2000000
     call UMD21I (KEEP, CNTL, ICNTL)
     do ii=1,matcount
        value(ii)=matval(ii)%v
        index(ii)=matrow(ii)
        index(ii+matcount)=matcol(ii)
     end do
! tape it
     CALL oad_tape_push(value(1:matcount))
     CALL oad_tape_push(index(1:matcount*2))
     CALL oad_tape_push(matcount)
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
     if (INFO (1) .lt. 0) stop
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
     if (INFO (1).lt.0) stop
     do ii=1,totpts
        x(ii)%v=sol(ii)
     end do
     CALL oad_tape_push(sol)
  end if
  if (our_rev_mode%adjoint) then
     print *, "UMF adjoint"
     lindex = 2000000
     lvalue = 2000000
     ! restore it
! restore in reverse order
     CALL oad_tape_pop(forwardSol)
     do ii=1,totpts
        if (forwardSol(ii).ne.forwardSol(ii)) print *, "NaN in forwardSol"
        if (x(ii)%d.ne.x(ii)%d) then 
           print *, "NaN in x%d", ii
           stop
        end if
     end do
     CALL oad_tape_pop(matcount)
     CALL oad_tape_pop(index(1:matcount*2))
     CALL oad_tape_pop(value(1:matcount))
     do ii=1,matcount
        if (value(ii).ne.value(ii)) print *, "NaN in value"
        matrow(ii)=index(ii)
        matcol(ii)=index(ii+matcount)
     end do
     call UMD21I (KEEP, CNTL, ICNTL)
     call UMD2FA(totpts, &
                  matcount, &          
                  1, &                 
                  .true., & ! transposed
                  lvalue, & 
                  lindex, & 
                  value, &  
                  index, &  
                  KEEP, &   
                  CNTL, &   
                  ICNTL, &  
                  INFO, &   
                  RINFO ) 
     if (INFO (1) .lt. 0) stop
     call UMD2SO (totpts, &
                  0, &
                  .false., & 
                  lvalue, &  
                  lindex, &  
                  value, &   
                  index, &   
                  KEEP, &    
                  x%d, &       
                  sol, &       
                  w, &       
                  CNTL, &    
                  ICNTL, &   
                  INFO, &    
                  RINFO)   
     if (INFO (1).lt.0) stop
!    update RHS adjoint
     do ii=1,totpts
        if (b(ii)%d.ne.b(ii)%d) print *, "NaN in b%d"
        if (sol(ii).ne.sol(ii)) print *, "NaN in sol"
        b(ii)%d=b(ii)%d+sol(ii)
        x(ii)%d=0.0D0
     end do
!    update the matrix adjoint
     do ii=1,matcount
        if (matval(ii)%d.ne.matval(ii)%d) print *, "NaN in matval%d"
        matval(ii)%d=matval(ii)%d-sol(matrow(ii))*forwardSol(matcol(ii))
     end do
  end if
end subroutine template
