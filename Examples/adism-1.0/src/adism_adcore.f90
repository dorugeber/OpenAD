
SUBROUTINE adism_umf(TOTPTS, MATCOUNT, MATROW, MATCOL, MATVAL, B, X)
  use OAD_tape
  use OAD_rev
  use OAD_cp
  use w2f__types

use OAD_active
use w2f__types
IMPLICIT NONE
!
!     **** Parameters and Result ****
!
INTEGER(w2f__i4) TOTPTS
INTEGER(w2f__i4) MATCOUNT
INTEGER(w2f__i4) MATROW(1 : INT((TOTPTS * 5)))
INTEGER(w2f__i4) MATCOL(1 : INT((TOTPTS * 5)))
type(active) :: MATVAL(1:INT((TOTPTS*5)))
type(active) :: B(1:TOTPTS)
type(active) :: X(1:TOTPTS)
!
!     **** Local Variables and Functions ****
!
INTEGER(w2f__i4) DP
PARAMETER ( DP = 8)
!
!     **** Statements ****
!

  integer :: i,j
  real(w2f__8),dimension(20) :: RINFO
  integer,dimension(40) :: INFO

  integer :: lvalue,lindex
  integer,dimension(2000000) :: index
  real(w2f__8),dimension(2000000) :: value
  integer :: ii

  integer,dimension(20) :: ICNTL
  integer,dimension(20) :: KEEP
  real(w2f__8),dimension(10) :: CNTL
  integer,parameter :: JOB=1
  character(len=50) :: msg
  real(w2f__8),dimension(2*totpts) :: w

  real(w2f__8),dimension(totpts) :: sol,forwardSol
  type(modeType) :: our_orig_mode
!$OPENAD XXX Template umfpack_template.split.f90
!$OPENAD XXX Template umfpack_template.split.f90

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

     call UMD2FA(totpts, matcount, 1, .false., lvalue, lindex, value, index, KEE&
     &P, CNTL, ICNTL, INFO, RINFO )


     if (INFO (1) .lt. 0) stop
! if info(1) is negative there is an error

     call UMD2SO (totpts, 0, .false., lvalue, lindex, value, index, KEEP, b%v, s&
     &ol, w, CNTL, ICNTL, INFO, RINFO)


     if (INFO (1).lt.0) stop
! info(1) is the error
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
     call UMD2FA(totpts, matcount, 1, .false., lvalue, lindex, value, index, KEE&
     &P, CNTL, ICNTL, INFO, RINFO )

     if (INFO (1) .lt. 0) stop
     call UMD2SO (totpts, 0, .false., lvalue, lindex, value, index, KEEP, b%v, s&
     &ol, w, CNTL, ICNTL, INFO, RINFO)

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
     call UMD2FA(totpts, matcount, 1, .true., lvalue, lindex, value, index, KEEP&
     &, CNTL, ICNTL, INFO, RINFO )

! transposed
     if (INFO (1) .lt. 0) stop
     call UMD2SO (totpts, 0, .false., lvalue, lindex, value, index, KEEP, x%d, s&
     &ol, w, CNTL, ICNTL, INFO, RINFO)

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
end subroutine adism_umf


!$OPENAD XXX File_start [OAD_intrinsics.f90]
MODULE oad_intrinsics
use OAD_active
use w2f__types
IMPLICIT NONE
SAVE
!
!     **** Top Level Pragmas ****
!
interface OAD_S_MAX
  module procedure OAD_S_MAX_D
end interface
interface OAD_S_MAXVAL
  module procedure OAD_S_MAXVAL_D_1
end interface
interface OAD_S_MIN
  module procedure OAD_S_MIN_D_11
  module procedure OAD_S_MIN_D_02
  module procedure OAD_S_MIN_D
end interface

!
!     **** Statements ****
!
CONTAINS
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_MAXVAL_D_1(A, R)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) A(1 :)
  REAL(w2f__8) R
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) I(1 : 1)


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  I(1 : 1) = MAXLOC(A)
  R = A(I(1))
    end if
    if (our_rev_mode%tape) then
! taping
  I(1 : 1) = MAXLOC(A)
  R = A(I(1))
    end if
    if (our_rev_mode%adjoint) then
! adjoint
    end if
  end subroutine OAD_S_MAXVAL_D_1
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_MAX_D(A0, A1, R)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_0
  INTEGER(w2f__i8) OpenAD_Symbol_1
  INTEGER(w2f__i8) OpenAD_Symbol_2
  INTEGER(w2f__i8) OpenAD_Symbol_3
  INTEGER(w2f__i8) OpenAD_Symbol_4
  INTEGER(w2f__i8) OpenAD_Symbol_5
!
!       **** Parameters and Result ****
!
  type(active) :: A0
  type(active) :: A1
  type(active) :: R


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  IF (A0%v.GT.A1%v) THEN
    R%v = A0%v
  ELSE
    R%v = A1%v
  ENDIF
    end if
    if (our_rev_mode%tape) then
! taping
  IF (A0%v.GT.A1%v) THEN
    R%v = A0%v
    OpenAD_Symbol_1 = 1_w2f__i8
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_1
    oad_it_ptr = oad_it_ptr+1
  ELSE
    R%v = A1%v
    OpenAD_Symbol_2 = 0_w2f__i8
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_2
    oad_it_ptr = oad_it_ptr+1
  ENDIF
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_0 = oad_it(oad_it_ptr)
  IF (OpenAD_Symbol_0.ne.0) THEN
    A0%d = A0%d+R%d
    R%d = 0.0d0
  ELSE
    A1%d = A1%d+R%d
    R%d = 0.0d0
  ENDIF
    end if
  end subroutine OAD_S_MAX_D
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_MIN_D(A0, A1, R)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_10
  INTEGER(w2f__i8) OpenAD_Symbol_11
  INTEGER(w2f__i8) OpenAD_Symbol_6
  INTEGER(w2f__i8) OpenAD_Symbol_7
  INTEGER(w2f__i8) OpenAD_Symbol_8
  INTEGER(w2f__i8) OpenAD_Symbol_9
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) A0
  type(active) :: A1
  type(active) :: R


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  IF (A0.LT.A1%v) THEN
    R%v = A0
  ELSE
    R%v = A1%v
  ENDIF
    end if
    if (our_rev_mode%tape) then
! taping
  IF (A0.LT.A1%v) THEN
    R%v = A0
    OpenAD_Symbol_7 = 1_w2f__i8
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_7
    oad_it_ptr = oad_it_ptr+1
  ELSE
    R%v = A1%v
    OpenAD_Symbol_8 = 0_w2f__i8
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_8
    oad_it_ptr = oad_it_ptr+1
  ENDIF
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_6 = oad_it(oad_it_ptr)
  IF (OpenAD_Symbol_6.ne.0) THEN
    R%d = 0.0d0
  ELSE
    A1%d = A1%d+R%d
    R%d = 0.0d0
  ENDIF
    end if
  end subroutine OAD_S_MIN_D
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_MIN_D_02(A0, A1, R)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_12
  INTEGER(w2f__i8) OpenAD_Symbol_13
  INTEGER(w2f__i8) OpenAD_Symbol_14
  INTEGER(w2f__i8) OpenAD_Symbol_15
  INTEGER(w2f__i8) OpenAD_Symbol_16
  INTEGER(w2f__i8) OpenAD_Symbol_17
  INTEGER(w2f__i8) OpenAD_Symbol_18
  INTEGER(w2f__i8) OpenAD_Symbol_19
  INTEGER(w2f__i8) OpenAD_Symbol_20
  INTEGER(w2f__i8) OpenAD_Symbol_21
  INTEGER(w2f__i8) OpenAD_Symbol_22
  INTEGER(w2f__i8) OpenAD_Symbol_23
  INTEGER(w2f__i8) OpenAD_Symbol_24
  INTEGER(w2f__i8) OpenAD_Symbol_25
  INTEGER(w2f__i8) OpenAD_Symbol_26
  INTEGER(w2f__i8) OpenAD_Symbol_27
  INTEGER(w2f__i8) OpenAD_Symbol_28
  INTEGER(w2f__i8) OpenAD_Symbol_29
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) A0
  REAL(w2f__8) A1(1 :, 1 :)
  REAL(w2f__8) R(1 :, 1 :)
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) I1
  INTEGER(w2f__i4) I2
  INTEGER(w2f__i4) L1
  INTEGER(w2f__i4) L2
  INTEGER(w2f__i4) U1
  INTEGER(w2f__i4) U2


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  L1 = LBOUND(A1, 1)
  U1 = UBOUND(A1, 1)
  L2 = LBOUND(A1, 2)
  U2 = UBOUND(A1, 2)
  DO I1 = L1, U1, 1
    DO I2 = L2, U2, 1
      IF(A1(I1, I2) .GT. A0) THEN
        R(INT(I1), INT(I2)) = A0
      ELSE
        R(INT(I1), INT(I2)) = A1(I1, I2)
      ENDIF
    END DO
  END DO
    end if
    if (our_rev_mode%tape) then
! taping
  L1 = LBOUND(A1, 1)
  U1 = UBOUND(A1, 1)
  L2 = LBOUND(A1, 2)
  U2 = UBOUND(A1, 2)
  OpenAD_Symbol_17 = 0_w2f__i8
  DO I1 = L1, U1, 1
    OpenAD_Symbol_18 = 0_w2f__i8
    DO I2 = L2, U2, 1
      IF(A1(I1, I2) .GT. A0) THEN
        R(INT(I1), INT(I2)) = A0
        OpenAD_Symbol_19 = 1_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_19
        oad_it_ptr = oad_it_ptr+1
      ELSE
        R(INT(I1), INT(I2)) = A1(I1, I2)
        OpenAD_Symbol_20 = 0_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_20
        oad_it_ptr = oad_it_ptr+1
      ENDIF
      OpenAD_Symbol_18 = (INT(OpenAD_Symbol_18) + INT(1_w2f__i8))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_18
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_17 = (INT(OpenAD_Symbol_17) + INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_17
  oad_it_ptr = oad_it_ptr+1
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_12 = oad_it(oad_it_ptr)
  OpenAD_Symbol_13 = 1
  DO WHILE(INT(OpenAD_Symbol_13) .LE. INT(OpenAD_Symbol_12))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_14 = oad_it(oad_it_ptr)
    OpenAD_Symbol_15 = 1
    DO WHILE(INT(OpenAD_Symbol_15) .LE. INT(OpenAD_Symbol_14))
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_16 = oad_it(oad_it_ptr)
      IF(OpenAD_Symbol_16 .ne. 0) THEN
      ENDIF
      OpenAD_Symbol_15 = INT(OpenAD_Symbol_15) + 1
    END DO
    OpenAD_Symbol_13 = INT(OpenAD_Symbol_13) + 1
  END DO
    end if
  end subroutine OAD_S_MIN_D_02
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_MIN_D_11(A0, A1, R)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_30
  INTEGER(w2f__i8) OpenAD_Symbol_31
  INTEGER(w2f__i8) OpenAD_Symbol_32
  INTEGER(w2f__i8) OpenAD_Symbol_33
  INTEGER(w2f__i8) OpenAD_Symbol_34
  INTEGER(w2f__i8) OpenAD_Symbol_35
  INTEGER(w2f__i8) OpenAD_Symbol_36
  INTEGER(w2f__i8) OpenAD_Symbol_37
  INTEGER(w2f__i8) OpenAD_Symbol_38
  INTEGER(w2f__i8) OpenAD_Symbol_39
  INTEGER(w2f__i8) OpenAD_Symbol_40
  INTEGER(w2f__i8) OpenAD_Symbol_41
!
!       **** Parameters and Result ****
!
  type(active) :: A0(1:)
  type(active) :: A1(1:)
  type(active) :: R(1:)
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) I1
  INTEGER(w2f__i4) L1
  INTEGER(w2f__i4) U1
  INTEGER(w2f__i4) OpenAD_Symbol_1363
  INTEGER(w2f__i4) OpenAD_Symbol_1364


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  L1 = LBOUND(A1, 1)
  U1 = UBOUND(A1, 1)
  DO I1 = L1, U1, 1
    IF (A0(I1)%v.LT.A1(I1)%v) THEN
      R(INT(I1))%v = A0(I1)%v
    ELSE
      R(INT(I1))%v = A1(I1)%v
    ENDIF
  END DO
    end if
    if (our_rev_mode%tape) then
! taping
  L1 = LBOUND(A1,1)
  U1 = UBOUND(A1,1)
  OpenAD_Symbol_33 = 0_w2f__i8
  DO I1 = L1,U1,1
    IF (A0(I1)%v.LT.A1(I1)%v) THEN
      R(INT(I1))%v = A0(I1)%v
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = I1
      oad_it_ptr = oad_it_ptr+1
      OpenAD_Symbol_34 = 1_w2f__i8
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = OpenAD_Symbol_34
      oad_it_ptr = oad_it_ptr+1
    ELSE
      R(INT(I1))%v = A1(I1)%v
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = I1
      oad_it_ptr = oad_it_ptr+1
      OpenAD_Symbol_35 = 0_w2f__i8
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = OpenAD_Symbol_35
      oad_it_ptr = oad_it_ptr+1
    ENDIF
    OpenAD_Symbol_33 = (INT(OpenAD_Symbol_33)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_33
  oad_it_ptr = oad_it_ptr+1
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_30 = oad_it(oad_it_ptr)
  OpenAD_Symbol_31 = 1
  do while (INT(OpenAD_Symbol_31).LE.INT(OpenAD_Symbol_30))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_32 = oad_it(oad_it_ptr)
    IF (OpenAD_Symbol_32.ne.0) THEN
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1363 = oad_it(oad_it_ptr)
      A0(OpenAD_Symbol_1363)%d = A0(OpenAD_Symbol_1363)%d+R(OpenAD_Symbol_1363)%&
     &d

      R(OpenAD_Symbol_1363)%d = 0.0d0
    ELSE
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1364 = oad_it(oad_it_ptr)
      A1(OpenAD_Symbol_1364)%d = A1(OpenAD_Symbol_1364)%d+R(OpenAD_Symbol_1364)%&
     &d

      R(OpenAD_Symbol_1364)%d = 0.0d0
    ENDIF
    OpenAD_Symbol_31 = INT(OpenAD_Symbol_31)+1
  END DO
    end if
  end subroutine OAD_S_MIN_D_11
END

!$OPENAD XXX File_start [glimmer_global.F90]
MODULE glimmer_global
use OAD_active
use w2f__types
IMPLICIT NONE
SAVE
!
!     **** Global Variables & Derived Type Definitions ****
!
REAL(w2f__4) DUMMYARRAY(1 : 2)
PRIVATE DUMMYARRAY
!
!     **** Local Variables and Functions ****
!
CHARACTER(1) CHAR_RET
PARAMETER ( CHAR_RET = '\r')
CHARACTER(2) CR_LF
PARAMETER ( CR_LF = '\r\n')
CHARACTER(1) DIRSEP
PARAMETER ( DIRSEP = '/')
INTEGER(w2f__i4) DP
PARAMETER ( DP = 8)
CHARACTER(1) ENDLINE
PARAMETER ( ENDLINE = '\n')
INTEGER(w2f__i4) FNAME_LENGTH
PARAMETER ( FNAME_LENGTH = 200)
CHARACTER(1) LINEFEED
PARAMETER ( LINEFEED = '\n')
INTEGER(w2f__i4) MSG_LENGTH
PARAMETER ( MSG_LENGTH = 500)
INTEGER(w2f__i4) RK
PARAMETER ( RK = 8)
INTEGER(w2f__i4) SIZEK
PARAMETER ( SIZEK = 4)
INTEGER(w2f__i4) SP
PARAMETER ( SP = 4)
!
!     **** Statements ****
!
END MODULE

!$OPENAD XXX File_start [glimmer_physcon.F90]
MODULE physcon
use OAD_active
use w2f__types
use glimmer_global ,only: DP , SP
IMPLICIT NONE
SAVE
!
!     **** Local Variables and Functions ****
!
REAL(w2f__8) ACTENH
PARAMETER ( ACTENH = 1.39D+05)
REAL(w2f__8) ACTENL
PARAMETER ( ACTENL = 6.0D+04)
REAL(w2f__8) ARRMLH
PARAMETER ( ARRMLH = 1.733D+03)
REAL(w2f__8) ARRMLL
PARAMETER ( ARRMLL = 3.612999999999999852D-13)
REAL(w2f__8) CONI
PARAMETER ( CONI = 2.10000000000000008882D00)
REAL(w2f__8) F
PARAMETER ( F = -1.12967032967032965374D00)
REAL(w2f__8) GASCON
PARAMETER ( GASCON = 8.31400000000000005684D00)
INTEGER(w2f__i4) GN
PARAMETER ( GN = 3)
REAL(w2f__8) GRAV
PARAMETER ( GRAV = 9.81000000000000049738D00)
REAL(w2f__8) LHCI
PARAMETER ( LHCI = 3.35D+05)
REAL(w2f__8) PI
PARAMETER ( PI = 3.14159265358969985726D00)
REAL(w2f__8) PMLT
PARAMETER ( PMLT = 9.7455999999999995988D-08)
REAL(w2f__8) RHOI
PARAMETER ( RHOI = 9.1D+02)
REAL(w2f__8) RHOM
PARAMETER ( RHOM = 3.3D+03)
REAL(w2f__8) RHOO
PARAMETER ( RHOO = 1.028D+03)
REAL(w2f__8) RHOW
PARAMETER ( RHOW = 1.0D+03)
REAL(w2f__8) SCYR
PARAMETER ( SCYR = 3.1556926D+07)
REAL(w2f__8) SHCI
PARAMETER ( SHCI = 2.009D+03)
REAL(w2f__8) TRPT
PARAMETER ( TRPT = 2.73149999999999977263D+02)
!
!     **** Statements ****
!
END MODULE

!$OPENAD XXX File_start [glimmer_utils.F90]
MODULE glimmer_utils
use OAD_active
use w2f__types
use oad_intrinsics
use glimmer_global
IMPLICIT NONE
SAVE
!
!     **** Top Level Pragmas ****
!
interface ARRAY_BCS
  module procedure ARRAY_BCS2D
  module procedure ARRAY_BCS1D
end interface
interface OAD_S_ARRAY_BCS
  module procedure OAD_S_ARRAY_BCS2D
  module procedure OAD_S_ARRAY_BCS1D
end interface

!
!     **** Statements ****
!
CONTAINS

  Function ARRAY_BCS1D(ARRAY, I)
  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) ARRAY(1 :)
  INTENT(IN) ARRAY
  INTEGER(w2f__i4) I
  INTENT(IN) I
  REAL(w2f__8) ARRAY_BCS1D
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) II
  INTEGER(w2f__i4) N
!
!       **** Statements ****
!
  N = SIZE(ARRAY)
  II = I
  IF((I .GE. 1) .AND.(I .LE. N)) THEN
    ARRAY_BCS1D = ARRAY(I)
  ENDIF
  DO WHILE(II .GT. N)
    II = II - N
  END DO
  DO WHILE(II .LT. 1)
    II = II + N
  END DO
  ARRAY_BCS1D = ARRAY(II)
  RETURN
  END FUNCTION

  Function ARRAY_BCS_LATS(ARRAY, I)
  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) ARRAY(1 :)
  INTENT(IN) ARRAY
  INTEGER(w2f__i4) I
  INTENT(IN) I
  REAL(w2f__8) ARRAY_BCS_LATS
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) II
  INTEGER(w2f__i4) N
!
!       **** Statements ****
!
  N = SIZE(ARRAY)
  II = I
  IF((I .GE. 1) .AND.(I .LE. N)) THEN
    ARRAY_BCS_LATS = ARRAY(I)
    RETURN
  ENDIF
  IF(II .GT. N) THEN
    II = N * 2 - II
    ARRAY_BCS_LATS = ARRAY(II)+(-1.8D+02)
  ENDIF
  IF(II .LT. 1) THEN
    II = 1 - II
    ARRAY_BCS_LATS = 1.8D+02-ARRAY(II)
  ENDIF
  RETURN
  END FUNCTION

  Function ARRAY_BCS2D(ARRAY, I, J)
  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) ARRAY(1 :, 1 :)
  INTENT(IN) ARRAY
  INTEGER(w2f__i4) I
  INTENT(IN) I
  INTEGER(w2f__i4) J
  INTENT(IN) J
  REAL(w2f__8) ARRAY_BCS2D
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) II
  INTEGER(w2f__i4) JJ
  INTEGER(w2f__i4) NX
  INTEGER(w2f__i4) NY
!
!       **** Statements ****
!
  NX = SIZE(ARRAY, 1)
  NY = SIZE(ARRAY, 2)
  IF((J .LE. NY) .AND.((J .GE. 1) .AND.((I .GE. 1) .AND.(I .LE. NX)))) THEN
    ARRAY_BCS2D = ARRAY(I,J)
    RETURN
  ENDIF
  II = I
  JJ = J
  IF(JJ .GT. NY) THEN
    JJ = NY * 2 - JJ
    II = II +(NX / 2)
  ENDIF
  IF(JJ .LT. 1) THEN
    JJ = 1 - JJ
    II = II +(NX / 2)
  ENDIF
  DO WHILE(II .GT. NX)
    II = II - NX
  END DO
  DO WHILE(II .LT. 1)
    II = II + NX
  END DO
  ARRAY_BCS2D = ARRAY(II,JJ)
  RETURN
  END FUNCTION
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE FIX_BCS2D(I, J, NX, NY)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_42
  INTEGER(w2f__i8) OpenAD_Symbol_43
  INTEGER(w2f__i8) OpenAD_Symbol_44
  INTEGER(w2f__i8) OpenAD_Symbol_45
  INTEGER(w2f__i8) OpenAD_Symbol_46
  INTEGER(w2f__i8) OpenAD_Symbol_47
  INTEGER(w2f__i8) OpenAD_Symbol_48
  INTEGER(w2f__i8) OpenAD_Symbol_49
  INTEGER(w2f__i8) OpenAD_Symbol_50
  INTEGER(w2f__i8) OpenAD_Symbol_51
  INTEGER(w2f__i8) OpenAD_Symbol_52
  INTEGER(w2f__i8) OpenAD_Symbol_53
  INTEGER(w2f__i8) OpenAD_Symbol_54
  INTEGER(w2f__i8) OpenAD_Symbol_56
  INTEGER(w2f__i8) OpenAD_Symbol_57
  INTEGER(w2f__i8) OpenAD_Symbol_58
  INTEGER(w2f__i8) OpenAD_Symbol_59
  INTEGER(w2f__i8) OpenAD_Symbol_60
  INTEGER(w2f__i8) OpenAD_Symbol_61
  INTEGER(w2f__i8) OpenAD_Symbol_62
  INTEGER(w2f__i8) OpenAD_Symbol_63
  INTEGER(w2f__i8) OpenAD_Symbol_64
  INTEGER(w2f__i8) OpenAD_Symbol_65
  INTEGER(w2f__i8) OpenAD_Symbol_66
  INTEGER(w2f__i8) OpenAD_Symbol_67
  INTEGER(w2f__i8) OpenAD_Symbol_68
!
!       **** Parameters and Result ****
!
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  INTEGER(w2f__i4) NX
  INTEGER(w2f__i4) NY
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_55
  INTEGER(w2f__i8) OpenAD_Symbol_69


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
 2      CONTINUE
  GO TO 3
 3      CONTINUE
  GO TO 4
 4      CONTINUE
  IF((J .LE. NY) .AND.((J .GE. 1) .AND.((I .GE. 1) .AND.(I .LE. NX)))) THEN
    GO TO 24
  ELSE
    GO TO 5
  ENDIF
 5      CONTINUE
  GO TO 6
 6      CONTINUE
  GO TO 7
 7      CONTINUE
  IF(J .GT. NY) THEN
    GO TO 9
  ELSE
    GO TO 8
  ENDIF
 8      CONTINUE
  GO TO 10
 9      CONTINUE
  J = (NY * 2 - J)
  I = (I +(NX / 2))
  GO TO 10
 10     CONTINUE
  GO TO 11
 11     CONTINUE
  IF(J .LT. 1) THEN
    GO TO 13
  ELSE
    GO TO 12
  ENDIF
 12     CONTINUE
  GO TO 14
 13     CONTINUE
  J = (1 - J)
  I = (I +(NX / 2))
  GO TO 14
 14     CONTINUE
  GO TO 15
 15     CONTINUE
  GO TO 16
 16     CONTINUE
  IF(I .GT. NX) THEN
    GO TO 22
  ELSE
    GO TO 17
  ENDIF
 17     CONTINUE
  GO TO 18
 18     CONTINUE
  IF(I .LT. 1) THEN
    GO TO 20
  ELSE
    GO TO 19
  ENDIF
 19     CONTINUE
  GO TO 25
 20     CONTINUE
  I = (I + NX)
  GO TO 21
 21     CONTINUE
  GO TO 18
 22     CONTINUE
  I = (I - NX)
  GO TO 23
 23     CONTINUE
  GO TO 16
 24     CONTINUE
  GO TO 1
  GO TO 25
 25     CONTINUE
  GO TO 1
 1      CONTINUE
    end if
    if (our_rev_mode%tape) then
! taping
 27     CONTINUE
  GO TO 28
 28     CONTINUE
  OpenAD_Symbol_42 = 0_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_42
  oad_it_ptr = oad_it_ptr+1
  GO TO 29
 29     CONTINUE
  GO TO 30
 30     CONTINUE
  OpenAD_Symbol_43 = 1_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_43
  oad_it_ptr = oad_it_ptr+1
  GO TO 31
 31     CONTINUE
  IF((J .LE. NY) .AND.((J .GE. 1) .AND.((I .GE. 1) .AND.(I .LE. NX)))) THEN
    GO TO 61
  ELSE
    GO TO 32
  ENDIF
 32     CONTINUE
  GO TO 33
 33     CONTINUE
  OpenAD_Symbol_45 = 3_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_45
  oad_it_ptr = oad_it_ptr+1
  GO TO 34
 34     CONTINUE
  GO TO 35
 35     CONTINUE
  IF(J .GT. NY) THEN
    GO TO 38
  ELSE
    GO TO 36
  ENDIF
 36     CONTINUE
  GO TO 37
 37     CONTINUE
  OpenAD_Symbol_54 = 12_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_54
  oad_it_ptr = oad_it_ptr+1
  GO TO 40
 38     CONTINUE
  J = (NY * 2 - J)
  I = (I +(NX / 2))
  GO TO 39
 39     CONTINUE
  OpenAD_Symbol_46 = 4_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_46
  oad_it_ptr = oad_it_ptr+1
  GO TO 40
 40     CONTINUE
  GO TO 41
 41     CONTINUE
  IF(J .LT. 1) THEN
    GO TO 44
  ELSE
    GO TO 42
  ENDIF
 42     CONTINUE
  GO TO 43
 43     CONTINUE
  OpenAD_Symbol_53 = 11_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_53
  oad_it_ptr = oad_it_ptr+1
  GO TO 46
 44     CONTINUE
  J = (1 - J)
  I = (I +(NX / 2))
  GO TO 45
 45     CONTINUE
  OpenAD_Symbol_47 = 5_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_47
  oad_it_ptr = oad_it_ptr+1
  GO TO 46
 46     CONTINUE
  GO TO 47
 47     CONTINUE
  GO TO 48
 48     CONTINUE
  OpenAD_Symbol_48 = 6_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_48
  oad_it_ptr = oad_it_ptr+1
  GO TO 49
 49     CONTINUE
  IF(I .GT. NX) THEN
    GO TO 58
  ELSE
    GO TO 50
  ENDIF
 50     CONTINUE
  GO TO 51
 51     CONTINUE
  OpenAD_Symbol_50 = 8_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_50
  oad_it_ptr = oad_it_ptr+1
  GO TO 52
 52     CONTINUE
  IF(I .LT. 1) THEN
    GO TO 55
  ELSE
    GO TO 53
  ENDIF
 53     CONTINUE
  GO TO 54
 54     CONTINUE
  OpenAD_Symbol_52 = 10_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_52
  oad_it_ptr = oad_it_ptr+1
  GO TO 63
 55     CONTINUE
  I = (I + NX)
  GO TO 56
 56     CONTINUE
  OpenAD_Symbol_51 = 9_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_51
  oad_it_ptr = oad_it_ptr+1
  GO TO 57
 57     CONTINUE
  GO TO 52
 58     CONTINUE
  I = (I - NX)
  GO TO 59
 59     CONTINUE
  OpenAD_Symbol_49 = 7_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_49
  oad_it_ptr = oad_it_ptr+1
  GO TO 60
 60     CONTINUE
  GO TO 49
 61     CONTINUE
  GO TO 26
  GO TO 62
 62     CONTINUE
  OpenAD_Symbol_44 = 2_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_44
  oad_it_ptr = oad_it_ptr+1
  GO TO 63
 63     CONTINUE
  GO TO 26
 26     CONTINUE
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_55 = oad_it(oad_it_ptr)
  DO WHILE(INT(OpenAD_Symbol_55) .ne. INT(0_w2f__i8))
    IF ( OpenAD_Symbol_55 .EQ. 1) GO TO 92
    IF ( OpenAD_Symbol_55 .EQ. 2) GO TO 93
    IF ( OpenAD_Symbol_55 .EQ. 3) GO TO 94
    IF ( OpenAD_Symbol_55 .EQ. 4) GO TO 95
    IF ( OpenAD_Symbol_55 .EQ. 5) GO TO 96
    IF ( OpenAD_Symbol_55 .EQ. 6) GO TO 97
    IF ( OpenAD_Symbol_55 .EQ. 7) GO TO 98
    IF ( OpenAD_Symbol_55 .EQ. 8) GO TO 99
    IF ( OpenAD_Symbol_55 .EQ. 9) GO TO 100
    IF ( OpenAD_Symbol_55 .EQ. 10) GO TO 101
    IF ( OpenAD_Symbol_55 .EQ. 11) GO TO 102
    IF ( OpenAD_Symbol_55 .EQ. 12) GO TO 103
 92       CONTINUE
    GO TO 89
 93       CONTINUE
    GO TO 89
 94       CONTINUE
    GO TO 89
 95       CONTINUE
    GO TO 89
 96       CONTINUE
    GO TO 89
 97       CONTINUE
    GO TO 89
 98       CONTINUE
    GO TO 89
 99       CONTINUE
    GO TO 89
 100      CONTINUE
    GO TO 89
 101      CONTINUE
    GO TO 89
 102      CONTINUE
    GO TO 89
 103      CONTINUE
    GO TO 89
 89       CONTINUE
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_55 = oad_it(oad_it_ptr)
  END DO
    end if
  end subroutine FIX_BCS2D
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE HSUM(INP, OUTPUT, NX, NY, NZ)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_70
  INTEGER(w2f__i8) OpenAD_Symbol_71
  INTEGER(w2f__i8) OpenAD_Symbol_72
  INTEGER(w2f__i8) OpenAD_Symbol_73
  INTEGER(w2f__i8) OpenAD_Symbol_74
  INTEGER(w2f__i8) OpenAD_Symbol_75
  INTEGER(w2f__i8) OpenAD_Symbol_76
  INTEGER(w2f__i8) OpenAD_Symbol_77
  INTEGER(w2f__i8) OpenAD_Symbol_78
  INTEGER(w2f__i8) OpenAD_Symbol_79
  INTEGER(w2f__i8) OpenAD_Symbol_80
  INTEGER(w2f__i8) OpenAD_Symbol_81
  INTEGER(w2f__i8) OpenAD_Symbol_82
  INTEGER(w2f__i8) OpenAD_Symbol_83
  INTEGER(w2f__i8) OpenAD_Symbol_84
  INTEGER(w2f__i8) OpenAD_Symbol_85
  INTEGER(w2f__i8) OpenAD_Symbol_86
  INTEGER(w2f__i8) OpenAD_Symbol_87
!
!       **** Parameters and Result ****
!
  INTEGER(w2f__i4) NX
  INTEGER(w2f__i4) NY
  INTEGER(w2f__i4) NZ
  REAL(w2f__8) INP(1 : NZ, 1 : NX, 1 : NY)
  REAL(w2f__8) OUTPUT(1 : NZ)
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  REAL(w2f__8) TEMPVAR
  INTEGER(w2f__i4) UP


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  DO UP = 1, SIZE(INP, 1), 1
    TEMPVAR = 0.0D00
    DO I = 1, NX, 1
      DO J = 1, NY, 1
        TEMPVAR = (INP(UP, I, J) + TEMPVAR)
      END DO
    END DO
    OUTPUT(INT(UP)) = TEMPVAR
  END DO
    end if
    if (our_rev_mode%tape) then
! taping
  OpenAD_Symbol_76 = 0_w2f__i8
  DO UP = 1, SIZE(INP, 1), 1
    TEMPVAR = 0.0D00
    OpenAD_Symbol_77 = 0_w2f__i8
    DO I = 1, NX, 1
      OpenAD_Symbol_78 = 0_w2f__i8
      DO J = 1, NY, 1
        TEMPVAR = (INP(UP, I, J) + TEMPVAR)
        OpenAD_Symbol_78 = (INT(OpenAD_Symbol_78) + INT(1_w2f__i8 ))
      END DO
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = OpenAD_Symbol_78
      oad_it_ptr = oad_it_ptr+1
      OpenAD_Symbol_77 = (INT(OpenAD_Symbol_77) + INT(1_w2f__i8))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_77
    oad_it_ptr = oad_it_ptr+1
    OUTPUT(INT(UP)) = TEMPVAR
    OpenAD_Symbol_76 = (INT(OpenAD_Symbol_76) + INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_76
  oad_it_ptr = oad_it_ptr+1
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_70 = oad_it(oad_it_ptr)
  OpenAD_Symbol_71 = 1
  DO WHILE(INT(OpenAD_Symbol_71) .LE. INT(OpenAD_Symbol_70))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_72 = oad_it(oad_it_ptr)
    OpenAD_Symbol_73 = 1
    DO WHILE(INT(OpenAD_Symbol_73) .LE. INT(OpenAD_Symbol_72))
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_74 = oad_it(oad_it_ptr)
      OpenAD_Symbol_75 = 1
      DO WHILE(INT(OpenAD_Symbol_75) .LE. INT(OpenAD_Symbol_74))
        OpenAD_Symbol_75 = INT(OpenAD_Symbol_75) + 1
      END DO
      OpenAD_Symbol_73 = INT(OpenAD_Symbol_73) + 1
    END DO
    OpenAD_Symbol_71 = INT(OpenAD_Symbol_71) + 1
  END DO
    end if
  end subroutine HSUM
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE HSUM4(INP, OUTPUT, NX, NY, NZ)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  INTEGER(w2f__i4) NX
  INTEGER(w2f__i4) NY
  INTEGER(w2f__i4) NZ
  type(active) :: INP(1:NZ,1:NX,1:NY)
  type(active) :: OUTPUT(1:NZ)
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) OpenAD_Symbol_1365
!
!       **** Statements ****
!


    integer iaddr
    external iaddr

   if (our_rev_mode%plain) then
! original function
  OUTPUT(1:INT(NZ))%v = (INP(1:NZ,2,2)%v+INP(1:NZ,1,2)%v+INP(1:NZ,1,1)%v+INP(1:N&
     &Z,2,1)%v)

    end if
    if (our_rev_mode%tape) then
! taping
  OUTPUT(1:INT(NZ))%v = (INP(1:NZ,2,2)%v+INP(1:NZ,1,2)%v+INP(1:NZ,1,1)%v+INP(1:N&
     &Z,2,1)%v)

  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = NZ
  oad_it_ptr = oad_it_ptr+1
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1365 = oad_it(oad_it_ptr)
  INP(1:OpenAD_Symbol_1365,2,1)%d = INP(1:OpenAD_Symbol_1365,2,1)%d+OUTPUT(1:Ope&
     &nAD_Symbol_1365)%d

  INP(1:OpenAD_Symbol_1365,1,1)%d = INP(1:OpenAD_Symbol_1365,1,1)%d+OUTPUT(1:Ope&
     &nAD_Symbol_1365)%d

  INP(1:OpenAD_Symbol_1365,1,2)%d = INP(1:OpenAD_Symbol_1365,1,2)%d+OUTPUT(1:Ope&
     &nAD_Symbol_1365)%d

  INP(1:OpenAD_Symbol_1365,2,2)%d = INP(1:OpenAD_Symbol_1365,2,2)%d+OUTPUT(1:Ope&
     &nAD_Symbol_1365)%d

  OUTPUT(1:OpenAD_Symbol_1365)%d = 0.0d0
    end if
  end subroutine HSUM4
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE STAGVARB(IPVR, OPVR, EWN, NSN)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  use glimmer_global ,only: DP
  use glimmer_global ,only: DP
  use glimmer_global ,only: DP
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  type(active) :: IPVR(1:,1:)
  type(active) :: OPVR(1:,1:)
  INTEGER(w2f__i4) EWN
  INTEGER(w2f__i4) NSN
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) OpenAD_Symbol_1366
  INTEGER(w2f__i4) OpenAD_Symbol_1367
  INTEGER(w2f__i4) OpenAD_Symbol_1368
  INTEGER(w2f__i4) OpenAD_Symbol_1369
  INTEGER(w2f__i4) OpenAD_Symbol_1370
  INTEGER(w2f__i4) OpenAD_Symbol_1371
  INTEGER(w2f__i4) OpenAD_Symbol_1372
  INTEGER(w2f__i4) OpenAD_Symbol_1373
  INTEGER(w2f__i4) OpenAD_Symbol_88
  INTEGER(w2f__i4) OpenAD_Symbol_89
  INTEGER(w2f__i4) OpenAD_Symbol_90
  INTEGER(w2f__i4) OpenAD_Symbol_91
  INTEGER(w2f__i4) OpenAD_Symbol_92
  INTEGER(w2f__i4) OpenAD_Symbol_93
  REAL(w2f__8) OpenAD_aux_0(:, :)
  ALLOCATABLE OpenAD_aux_0
  type(active) :: OpenAD_prp_0(:,:)
  ALLOCATABLE OpenAD_prp_0
!
!       **** Statements ****
!


    integer iaddr
    external iaddr

   if (our_rev_mode%plain) then
! original function
  OPVR(1:EWN+(-1),1:NSN+(-1))%v = ((IPVR(1:EWN+(-1),1:NSN+(-1))%v+IPVR(2:EWN,2:N&
     &SN)%v+IPVR(2:EWN,1:NSN+(-1))%v+IPVR(1:EWN+(-1),2:NSN)%v)*2.5D-01)

    end if
    if (our_rev_mode%tape) then
! taping
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_aux_0,IPVR(1:EWN+(-1),2:NSN))
  OpenAD_aux_0 = (IPVR(1:EWN+(-1),1:NSN+(-1))%v+IPVR(2:EWN,2:NSN)%v+IPVR(2:EWN,1&
     &:NSN+(-1))%v+IPVR(1:EWN+(-1),2:NSN)%v)

  OPVR(1:EWN+(-1),1:NSN+(-1))%v = (OpenAD_aux_0*2.5D-01)
  OpenAD_Symbol_88 = (EWN+(-1))
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_88
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_89 = (NSN+(-1))
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_89
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_90 = (NSN+(-1))
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_90
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_91 = (EWN+(-1))
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_91
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_92 = (EWN+(-1))
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_92
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_93 = (NSN+(-1))
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_93
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = EWN
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = NSN
  oad_it_ptr = oad_it_ptr+1
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1366 = oad_it(oad_it_ptr)
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1367 = oad_it(oad_it_ptr)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_prp_0,IPVR(1:EWN+(-1),1:NSN+(-1)))
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1368 = oad_it(oad_it_ptr)
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1369 = oad_it(oad_it_ptr)
  OpenAD_prp_0%d = OpenAD_prp_0%d+OPVR(1:OpenAD_Symbol_1369,1:OpenAD_Symbol_1368&
     &)%d*(2.5D-01)

  OPVR(1:OpenAD_Symbol_1369,1:OpenAD_Symbol_1368)%d = 0.0d0
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1370 = oad_it(oad_it_ptr)
  IPVR(1:OpenAD_Symbol_1370,2:OpenAD_Symbol_1366)%d = IPVR(1:OpenAD_Symbol_1370,&
     &2:OpenAD_Symbol_1366)%d+OpenAD_prp_0%d

  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1371 = oad_it(oad_it_ptr)
  IPVR(2:OpenAD_Symbol_1367,1:OpenAD_Symbol_1371)%d = IPVR(2:OpenAD_Symbol_1367,&
     &1:OpenAD_Symbol_1371)%d+OpenAD_prp_0%d

  IPVR(2:OpenAD_Symbol_1367,2:OpenAD_Symbol_1366)%d = IPVR(2:OpenAD_Symbol_1367,&
     &2:OpenAD_Symbol_1366)%d+OpenAD_prp_0%d

  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1372 = oad_it(oad_it_ptr)
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1373 = oad_it(oad_it_ptr)
  IPVR(1:OpenAD_Symbol_1373,1:OpenAD_Symbol_1372)%d = IPVR(1:OpenAD_Symbol_1373,&
     &1:OpenAD_Symbol_1372)%d+OpenAD_prp_0%d

  OpenAD_prp_0%d = 0.0d0
    end if
  end subroutine STAGVARB
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE TRIDIAG(A, B, C, X, Y, N)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_100
  INTEGER(w2f__i8) OpenAD_Symbol_101
  INTEGER(w2f__i8) OpenAD_Symbol_102
  INTEGER(w2f__i8) OpenAD_Symbol_103
  INTEGER(w2f__i8) OpenAD_Symbol_104
  INTEGER(w2f__i8) OpenAD_Symbol_105
  INTEGER(w2f__i8) OpenAD_Symbol_94
  INTEGER(w2f__i8) OpenAD_Symbol_95
  INTEGER(w2f__i8) OpenAD_Symbol_96
  INTEGER(w2f__i8) OpenAD_Symbol_97
  INTEGER(w2f__i8) OpenAD_Symbol_98
  INTEGER(w2f__i8) OpenAD_Symbol_99
!
!       **** Parameters and Result ****
!
  INTEGER(w2f__i4) N
  type(active) :: A(1:N)
  type(active) :: B(1:N)
  type(active) :: C(1:N)
  type(active) :: X(1:N)
  type(active) :: Y(1:N)
!
!       **** Local Variables and Functions ****
!
  type(active) :: AA(1:N)
  type(active) :: BB(1:N)
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) OpenAD_Symbol_106
  INTEGER(w2f__i4) OpenAD_Symbol_107
  INTEGER(w2f__i4) OpenAD_Symbol_108
  INTEGER(w2f__i4) OpenAD_Symbol_109
  REAL(w2f__8) OpenAD_Symbol_1374
  REAL(w2f__8) OpenAD_Symbol_1375
  REAL(w2f__8) OpenAD_Symbol_1376
  REAL(w2f__8) OpenAD_Symbol_1377
  INTEGER(w2f__i4) OpenAD_Symbol_1378
  REAL(w2f__8) OpenAD_Symbol_1379
  REAL(w2f__8) OpenAD_Symbol_1380
  REAL(w2f__8) OpenAD_Symbol_1381
  REAL(w2f__8) OpenAD_Symbol_1382
  REAL(w2f__8) OpenAD_Symbol_1383
  REAL(w2f__8) OpenAD_Symbol_1384
  INTEGER(w2f__i4) OpenAD_Symbol_1385
  INTEGER(w2f__i4) OpenAD_Symbol_1386
  INTEGER(w2f__i4) OpenAD_Symbol_1387
  REAL(w2f__8) OpenAD_Symbol_1388
  REAL(w2f__8) OpenAD_Symbol_1389
  REAL(w2f__8) OpenAD_Symbol_1390
  REAL(w2f__8) OpenAD_Symbol_1391
  INTEGER(w2f__i4) OpenAD_Symbol_1392
  INTEGER(w2f__i4) OpenAD_Symbol_1393
  INTEGER(w2f__i4) OpenAD_Symbol_1394
  REAL(w2f__8) OpenAD_Symbol_1395
  REAL(w2f__8) OpenAD_Symbol_1396
  INTEGER(w2f__i4) OpenAD_Symbol_1397
  REAL(w2f__8) OpenAD_acc_0
  REAL(w2f__8) OpenAD_acc_1
  REAL(w2f__8) OpenAD_acc_2
  REAL(w2f__8) OpenAD_acc_3
  REAL(w2f__8) OpenAD_acc_4
  REAL(w2f__8) OpenAD_acc_5
  REAL(w2f__8) OpenAD_acc_6
  REAL(w2f__8) OpenAD_acc_7
  REAL(w2f__8) OpenAD_aux_1
  REAL(w2f__8) OpenAD_aux_2
  REAL(w2f__8) OpenAD_aux_3
  REAL(w2f__8) OpenAD_lin_0
  REAL(w2f__8) OpenAD_lin_1
  REAL(w2f__8) OpenAD_lin_10
  REAL(w2f__8) OpenAD_lin_11
  REAL(w2f__8) OpenAD_lin_12
  REAL(w2f__8) OpenAD_lin_13
  REAL(w2f__8) OpenAD_lin_14
  REAL(w2f__8) OpenAD_lin_15
  REAL(w2f__8) OpenAD_lin_2
  REAL(w2f__8) OpenAD_lin_3
  REAL(w2f__8) OpenAD_lin_4
  REAL(w2f__8) OpenAD_lin_5
  REAL(w2f__8) OpenAD_lin_6
  REAL(w2f__8) OpenAD_lin_7
  REAL(w2f__8) OpenAD_lin_8
  REAL(w2f__8) OpenAD_lin_9
  type(active) :: OpenAD_prp_1
  type(active) :: OpenAD_prp_2
  type(active) :: OpenAD_prp_3
  type(active) :: OpenAD_prp_4
  type(active) :: OpenAD_prp_5
  type(active) :: OpenAD_prp_6
!
!       **** Statements ****
!


    integer iaddr
    external iaddr

   if (our_rev_mode%plain) then
! original function
  AA(1)%v = (C(1)%v/B(1)%v)
  BB(1)%v = (Y(1)%v/B(1)%v)
  DO I = 2,N,1
    AA(INT(I))%v = (C(I)%v/(B(I)%v-A(I)%v*AA(I+(-1))%v))
    BB(INT(I))%v = ((Y(I)%v-A(I)%v*BB(I+(-1))%v)/(B(I)%v-A(I)%v*AA(I+(-1))%v))
  END DO
  X(INT(N))%v = BB(N)%v
  DO I = N+(-1),1,(-1)
    X(INT(I))%v = (BB(I)%v-X(I+1)%v*AA(I)%v)
  END DO
    end if
    if (our_rev_mode%tape) then
! taping
  OpenAD_lin_0 = (INT(1_w2f__i8)/B(1)%v)
  OpenAD_lin_1 = (-(C(1)%v/(B(1)%v*B(1)%v)))
  AA(1)%v = (C(1)%v/B(1)%v)
  OpenAD_lin_2 = (INT(1_w2f__i8)/B(1)%v)
  OpenAD_lin_3 = (-(Y(1)%v/(B(1)%v*B(1)%v)))
  BB(1)%v = (Y(1)%v/B(1)%v)
  if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
  oad_dt(oad_dt_ptr) = OpenAD_lin_0
  oad_dt_ptr = oad_dt_ptr+1
  if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
  oad_dt(oad_dt_ptr) = OpenAD_lin_1
  oad_dt_ptr = oad_dt_ptr+1
  if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
  oad_dt(oad_dt_ptr) = OpenAD_lin_2
  oad_dt_ptr = oad_dt_ptr+1
  if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
  oad_dt(oad_dt_ptr) = OpenAD_lin_3
  oad_dt_ptr = oad_dt_ptr+1
  OpenAD_Symbol_98 = 0_w2f__i8
  DO I = 2,N,1
    OpenAD_aux_1 = (B(I)%v-A(I)%v*AA(I+(-1))%v)
    OpenAD_lin_4 = (INT(1_w2f__i8)/OpenAD_aux_1)
    OpenAD_lin_6 = AA(I+(-1))%v
    OpenAD_lin_7 = A(I)%v
    OpenAD_lin_5 = (-(C(I)%v/(OpenAD_aux_1*OpenAD_aux_1)))
    AA(INT(I))%v = (C(I)%v/OpenAD_aux_1)
    OpenAD_acc_0 = (OpenAD_lin_6*INT((-1_w2f__i8)))
    OpenAD_acc_1 = (OpenAD_lin_7*INT((-1_w2f__i8)))
    OpenAD_Symbol_106 = (I+(-1))
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_106
    oad_it_ptr = oad_it_ptr+1
    if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
    oad_dt(oad_dt_ptr) = OpenAD_acc_0
    oad_dt_ptr = oad_dt_ptr+1
    if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
    oad_dt(oad_dt_ptr) = OpenAD_acc_1
    oad_dt_ptr = oad_dt_ptr+1
    if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
    oad_dt(oad_dt_ptr) = OpenAD_lin_4
    oad_dt_ptr = oad_dt_ptr+1
    if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
    oad_dt(oad_dt_ptr) = OpenAD_lin_5
    oad_dt_ptr = oad_dt_ptr+1
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = I
    oad_it_ptr = oad_it_ptr+1
    OpenAD_aux_2 = (Y(I)%v-A(I)%v*BB(I+(-1))%v)
    OpenAD_aux_3 = (B(I)%v-A(I)%v*AA(I+(-1))%v)
    OpenAD_lin_10 = BB(I+(-1))%v
    OpenAD_lin_11 = A(I)%v
    OpenAD_lin_8 = (INT(1_w2f__i8)/OpenAD_aux_3)
    OpenAD_lin_12 = AA(I+(-1))%v
    OpenAD_lin_13 = A(I)%v
    OpenAD_lin_9 = (-(OpenAD_aux_2/(OpenAD_aux_3*OpenAD_aux_3)))
    BB(INT(I))%v = (OpenAD_aux_2/OpenAD_aux_3)
    OpenAD_acc_2 = (OpenAD_lin_10*INT((-1_w2f__i8)))
    OpenAD_acc_3 = (OpenAD_lin_11*INT((-1_w2f__i8)))
    OpenAD_acc_4 = (OpenAD_lin_12*INT((-1_w2f__i8)))
    OpenAD_acc_5 = (OpenAD_lin_13*INT((-1_w2f__i8)))
    OpenAD_Symbol_107 = (I+(-1))
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_107
    oad_it_ptr = oad_it_ptr+1
    if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
    oad_dt(oad_dt_ptr) = OpenAD_acc_2
    oad_dt_ptr = oad_dt_ptr+1
    if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
    oad_dt(oad_dt_ptr) = OpenAD_acc_3
    oad_dt_ptr = oad_dt_ptr+1
    if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
    oad_dt(oad_dt_ptr) = OpenAD_acc_4
    oad_dt_ptr = oad_dt_ptr+1
    if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
    oad_dt(oad_dt_ptr) = OpenAD_acc_5
    oad_dt_ptr = oad_dt_ptr+1
    OpenAD_Symbol_108 = (I+(-1))
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_108
    oad_it_ptr = oad_it_ptr+1
    if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
    oad_dt(oad_dt_ptr) = OpenAD_lin_8
    oad_dt_ptr = oad_dt_ptr+1
    if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
    oad_dt(oad_dt_ptr) = OpenAD_lin_9
    oad_dt_ptr = oad_dt_ptr+1
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = I
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_98 = (INT(OpenAD_Symbol_98)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_98
  oad_it_ptr = oad_it_ptr+1
  X(INT(N))%v = BB(N)%v
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = N
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_99 = 0_w2f__i8
  DO I = N+(-1),1,(-1)
    OpenAD_lin_14 = AA(I)%v
    OpenAD_lin_15 = X(I+1)%v
    X(INT(I))%v = (BB(I)%v-X(I+1)%v*AA(I)%v)
    OpenAD_acc_6 = (OpenAD_lin_14*INT((-1_w2f__i8)))
    OpenAD_acc_7 = (OpenAD_lin_15*INT((-1_w2f__i8)))
    OpenAD_Symbol_109 = (I+1)
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_109
    oad_it_ptr = oad_it_ptr+1
    if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
    oad_dt(oad_dt_ptr) = OpenAD_acc_6
    oad_dt_ptr = oad_dt_ptr+1
    if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
    oad_dt(oad_dt_ptr) = OpenAD_acc_7
    oad_dt_ptr = oad_dt_ptr+1
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = I
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_99 = (INT(OpenAD_Symbol_99)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_99
  oad_it_ptr = oad_it_ptr+1
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_94 = oad_it(oad_it_ptr)
  OpenAD_Symbol_95 = 1
  do while (INT(OpenAD_Symbol_95).LE.INT(OpenAD_Symbol_94))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1394 = oad_it(oad_it_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1395 = oad_dt(oad_dt_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1396 = oad_dt(oad_dt_ptr)
    AA(OpenAD_Symbol_1394)%d = AA(OpenAD_Symbol_1394)%d+X(OpenAD_Symbol_1394)%d*&
     &(OpenAD_Symbol_1395)

    OpenAD_prp_6%d = OpenAD_prp_6%d+X(OpenAD_Symbol_1394)%d*(OpenAD_Symbol_1396)
    BB(OpenAD_Symbol_1394)%d = BB(OpenAD_Symbol_1394)%d+X(OpenAD_Symbol_1394)%d
    X(OpenAD_Symbol_1394)%d = 0.0d0
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1397 = oad_it(oad_it_ptr)
    X(OpenAD_Symbol_1397)%d = X(OpenAD_Symbol_1397)%d+OpenAD_prp_6%d
    OpenAD_prp_6%d = 0.0d0
    OpenAD_Symbol_95 = INT(OpenAD_Symbol_95)+1
  END DO
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1393 = oad_it(oad_it_ptr)
  BB(OpenAD_Symbol_1393)%d = BB(OpenAD_Symbol_1393)%d+X(OpenAD_Symbol_1393)%d
  X(OpenAD_Symbol_1393)%d = 0.0d0
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_96 = oad_it(oad_it_ptr)
  OpenAD_Symbol_97 = 1
  do while (INT(OpenAD_Symbol_97).LE.INT(OpenAD_Symbol_96))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1378 = oad_it(oad_it_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1379 = oad_dt(oad_dt_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1380 = oad_dt(oad_dt_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1381 = oad_dt(oad_dt_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1382 = oad_dt(oad_dt_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1383 = oad_dt(oad_dt_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1384 = oad_dt(oad_dt_ptr)
    OpenAD_prp_5%d = OpenAD_prp_5%d+BB(OpenAD_Symbol_1378)%d*(OpenAD_Symbol_1379&
     &)

    OpenAD_prp_4%d = OpenAD_prp_4%d+BB(OpenAD_Symbol_1378)%d*(OpenAD_Symbol_1380&
     &)

    BB(OpenAD_Symbol_1378)%d = 0.0d0
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1385 = oad_it(oad_it_ptr)
    AA(OpenAD_Symbol_1385)%d = AA(OpenAD_Symbol_1385)%d+OpenAD_prp_5%d*(OpenAD_S&
     &ymbol_1381)

    A(OpenAD_Symbol_1378)%d = A(OpenAD_Symbol_1378)%d+OpenAD_prp_5%d*(OpenAD_Sym&
     &bol_1382)

    B(OpenAD_Symbol_1378)%d = B(OpenAD_Symbol_1378)%d+OpenAD_prp_5%d
    OpenAD_prp_5%d = 0.0d0
    OpenAD_prp_3%d = OpenAD_prp_3%d+OpenAD_prp_4%d*(OpenAD_Symbol_1383)
    A(OpenAD_Symbol_1378)%d = A(OpenAD_Symbol_1378)%d+OpenAD_prp_4%d*(OpenAD_Sym&
     &bol_1384)

    Y(OpenAD_Symbol_1378)%d = Y(OpenAD_Symbol_1378)%d+OpenAD_prp_4%d
    OpenAD_prp_4%d = 0.0d0
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1386 = oad_it(oad_it_ptr)
    BB(OpenAD_Symbol_1386)%d = BB(OpenAD_Symbol_1386)%d+OpenAD_prp_3%d
    OpenAD_prp_3%d = 0.0d0
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1387 = oad_it(oad_it_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1388 = oad_dt(oad_dt_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1389 = oad_dt(oad_dt_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1390 = oad_dt(oad_dt_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1391 = oad_dt(oad_dt_ptr)
    OpenAD_prp_2%d = OpenAD_prp_2%d+AA(OpenAD_Symbol_1387)%d*(OpenAD_Symbol_1388&
     &)

    C(OpenAD_Symbol_1387)%d = C(OpenAD_Symbol_1387)%d+AA(OpenAD_Symbol_1387)%d*(&
     &OpenAD_Symbol_1389)

    AA(OpenAD_Symbol_1387)%d = 0.0d0
    OpenAD_prp_1%d = OpenAD_prp_1%d+OpenAD_prp_2%d*(OpenAD_Symbol_1390)
    A(OpenAD_Symbol_1387)%d = A(OpenAD_Symbol_1387)%d+OpenAD_prp_2%d*(OpenAD_Sym&
     &bol_1391)

    B(OpenAD_Symbol_1387)%d = B(OpenAD_Symbol_1387)%d+OpenAD_prp_2%d
    OpenAD_prp_2%d = 0.0d0
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1392 = oad_it(oad_it_ptr)
    AA(OpenAD_Symbol_1392)%d = AA(OpenAD_Symbol_1392)%d+OpenAD_prp_1%d
    OpenAD_prp_1%d = 0.0d0
    OpenAD_Symbol_97 = INT(OpenAD_Symbol_97)+1
  END DO
  oad_dt_ptr = oad_dt_ptr-1
  OpenAD_Symbol_1374 = oad_dt(oad_dt_ptr)
  oad_dt_ptr = oad_dt_ptr-1
  OpenAD_Symbol_1375 = oad_dt(oad_dt_ptr)
  oad_dt_ptr = oad_dt_ptr-1
  OpenAD_Symbol_1376 = oad_dt(oad_dt_ptr)
  oad_dt_ptr = oad_dt_ptr-1
  OpenAD_Symbol_1377 = oad_dt(oad_dt_ptr)
  B(1)%d = B(1)%d+BB(1)%d*(OpenAD_Symbol_1374)
  Y(1)%d = Y(1)%d+BB(1)%d*(OpenAD_Symbol_1375)
  BB(1)%d = 0.0d0
  B(1)%d = B(1)%d+AA(1)%d*(OpenAD_Symbol_1376)
  C(1)%d = C(1)%d+AA(1)%d*(OpenAD_Symbol_1377)
  AA(1)%d = 0.0d0
    end if
  end subroutine TRIDIAG

  Function VERTINTG(DUPS, IN)
  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) DUPS(1 :)
  INTENT(IN) DUPS
  REAL(w2f__8) IN(1 : INT(SIZE(DUPS)))
  INTENT(IN) IN
  REAL(w2f__8) VERTINTG
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) UP
  INTEGER(w2f__i4) UPN
!
!       **** Statements ****
!
  UPN = SIZE(IN)
  VERTINTG = 0.0D00
  DO UP = UPN +(-1), 1, (-1)
    VERTINTG = VERTINTG+DUPS(UP)*(IN(UP)+IN(UP+1))
  END DO
  VERTINTG = VERTINTG * 5.0D-01
  RETURN
  END FUNCTION
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE MYSUM(INPUT, OUTPUT, NX, NY)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_110
  INTEGER(w2f__i8) OpenAD_Symbol_111
  INTEGER(w2f__i8) OpenAD_Symbol_112
  INTEGER(w2f__i8) OpenAD_Symbol_113
  INTEGER(w2f__i8) OpenAD_Symbol_114
  INTEGER(w2f__i8) OpenAD_Symbol_115
  INTEGER(w2f__i8) OpenAD_Symbol_116
  INTEGER(w2f__i8) OpenAD_Symbol_117
  INTEGER(w2f__i8) OpenAD_Symbol_118
  INTEGER(w2f__i8) OpenAD_Symbol_119
  INTEGER(w2f__i8) OpenAD_Symbol_120
  INTEGER(w2f__i8) OpenAD_Symbol_121
!
!       **** Parameters and Result ****
!
  type(active) :: OUTPUT
  INTEGER(w2f__i4) NX
  INTEGER(w2f__i4) NY
  type(active) :: INPUT(1:NX,1:NY)
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  INTEGER(w2f__i4) OpenAD_Symbol_1398
  INTEGER(w2f__i4) OpenAD_Symbol_1399
  type(active) :: OpenAD_prp_7
!
!       **** Statements ****
!


    integer iaddr
    external iaddr

   if (our_rev_mode%plain) then
! original function
  OUTPUT%v = 0.0D00
  DO I = 1,NX,1
    DO J = 1,NY,1
      OUTPUT%v = (INPUT(I,J)%v+OUTPUT%v)
    END DO
  END DO
    end if
    if (our_rev_mode%tape) then
! taping
  OUTPUT%v = 0.0D00
  OpenAD_Symbol_114 = 0_w2f__i8
  DO I = 1,NX,1
    OpenAD_Symbol_115 = 0_w2f__i8
    DO J = 1,NY,1
      OUTPUT%v = (INPUT(I,J)%v+OUTPUT%v)
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = I
      oad_it_ptr = oad_it_ptr+1
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = J
      oad_it_ptr = oad_it_ptr+1
      OpenAD_Symbol_115 = (INT(OpenAD_Symbol_115)+INT(1_w2f__i8))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_115
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_114 = (INT(OpenAD_Symbol_114)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_114
  oad_it_ptr = oad_it_ptr+1
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_110 = oad_it(oad_it_ptr)
  OpenAD_Symbol_111 = 1
  do while (INT(OpenAD_Symbol_111).LE.INT(OpenAD_Symbol_110))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_112 = oad_it(oad_it_ptr)
    OpenAD_Symbol_113 = 1
    do while (INT(OpenAD_Symbol_113).LE.INT(OpenAD_Symbol_112))
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1398 = oad_it(oad_it_ptr)
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1399 = oad_it(oad_it_ptr)
      OpenAD_prp_7%d = OpenAD_prp_7%d+OUTPUT%d
      INPUT(OpenAD_Symbol_1399,OpenAD_Symbol_1398)%d = INPUT(OpenAD_Symbol_1399,&
     &OpenAD_Symbol_1398)%d+OUTPUT%d

      OUTPUT%d = 0.0d0
      OUTPUT%d = OUTPUT%d+OpenAD_prp_7%d
      OpenAD_prp_7%d = 0.0d0
      OpenAD_Symbol_113 = INT(OpenAD_Symbol_113)+1
    END DO
    OpenAD_Symbol_111 = INT(OpenAD_Symbol_111)+1
  END DO
  OUTPUT%d = 0.0d0
    end if
  end subroutine MYSUM
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE MYSUM3D(INPUT, OUTPUT, NX, NY, NZ)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_122
  INTEGER(w2f__i8) OpenAD_Symbol_123
  INTEGER(w2f__i8) OpenAD_Symbol_124
  INTEGER(w2f__i8) OpenAD_Symbol_125
  INTEGER(w2f__i8) OpenAD_Symbol_126
  INTEGER(w2f__i8) OpenAD_Symbol_127
  INTEGER(w2f__i8) OpenAD_Symbol_128
  INTEGER(w2f__i8) OpenAD_Symbol_129
  INTEGER(w2f__i8) OpenAD_Symbol_130
  INTEGER(w2f__i8) OpenAD_Symbol_131
  INTEGER(w2f__i8) OpenAD_Symbol_132
  INTEGER(w2f__i8) OpenAD_Symbol_133
  INTEGER(w2f__i8) OpenAD_Symbol_134
  INTEGER(w2f__i8) OpenAD_Symbol_135
  INTEGER(w2f__i8) OpenAD_Symbol_136
  INTEGER(w2f__i8) OpenAD_Symbol_137
  INTEGER(w2f__i8) OpenAD_Symbol_138
  INTEGER(w2f__i8) OpenAD_Symbol_139
!
!       **** Parameters and Result ****
!
  type(active) :: OUTPUT
  INTEGER(w2f__i4) NX
  INTEGER(w2f__i4) NY
  INTEGER(w2f__i4) NZ
  type(active) :: INPUT(1:NZ,1:NX,1:NY)
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  INTEGER(w2f__i4) K
  INTEGER(w2f__i4) OpenAD_Symbol_1400
  INTEGER(w2f__i4) OpenAD_Symbol_1401
  INTEGER(w2f__i4) OpenAD_Symbol_1402
  type(active) :: OpenAD_prp_8
!
!       **** Statements ****
!


    integer iaddr
    external iaddr

   if (our_rev_mode%plain) then
! original function
  OUTPUT%v = 0.0D00
  DO I = 1,NX,1
    DO J = 1,NY,1
      DO K = 1,NZ,1
        OUTPUT%v = (INPUT(K,I,J)%v+OUTPUT%v)
      END DO
    END DO
  END DO
    end if
    if (our_rev_mode%tape) then
! taping
  OUTPUT%v = 0.0D00
  OpenAD_Symbol_128 = 0_w2f__i8
  DO I = 1,NX,1
    OpenAD_Symbol_129 = 0_w2f__i8
    DO J = 1,NY,1
      OpenAD_Symbol_130 = 0_w2f__i8
      DO K = 1,NZ,1
        OUTPUT%v = (INPUT(K,I,J)%v+OUTPUT%v)
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = K
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = I
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = J
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_130 = (INT(OpenAD_Symbol_130)+INT(1_w2f__i8))
      END DO
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = OpenAD_Symbol_130
      oad_it_ptr = oad_it_ptr+1
      OpenAD_Symbol_129 = (INT(OpenAD_Symbol_129)+INT(1_w2f__i8))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_129
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_128 = (INT(OpenAD_Symbol_128)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_128
  oad_it_ptr = oad_it_ptr+1
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_122 = oad_it(oad_it_ptr)
  OpenAD_Symbol_123 = 1
  do while (INT(OpenAD_Symbol_123).LE.INT(OpenAD_Symbol_122))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_124 = oad_it(oad_it_ptr)
    OpenAD_Symbol_125 = 1
    do while (INT(OpenAD_Symbol_125).LE.INT(OpenAD_Symbol_124))
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_126 = oad_it(oad_it_ptr)
      OpenAD_Symbol_127 = 1
      do while (INT(OpenAD_Symbol_127).LE.INT(OpenAD_Symbol_126))
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1400 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1401 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1402 = oad_it(oad_it_ptr)
        OpenAD_prp_8%d = OpenAD_prp_8%d+OUTPUT%d
        INPUT(OpenAD_Symbol_1402,OpenAD_Symbol_1401,OpenAD_Symbol_1400)%d = INPU&
     &T(OpenAD_Symbol_1402,OpenAD_Symbol_1401,OpenAD_Symbol_1400)%d+OUTPUT%d

        OUTPUT%d = 0.0d0
        OUTPUT%d = OUTPUT%d+OpenAD_prp_8%d
        OpenAD_prp_8%d = 0.0d0
        OpenAD_Symbol_127 = INT(OpenAD_Symbol_127)+1
      END DO
      OpenAD_Symbol_125 = INT(OpenAD_Symbol_125)+1
    END DO
    OpenAD_Symbol_123 = INT(OpenAD_Symbol_123)+1
  END DO
  OUTPUT%d = 0.0d0
    end if
  end subroutine MYSUM3D
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_ARRAY_BCS1D(ARRAY, I, ARRAY_BCS1D)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_140
  INTEGER(w2f__i8) OpenAD_Symbol_141
  INTEGER(w2f__i8) OpenAD_Symbol_142
  INTEGER(w2f__i8) OpenAD_Symbol_143
  INTEGER(w2f__i8) OpenAD_Symbol_144
  INTEGER(w2f__i8) OpenAD_Symbol_145
  INTEGER(w2f__i8) OpenAD_Symbol_146
  INTEGER(w2f__i8) OpenAD_Symbol_147
  INTEGER(w2f__i8) OpenAD_Symbol_148
  INTEGER(w2f__i8) OpenAD_Symbol_149
  INTEGER(w2f__i8) OpenAD_Symbol_150
  INTEGER(w2f__i8) OpenAD_Symbol_151
  INTEGER(w2f__i8) OpenAD_Symbol_152
  INTEGER(w2f__i8) OpenAD_Symbol_153
  INTEGER(w2f__i8) OpenAD_Symbol_154
  INTEGER(w2f__i8) OpenAD_Symbol_155
  INTEGER(w2f__i8) OpenAD_Symbol_156
  INTEGER(w2f__i8) OpenAD_Symbol_157
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) ARRAY(1 :)
  INTEGER(w2f__i4) I
  REAL(w2f__8) ARRAY_BCS1D
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) II
  INTEGER(w2f__i4) N


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  N = SIZE(ARRAY)
  II = I
  IF((I .GE. 1) .AND.(I .LE. N)) THEN
    ARRAY_BCS1D = ARRAY(I)
  ENDIF
  DO WHILE(II .GT. N)
    II = (II - N)
  END DO
  DO WHILE(II .LT. 1)
    II = (II + N)
  END DO
  ARRAY_BCS1D = ARRAY(II)
    end if
    if (our_rev_mode%tape) then
! taping
  N = SIZE(ARRAY)
  II = I
  IF((I .GE. 1) .AND.(I .LE. N)) THEN
    ARRAY_BCS1D = ARRAY(I)
    OpenAD_Symbol_145 = 1_w2f__i8
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_145
    oad_it_ptr = oad_it_ptr+1
  ELSE
    OpenAD_Symbol_146 = 0_w2f__i8
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_146
    oad_it_ptr = oad_it_ptr+1
  ENDIF
  OpenAD_Symbol_147 = 0_w2f__i8
  DO WHILE(II .GT. N)
    II = (II - N)
    OpenAD_Symbol_147 = (INT(OpenAD_Symbol_147) + INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_147
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_148 = 0_w2f__i8
  DO WHILE(II .LT. 1)
    II = (II + N)
    OpenAD_Symbol_148 = (INT(OpenAD_Symbol_148) + INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_148
  oad_it_ptr = oad_it_ptr+1
  ARRAY_BCS1D = ARRAY(II)
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_140 = oad_it(oad_it_ptr)
  OpenAD_Symbol_141 = 1
  DO WHILE(INT(OpenAD_Symbol_141) .LE. INT(OpenAD_Symbol_140))
    OpenAD_Symbol_141 = INT(OpenAD_Symbol_141) + 1
  END DO
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_142 = oad_it(oad_it_ptr)
  OpenAD_Symbol_143 = 1
  DO WHILE(INT(OpenAD_Symbol_143) .LE. INT(OpenAD_Symbol_142))
    OpenAD_Symbol_143 = INT(OpenAD_Symbol_143) + 1
  END DO
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_144 = oad_it(oad_it_ptr)
  IF(OpenAD_Symbol_144 .ne. 0) THEN
  ENDIF
    end if
  end subroutine OAD_S_ARRAY_BCS1D
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_ARRAY_BCS_LATS(ARRAY, I, ARRAY_BCS_LATS)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_158
  INTEGER(w2f__i8) OpenAD_Symbol_159
  INTEGER(w2f__i8) OpenAD_Symbol_160
  INTEGER(w2f__i8) OpenAD_Symbol_161
  INTEGER(w2f__i8) OpenAD_Symbol_162
  INTEGER(w2f__i8) OpenAD_Symbol_163
  INTEGER(w2f__i8) OpenAD_Symbol_164
  INTEGER(w2f__i8) OpenAD_Symbol_165
  INTEGER(w2f__i8) OpenAD_Symbol_166
  INTEGER(w2f__i8) OpenAD_Symbol_168
  INTEGER(w2f__i8) OpenAD_Symbol_169
  INTEGER(w2f__i8) OpenAD_Symbol_170
  INTEGER(w2f__i8) OpenAD_Symbol_171
  INTEGER(w2f__i8) OpenAD_Symbol_172
  INTEGER(w2f__i8) OpenAD_Symbol_173
  INTEGER(w2f__i8) OpenAD_Symbol_174
  INTEGER(w2f__i8) OpenAD_Symbol_175
  INTEGER(w2f__i8) OpenAD_Symbol_176
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) ARRAY(1 :)
  INTEGER(w2f__i4) I
  REAL(w2f__8) ARRAY_BCS_LATS
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) II
  INTEGER(w2f__i4) N
  INTEGER(w2f__i8) OpenAD_Symbol_167
  INTEGER(w2f__i8) OpenAD_Symbol_177


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
 2      CONTINUE
  GO TO 3
 3      CONTINUE
  N = SIZE(ARRAY)
  II = I
  GO TO 4
 4      CONTINUE
  IF((I .GE. 1) .AND.(I .LE. N)) THEN
    GO TO 16
  ELSE
    GO TO 5
  ENDIF
 5      CONTINUE
  GO TO 6
 6      CONTINUE
  GO TO 7
 7      CONTINUE
  IF(II .GT. N) THEN
    GO TO 9
  ELSE
    GO TO 8
  ENDIF
 8      CONTINUE
  GO TO 10
 9      CONTINUE
  II = (N * 2 - II)
  ARRAY_BCS_LATS = (ARRAY(II) +(-1.8D+02))
  GO TO 10
 10     CONTINUE
  GO TO 11
 11     CONTINUE
  IF(II .LT. 1) THEN
    GO TO 13
  ELSE
    GO TO 12
  ENDIF
 12     CONTINUE
  GO TO 14
 13     CONTINUE
  II = (1 - II)
  ARRAY_BCS_LATS = (1.8D+02 - ARRAY(II))
  GO TO 14
 14     CONTINUE
  GO TO 15
 15     CONTINUE
  GO TO 17
 16     CONTINUE
  ARRAY_BCS_LATS = ARRAY(I)
  GO TO 1
  GO TO 17
 17     CONTINUE
  GO TO 1
 1      CONTINUE
    end if
    if (our_rev_mode%tape) then
! taping
 19     CONTINUE
  GO TO 20
 20     CONTINUE
  OpenAD_Symbol_158 = 0_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_158
  oad_it_ptr = oad_it_ptr+1
  GO TO 21
 21     CONTINUE
  N = SIZE(ARRAY)
  II = I
  GO TO 22
 22     CONTINUE
  OpenAD_Symbol_159 = 1_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_159
  oad_it_ptr = oad_it_ptr+1
  GO TO 23
 23     CONTINUE
  IF((I .GE. 1) .AND.(I .LE. N)) THEN
    GO TO 41
  ELSE
    GO TO 24
  ENDIF
 24     CONTINUE
  GO TO 25
 25     CONTINUE
  OpenAD_Symbol_161 = 3_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_161
  oad_it_ptr = oad_it_ptr+1
  GO TO 26
 26     CONTINUE
  GO TO 27
 27     CONTINUE
  IF(II .GT. N) THEN
    GO TO 30
  ELSE
    GO TO 28
  ENDIF
 28     CONTINUE
  GO TO 29
 29     CONTINUE
  OpenAD_Symbol_166 = 8_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_166
  oad_it_ptr = oad_it_ptr+1
  GO TO 32
 30     CONTINUE
  II = (N * 2 - II)
  ARRAY_BCS_LATS = (ARRAY(II) +(-1.8D+02))
  GO TO 31
 31     CONTINUE
  OpenAD_Symbol_162 = 4_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_162
  oad_it_ptr = oad_it_ptr+1
  GO TO 32
 32     CONTINUE
  GO TO 33
 33     CONTINUE
  IF(II .LT. 1) THEN
    GO TO 36
  ELSE
    GO TO 34
  ENDIF
 34     CONTINUE
  GO TO 35
 35     CONTINUE
  OpenAD_Symbol_165 = 7_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_165
  oad_it_ptr = oad_it_ptr+1
  GO TO 38
 36     CONTINUE
  II = (1 - II)
  ARRAY_BCS_LATS = (1.8D+02 - ARRAY(II))
  GO TO 37
 37     CONTINUE
  OpenAD_Symbol_163 = 5_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_163
  oad_it_ptr = oad_it_ptr+1
  GO TO 38
 38     CONTINUE
  GO TO 39
 39     CONTINUE
  GO TO 40
 40     CONTINUE
  OpenAD_Symbol_164 = 6_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_164
  oad_it_ptr = oad_it_ptr+1
  GO TO 43
 41     CONTINUE
  ARRAY_BCS_LATS = ARRAY(I)
  GO TO 18
  GO TO 42
 42     CONTINUE
  OpenAD_Symbol_160 = 2_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_160
  oad_it_ptr = oad_it_ptr+1
  GO TO 43
 43     CONTINUE
  GO TO 18
 18     CONTINUE
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_167 = oad_it(oad_it_ptr)
  DO WHILE(INT(OpenAD_Symbol_167) .ne. INT(0_w2f__i8))
    IF ( OpenAD_Symbol_167 .EQ. 1) GO TO 68
    IF ( OpenAD_Symbol_167 .EQ. 2) GO TO 69
    IF ( OpenAD_Symbol_167 .EQ. 3) GO TO 70
    IF ( OpenAD_Symbol_167 .EQ. 4) GO TO 71
    IF ( OpenAD_Symbol_167 .EQ. 5) GO TO 72
    IF ( OpenAD_Symbol_167 .EQ. 6) GO TO 73
    IF ( OpenAD_Symbol_167 .EQ. 7) GO TO 74
    IF ( OpenAD_Symbol_167 .EQ. 8) GO TO 75
 68       CONTINUE
    GO TO 65
 69       CONTINUE
    GO TO 65
 70       CONTINUE
    GO TO 65
 71       CONTINUE
    GO TO 65
 72       CONTINUE
    GO TO 65
 73       CONTINUE
    GO TO 65
 74       CONTINUE
    GO TO 65
 75       CONTINUE
    GO TO 65
 65       CONTINUE
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_167 = oad_it(oad_it_ptr)
  END DO
    end if
  end subroutine OAD_S_ARRAY_BCS_LATS
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_ARRAY_BCS2D(ARRAY, I, J, ARRAY_BCS2D)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_178
  INTEGER(w2f__i8) OpenAD_Symbol_179
  INTEGER(w2f__i8) OpenAD_Symbol_180
  INTEGER(w2f__i8) OpenAD_Symbol_181
  INTEGER(w2f__i8) OpenAD_Symbol_182
  INTEGER(w2f__i8) OpenAD_Symbol_183
  INTEGER(w2f__i8) OpenAD_Symbol_184
  INTEGER(w2f__i8) OpenAD_Symbol_185
  INTEGER(w2f__i8) OpenAD_Symbol_186
  INTEGER(w2f__i8) OpenAD_Symbol_187
  INTEGER(w2f__i8) OpenAD_Symbol_188
  INTEGER(w2f__i8) OpenAD_Symbol_189
  INTEGER(w2f__i8) OpenAD_Symbol_190
  INTEGER(w2f__i8) OpenAD_Symbol_191
  INTEGER(w2f__i8) OpenAD_Symbol_193
  INTEGER(w2f__i8) OpenAD_Symbol_194
  INTEGER(w2f__i8) OpenAD_Symbol_195
  INTEGER(w2f__i8) OpenAD_Symbol_196
  INTEGER(w2f__i8) OpenAD_Symbol_197
  INTEGER(w2f__i8) OpenAD_Symbol_198
  INTEGER(w2f__i8) OpenAD_Symbol_199
  INTEGER(w2f__i8) OpenAD_Symbol_200
  INTEGER(w2f__i8) OpenAD_Symbol_201
  INTEGER(w2f__i8) OpenAD_Symbol_202
  INTEGER(w2f__i8) OpenAD_Symbol_203
  INTEGER(w2f__i8) OpenAD_Symbol_204
  INTEGER(w2f__i8) OpenAD_Symbol_205
  INTEGER(w2f__i8) OpenAD_Symbol_206
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) ARRAY(1 :, 1 :)
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  REAL(w2f__8) ARRAY_BCS2D
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) II
  INTEGER(w2f__i4) JJ
  INTEGER(w2f__i4) NX
  INTEGER(w2f__i4) NY
  INTEGER(w2f__i8) OpenAD_Symbol_192
  INTEGER(w2f__i8) OpenAD_Symbol_207


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
 2      CONTINUE
  GO TO 3
 3      CONTINUE
  NX = SIZE(ARRAY, 1)
  NY = SIZE(ARRAY, 2)
  GO TO 4
 4      CONTINUE
  IF((J .LE. NY) .AND.((J .GE. 1) .AND.((I .GE. 1) .AND.(I .LE. NX)))) THEN
    GO TO 25
  ELSE
    GO TO 5
  ENDIF
 5      CONTINUE
  GO TO 6
 6      CONTINUE
  GO TO 7
 7      CONTINUE
  II = I
  JJ = J
  GO TO 8
 8      CONTINUE
  IF(JJ .GT. NY) THEN
    GO TO 10
  ELSE
    GO TO 9
  ENDIF
 9      CONTINUE
  GO TO 11
 10     CONTINUE
  JJ = (NY * 2 - JJ)
  II = (II +(NX / 2))
  GO TO 11
 11     CONTINUE
  GO TO 12
 12     CONTINUE
  IF(JJ .LT. 1) THEN
    GO TO 14
  ELSE
    GO TO 13
  ENDIF
 13     CONTINUE
  GO TO 15
 14     CONTINUE
  JJ = (1 - JJ)
  II = (II +(NX / 2))
  GO TO 15
 15     CONTINUE
  GO TO 16
 16     CONTINUE
  GO TO 17
 17     CONTINUE
  IF(II .GT. NX) THEN
    GO TO 23
  ELSE
    GO TO 18
  ENDIF
 18     CONTINUE
  GO TO 19
 19     CONTINUE
  IF(II .LT. 1) THEN
    GO TO 21
  ELSE
    GO TO 20
  ENDIF
 20     CONTINUE
  ARRAY_BCS2D = ARRAY(II, JJ)
  GO TO 26
 21     CONTINUE
  II = (II + NX)
  GO TO 22
 22     CONTINUE
  GO TO 19
 23     CONTINUE
  II = (II - NX)
  GO TO 24
 24     CONTINUE
  GO TO 17
 25     CONTINUE
  ARRAY_BCS2D = ARRAY(I, J)
  GO TO 1
  GO TO 26
 26     CONTINUE
  GO TO 1
 1      CONTINUE
    end if
    if (our_rev_mode%tape) then
! taping
 28     CONTINUE
  GO TO 29
 29     CONTINUE
  OpenAD_Symbol_178 = 0_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_178
  oad_it_ptr = oad_it_ptr+1
  GO TO 30
 30     CONTINUE
  NX = SIZE(ARRAY, 1)
  NY = SIZE(ARRAY, 2)
  GO TO 31
 31     CONTINUE
  OpenAD_Symbol_179 = 1_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_179
  oad_it_ptr = oad_it_ptr+1
  GO TO 32
 32     CONTINUE
  IF((J .LE. NY) .AND.((J .GE. 1) .AND.((I .GE. 1) .AND.(I .LE. NX)))) THEN
    GO TO 64
  ELSE
    GO TO 33
  ENDIF
 33     CONTINUE
  GO TO 34
 34     CONTINUE
  OpenAD_Symbol_181 = 3_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_181
  oad_it_ptr = oad_it_ptr+1
  GO TO 35
 35     CONTINUE
  GO TO 36
 36     CONTINUE
  II = I
  JJ = J
  GO TO 37
 37     CONTINUE
  OpenAD_Symbol_182 = 4_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_182
  oad_it_ptr = oad_it_ptr+1
  GO TO 38
 38     CONTINUE
  IF(JJ .GT. NY) THEN
    GO TO 41
  ELSE
    GO TO 39
  ENDIF
 39     CONTINUE
  GO TO 40
 40     CONTINUE
  OpenAD_Symbol_191 = 13_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_191
  oad_it_ptr = oad_it_ptr+1
  GO TO 43
 41     CONTINUE
  JJ = (NY * 2 - JJ)
  II = (II +(NX / 2))
  GO TO 42
 42     CONTINUE
  OpenAD_Symbol_183 = 5_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_183
  oad_it_ptr = oad_it_ptr+1
  GO TO 43
 43     CONTINUE
  GO TO 44
 44     CONTINUE
  IF(JJ .LT. 1) THEN
    GO TO 47
  ELSE
    GO TO 45
  ENDIF
 45     CONTINUE
  GO TO 46
 46     CONTINUE
  OpenAD_Symbol_190 = 12_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_190
  oad_it_ptr = oad_it_ptr+1
  GO TO 49
 47     CONTINUE
  JJ = (1 - JJ)
  II = (II +(NX / 2))
  GO TO 48
 48     CONTINUE
  OpenAD_Symbol_184 = 6_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_184
  oad_it_ptr = oad_it_ptr+1
  GO TO 49
 49     CONTINUE
  GO TO 50
 50     CONTINUE
  GO TO 51
 51     CONTINUE
  OpenAD_Symbol_185 = 7_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_185
  oad_it_ptr = oad_it_ptr+1
  GO TO 52
 52     CONTINUE
  IF(II .GT. NX) THEN
    GO TO 61
  ELSE
    GO TO 53
  ENDIF
 53     CONTINUE
  GO TO 54
 54     CONTINUE
  OpenAD_Symbol_187 = 9_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_187
  oad_it_ptr = oad_it_ptr+1
  GO TO 55
 55     CONTINUE
  IF(II .LT. 1) THEN
    GO TO 58
  ELSE
    GO TO 56
  ENDIF
 56     CONTINUE
  ARRAY_BCS2D = ARRAY(II, JJ)
  GO TO 57
 57     CONTINUE
  OpenAD_Symbol_189 = 11_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_189
  oad_it_ptr = oad_it_ptr+1
  GO TO 66
 58     CONTINUE
  II = (II + NX)
  GO TO 59
 59     CONTINUE
  OpenAD_Symbol_188 = 10_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_188
  oad_it_ptr = oad_it_ptr+1
  GO TO 60
 60     CONTINUE
  GO TO 55
 61     CONTINUE
  II = (II - NX)
  GO TO 62
 62     CONTINUE
  OpenAD_Symbol_186 = 8_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_186
  oad_it_ptr = oad_it_ptr+1
  GO TO 63
 63     CONTINUE
  GO TO 52
 64     CONTINUE
  ARRAY_BCS2D = ARRAY(I, J)
  GO TO 27
  GO TO 65
 65     CONTINUE
  OpenAD_Symbol_180 = 2_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_180
  oad_it_ptr = oad_it_ptr+1
  GO TO 66
 66     CONTINUE
  GO TO 27
 27     CONTINUE
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_192 = oad_it(oad_it_ptr)
  DO WHILE(INT(OpenAD_Symbol_192) .ne. INT(0_w2f__i8))
    IF ( OpenAD_Symbol_192 .EQ. 1) GO TO 96
    IF ( OpenAD_Symbol_192 .EQ. 2) GO TO 97
    IF ( OpenAD_Symbol_192 .EQ. 3) GO TO 98
    IF ( OpenAD_Symbol_192 .EQ. 4) GO TO 99
    IF ( OpenAD_Symbol_192 .EQ. 5) GO TO 100
    IF ( OpenAD_Symbol_192 .EQ. 6) GO TO 101
    IF ( OpenAD_Symbol_192 .EQ. 7) GO TO 102
    IF ( OpenAD_Symbol_192 .EQ. 8) GO TO 103
    IF ( OpenAD_Symbol_192 .EQ. 9) GO TO 104
    IF ( OpenAD_Symbol_192 .EQ. 10) GO TO 105
    IF ( OpenAD_Symbol_192 .EQ. 11) GO TO 106
    IF ( OpenAD_Symbol_192 .EQ. 12) GO TO 107
    IF ( OpenAD_Symbol_192 .EQ. 13) GO TO 108
 96       CONTINUE
    GO TO 93
 97       CONTINUE
    GO TO 93
 98       CONTINUE
    GO TO 93
 99       CONTINUE
    GO TO 93
 100      CONTINUE
    GO TO 93
 101      CONTINUE
    GO TO 93
 102      CONTINUE
    GO TO 93
 103      CONTINUE
    GO TO 93
 104      CONTINUE
    GO TO 93
 105      CONTINUE
    GO TO 93
 106      CONTINUE
    GO TO 93
 107      CONTINUE
    GO TO 93
 108      CONTINUE
    GO TO 93
 93       CONTINUE
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_192 = oad_it(oad_it_ptr)
  END DO
    end if
  end subroutine OAD_S_ARRAY_BCS2D
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_VERTINTG(DUPS, IN, VERTINTG)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_208
  INTEGER(w2f__i8) OpenAD_Symbol_209
  INTEGER(w2f__i8) OpenAD_Symbol_210
  INTEGER(w2f__i8) OpenAD_Symbol_211
  INTEGER(w2f__i8) OpenAD_Symbol_212
  INTEGER(w2f__i8) OpenAD_Symbol_213
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) DUPS(1 :)
  type(active) :: IN(1:INT(SIZE(DUPS)))
  type(active) :: VERTINTG
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) UP
  INTEGER(w2f__i4) UPN
  INTEGER(w2f__i4) OpenAD_Symbol_1403
  REAL(w2f__8) OpenAD_Symbol_1404
  INTEGER(w2f__i4) OpenAD_Symbol_1405
  INTEGER(w2f__i4) OpenAD_Symbol_214
  REAL(w2f__8) OpenAD_aux_4
  REAL(w2f__8) OpenAD_lin_16
  type(active) :: OpenAD_prp_10
  type(active) :: OpenAD_prp_11
  type(active) :: OpenAD_prp_9


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  UPN = SIZE(IN)
  VERTINTG%v = 0.0D00
  DO UP = UPN+(-1),1,(-1)
    VERTINTG%v = (VERTINTG%v+DUPS(UP)*(IN(UP)%v+IN(UP+1)%v))
  END DO
  VERTINTG%v = (VERTINTG%v*5.0D-01)
    end if
    if (our_rev_mode%tape) then
! taping
  UPN = SIZE(IN)
  VERTINTG%v = 0.0D00
  OpenAD_Symbol_210 = 0_w2f__i8
  DO UP = UPN+(-1),1,(-1)
    OpenAD_aux_4 = (IN(UP)%v+IN(UP+1)%v)
    OpenAD_lin_16 = DUPS(UP)
    VERTINTG%v = (VERTINTG%v+DUPS(UP)*OpenAD_aux_4)
    OpenAD_Symbol_214 = (UP+1)
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_214
    oad_it_ptr = oad_it_ptr+1
    if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
    oad_dt(oad_dt_ptr) = OpenAD_lin_16
    oad_dt_ptr = oad_dt_ptr+1
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = UP
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_210 = (INT(OpenAD_Symbol_210)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_210
  oad_it_ptr = oad_it_ptr+1
  VERTINTG%v = (VERTINTG%v*5.0D-01)
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  OpenAD_prp_11%d = OpenAD_prp_11%d+VERTINTG%d*(5.0D-01)
  VERTINTG%d = 0.0d0
  VERTINTG%d = VERTINTG%d+OpenAD_prp_11%d
  OpenAD_prp_11%d = 0.0d0
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_208 = oad_it(oad_it_ptr)
  OpenAD_Symbol_209 = 1
  do while (INT(OpenAD_Symbol_209).LE.INT(OpenAD_Symbol_208))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1403 = oad_it(oad_it_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1404 = oad_dt(oad_dt_ptr)
    OpenAD_prp_10%d = OpenAD_prp_10%d+VERTINTG%d*(OpenAD_Symbol_1404)
    OpenAD_prp_9%d = OpenAD_prp_9%d+VERTINTG%d
    VERTINTG%d = 0.0d0
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1405 = oad_it(oad_it_ptr)
    IN(OpenAD_Symbol_1405)%d = IN(OpenAD_Symbol_1405)%d+OpenAD_prp_10%d
    IN(OpenAD_Symbol_1403)%d = IN(OpenAD_Symbol_1403)%d+OpenAD_prp_10%d
    OpenAD_prp_10%d = 0.0d0
    VERTINTG%d = VERTINTG%d+OpenAD_prp_9%d
    OpenAD_prp_9%d = 0.0d0
    OpenAD_Symbol_209 = INT(OpenAD_Symbol_209)+1
  END DO
  VERTINTG%d = 0.0d0
    end if
  end subroutine OAD_S_VERTINTG
END

!$OPENAD XXX File_start [glimmer_deriv.F90]
MODULE glimmer_deriv
use OAD_active
use w2f__types
use oad_intrinsics
use glimmer_global ,only: DP , SP
IMPLICIT NONE
SAVE
!
!     **** Statements ****
!
CONTAINS

  Function DFDX_2D(F, I, J, DELTA)
  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :)
  INTENT(IN) F
  INTEGER(w2f__i4) I
  INTENT(IN) I
  INTEGER(w2f__i4) J
  INTENT(IN) J
  REAL(w2f__8) DELTA
  INTENT(IN) DELTA
  REAL(w2f__8) DFDX_2D
!
!       **** Statements ****
!
  DFDX_2D = F(I+(-1),J)*((-5.0D-01)/DELTA)+F(I+1,J)*(5.0D-01/DELTA)
  RETURN
  END FUNCTION

  Function DFDY_2D(F, I, J, DELTA)
  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :)
  INTENT(IN) F
  INTEGER(w2f__i4) I
  INTENT(IN) I
  INTEGER(w2f__i4) J
  INTENT(IN) J
  REAL(w2f__8) DELTA
  INTENT(IN) DELTA
  REAL(w2f__8) DFDY_2D
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) JM1
  INTEGER(w2f__i4) JP1
!
!       **** Statements ****
!
  JP1 = J + 1
  JM1 = J +(-1)
  IF(JP1 .eq.(SIZE(F, 2) + 1)) THEN
    JP1 = 2
  ENDIF
  IF(JM1 .eq. 0) THEN
    JM1 = SIZE(F, 2) +(-1)
  ENDIF
  DFDY_2D = F(I,J+(-1))*((-5.0D-01)/DELTA)+F(I,J+1)*(5.0D-01/DELTA)
  RETURN
  END FUNCTION

  Function DFDX_2D_STAG(F, I, J, DELTA)
  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :)
  INTENT(IN) F
  INTEGER(w2f__i4) I
  INTENT(IN) I
  INTEGER(w2f__i4) J
  INTENT(IN) J
  REAL(w2f__8) DELTA
  INTENT(IN) DELTA
  REAL(w2f__8) DFDX_2D_STAG
!
!       **** Statements ****
!
  DFDX_2D_STAG = (F(I+1,J)+F(I+1,J+1)-F(I,J)-F(I,J+1))/(DELTA*2.0D00)
  RETURN
  END FUNCTION

  Function DFDY_2D_STAG(F, I, J, DELTA)
  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :)
  INTENT(IN) F
  INTEGER(w2f__i4) I
  INTENT(IN) I
  INTEGER(w2f__i4) J
  INTENT(IN) J
  REAL(w2f__8) DELTA
  INTENT(IN) DELTA
  REAL(w2f__8) DFDY_2D_STAG
!
!       **** Statements ****
!
  DFDY_2D_STAG = (F(I,J+1)+F(I+1,J+1)-F(I,J)-F(I+1,J))/(DELTA*2.0D00)
  RETURN
  END FUNCTION

  Function DFDX_2D_UPWIND(F, I, J, DELTA)
  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :)
  INTENT(IN) F
  INTEGER(w2f__i4) I
  INTENT(IN) I
  INTEGER(w2f__i4) J
  INTENT(IN) J
  REAL(w2f__8) DELTA
  INTENT(IN) DELTA
  REAL(w2f__8) DFDX_2D_UPWIND
!
!       **** Statements ****
!
  DFDX_2D_UPWIND = (F(I,J)*1.5D00+F(I+(-2),J)*5.0D-01-F(I+(-1),J)*2.0D00)/DELTA
  RETURN
  END FUNCTION

  Function DFDY_2D_UPWIND(F, I, J, DELTA)
  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :)
  INTENT(IN) F
  INTEGER(w2f__i4) I
  INTENT(IN) I
  INTEGER(w2f__i4) J
  INTENT(IN) J
  REAL(w2f__8) DELTA
  INTENT(IN) DELTA
  REAL(w2f__8) DFDY_2D_UPWIND
!
!       **** Statements ****
!
  DFDY_2D_UPWIND = (F(I,J)*1.5D00+F(I,J+(-2))*5.0D-01-F(I,J+(-1))*2.0D00)/DELTA
  RETURN
  END FUNCTION

  Function DFDX_2D_DOWNWIND(F, I, J, DELTA)
  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :)
  INTENT(IN) F
  INTEGER(w2f__i4) I
  INTENT(IN) I
  INTEGER(w2f__i4) J
  INTENT(IN) J
  REAL(w2f__8) DELTA
  INTENT(IN) DELTA
  REAL(w2f__8) DFDX_2D_DOWNWIND
!
!       **** Statements ****
!
  DFDX_2D_DOWNWIND = (F(I,J)*(-1.5D00)+F(I+1,J)*2.0D00-F(I+2,J)*5.0D-01)/DELTA
  RETURN
  END FUNCTION

  Function DFDY_2D_DOWNWIND(F, I, J, DELTA)
  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :)
  INTENT(IN) F
  INTEGER(w2f__i4) I
  INTENT(IN) I
  INTEGER(w2f__i4) J
  INTENT(IN) J
  REAL(w2f__8) DELTA
  INTENT(IN) DELTA
  REAL(w2f__8) DFDY_2D_DOWNWIND
!
!       **** Statements ****
!
  DFDY_2D_DOWNWIND = (F(I,J)*(-1.5D00)+F(I,J+1)*2.0D00-F(I,J+2)*5.0D-01)/DELTA
  RETURN
  END FUNCTION
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE DF_FIELD_2D_STAGGERED(F, DELTAX, DELTAY, OUT_DFDX, OUT_DFDY)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_215
  INTEGER(w2f__i8) OpenAD_Symbol_216
  INTEGER(w2f__i8) OpenAD_Symbol_217
  INTEGER(w2f__i8) OpenAD_Symbol_218
  INTEGER(w2f__i8) OpenAD_Symbol_219
  INTEGER(w2f__i8) OpenAD_Symbol_220
  INTEGER(w2f__i8) OpenAD_Symbol_221
  INTEGER(w2f__i8) OpenAD_Symbol_222
  INTEGER(w2f__i8) OpenAD_Symbol_223
  INTEGER(w2f__i8) OpenAD_Symbol_224
  INTEGER(w2f__i8) OpenAD_Symbol_225
  INTEGER(w2f__i8) OpenAD_Symbol_226
!
!       **** Parameters and Result ****
!
  type(active) :: F(1:,1:)
  REAL(w2f__8) DELTAX
  REAL(w2f__8) DELTAY
  type(active) :: OUT_DFDX(1:,1:)
  type(active) :: OUT_DFDY(1:,1:)
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) NX
  INTEGER(w2f__i4) NY
  INTEGER(w2f__i4) X
  INTEGER(w2f__i4) Y
  INTEGER(w2f__i4) OpenAD_Symbol_1406
  INTEGER(w2f__i4) OpenAD_Symbol_1407
  REAL(w2f__8) OpenAD_Symbol_1408
  INTEGER(w2f__i4) OpenAD_Symbol_1409
  INTEGER(w2f__i4) OpenAD_Symbol_1410
  INTEGER(w2f__i4) OpenAD_Symbol_1411
  INTEGER(w2f__i4) OpenAD_Symbol_1412
  INTEGER(w2f__i4) OpenAD_Symbol_1413
  INTEGER(w2f__i4) OpenAD_Symbol_1414
  REAL(w2f__8) OpenAD_Symbol_1415
  INTEGER(w2f__i4) OpenAD_Symbol_1416
  INTEGER(w2f__i4) OpenAD_Symbol_1417
  INTEGER(w2f__i4) OpenAD_Symbol_1418
  INTEGER(w2f__i4) OpenAD_Symbol_1419
  INTEGER(w2f__i4) OpenAD_Symbol_227
  INTEGER(w2f__i4) OpenAD_Symbol_228
  INTEGER(w2f__i4) OpenAD_Symbol_229
  INTEGER(w2f__i4) OpenAD_Symbol_230
  INTEGER(w2f__i4) OpenAD_Symbol_231
  INTEGER(w2f__i4) OpenAD_Symbol_232
  INTEGER(w2f__i4) OpenAD_Symbol_233
  INTEGER(w2f__i4) OpenAD_Symbol_234
  REAL(w2f__8) OpenAD_aux_5
  REAL(w2f__8) OpenAD_aux_6
  REAL(w2f__8) OpenAD_aux_7
  REAL(w2f__8) OpenAD_aux_8
  REAL(w2f__8) OpenAD_lin_17
  REAL(w2f__8) OpenAD_lin_18
  type(active) :: OpenAD_prp_12
  type(active) :: OpenAD_prp_13


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  NX = SIZE(F, 1)
  NY = SIZE(F, 2)
  DO X = 1, (NX +(-1)), 1
    DO Y = 1, (NY +(-1)), 1
      OUT_DFDX(INT(X),INT(Y))%v = ((F(X+1,Y)%v+F(X+1,Y+1)%v-F(X,Y)%v-F(X,Y+1)%v)&
     &/(DELTAX*2.0D00))

      OUT_DFDY(INT(X),INT(Y))%v = ((F(X,Y+1)%v+F(X+1,Y+1)%v-F(X,Y)%v-F(X+1,Y)%v)&
     &/(DELTAY*2.0D00))

    END DO
  END DO
    end if
    if (our_rev_mode%tape) then
! taping
  NX = SIZE(F,1)
  NY = SIZE(F,2)
  OpenAD_Symbol_219 = 0_w2f__i8
  DO X = 1,(NX+(-1)),1
    OpenAD_Symbol_220 = 0_w2f__i8
    DO Y = 1,(NY+(-1)),1
      OpenAD_aux_5 = (F(X+1,Y)%v+F(X+1,Y+1)%v-F(X,Y)%v-F(X,Y+1)%v)
      OpenAD_aux_6 = (DELTAX*2.0D00)
      OpenAD_lin_17 = (INT(1_w2f__i8)/OpenAD_aux_6)
      OUT_DFDX(INT(X),INT(Y))%v = (OpenAD_aux_5/OpenAD_aux_6)
      OpenAD_Symbol_227 = (X+1)
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = OpenAD_Symbol_227
      oad_it_ptr = oad_it_ptr+1
      OpenAD_Symbol_228 = (X+1)
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = OpenAD_Symbol_228
      oad_it_ptr = oad_it_ptr+1
      OpenAD_Symbol_229 = (Y+1)
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = OpenAD_Symbol_229
      oad_it_ptr = oad_it_ptr+1
      OpenAD_Symbol_230 = (Y+1)
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = OpenAD_Symbol_230
      oad_it_ptr = oad_it_ptr+1
      if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
      oad_dt(oad_dt_ptr) = OpenAD_lin_17
      oad_dt_ptr = oad_dt_ptr+1
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = Y
      oad_it_ptr = oad_it_ptr+1
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = X
      oad_it_ptr = oad_it_ptr+1
      OpenAD_aux_7 = (F(X,Y+1)%v+F(X+1,Y+1)%v-F(X,Y)%v-F(X+1,Y)%v)
      OpenAD_aux_8 = (DELTAY*2.0D00)
      OpenAD_lin_18 = (INT(1_w2f__i8)/OpenAD_aux_8)
      OUT_DFDY(INT(X),INT(Y))%v = (OpenAD_aux_7/OpenAD_aux_8)
      OpenAD_Symbol_231 = (Y+1)
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = OpenAD_Symbol_231
      oad_it_ptr = oad_it_ptr+1
      OpenAD_Symbol_232 = (X+1)
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = OpenAD_Symbol_232
      oad_it_ptr = oad_it_ptr+1
      OpenAD_Symbol_233 = (Y+1)
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = OpenAD_Symbol_233
      oad_it_ptr = oad_it_ptr+1
      OpenAD_Symbol_234 = (X+1)
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = OpenAD_Symbol_234
      oad_it_ptr = oad_it_ptr+1
      if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
      oad_dt(oad_dt_ptr) = OpenAD_lin_18
      oad_dt_ptr = oad_dt_ptr+1
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = X
      oad_it_ptr = oad_it_ptr+1
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = Y
      oad_it_ptr = oad_it_ptr+1
      OpenAD_Symbol_220 = (INT(OpenAD_Symbol_220)+INT(1_w2f__i8))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_220
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_219 = (INT(OpenAD_Symbol_219)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_219
  oad_it_ptr = oad_it_ptr+1
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_215 = oad_it(oad_it_ptr)
  OpenAD_Symbol_216 = 1
  do while (INT(OpenAD_Symbol_216).LE.INT(OpenAD_Symbol_215))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_217 = oad_it(oad_it_ptr)
    OpenAD_Symbol_218 = 1
    do while (INT(OpenAD_Symbol_218).LE.INT(OpenAD_Symbol_217))
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1406 = oad_it(oad_it_ptr)
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1407 = oad_it(oad_it_ptr)
      oad_dt_ptr = oad_dt_ptr-1
      OpenAD_Symbol_1408 = oad_dt(oad_dt_ptr)
      OpenAD_prp_13%d = OpenAD_prp_13%d+OUT_DFDY(OpenAD_Symbol_1407,OpenAD_Symbo&
     &l_1406)%d*(OpenAD_Symbol_1408)

      OUT_DFDY(OpenAD_Symbol_1407,OpenAD_Symbol_1406)%d = 0.0d0
      F(OpenAD_Symbol_1407,OpenAD_Symbol_1406)%d = F(OpenAD_Symbol_1407,OpenAD_S&
     &ymbol_1406)%d-OpenAD_prp_13%d

      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1409 = oad_it(oad_it_ptr)
      F(OpenAD_Symbol_1409,OpenAD_Symbol_1406)%d = F(OpenAD_Symbol_1409,OpenAD_S&
     &ymbol_1406)%d-OpenAD_prp_13%d

      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1410 = oad_it(oad_it_ptr)
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1411 = oad_it(oad_it_ptr)
      F(OpenAD_Symbol_1411,OpenAD_Symbol_1410)%d = F(OpenAD_Symbol_1411,OpenAD_S&
     &ymbol_1410)%d+OpenAD_prp_13%d

      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1412 = oad_it(oad_it_ptr)
      F(OpenAD_Symbol_1407,OpenAD_Symbol_1412)%d = F(OpenAD_Symbol_1407,OpenAD_S&
     &ymbol_1412)%d+OpenAD_prp_13%d

      OpenAD_prp_13%d = 0.0d0
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1413 = oad_it(oad_it_ptr)
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1414 = oad_it(oad_it_ptr)
      oad_dt_ptr = oad_dt_ptr-1
      OpenAD_Symbol_1415 = oad_dt(oad_dt_ptr)
      OpenAD_prp_12%d = OpenAD_prp_12%d+OUT_DFDX(OpenAD_Symbol_1413,OpenAD_Symbo&
     &l_1414)%d*(OpenAD_Symbol_1415)

      OUT_DFDX(OpenAD_Symbol_1413,OpenAD_Symbol_1414)%d = 0.0d0
      F(OpenAD_Symbol_1413,OpenAD_Symbol_1414)%d = F(OpenAD_Symbol_1413,OpenAD_S&
     &ymbol_1414)%d-OpenAD_prp_12%d

      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1416 = oad_it(oad_it_ptr)
      F(OpenAD_Symbol_1413,OpenAD_Symbol_1416)%d = F(OpenAD_Symbol_1413,OpenAD_S&
     &ymbol_1416)%d-OpenAD_prp_12%d

      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1417 = oad_it(oad_it_ptr)
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1418 = oad_it(oad_it_ptr)
      F(OpenAD_Symbol_1418,OpenAD_Symbol_1417)%d = F(OpenAD_Symbol_1418,OpenAD_S&
     &ymbol_1417)%d+OpenAD_prp_12%d

      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1419 = oad_it(oad_it_ptr)
      F(OpenAD_Symbol_1419,OpenAD_Symbol_1414)%d = F(OpenAD_Symbol_1419,OpenAD_S&
     &ymbol_1414)%d+OpenAD_prp_12%d

      OpenAD_prp_12%d = 0.0d0
      OpenAD_Symbol_218 = INT(OpenAD_Symbol_218)+1
    END DO
    OpenAD_Symbol_216 = INT(OpenAD_Symbol_216)+1
  END DO
    end if
  end subroutine DF_FIELD_2D_STAGGERED

  Function DFDX_3D(F, I, J, K, DELTA)
  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :, 1 :)
  INTENT(IN) F
  INTEGER(w2f__i4) I
  INTENT(IN) I
  INTEGER(w2f__i4) J
  INTENT(IN) J
  INTEGER(w2f__i4) K
  INTENT(IN) K
  REAL(w2f__8) DELTA
  INTENT(IN) DELTA
  REAL(w2f__8) DFDX_3D
!
!       **** Statements ****
!
  DFDX_3D = F(K,I+(-1),J)*((-5.0D-01)/DELTA)+F(K,I+1,J)*(5.0D-01/DELTA)
  RETURN
  END FUNCTION

  Function DFDY_3D(F, I, J, K, DELTA)
  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :, 1 :)
  INTENT(IN) F
  INTEGER(w2f__i4) I
  INTENT(IN) I
  INTEGER(w2f__i4) J
  INTENT(IN) J
  INTEGER(w2f__i4) K
  INTENT(IN) K
  REAL(w2f__8) DELTA
  INTENT(IN) DELTA
  REAL(w2f__8) DFDY_3D
!
!       **** Statements ****
!
  DFDY_3D = F(K,I,J+(-1))*((-5.0D-01)/DELTA)+F(K,I,J+1)*(5.0D-01/DELTA)
  RETURN
  END FUNCTION

  Function DFDZ_3D_IRREGULAR(F, I, J, K, DELTAS)
  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :, 1 :)
  INTENT(IN) F
  INTEGER(w2f__i4) I
  INTENT(IN) I
  INTEGER(w2f__i4) J
  INTENT(IN) J
  INTEGER(w2f__i4) K
  INTENT(IN) K
  REAL(w2f__8) DELTAS(1 :)
  INTENT(IN) DELTAS
  REAL(w2f__8) DFDZ_3D_IRREGULAR
!
!       **** Statements ****
!
  DFDZ_3D_IRREGULAR = ((F(K+(-1),I,J)*(DELTAS(K)-DELTAS(K+1)))/((DELTAS(K)-DELTA&
     &S(K+(-1)))*(DELTAS(K+1)-DELTAS(K+(-1)))))+((F(K,I,J)*(DELTAS(K+(-1))+DELTA&
     &S(K+1)-DELTAS(K)*2.0D00))/((DELTAS(K)-DELTAS(K+(-1)))*(DELTAS(K+1)-DELTAS(&
     &K))))+((F(K+1,I,J)*(DELTAS(K)-DELTAS(K+(-1))))/((DELTAS(K+1)-DELTAS(K))*(D&
     &ELTAS(K+1)-DELTAS(K+(-1)))))

  RETURN
  END FUNCTION

  Function DFDZ_3D_UPWIND_IRREGULAR(F, I, J, K, DELTAS)
  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :, 1 :)
  INTENT(IN) F
  INTEGER(w2f__i4) I
  INTENT(IN) I
  INTEGER(w2f__i4) J
  INTENT(IN) J
  INTEGER(w2f__i4) K
  INTENT(IN) K
  REAL(w2f__8) DELTAS(1 :)
  INTENT(IN) DELTAS
  REAL(w2f__8) DFDZ_3D_UPWIND_IRREGULAR
!
!       **** Local Variables and Functions ****
!
  REAL(w2f__8) ZKM1MINUSZKM2
  REAL(w2f__8) ZKMINUSZKM1
  REAL(w2f__8) ZKMINUSZKM2
!
!       **** Statements ****
!
  ZKMINUSZKM1 = DELTAS(K)-DELTAS(K+(-1))
  ZKMINUSZKM2 = DELTAS(K)-DELTAS(K+(-2))
  ZKM1MINUSZKM2 = DELTAS(K+(-1))-DELTAS(K+(-2))
  DFDZ_3D_UPWIND_IRREGULAR = ((F(K,I,J)*(DELTAS(K)*2.0D00-DELTAS(K+(-1))-DELTAS(&
     &K+(-2))))/(ZKMINUSZKM1*ZKMINUSZKM2))+((F(K+(-2),I,J)*ZKMINUSZKM1)/(ZKM1MIN&
     &USZKM2*ZKMINUSZKM2))-((F(K+(-1),I,J)*ZKMINUSZKM2)/(ZKM1MINUSZKM2*ZKMINUSZK&
     &M1))

  RETURN
  END FUNCTION

  Function DFDX_3D_STAG(F, I, J, K, DELTA)
  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :, 1 :)
  INTENT(IN) F
  INTEGER(w2f__i4) I
  INTENT(IN) I
  INTEGER(w2f__i4) J
  INTENT(IN) J
  INTEGER(w2f__i4) K
  INTENT(IN) K
  REAL(w2f__8) DELTA
  INTENT(IN) DELTA
  REAL(w2f__8) DFDX_3D_STAG
!
!       **** Statements ****
!
  DFDX_3D_STAG = (F(K,I+1,J)+F(K,I+1,J+1)-F(K,I,J)-F(K,I,J+1))/(DELTA*2.0D00)
  RETURN
  END FUNCTION

  Function DFDY_3D_STAG(F, I, J, K, DELTA)
  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :, 1 :)
  INTENT(IN) F
  INTEGER(w2f__i4) I
  INTENT(IN) I
  INTEGER(w2f__i4) J
  INTENT(IN) J
  INTEGER(w2f__i4) K
  INTENT(IN) K
  REAL(w2f__8) DELTA
  INTENT(IN) DELTA
  REAL(w2f__8) DFDY_3D_STAG
!
!       **** Statements ****
!
  DFDY_3D_STAG = (F(K,I,J+1)+F(K,I+1,J+1)-F(K,I,J)-F(K,I+1,J))/(DELTA*2.0D00)
  RETURN
  END FUNCTION

  Function DFDX_3D_UPWIND(F, I, J, K, DELTA)
  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :, 1 :)
  INTENT(IN) F
  INTEGER(w2f__i4) I
  INTENT(IN) I
  INTEGER(w2f__i4) J
  INTENT(IN) J
  INTEGER(w2f__i4) K
  INTENT(IN) K
  REAL(w2f__8) DELTA
  INTENT(IN) DELTA
  REAL(w2f__8) DFDX_3D_UPWIND
!
!       **** Statements ****
!
  DFDX_3D_UPWIND = (F(K,I,J)*1.5D00+F(K,I+(-2),J)*5.0D-01-F(K,I+(-1),J)*2.0D00)/&
     &DELTA

  RETURN
  END FUNCTION

  Function DFDY_3D_UPWIND(F, I, J, K, DELTA)
  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :, 1 :)
  INTENT(IN) F
  INTEGER(w2f__i4) I
  INTENT(IN) I
  INTEGER(w2f__i4) J
  INTENT(IN) J
  INTEGER(w2f__i4) K
  INTENT(IN) K
  REAL(w2f__8) DELTA
  INTENT(IN) DELTA
  REAL(w2f__8) DFDY_3D_UPWIND
!
!       **** Statements ****
!
  DFDY_3D_UPWIND = (F(K,I,J)*1.5D00+F(K,I,J+(-2))*5.0D-01-F(K,I,J+(-1))*2.0D00)/&
     &DELTA

  RETURN
  END FUNCTION

  Function DFDX_3D_DOWNWIND(F, I, J, K, DELTA)
  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :, 1 :)
  INTENT(IN) F
  INTEGER(w2f__i4) I
  INTENT(IN) I
  INTEGER(w2f__i4) J
  INTENT(IN) J
  INTEGER(w2f__i4) K
  INTENT(IN) K
  REAL(w2f__8) DELTA
  INTENT(IN) DELTA
  REAL(w2f__8) DFDX_3D_DOWNWIND
!
!       **** Statements ****
!
  DFDX_3D_DOWNWIND = (F(K,I,J)*(-1.5D00)+F(K,I+1,J)*2.0D00-F(K,I+2,J)*5.0D-01)/D&
     &ELTA

  RETURN
  END FUNCTION

  Function DFDY_3D_DOWNWIND(F, I, J, K, DELTA)
  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :, 1 :)
  INTENT(IN) F
  INTEGER(w2f__i4) I
  INTENT(IN) I
  INTEGER(w2f__i4) J
  INTENT(IN) J
  INTEGER(w2f__i4) K
  INTENT(IN) K
  REAL(w2f__8) DELTA
  INTENT(IN) DELTA
  REAL(w2f__8) DFDY_3D_DOWNWIND
!
!       **** Statements ****
!
  DFDY_3D_DOWNWIND = (F(K,I,J)*(-1.5D00)+F(K,I,J+1)*2.0D00-F(K,I,J+2)*5.0D-01)/D&
     &ELTA

  RETURN
  END FUNCTION

  Function D2FDX2_2D(F, I, J, DELTA)
  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :)
  INTENT(IN) F
  INTEGER(w2f__i4) I
  INTENT(IN) I
  INTEGER(w2f__i4) J
  INTENT(IN) J
  REAL(w2f__8) DELTA
  INTENT(IN) DELTA
  REAL(w2f__8) D2FDX2_2D
!
!       **** Statements ****
!
  D2FDX2_2D = (F(I+1,J)+F(I+(-1),J)-F(I,J)*2.0D00)/(DELTA*DELTA)
  RETURN
  END FUNCTION

  Function D2FDX2_2D_DOWNWIND(F, I, J, DELTA)
  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :)
  INTENT(IN) F
  INTEGER(w2f__i4) I
  INTENT(IN) I
  INTEGER(w2f__i4) J
  INTENT(IN) J
  REAL(w2f__8) DELTA
  INTENT(IN) DELTA
  REAL(w2f__8) D2FDX2_2D_DOWNWIND
!
!       **** Statements ****
!
  D2FDX2_2D_DOWNWIND = (F(I+2,J)*5.0D00+F(I,J)*3.0D00-F(I+1,J)*7.0D00-F(I+3,J))/&
     &((DELTA**2)*2.0D00)

  RETURN
  END FUNCTION

  Function D2FDX2_2D_UPWIND(F, I, J, DELTA)
  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :)
  INTENT(IN) F
  INTEGER(w2f__i4) I
  INTENT(IN) I
  INTEGER(w2f__i4) J
  INTENT(IN) J
  REAL(w2f__8) DELTA
  INTENT(IN) DELTA
  REAL(w2f__8) D2FDX2_2D_UPWIND
!
!       **** Statements ****
!
  D2FDX2_2D_UPWIND = (F(I+(-2),J)*5.0D00+F(I,J)*3.0D00-F(I+(-1),J)*7.0D00-F(I+(-&
     &3),J))/((DELTA**2)*2.0D00)

  RETURN
  END FUNCTION

  Function D2FDY2_2D_DOWNWIND(F, I, J, DELTA)
  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :)
  INTENT(IN) F
  INTEGER(w2f__i4) I
  INTENT(IN) I
  INTEGER(w2f__i4) J
  INTENT(IN) J
  REAL(w2f__8) DELTA
  INTENT(IN) DELTA
  REAL(w2f__8) D2FDY2_2D_DOWNWIND
!
!       **** Statements ****
!
  D2FDY2_2D_DOWNWIND = (F(I,J+2)*5.0D00+F(I,J)*3.0D00-F(I,J+1)*7.0D00-F(I,J+3))/&
     &((DELTA**2)*2.0D00)

  RETURN
  END FUNCTION

  Function D2FDY2_2D_UPWIND(F, I, J, DELTA)
  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :)
  INTENT(IN) F
  INTEGER(w2f__i4) I
  INTENT(IN) I
  INTEGER(w2f__i4) J
  INTENT(IN) J
  REAL(w2f__8) DELTA
  INTENT(IN) DELTA
  REAL(w2f__8) D2FDY2_2D_UPWIND
!
!       **** Statements ****
!
  D2FDY2_2D_UPWIND = (F(I,J+(-2))*5.0D00+F(I,J)*3.0D00-F(I,J+(-1))*7.0D00-F(I,J+&
     &(-3)))/((DELTA**2)*2.0D00)

  RETURN
  END FUNCTION

  Function D2FDY2_2D(F, I, J, DELTA)
  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :)
  INTENT(IN) F
  INTEGER(w2f__i4) I
  INTENT(IN) I
  INTEGER(w2f__i4) J
  INTENT(IN) J
  REAL(w2f__8) DELTA
  INTENT(IN) DELTA
  REAL(w2f__8) D2FDY2_2D
!
!       **** Statements ****
!
  D2FDY2_2D = (F(I,J+(-1))+F(I,J+1)-F(I,J)*2.0D00)/(DELTA*DELTA)
  RETURN
  END FUNCTION

  Function D2FDXY_3D(F, I, J, K, DELTA_X, DELTA_Y)
  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :, 1 :)
  INTENT(IN) F
  INTEGER(w2f__i4) I
  INTENT(IN) I
  INTEGER(w2f__i4) J
  INTENT(IN) J
  INTEGER(w2f__i4) K
  INTENT(IN) K
  REAL(w2f__8) DELTA_X
  INTENT(IN) DELTA_X
  REAL(w2f__8) DELTA_Y
  INTENT(IN) DELTA_Y
  REAL(w2f__8) D2FDXY_3D
!
!       **** Statements ****
!
  D2FDXY_3D = (F(K,I+1,J+1)+F(K,I+(-1),J+(-1))-F(K,I+(-1),J+1)-F(K,I+1,J+(-1)))/&
     &(DELTA_Y*DELTA_X*4.0D00)

  RETURN
  END FUNCTION

  Function D2FDXZ_3D(F, I, J, K, DELTA_X, DELTAS)
  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :, 1 :)
  INTENT(IN) F
  INTEGER(w2f__i4) I
  INTENT(IN) I
  INTEGER(w2f__i4) J
  INTENT(IN) J
  INTEGER(w2f__i4) K
  INTENT(IN) K
  REAL(w2f__8) DELTA_X
  INTENT(IN) DELTA_X
  REAL(w2f__8) DELTAS(1 :)
  INTENT(IN) DELTAS
  REAL(w2f__8) D2FDXZ_3D
!
!       **** Statements ****
!
  D2FDXZ_3D = ((((DELTAS(K)-DELTAS(K+1))*(F(K+(-1),I+1,J)-F(K+(-1),I+(-1),J)))/(&
     &(DELTAS(K)-DELTAS(K+(-1)))*(DELTAS(K+1)-DELTAS(K+(-1)))))+(((F(K,I+1,J)-F(&
     &K,I+(-1),J))*(DELTAS(K+(-1))+DELTAS(K+1)-DELTAS(K)*2.0D00))/((DELTAS(K)-DE&
     &LTAS(K+(-1)))*(DELTAS(K+1)-DELTAS(K))))+(((DELTAS(K)-DELTAS(K+(-1)))*(F(K+&
     &1,I+1,J)-F(K+1,I+(-1),J)))/((DELTAS(K+1)-DELTAS(K))*(DELTAS(K+1)-DELTAS(K+&
     &(-1))))))*(5.0D-01/DELTA_X)

  RETURN
  END FUNCTION

  Function D2FDYZ_3D(F, I, J, K, DELTA_Y, DELTAS)
  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :, 1 :)
  INTENT(IN) F
  INTEGER(w2f__i4) I
  INTENT(IN) I
  INTEGER(w2f__i4) J
  INTENT(IN) J
  INTEGER(w2f__i4) K
  INTENT(IN) K
  REAL(w2f__8) DELTA_Y
  INTENT(IN) DELTA_Y
  REAL(w2f__8) DELTAS(1 :)
  INTENT(IN) DELTAS
  REAL(w2f__8) D2FDYZ_3D
!
!       **** Statements ****
!
  D2FDYZ_3D = ((((DELTAS(K)-DELTAS(K+1))*(F(K+(-1),I,J+1)-F(K+(-1),I,J+(-1))))/(&
     &(DELTAS(K)-DELTAS(K+(-1)))*(DELTAS(K+1)-DELTAS(K+(-1)))))+(((F(K,I,J+1)-F(&
     &K,I,J+(-1)))*(DELTAS(K+(-1))+DELTAS(K+1)-DELTAS(K)*2.0D00))/((DELTAS(K)-DE&
     &LTAS(K+(-1)))*(DELTAS(K+1)-DELTAS(K))))+(((DELTAS(K)-DELTAS(K+(-1)))*(F(K+&
     &1,I,J+1)-F(K+1,I,J+(-1))))/((DELTAS(K+1)-DELTAS(K))*(DELTAS(K+1)-DELTAS(K+&
     &(-1))))))*(5.0D-01/DELTA_Y)

  RETURN
  END FUNCTION

  Function D2FDZ2_3D_IRREGULAR(F, I, J, K, DELTAS)
  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :, 1 :)
  INTENT(IN) F
  INTEGER(w2f__i4) I
  INTENT(IN) I
  INTEGER(w2f__i4) J
  INTENT(IN) J
  INTEGER(w2f__i4) K
  INTENT(IN) K
  REAL(w2f__8) DELTAS(1 :)
  INTENT(IN) DELTAS
  REAL(w2f__8) D2FDZ2_3D_IRREGULAR
!
!       **** Local Variables and Functions ****
!
  REAL(w2f__8) ZKMINUSZKM1
  REAL(w2f__8) ZKMINUSZKP1
  REAL(w2f__8) ZKP1MINUSZK
  REAL(w2f__8) ZKP1MINUSZKM1
!
!       **** Statements ****
!
  ZKMINUSZKP1 = DELTAS(K)-DELTAS(K+1)
  ZKMINUSZKM1 = DELTAS(K)-DELTAS(K+(-1))
  ZKP1MINUSZKM1 = DELTAS(K+1)-DELTAS(K+(-1))
  ZKP1MINUSZK = - ZKMINUSZKP1
  D2FDZ2_3D_IRREGULAR = ((F(K+1,I,J)*2.0D00)/(ZKP1MINUSZK*ZKP1MINUSZKM1))+((F(K+&
     &(-1),I,J)*2.0D00)/(ZKMINUSZKM1*ZKP1MINUSZKM1))-((F(K,I,J)*2.0D00)/(ZKMINUS&
     &ZKM1*ZKP1MINUSZK))

  RETURN
  END FUNCTION
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_DFDX_2D(F, I, J, DELTA, DFDX_2D)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :)
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  REAL(w2f__8) DELTA
  REAL(w2f__8) DFDX_2D


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  DFDX_2D = (F(I + (-1), J) *((-5.0D-01) / DELTA) + F(I + 1, J) * (5.0D-01 / DEL&
     &TA))

    end if
    if (our_rev_mode%tape) then
! taping
  DFDX_2D = (F(I + (-1), J) *((-5.0D-01) / DELTA) + F(I + 1, J) * (5.0D-01 / DEL&
     &TA))

    end if
    if (our_rev_mode%adjoint) then
! adjoint
    end if
  end subroutine OAD_S_DFDX_2D
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_DFDY_2D(F, I, J, DELTA, DFDY_2D)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_235
  INTEGER(w2f__i8) OpenAD_Symbol_236
  INTEGER(w2f__i8) OpenAD_Symbol_237
  INTEGER(w2f__i8) OpenAD_Symbol_238
  INTEGER(w2f__i8) OpenAD_Symbol_239
  INTEGER(w2f__i8) OpenAD_Symbol_240
  INTEGER(w2f__i8) OpenAD_Symbol_241
  INTEGER(w2f__i8) OpenAD_Symbol_242
  INTEGER(w2f__i8) OpenAD_Symbol_243
  INTEGER(w2f__i8) OpenAD_Symbol_244
  INTEGER(w2f__i8) OpenAD_Symbol_245
  INTEGER(w2f__i8) OpenAD_Symbol_246
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :)
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  REAL(w2f__8) DELTA
  REAL(w2f__8) DFDY_2D
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) JM1
  INTEGER(w2f__i4) JP1


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  JP1 = (J + 1)
  JM1 = (J +(-1))
  IF(JP1 .eq.(SIZE(F, 2) + 1)) THEN
    JP1 = 2
  ENDIF
  IF(JM1 .eq. 0) THEN
    JM1 = (SIZE(F, 2) +(-1))
  ENDIF
  DFDY_2D = (F(I, J + (-1)) *((-5.0D-01) / DELTA) + F(I, J + 1) * (5.0D-01 / DEL&
     &TA))

    end if
    if (our_rev_mode%tape) then
! taping
  JP1 = (J + 1)
  JM1 = (J +(-1))
  IF(JP1 .eq.(SIZE(F, 2) + 1)) THEN
    JP1 = 2
    OpenAD_Symbol_237 = 1_w2f__i8
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_237
    oad_it_ptr = oad_it_ptr+1
  ELSE
    OpenAD_Symbol_238 = 0_w2f__i8
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_238
    oad_it_ptr = oad_it_ptr+1
  ENDIF
  IF(JM1 .eq. 0) THEN
    JM1 = (SIZE(F, 2) +(-1))
    OpenAD_Symbol_239 = 1_w2f__i8
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_239
    oad_it_ptr = oad_it_ptr+1
  ELSE
    OpenAD_Symbol_240 = 0_w2f__i8
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_240
    oad_it_ptr = oad_it_ptr+1
  ENDIF
  DFDY_2D = (F(I, J + (-1)) *((-5.0D-01) / DELTA) + F(I, J + 1) * (5.0D-01 / DEL&
     &TA))

    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_235 = oad_it(oad_it_ptr)
  IF(OpenAD_Symbol_235 .ne. 0) THEN
  ENDIF
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_236 = oad_it(oad_it_ptr)
  IF(OpenAD_Symbol_236 .ne. 0) THEN
  ENDIF
    end if
  end subroutine OAD_S_DFDY_2D
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_DFDX_2D_STAG(F, I, J, DELTA, DFDX_2D_STAG)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :)
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  REAL(w2f__8) DELTA
  REAL(w2f__8) DFDX_2D_STAG


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  DFDX_2D_STAG = ((F(I + 1, J) + F(I + 1, J + 1) - F(I, J) - F(I, J + 1)) /(DELT&
     &A * 2.0D00))

    end if
    if (our_rev_mode%tape) then
! taping
  DFDX_2D_STAG = ((F(I + 1, J) + F(I + 1, J + 1) - F(I, J) - F(I, J + 1)) /(DELT&
     &A * 2.0D00))

    end if
    if (our_rev_mode%adjoint) then
! adjoint
    end if
  end subroutine OAD_S_DFDX_2D_STAG
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_DFDY_2D_STAG(F, I, J, DELTA, DFDY_2D_STAG)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :)
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  REAL(w2f__8) DELTA
  REAL(w2f__8) DFDY_2D_STAG


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  DFDY_2D_STAG = ((F(I, J + 1) + F(I + 1, J + 1) - F(I, J) - F(I + 1, J)) /(DELT&
     &A * 2.0D00))

    end if
    if (our_rev_mode%tape) then
! taping
  DFDY_2D_STAG = ((F(I, J + 1) + F(I + 1, J + 1) - F(I, J) - F(I + 1, J)) /(DELT&
     &A * 2.0D00))

    end if
    if (our_rev_mode%adjoint) then
! adjoint
    end if
  end subroutine OAD_S_DFDY_2D_STAG
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_DFDX_2D_UPWIND(F, I, J, DELTA, DFDX_2D_UPWIND)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :)
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  REAL(w2f__8) DELTA
  REAL(w2f__8) DFDX_2D_UPWIND


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  DFDX_2D_UPWIND = ((F(I, J) * 1.5D00 + F(I + (-2), J) * 5.0D-01 - F(I + (-1), J&
     &) * 2.0D00) / DELTA)

    end if
    if (our_rev_mode%tape) then
! taping
  DFDX_2D_UPWIND = ((F(I, J) * 1.5D00 + F(I + (-2), J) * 5.0D-01 - F(I + (-1), J&
     &) * 2.0D00) / DELTA)

    end if
    if (our_rev_mode%adjoint) then
! adjoint
    end if
  end subroutine OAD_S_DFDX_2D_UPWIND
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_DFDY_2D_UPWIND(F, I, J, DELTA, DFDY_2D_UPWIND)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :)
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  REAL(w2f__8) DELTA
  REAL(w2f__8) DFDY_2D_UPWIND


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  DFDY_2D_UPWIND = ((F(I, J) * 1.5D00 + F(I, J + (-2)) * 5.0D-01 - F(I, J + (-1)&
     &) * 2.0D00) / DELTA)

    end if
    if (our_rev_mode%tape) then
! taping
  DFDY_2D_UPWIND = ((F(I, J) * 1.5D00 + F(I, J + (-2)) * 5.0D-01 - F(I, J + (-1)&
     &) * 2.0D00) / DELTA)

    end if
    if (our_rev_mode%adjoint) then
! adjoint
    end if
  end subroutine OAD_S_DFDY_2D_UPWIND
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_DFDX_2D_DOWNWIND(F, I, J, DELTA, DFDX_2D_DOWNWIND)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :)
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  REAL(w2f__8) DELTA
  REAL(w2f__8) DFDX_2D_DOWNWIND


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  DFDX_2D_DOWNWIND = ((F(I, J) *(-1.5D00) + F(I + 1, J) * 2.0D00 - F(I + 2, J) *&
     & 5.0D-01) / DELTA)

    end if
    if (our_rev_mode%tape) then
! taping
  DFDX_2D_DOWNWIND = ((F(I, J) *(-1.5D00) + F(I + 1, J) * 2.0D00 - F(I + 2, J) *&
     & 5.0D-01) / DELTA)

    end if
    if (our_rev_mode%adjoint) then
! adjoint
    end if
  end subroutine OAD_S_DFDX_2D_DOWNWIND
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_DFDY_2D_DOWNWIND(F, I, J, DELTA, DFDY_2D_DOWNWIND)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :)
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  REAL(w2f__8) DELTA
  REAL(w2f__8) DFDY_2D_DOWNWIND


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  DFDY_2D_DOWNWIND = ((F(I, J) *(-1.5D00) + F(I, J + 1) * 2.0D00 - F(I, J + 2) *&
     & 5.0D-01) / DELTA)

    end if
    if (our_rev_mode%tape) then
! taping
  DFDY_2D_DOWNWIND = ((F(I, J) *(-1.5D00) + F(I, J + 1) * 2.0D00 - F(I, J + 2) *&
     & 5.0D-01) / DELTA)

    end if
    if (our_rev_mode%adjoint) then
! adjoint
    end if
  end subroutine OAD_S_DFDY_2D_DOWNWIND
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_DFDX_3D(F, I, J, K, DELTA, DFDX_3D)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :, 1 :)
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  INTEGER(w2f__i4) K
  REAL(w2f__8) DELTA
  REAL(w2f__8) DFDX_3D


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  DFDX_3D = (F(K, I + (-1), J) *((-5.0D-01) / DELTA) + F(K, I + 1 , J) *(5.0D-01&
     & / DELTA))

    end if
    if (our_rev_mode%tape) then
! taping
  DFDX_3D = (F(K, I + (-1), J) *((-5.0D-01) / DELTA) + F(K, I + 1 , J) *(5.0D-01&
     & / DELTA))

    end if
    if (our_rev_mode%adjoint) then
! adjoint
    end if
  end subroutine OAD_S_DFDX_3D
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_DFDY_3D(F, I, J, K, DELTA, DFDY_3D)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :, 1 :)
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  INTEGER(w2f__i4) K
  REAL(w2f__8) DELTA
  REAL(w2f__8) DFDY_3D


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  DFDY_3D = (F(K, I, J + (-1)) *((-5.0D-01) / DELTA) + F(K, I, J + 1) *(5.0D-01 &
     &/ DELTA))

    end if
    if (our_rev_mode%tape) then
! taping
  DFDY_3D = (F(K, I, J + (-1)) *((-5.0D-01) / DELTA) + F(K, I, J + 1) *(5.0D-01 &
     &/ DELTA))

    end if
    if (our_rev_mode%adjoint) then
! adjoint
    end if
  end subroutine OAD_S_DFDY_3D
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_DFDZ_3D_IRREGULAR(F, I, J, K, DELTAS, DFDZ_3D_IRREGULAR)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :, 1 :)
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  INTEGER(w2f__i4) K
  REAL(w2f__8) DELTAS(1 :)
  REAL(w2f__8) DFDZ_3D_IRREGULAR


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  DFDZ_3D_IRREGULAR = (((F(K + (-1), I, J) *(DELTAS(K) - DELTAS(K + 1))) /((DELT&
     &AS(K) - DELTAS(K + (-1))) *(DELTAS(K + 1) - DELTAS(K + (-1))))) +((F(K, I,&
     & J) *(DELTAS(K + (-1)) + DELTAS(K + 1) - DELTAS(K) * 2.0D00)) /((DELTAS(K)&
     & - DELTAS(K + (-1))) *( DELTAS(K + 1) - DELTAS(K)))) +((F(K + 1, I, J) *(D&
     &ELTAS(K) - DELTAS(K + (-1)))) /((DELTAS(K + 1) - DELTAS(K)) *(DELTAS(K + 1&
     & ) - DELTAS(K + (-1))))))

    end if
    if (our_rev_mode%tape) then
! taping
  DFDZ_3D_IRREGULAR = (((F(K + (-1), I, J) *(DELTAS(K) - DELTAS(K + 1))) /((DELT&
     &AS(K) - DELTAS(K + (-1))) *(DELTAS(K + 1) - DELTAS(K + (-1))))) +((F(K, I,&
     & J) *(DELTAS(K + (-1)) + DELTAS(K + 1) - DELTAS(K) * 2.0D00)) /((DELTAS(K)&
     & - DELTAS(K + (-1))) *( DELTAS(K + 1) - DELTAS(K)))) +((F(K + 1, I, J) *(D&
     &ELTAS(K) - DELTAS(K + (-1)))) /((DELTAS(K + 1) - DELTAS(K)) *(DELTAS(K + 1&
     & ) - DELTAS(K + (-1))))))

    end if
    if (our_rev_mode%adjoint) then
! adjoint
    end if
  end subroutine OAD_S_DFDZ_3D_IRREGULAR
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_DFDZ_3D_UPWIND_IRREGULAR(F, I, J, K, DELTAS, DFDZ_3D_UPWIND_I&
     &RREGULAR)

    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :, 1 :)
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  INTEGER(w2f__i4) K
  REAL(w2f__8) DELTAS(1 :)
  REAL(w2f__8) DFDZ_3D_UPWIND_IRREGULAR
!
!       **** Local Variables and Functions ****
!
  REAL(w2f__8) ZKM1MINUSZKM2
  REAL(w2f__8) ZKMINUSZKM1
  REAL(w2f__8) ZKMINUSZKM2


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  ZKMINUSZKM1 = (DELTAS(K) - DELTAS(K + (-1)))
  ZKMINUSZKM2 = (DELTAS(K) - DELTAS(K + (-2)))
  ZKM1MINUSZKM2 = (DELTAS(K + (-1)) - DELTAS(K + (-2)))
  DFDZ_3D_UPWIND_IRREGULAR = (((F(K, I, J) *(DELTAS(K) * 2.0D00 - DELTAS(K + (-1&
     &)) - DELTAS(K + (-2)))) /(ZKMINUSZKM1 * ZKMINUSZKM2)) +((F(K + (-2), I, J)&
     & * ZKMINUSZKM1) /( ZKM1MINUSZKM2 * ZKMINUSZKM2)) -((F(K + (-1), I, J) * ZK&
     &MINUSZKM2 ) /(ZKM1MINUSZKM2 * ZKMINUSZKM1)))

    end if
    if (our_rev_mode%tape) then
! taping
  ZKMINUSZKM1 = (DELTAS(K) - DELTAS(K + (-1)))
  ZKMINUSZKM2 = (DELTAS(K) - DELTAS(K + (-2)))
  ZKM1MINUSZKM2 = (DELTAS(K + (-1)) - DELTAS(K + (-2)))
  DFDZ_3D_UPWIND_IRREGULAR = (((F(K, I, J) *(DELTAS(K) * 2.0D00 - DELTAS(K + (-1&
     &)) - DELTAS(K + (-2)))) /(ZKMINUSZKM1 * ZKMINUSZKM2)) +((F(K + (-2), I, J)&
     & * ZKMINUSZKM1) /( ZKM1MINUSZKM2 * ZKMINUSZKM2)) -((F(K + (-1), I, J) * ZK&
     &MINUSZKM2 ) /(ZKM1MINUSZKM2 * ZKMINUSZKM1)))

    end if
    if (our_rev_mode%adjoint) then
! adjoint
    end if
  end subroutine OAD_S_DFDZ_3D_UPWIND_IRREGULAR
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_DFDX_3D_STAG(F, I, J, K, DELTA, DFDX_3D_STAG)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :, 1 :)
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  INTEGER(w2f__i4) K
  REAL(w2f__8) DELTA
  REAL(w2f__8) DFDX_3D_STAG


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  DFDX_3D_STAG = ((F(K, I + 1, J) + F(K, I + 1, J + 1) - F(K, I, J) - F(K, I, J &
     &+ 1)) /(DELTA * 2.0D00))

    end if
    if (our_rev_mode%tape) then
! taping
  DFDX_3D_STAG = ((F(K, I + 1, J) + F(K, I + 1, J + 1) - F(K, I, J) - F(K, I, J &
     &+ 1)) /(DELTA * 2.0D00))

    end if
    if (our_rev_mode%adjoint) then
! adjoint
    end if
  end subroutine OAD_S_DFDX_3D_STAG
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_DFDY_3D_STAG(F, I, J, K, DELTA, DFDY_3D_STAG)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :, 1 :)
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  INTEGER(w2f__i4) K
  REAL(w2f__8) DELTA
  REAL(w2f__8) DFDY_3D_STAG


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  DFDY_3D_STAG = ((F(K, I, J + 1) + F(K, I + 1, J + 1) - F(K, I, J) - F(K, I + 1&
     &, J)) /(DELTA * 2.0D00))

    end if
    if (our_rev_mode%tape) then
! taping
  DFDY_3D_STAG = ((F(K, I, J + 1) + F(K, I + 1, J + 1) - F(K, I, J) - F(K, I + 1&
     &, J)) /(DELTA * 2.0D00))

    end if
    if (our_rev_mode%adjoint) then
! adjoint
    end if
  end subroutine OAD_S_DFDY_3D_STAG
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_DFDX_3D_UPWIND(F, I, J, K, DELTA, DFDX_3D_UPWIND)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :, 1 :)
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  INTEGER(w2f__i4) K
  REAL(w2f__8) DELTA
  REAL(w2f__8) DFDX_3D_UPWIND


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  DFDX_3D_UPWIND = ((F(K, I, J) * 1.5D00 + F(K, I + (-2), J) * 5.0D-01 - F(K, I &
     &+ (-1), J) * 2.0D00) / DELTA)

    end if
    if (our_rev_mode%tape) then
! taping
  DFDX_3D_UPWIND = ((F(K, I, J) * 1.5D00 + F(K, I + (-2), J) * 5.0D-01 - F(K, I &
     &+ (-1), J) * 2.0D00) / DELTA)

    end if
    if (our_rev_mode%adjoint) then
! adjoint
    end if
  end subroutine OAD_S_DFDX_3D_UPWIND
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_DFDY_3D_UPWIND(F, I, J, K, DELTA, DFDY_3D_UPWIND)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :, 1 :)
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  INTEGER(w2f__i4) K
  REAL(w2f__8) DELTA
  REAL(w2f__8) DFDY_3D_UPWIND


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  DFDY_3D_UPWIND = ((F(K, I, J) * 1.5D00 + F(K, I, J + (-2)) * 5.0D-01 - F(K, I,&
     & J + (-1)) * 2.0D00) / DELTA)

    end if
    if (our_rev_mode%tape) then
! taping
  DFDY_3D_UPWIND = ((F(K, I, J) * 1.5D00 + F(K, I, J + (-2)) * 5.0D-01 - F(K, I,&
     & J + (-1)) * 2.0D00) / DELTA)

    end if
    if (our_rev_mode%adjoint) then
! adjoint
    end if
  end subroutine OAD_S_DFDY_3D_UPWIND
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_DFDX_3D_DOWNWIND(F, I, J, K, DELTA, DFDX_3D_DOWNWIND)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :, 1 :)
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  INTEGER(w2f__i4) K
  REAL(w2f__8) DELTA
  REAL(w2f__8) DFDX_3D_DOWNWIND


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  DFDX_3D_DOWNWIND = ((F(K, I, J) *(-1.5D00) + F(K, I + 1, J) * 2.0D00 - F(K, I &
     &+ 2, J) * 5.0D-01) / DELTA)

    end if
    if (our_rev_mode%tape) then
! taping
  DFDX_3D_DOWNWIND = ((F(K, I, J) *(-1.5D00) + F(K, I + 1, J) * 2.0D00 - F(K, I &
     &+ 2, J) * 5.0D-01) / DELTA)

    end if
    if (our_rev_mode%adjoint) then
! adjoint
    end if
  end subroutine OAD_S_DFDX_3D_DOWNWIND
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_DFDY_3D_DOWNWIND(F, I, J, K, DELTA, DFDY_3D_DOWNWIND)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :, 1 :)
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  INTEGER(w2f__i4) K
  REAL(w2f__8) DELTA
  REAL(w2f__8) DFDY_3D_DOWNWIND


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  DFDY_3D_DOWNWIND = ((F(K, I, J) *(-1.5D00) + F(K, I, J + 1) * 2.0D00 - F(K, I,&
     & J + 2) * 5.0D-01) / DELTA)

    end if
    if (our_rev_mode%tape) then
! taping
  DFDY_3D_DOWNWIND = ((F(K, I, J) *(-1.5D00) + F(K, I, J + 1) * 2.0D00 - F(K, I,&
     & J + 2) * 5.0D-01) / DELTA)

    end if
    if (our_rev_mode%adjoint) then
! adjoint
    end if
  end subroutine OAD_S_DFDY_3D_DOWNWIND
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_D2FDX2_2D(F, I, J, DELTA, D2FDX2_2D)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :)
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  REAL(w2f__8) DELTA
  REAL(w2f__8) D2FDX2_2D


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  D2FDX2_2D = ((F(I + 1, J) + F(I + (-1), J) - F(I, J) * 2.0D00) /(DELTA * DELTA&
     &))

    end if
    if (our_rev_mode%tape) then
! taping
  D2FDX2_2D = ((F(I + 1, J) + F(I + (-1), J) - F(I, J) * 2.0D00) /(DELTA * DELTA&
     &))

    end if
    if (our_rev_mode%adjoint) then
! adjoint
    end if
  end subroutine OAD_S_D2FDX2_2D
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_D2FDX2_2D_DOWNWIND(F, I, J, DELTA, D2FDX2_2D_DOWNWIND)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :)
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  REAL(w2f__8) DELTA
  REAL(w2f__8) D2FDX2_2D_DOWNWIND


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  D2FDX2_2D_DOWNWIND = ((F(I + 2, J) * 5.0D00 + F(I, J) * 3.0D00 - F(I + 1, J) *&
     & 7.0D00 - F(I + 3, J)) /((DELTA ** 2) * 2.0D00))

    end if
    if (our_rev_mode%tape) then
! taping
  D2FDX2_2D_DOWNWIND = ((F(I + 2, J) * 5.0D00 + F(I, J) * 3.0D00 - F(I + 1, J) *&
     & 7.0D00 - F(I + 3, J)) /((DELTA ** 2) * 2.0D00))

    end if
    if (our_rev_mode%adjoint) then
! adjoint
    end if
  end subroutine OAD_S_D2FDX2_2D_DOWNWIND
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_D2FDX2_2D_UPWIND(F, I, J, DELTA, D2FDX2_2D_UPWIND)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :)
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  REAL(w2f__8) DELTA
  REAL(w2f__8) D2FDX2_2D_UPWIND


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  D2FDX2_2D_UPWIND = ((F(I + (-2), J) * 5.0D00 + F(I, J) * 3.0D00 - F(I + (-1), &
     &J) * 7.0D00 - F(I + (-3), J)) /((DELTA ** 2) * 2.0D00))

    end if
    if (our_rev_mode%tape) then
! taping
  D2FDX2_2D_UPWIND = ((F(I + (-2), J) * 5.0D00 + F(I, J) * 3.0D00 - F(I + (-1), &
     &J) * 7.0D00 - F(I + (-3), J)) /((DELTA ** 2) * 2.0D00))

    end if
    if (our_rev_mode%adjoint) then
! adjoint
    end if
  end subroutine OAD_S_D2FDX2_2D_UPWIND
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_D2FDY2_2D_DOWNWIND(F, I, J, DELTA, D2FDY2_2D_DOWNWIND)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :)
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  REAL(w2f__8) DELTA
  REAL(w2f__8) D2FDY2_2D_DOWNWIND


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  D2FDY2_2D_DOWNWIND = ((F(I, J + 2) * 5.0D00 + F(I, J) * 3.0D00 - F(I, J + 1) *&
     & 7.0D00 - F(I, J + 3)) /((DELTA ** 2) * 2.0D00))

    end if
    if (our_rev_mode%tape) then
! taping
  D2FDY2_2D_DOWNWIND = ((F(I, J + 2) * 5.0D00 + F(I, J) * 3.0D00 - F(I, J + 1) *&
     & 7.0D00 - F(I, J + 3)) /((DELTA ** 2) * 2.0D00))

    end if
    if (our_rev_mode%adjoint) then
! adjoint
    end if
  end subroutine OAD_S_D2FDY2_2D_DOWNWIND
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_D2FDY2_2D_UPWIND(F, I, J, DELTA, D2FDY2_2D_UPWIND)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :)
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  REAL(w2f__8) DELTA
  REAL(w2f__8) D2FDY2_2D_UPWIND


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  D2FDY2_2D_UPWIND = ((F(I, J + (-2)) * 5.0D00 + F(I, J) * 3.0D00 - F(I, J + (-1&
     &)) * 7.0D00 - F(I, J + (-3))) /((DELTA ** 2) * 2.0D00))

    end if
    if (our_rev_mode%tape) then
! taping
  D2FDY2_2D_UPWIND = ((F(I, J + (-2)) * 5.0D00 + F(I, J) * 3.0D00 - F(I, J + (-1&
     &)) * 7.0D00 - F(I, J + (-3))) /((DELTA ** 2) * 2.0D00))

    end if
    if (our_rev_mode%adjoint) then
! adjoint
    end if
  end subroutine OAD_S_D2FDY2_2D_UPWIND
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_D2FDY2_2D(F, I, J, DELTA, D2FDY2_2D)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :)
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  REAL(w2f__8) DELTA
  REAL(w2f__8) D2FDY2_2D


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  D2FDY2_2D = ((F(I, J + (-1)) + F(I, J + 1) - F(I, J) * 2.0D00) /(DELTA * DELTA&
     &))

    end if
    if (our_rev_mode%tape) then
! taping
  D2FDY2_2D = ((F(I, J + (-1)) + F(I, J + 1) - F(I, J) * 2.0D00) /(DELTA * DELTA&
     &))

    end if
    if (our_rev_mode%adjoint) then
! adjoint
    end if
  end subroutine OAD_S_D2FDY2_2D
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_D2FDXY_3D(F, I, J, K, DELTA_X, DELTA_Y, D2FDXY_3D)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :, 1 :)
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  INTEGER(w2f__i4) K
  REAL(w2f__8) DELTA_X
  REAL(w2f__8) DELTA_Y
  REAL(w2f__8) D2FDXY_3D


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  D2FDXY_3D = ((F(K, I + 1, J + 1) + F(K, I + (-1), J + (-1)) - F (K, I + (-1), &
     &J + 1) - F(K, I + 1, J + (-1))) /(DELTA_Y * DELTA_X * 4.0D00))

    end if
    if (our_rev_mode%tape) then
! taping
  D2FDXY_3D = ((F(K, I + 1, J + 1) + F(K, I + (-1), J + (-1)) - F (K, I + (-1), &
     &J + 1) - F(K, I + 1, J + (-1))) /(DELTA_Y * DELTA_X * 4.0D00))

    end if
    if (our_rev_mode%adjoint) then
! adjoint
    end if
  end subroutine OAD_S_D2FDXY_3D
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_D2FDXZ_3D(F, I, J, K, DELTA_X, DELTAS, D2FDXZ_3D)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :, 1 :)
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  INTEGER(w2f__i4) K
  REAL(w2f__8) DELTA_X
  REAL(w2f__8) DELTAS(1 :)
  REAL(w2f__8) D2FDXZ_3D


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  D2FDXZ_3D = (((((DELTAS(K) - DELTAS(K + 1)) *(F(K + (-1), I + 1 , J) - F(K + (&
     &-1), I + (-1), J))) /((DELTAS(K) - DELTAS(K + (-1) )) *(DELTAS(K + 1) - DE&
     &LTAS(K + (-1))))) +(((F(K, I + 1, J) - F( K, I + (-1), J)) *(DELTAS(K + (-&
     &1)) + DELTAS(K + 1) - DELTAS(K) * 2.0D00)) /((DELTAS(K) - DELTAS(K + (-1))&
     &) *(DELTAS(K + 1) - DELTAS(K)))) +(((DELTAS(K) - DELTAS(K + (-1))) *(F(K +&
     & 1, I + 1 , J) - F(K + 1, I + (-1), J))) /((DELTAS(K + 1) - DELTAS(K)) *( &
     &DELTAS(K + 1) - DELTAS(K + (-1)))))) *(5.0D-01 / DELTA_X))

    end if
    if (our_rev_mode%tape) then
! taping
  D2FDXZ_3D = (((((DELTAS(K) - DELTAS(K + 1)) *(F(K + (-1), I + 1 , J) - F(K + (&
     &-1), I + (-1), J))) /((DELTAS(K) - DELTAS(K + (-1) )) *(DELTAS(K + 1) - DE&
     &LTAS(K + (-1))))) +(((F(K, I + 1, J) - F( K, I + (-1), J)) *(DELTAS(K + (-&
     &1)) + DELTAS(K + 1) - DELTAS(K) * 2.0D00)) /((DELTAS(K) - DELTAS(K + (-1))&
     &) *(DELTAS(K + 1) - DELTAS(K)))) +(((DELTAS(K) - DELTAS(K + (-1))) *(F(K +&
     & 1, I + 1 , J) - F(K + 1, I + (-1), J))) /((DELTAS(K + 1) - DELTAS(K)) *( &
     &DELTAS(K + 1) - DELTAS(K + (-1)))))) *(5.0D-01 / DELTA_X))

    end if
    if (our_rev_mode%adjoint) then
! adjoint
    end if
  end subroutine OAD_S_D2FDXZ_3D
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_D2FDYZ_3D(F, I, J, K, DELTA_Y, DELTAS, D2FDYZ_3D)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :, 1 :)
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  INTEGER(w2f__i4) K
  REAL(w2f__8) DELTA_Y
  REAL(w2f__8) DELTAS(1 :)
  REAL(w2f__8) D2FDYZ_3D


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  D2FDYZ_3D = (((((DELTAS(K) - DELTAS(K + 1)) *(F(K + (-1), I, J + 1) - F(K + (-&
     &1), I, J + (-1)))) /((DELTAS(K) - DELTAS(K + (-1 ))) *(DELTAS(K + 1) - DEL&
     &TAS(K + (-1))))) +(((F(K, I, J + 1) - F (K, I, J + (-1))) *(DELTAS(K + (-1&
     &)) + DELTAS(K + 1) - DELTAS(K) * 2.0D00)) /((DELTAS(K) - DELTAS(K + (-1)))&
     & *(DELTAS(K + 1) - DELTAS(K)))) +(((DELTAS(K) - DELTAS(K + (-1))) *(F(K + &
     &1, I, J + 1) - F(K + 1, I, J + (-1)))) /((DELTAS(K + 1) - DELTAS(K)) *( DE&
     &LTAS(K + 1) - DELTAS(K + (-1)))))) *(5.0D-01 / DELTA_Y))

    end if
    if (our_rev_mode%tape) then
! taping
  D2FDYZ_3D = (((((DELTAS(K) - DELTAS(K + 1)) *(F(K + (-1), I, J + 1) - F(K + (-&
     &1), I, J + (-1)))) /((DELTAS(K) - DELTAS(K + (-1 ))) *(DELTAS(K + 1) - DEL&
     &TAS(K + (-1))))) +(((F(K, I, J + 1) - F (K, I, J + (-1))) *(DELTAS(K + (-1&
     &)) + DELTAS(K + 1) - DELTAS(K) * 2.0D00)) /((DELTAS(K) - DELTAS(K + (-1)))&
     & *(DELTAS(K + 1) - DELTAS(K)))) +(((DELTAS(K) - DELTAS(K + (-1))) *(F(K + &
     &1, I, J + 1) - F(K + 1, I, J + (-1)))) /((DELTAS(K + 1) - DELTAS(K)) *( DE&
     &LTAS(K + 1) - DELTAS(K + (-1)))))) *(5.0D-01 / DELTA_Y))

    end if
    if (our_rev_mode%adjoint) then
! adjoint
    end if
  end subroutine OAD_S_D2FDYZ_3D
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_D2FDZ2_3D_IRREGULAR(F, I, J, K, DELTAS, D2FDZ2_3D_IRREGULAR)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) F(1 :, 1 :, 1 :)
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  INTEGER(w2f__i4) K
  REAL(w2f__8) DELTAS(1 :)
  REAL(w2f__8) D2FDZ2_3D_IRREGULAR
!
!       **** Local Variables and Functions ****
!
  REAL(w2f__8) ZKMINUSZKM1
  REAL(w2f__8) ZKMINUSZKP1
  REAL(w2f__8) ZKP1MINUSZK
  REAL(w2f__8) ZKP1MINUSZKM1


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  ZKMINUSZKP1 = (DELTAS(K) - DELTAS(K + 1))
  ZKMINUSZKM1 = (DELTAS(K) - DELTAS(K + (-1)))
  ZKP1MINUSZKM1 = (DELTAS(K + 1) - DELTAS(K + (-1)))
  ZKP1MINUSZK = (- ZKMINUSZKP1)
  D2FDZ2_3D_IRREGULAR = (((F(K + 1, I, J) * 2.0D00) /(ZKP1MINUSZK * ZKP1MINUSZKM&
     &1)) +((F(K + (-1), I, J) * 2.0D00) /(ZKMINUSZKM1 * ZKP1MINUSZKM1)) -((F(K,&
     & I, J) * 2.0D00) /(ZKMINUSZKM1 * ZKP1MINUSZK)))

    end if
    if (our_rev_mode%tape) then
! taping
  ZKMINUSZKP1 = (DELTAS(K) - DELTAS(K + 1))
  ZKMINUSZKM1 = (DELTAS(K) - DELTAS(K + (-1)))
  ZKP1MINUSZKM1 = (DELTAS(K + 1) - DELTAS(K + (-1)))
  ZKP1MINUSZK = (- ZKMINUSZKP1)
  D2FDZ2_3D_IRREGULAR = (((F(K + 1, I, J) * 2.0D00) /(ZKP1MINUSZK * ZKP1MINUSZKM&
     &1)) +((F(K + (-1), I, J) * 2.0D00) /(ZKMINUSZKM1 * ZKP1MINUSZKM1)) -((F(K,&
     & I, J) * 2.0D00) /(ZKMINUSZKM1 * ZKP1MINUSZK)))

    end if
    if (our_rev_mode%adjoint) then
! adjoint
    end if
  end subroutine OAD_S_D2FDZ2_3D_IRREGULAR
END

!$OPENAD XXX File_start [glimmer_pmpt.F90]
MODULE glimmer_pmpt
use OAD_active
use w2f__types
use oad_intrinsics
use glimmer_global ,only: DP
IMPLICIT NONE
SAVE
!
!     **** Statements ****
!
CONTAINS
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE CORRPMPT(TEMP, THCK, BWAT, SIGMA, NZ)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  use glimmer_global ,only: DP
  use glimmer_global ,only: DP
  use glimmer_global ,only: DP
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_247
  INTEGER(w2f__i8) OpenAD_Symbol_248
  INTEGER(w2f__i8) OpenAD_Symbol_249
  INTEGER(w2f__i8) OpenAD_Symbol_250
  INTEGER(w2f__i8) OpenAD_Symbol_251
  INTEGER(w2f__i8) OpenAD_Symbol_252
!
!       **** Parameters and Result ****
!
  type(active) :: THCK
  REAL(w2f__8) BWAT
  INTEGER(w2f__i4) NZ
  type(active) :: TEMP(1:NZ)
  REAL(w2f__8) SIGMA(1 : NZ)
!
!       **** Local Variables and Functions ****
!
  type(active) :: OAD_CTMP0(1:NZ)
  type(active) :: PMPTEMP(1:NZ)
  INTEGER(w2f__i4) OpenAD_Symbol_1420
  INTEGER(w2f__i4) OpenAD_Symbol_1421


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  CALL CALCPMPT(PMPTEMP,THCK,SIGMA)
  CALL OAD_S_MIN_D_11(TEMP,PMPTEMP,OAD_CTMP0)
  TEMP(1:INT(NZ))%v = OAD_CTMP0(1:NZ)%v
  IF (BWAT.GT.0.0D00) THEN
    TEMP(INT(NZ))%v = PMPTEMP(NZ)%v
  ENDIF
    end if
    if (our_rev_mode%tape) then
! taping
  CALL CALCPMPT(PMPTEMP,THCK,SIGMA)
  CALL OAD_S_MIN_D_11(TEMP,PMPTEMP,OAD_CTMP0)
  TEMP(1:INT(NZ))%v = OAD_CTMP0(1:NZ)%v
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = NZ
  oad_it_ptr = oad_it_ptr+1
  IF (BWAT.GT.0.0D00) THEN
    TEMP(INT(NZ))%v = PMPTEMP(NZ)%v
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = NZ
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_248 = 1_w2f__i8
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_248
    oad_it_ptr = oad_it_ptr+1
  ELSE
    OpenAD_Symbol_249 = 0_w2f__i8
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_249
    oad_it_ptr = oad_it_ptr+1
  ENDIF
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_247 = oad_it(oad_it_ptr)
  IF (OpenAD_Symbol_247.ne.0) THEN
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1421 = oad_it(oad_it_ptr)
    PMPTEMP(OpenAD_Symbol_1421)%d = PMPTEMP(OpenAD_Symbol_1421)%d+TEMP(OpenAD_Sy&
     &mbol_1421)%d

    TEMP(OpenAD_Symbol_1421)%d = 0.0d0
  ENDIF
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1420 = oad_it(oad_it_ptr)
  OAD_CTMP0(1:OpenAD_Symbol_1420)%d = OAD_CTMP0(1:OpenAD_Symbol_1420)%d+TEMP(1:O&
     &penAD_Symbol_1420)%d

  TEMP(1:OpenAD_Symbol_1420)%d = 0.0d0
  CALL OAD_S_MIN_D_11(TEMP,PMPTEMP,OAD_CTMP0)
  CALL CALCPMPT(PMPTEMP,THCK,SIGMA)
    end if
  end subroutine CORRPMPT
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE CALCPMPT(PMPTEMP, THCK, SIGMA)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  use glimmer_global ,only: DP
  use physcon ,only: GRAV , PMLT , RHOI
  use glimmer_global ,only: DP
  use physcon ,only: GRAV , PMLT , RHOI
  use glimmer_global ,only: DP
  use physcon ,only: GRAV , PMLT , RHOI
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_253
  INTEGER(w2f__i8) OpenAD_Symbol_254
  INTEGER(w2f__i8) OpenAD_Symbol_255
  INTEGER(w2f__i8) OpenAD_Symbol_256
  INTEGER(w2f__i8) OpenAD_Symbol_257
  INTEGER(w2f__i8) OpenAD_Symbol_258
!
!       **** Parameters and Result ****
!
  type(active) :: PMPTEMP(1:)
  type(active) :: THCK
  REAL(w2f__8) SIGMA(1 :)
!
!       **** Local Variables and Functions ****
!
  REAL(w2f__8) FACT
  PARAMETER ( FACT = -8.69999457599999982335D-04)
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) NZ
  INTEGER(w2f__i4) OpenAD_Symbol_1422
  REAL(w2f__8) OpenAD_Symbol_1423
  REAL(w2f__8) OpenAD_acc_8
  REAL(w2f__8) OpenAD_aux_9
  REAL(w2f__8) OpenAD_lin_19


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  NZ = SIZE(SIGMA)
  DO I = 1, NZ, 1
    PMPTEMP(INT(I))%v = (SIGMA(I)*THCK%v*(-8.69999457599999982335D-04))
  END DO
    end if
    if (our_rev_mode%tape) then
! taping
  NZ = SIZE(SIGMA)
  OpenAD_Symbol_255 = 0_w2f__i8
  DO I = 1,NZ,1
    OpenAD_aux_9 = (THCK%v*(-8.69999457599999982335D-04))
    OpenAD_lin_19 = SIGMA(I)
    PMPTEMP(INT(I))%v = (SIGMA(I)*OpenAD_aux_9)
    OpenAD_acc_8 = ((-8.69999457599999982335D-04)*OpenAD_lin_19)
    if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
    oad_dt(oad_dt_ptr) = OpenAD_acc_8
    oad_dt_ptr = oad_dt_ptr+1
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = I
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_255 = (INT(OpenAD_Symbol_255)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_255
  oad_it_ptr = oad_it_ptr+1
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_253 = oad_it(oad_it_ptr)
  OpenAD_Symbol_254 = 1
  do while (INT(OpenAD_Symbol_254).LE.INT(OpenAD_Symbol_253))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1422 = oad_it(oad_it_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1423 = oad_dt(oad_dt_ptr)
    THCK%d = THCK%d+PMPTEMP(OpenAD_Symbol_1422)%d*(OpenAD_Symbol_1423)
    PMPTEMP(OpenAD_Symbol_1422)%d = 0.0d0
    OpenAD_Symbol_254 = INT(OpenAD_Symbol_254)+1
  END DO
    end if
  end subroutine CALCPMPT
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE CALCBPMP(THCK, BPMP)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  use glimmer_global ,only: DP
  use glimmer_global ,only: DP
  use glimmer_global ,only: DP
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_259
  INTEGER(w2f__i8) OpenAD_Symbol_260
  INTEGER(w2f__i8) OpenAD_Symbol_261
  INTEGER(w2f__i8) OpenAD_Symbol_262
  INTEGER(w2f__i8) OpenAD_Symbol_263
  INTEGER(w2f__i8) OpenAD_Symbol_264
  INTEGER(w2f__i8) OpenAD_Symbol_265
  INTEGER(w2f__i8) OpenAD_Symbol_266
  INTEGER(w2f__i8) OpenAD_Symbol_267
  INTEGER(w2f__i8) OpenAD_Symbol_268
  INTEGER(w2f__i8) OpenAD_Symbol_269
  INTEGER(w2f__i8) OpenAD_Symbol_270
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) THCK(1 :, 1 :)
  REAL(w2f__8) BPMP(1 :, 1 :)
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) EW
  INTEGER(w2f__i4) EWN
  INTEGER(w2f__i4) NS
  INTEGER(w2f__i4) NSN
  type(active) :: OpenAD_tyc_0
  type(active) :: OpenAD_tyc_1
  type(active) :: OpenAD_tyc_57
  type(active) :: OpenAD_tyc_58


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  EWN = SIZE(THCK, 1)
  NSN = SIZE(THCK, 2)
  BPMP(1 : , 1 : ) = 0.0D00
  DO NS = 2, (NSN +(-1)), 1
    DO EW = 2, (EWN +(-1)), 1
!!! requested inline of 'oad_convert' has no defn
      CALL oad_convert(OpenAD_tyc_1,THCK(EW,NS))
      CALL CALCPMPTB(OpenAD_tyc_0,OpenAD_tyc_1)
!!! requested inline of 'oad_convert' has no defn
      CALL oad_convert(BPMP(EW,NS),OpenAD_tyc_0)
    END DO
  END DO
    end if
    if (our_rev_mode%tape) then
! taping
  EWN = SIZE(THCK,1)
  NSN = SIZE(THCK,2)
  BPMP(1:,1:) = 0.0D00
  OpenAD_Symbol_263 = 0_w2f__i8
  DO NS = 2,(NSN+(-1)),1
    OpenAD_Symbol_264 = 0_w2f__i8
    DO EW = 2,(EWN+(-1)),1
!!! requested inline of 'oad_convert' has no defn
      CALL oad_convert(OpenAD_tyc_1,THCK(EW,NS))
      CALL CALCPMPTB(OpenAD_tyc_0,OpenAD_tyc_1)
!!! requested inline of 'oad_convert' has no defn
      CALL oad_convert(BPMP(EW,NS),OpenAD_tyc_0)
      OpenAD_Symbol_264 = (INT(OpenAD_Symbol_264)+INT(1_w2f__i8))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_264
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_263 = (INT(OpenAD_Symbol_263)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_263
  oad_it_ptr = oad_it_ptr+1
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_259 = oad_it(oad_it_ptr)
  OpenAD_Symbol_260 = 1
  do while (INT(OpenAD_Symbol_260).LE.INT(OpenAD_Symbol_259))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_261 = oad_it(oad_it_ptr)
    OpenAD_Symbol_262 = 1
    do while (INT(OpenAD_Symbol_262).LE.INT(OpenAD_Symbol_261))
      CALL CALCPMPTB(OpenAD_tyc_57,OpenAD_tyc_58)
      OpenAD_Symbol_262 = INT(OpenAD_Symbol_262)+1
    END DO
    OpenAD_Symbol_260 = INT(OpenAD_Symbol_260)+1
  END DO
    end if
  end subroutine CALCBPMP
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE CALCPMPTB(PMPTEMP, THCK)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  use glimmer_global ,only: DP
  use physcon ,only: GRAV , PMLT , RHOI
  use glimmer_global ,only: DP
  use physcon ,only: GRAV , PMLT , RHOI
  use glimmer_global ,only: DP
  use physcon ,only: GRAV , PMLT , RHOI
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  type(active) :: PMPTEMP
  type(active) :: THCK
!
!       **** Local Variables and Functions ****
!
  REAL(w2f__8) FACT
  PARAMETER ( FACT = -8.69999457599999982335D-04)
!
!       **** Statements ****
!


    integer iaddr
    external iaddr

   if (our_rev_mode%plain) then
! original function
  PMPTEMP%v = (THCK%v*(-8.69999457599999982335D-04))
    end if
    if (our_rev_mode%tape) then
! taping
  PMPTEMP%v = (THCK%v*(-8.69999457599999982335D-04))
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  THCK%d = THCK%d+PMPTEMP%d*(-8.69999457599999982335D-04)
  PMPTEMP%d = 0.0d0
    end if
  end subroutine CALCPMPTB
END

!$OPENAD XXX File_start [glide_tempFullSoln.F90]
MODULE glide_tempfullsoln
use OAD_active
use w2f__types
use oad_intrinsics
use glimmer_global ,only: DP
IMPLICIT NONE
SAVE
!
!     **** Statements ****
!
CONTAINS
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE TSTEP_TEMPFULLSOLN(NX, NY, NZ, DX, DY, SIGMA, TEMP, ARTM, THCK, USR&
     &F, UVEL, VVEL, UBAS, VBAS, WVEL, WGRD, FLWA, BHEATFLX, BWAT, BMLT, DT, THK&
     &LIM, FLOATMASK)

    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  use glimmer_utils
  use glimmer_deriv ,only: DF_FIELD_2D_STAGGERED
  use glimmer_global ,only: DP
  use glimmer_pmpt ,only: CORRPMPT
  use physcon ,only: CONI , GN , GRAV , LHCI , RHOI , SCYR , SHCI
  use glimmer_utils
  use glimmer_deriv ,only: DF_FIELD_2D_STAGGERED
  use glimmer_global ,only: DP
  use glimmer_pmpt ,only: CORRPMPT
  use physcon ,only: CONI , GN , GRAV , LHCI , RHOI , SCYR , SHCI
  use glimmer_utils
  use glimmer_deriv ,only: DF_FIELD_2D_STAGGERED
  use glimmer_global ,only: DP
  use glimmer_pmpt ,only: CORRPMPT
  use physcon ,only: CONI , GN , GRAV , LHCI , RHOI , SCYR , SHCI
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_271
  INTEGER(w2f__i8) OpenAD_Symbol_272
  INTEGER(w2f__i8) OpenAD_Symbol_273
  INTEGER(w2f__i8) OpenAD_Symbol_274
  INTEGER(w2f__i8) OpenAD_Symbol_275
  INTEGER(w2f__i8) OpenAD_Symbol_276
  INTEGER(w2f__i8) OpenAD_Symbol_277
  INTEGER(w2f__i8) OpenAD_Symbol_278
  INTEGER(w2f__i8) OpenAD_Symbol_279
  INTEGER(w2f__i8) OpenAD_Symbol_280
  INTEGER(w2f__i8) OpenAD_Symbol_281
  INTEGER(w2f__i8) OpenAD_Symbol_282
  INTEGER(w2f__i8) OpenAD_Symbol_283
  INTEGER(w2f__i8) OpenAD_Symbol_284
  INTEGER(w2f__i8) OpenAD_Symbol_285
  INTEGER(w2f__i8) OpenAD_Symbol_286
  INTEGER(w2f__i8) OpenAD_Symbol_287
  INTEGER(w2f__i8) OpenAD_Symbol_288
  INTEGER(w2f__i8) OpenAD_Symbol_289
  INTEGER(w2f__i8) OpenAD_Symbol_290
  INTEGER(w2f__i8) OpenAD_Symbol_291
  INTEGER(w2f__i8) OpenAD_Symbol_292
  INTEGER(w2f__i8) OpenAD_Symbol_293
  INTEGER(w2f__i8) OpenAD_Symbol_294
  INTEGER(w2f__i8) OpenAD_Symbol_295
  INTEGER(w2f__i8) OpenAD_Symbol_296
  INTEGER(w2f__i8) OpenAD_Symbol_297
  INTEGER(w2f__i8) OpenAD_Symbol_298
  INTEGER(w2f__i8) OpenAD_Symbol_299
  INTEGER(w2f__i8) OpenAD_Symbol_300
  INTEGER(w2f__i8) OpenAD_Symbol_301
  INTEGER(w2f__i8) OpenAD_Symbol_302
  INTEGER(w2f__i8) OpenAD_Symbol_303
  INTEGER(w2f__i8) OpenAD_Symbol_304
  INTEGER(w2f__i8) OpenAD_Symbol_305
  INTEGER(w2f__i8) OpenAD_Symbol_306
  INTEGER(w2f__i8) OpenAD_Symbol_307
  INTEGER(w2f__i8) OpenAD_Symbol_308
  INTEGER(w2f__i8) OpenAD_Symbol_309
  INTEGER(w2f__i8) OpenAD_Symbol_310
  INTEGER(w2f__i8) OpenAD_Symbol_311
  INTEGER(w2f__i8) OpenAD_Symbol_312
  INTEGER(w2f__i8) OpenAD_Symbol_313
  INTEGER(w2f__i8) OpenAD_Symbol_314
  INTEGER(w2f__i8) OpenAD_Symbol_315
  INTEGER(w2f__i8) OpenAD_Symbol_316
  INTEGER(w2f__i8) OpenAD_Symbol_317
  INTEGER(w2f__i8) OpenAD_Symbol_318
  INTEGER(w2f__i8) OpenAD_Symbol_319
  INTEGER(w2f__i8) OpenAD_Symbol_320
  INTEGER(w2f__i8) OpenAD_Symbol_321
  INTEGER(w2f__i8) OpenAD_Symbol_322
  INTEGER(w2f__i8) OpenAD_Symbol_323
  INTEGER(w2f__i8) OpenAD_Symbol_324
  INTEGER(w2f__i8) OpenAD_Symbol_325
  INTEGER(w2f__i8) OpenAD_Symbol_326
  INTEGER(w2f__i8) OpenAD_Symbol_327
  INTEGER(w2f__i8) OpenAD_Symbol_328
  INTEGER(w2f__i8) OpenAD_Symbol_329
  INTEGER(w2f__i8) OpenAD_Symbol_330
  INTEGER(w2f__i8) OpenAD_Symbol_331
  INTEGER(w2f__i8) OpenAD_Symbol_332
  INTEGER(w2f__i8) OpenAD_Symbol_333
  INTEGER(w2f__i8) OpenAD_Symbol_334
  INTEGER(w2f__i8) OpenAD_Symbol_335
  INTEGER(w2f__i8) OpenAD_Symbol_336
  INTEGER(w2f__i8) OpenAD_Symbol_337
  INTEGER(w2f__i8) OpenAD_Symbol_338
  INTEGER(w2f__i8) OpenAD_Symbol_339
  INTEGER(w2f__i8) OpenAD_Symbol_340
  INTEGER(w2f__i8) OpenAD_Symbol_341
  INTEGER(w2f__i8) OpenAD_Symbol_342
  INTEGER(w2f__i8) OpenAD_Symbol_343
  INTEGER(w2f__i8) OpenAD_Symbol_344
  INTEGER(w2f__i8) OpenAD_Symbol_345
  INTEGER(w2f__i8) OpenAD_Symbol_346
  INTEGER(w2f__i8) OpenAD_Symbol_347
  INTEGER(w2f__i8) OpenAD_Symbol_348
  INTEGER(w2f__i8) OpenAD_Symbol_349
  INTEGER(w2f__i8) OpenAD_Symbol_350
  INTEGER(w2f__i8) OpenAD_Symbol_351
  INTEGER(w2f__i8) OpenAD_Symbol_352
  INTEGER(w2f__i8) OpenAD_Symbol_353
  INTEGER(w2f__i8) OpenAD_Symbol_354
  INTEGER(w2f__i8) OpenAD_Symbol_355
  INTEGER(w2f__i8) OpenAD_Symbol_356
  INTEGER(w2f__i8) OpenAD_Symbol_357
  INTEGER(w2f__i8) OpenAD_Symbol_358
  INTEGER(w2f__i8) OpenAD_Symbol_359
  INTEGER(w2f__i8) OpenAD_Symbol_360
  INTEGER(w2f__i8) OpenAD_Symbol_361
  INTEGER(w2f__i8) OpenAD_Symbol_362
  INTEGER(w2f__i8) OpenAD_Symbol_363
  INTEGER(w2f__i8) OpenAD_Symbol_364
  INTEGER(w2f__i8) OpenAD_Symbol_365
  INTEGER(w2f__i8) OpenAD_Symbol_366
  INTEGER(w2f__i8) OpenAD_Symbol_367
  INTEGER(w2f__i8) OpenAD_Symbol_368
  INTEGER(w2f__i8) OpenAD_Symbol_369
  INTEGER(w2f__i8) OpenAD_Symbol_370
  INTEGER(w2f__i8) OpenAD_Symbol_371
  INTEGER(w2f__i8) OpenAD_Symbol_372
  INTEGER(w2f__i8) OpenAD_Symbol_373
  INTEGER(w2f__i8) OpenAD_Symbol_374
  INTEGER(w2f__i8) OpenAD_Symbol_375
  INTEGER(w2f__i8) OpenAD_Symbol_376
  INTEGER(w2f__i8) OpenAD_Symbol_377
  INTEGER(w2f__i8) OpenAD_Symbol_378
!
!       **** Parameters and Result ****
!
  INTEGER(w2f__i4) NX
  INTEGER(w2f__i4) NY
  INTEGER(w2f__i4) NZ
  REAL(w2f__8) DX
  REAL(w2f__8) DY
  REAL(w2f__8) SIGMA(1 : NZ)
  type(active) :: TEMP(1:NZ,0:INT((NX+1)),0:INT((NY+1)))
  type(active) :: ARTM(1:NX,1:NY)
  type(active) :: THCK(1:NX,1:NY)
  type(active) :: USRF(1:NX,1:NY)
  type(active) :: UVEL(1:NZ,1:INT((NX+(-1))),1:INT((NY+(-1))))
  type(active) :: VVEL(1:NZ,1:INT((NX+(-1))),1:INT((NY+(-1))))
  type(active) :: UBAS(1:INT((NX+(-1))),1:INT((NY+(-1))))
  type(active) :: VBAS(1:INT((NX+(-1))),1:INT((NY+(-1))))
  type(active) :: WVEL(1:NZ,1:NX,1:NY)
  type(active) :: WGRD(1:NZ,1:NX,1:NY)
  type(active) :: FLWA(1:NZ,1:NX,1:NY)
  REAL(w2f__8) BHEATFLX(1 : NX, 1 : NY)
  REAL(w2f__8) BWAT(1 : NX, 1 : NY)
  type(active) :: BMLT(1:NX,1:NY)
  REAL(w2f__8) DT
  REAL(w2f__8) THKLIM
  LOGICAL(w2f__i4) FLOATMASK(1 : NX, 1 : NY)
!
!       **** Local Variables and Functions ****
!
  REAL(w2f__8) ADVCONST1
  REAL(w2f__8) ADVCONST2
  REAL(w2f__8) C1(1 : NZ)
  REAL(w2f__8) CONS1
  REAL(w2f__8) CONS2
  REAL(w2f__8) CONS3
  REAL(w2f__8) CONS4
  type(active) :: DIAG(1:NZ)
  type(active) :: DIAGADVT(1:NZ)
  type(active) :: DISSIP(1:NZ,1:NX,1:NY)
  REAL(w2f__8) DUPA(1 : NZ)
  REAL(w2f__8) DUPB(1 : NZ)
  REAL(w2f__8) DUPC(1 : NZ)
  REAL(w2f__8) DUPN
  REAL(w2f__8) DUPS(1 : NZ, 1 : 3)
  type(active) :: DUSRFDEW(1:INT((NX+(-1))),1:INT((NY+(-1))))
  type(active) :: DUSRFDNS(1:INT((NX+(-1))),1:INT((NY+(-1))))
  INTEGER(w2f__i4) EW
  REAL(w2f__8) F3
  type(active) :: HADV_U(1:NZ,1:NX,1:NY)
  type(active) :: HADV_V(1:NZ,1:NX,1:NY)
  INTEGER(w2f__i4) I1
  INTEGER(w2f__i4) I2
  type(active) :: INITADVT(1:NZ,1:NX,1:NY)
  type(active) :: INITTEMP(1:NZ,1:NX,1:NY)
  INTEGER(w2f__i4) ITER
  type(active) :: ITERADVT(1:NZ)
  type(active) :: MINARTM
  INTEGER(w2f__i4) MXIT
  PARAMETER ( MXIT = 100)
  INTEGER(w2f__i4) NS
  REAL(w2f__8) OAD_CTMP0
  REAL(w2f__8) OAD_CTMP1(1 : NZ)
  REAL(w2f__8) OAD_CTMP2
  REAL(w2f__8) OAD_CTMP3
  REAL(w2f__8) OAD_CTMP4(1 : NZ)
  type(active) :: OAD_CTMP5
  type(active) :: OAD_CTMP6
  REAL(w2f__8) PREVTEMP(1 : NZ)
  type(active) :: RHSD(1:NZ)
  REAL(w2f__8) SLIDEF1
  REAL(w2f__8) SLIDEF2
  type(active) :: STAGTHCK(1:INT((NX+(-1))),1:INT((NY+(-1))))
  type(active) :: SUBD(1:NZ)
  type(active) :: SUMTEMP(1:NZ)
  type(active) :: SUPD(1:NZ)
  REAL(w2f__8) TEMPRESID
  REAL(w2f__8) TEMPTHRES
  PARAMETER ( TEMPTHRES = 1.00000000000000002082D-03)
  type(active) :: TEMPX(1:NZ,1:5)
  type(active) :: TEMPY(1:NZ,1:5)
  INTEGER(w2f__i4) UP
  type(active) :: WEFF(1:NZ)
  REAL(w2f__8) WMAX
  PARAMETER ( WMAX = 5.0D00)
  INTEGER(w2f__i4) OpenAD_Symbol_1424
  INTEGER(w2f__i4) OpenAD_Symbol_1425
  INTEGER(w2f__i4) OpenAD_Symbol_1426
  INTEGER(w2f__i4) OpenAD_Symbol_1427
  INTEGER(w2f__i4) OpenAD_Symbol_1428
  INTEGER(w2f__i4) OpenAD_Symbol_1429
  REAL(w2f__8) OpenAD_Symbol_1430
  INTEGER(w2f__i4) OpenAD_Symbol_1431
  INTEGER(w2f__i4) OpenAD_Symbol_1432
  INTEGER(w2f__i4) OpenAD_Symbol_1433
  REAL(w2f__8) OpenAD_Symbol_1434
  INTEGER(w2f__i4) OpenAD_Symbol_1435
  INTEGER(w2f__i4) OpenAD_Symbol_1436
  INTEGER(w2f__i4) OpenAD_Symbol_1437
  INTEGER(w2f__i4) OpenAD_Symbol_1438
  INTEGER(w2f__i4) OpenAD_Symbol_1439
  INTEGER(w2f__i4) OpenAD_Symbol_1440
  INTEGER(w2f__i4) OpenAD_Symbol_1441
  INTEGER(w2f__i4) OpenAD_Symbol_1442
  INTEGER(w2f__i4) OpenAD_Symbol_1443
  INTEGER(w2f__i4) OpenAD_Symbol_1444
  INTEGER(w2f__i4) OpenAD_Symbol_1445
  INTEGER(w2f__i4) OpenAD_Symbol_1446
  INTEGER(w2f__i4) OpenAD_Symbol_1447
  INTEGER(w2f__i4) OpenAD_Symbol_1448
  INTEGER(w2f__i4) OpenAD_Symbol_1449
  INTEGER(w2f__i4) OpenAD_Symbol_1450
  INTEGER(w2f__i4) OpenAD_Symbol_1451
  INTEGER(w2f__i4) OpenAD_Symbol_1452
  INTEGER(w2f__i4) OpenAD_Symbol_1453
  INTEGER(w2f__i4) OpenAD_Symbol_1454
  INTEGER(w2f__i4) OpenAD_Symbol_1455
  INTEGER(w2f__i4) OpenAD_Symbol_1456
  INTEGER(w2f__i4) OpenAD_Symbol_1457
  INTEGER(w2f__i4) OpenAD_Symbol_1458
  INTEGER(w2f__i4) OpenAD_Symbol_1459
  INTEGER(w2f__i4) OpenAD_Symbol_1460
  INTEGER(w2f__i4) OpenAD_Symbol_379
  INTEGER(w2f__i4) OpenAD_Symbol_380
  INTEGER(w2f__i4) OpenAD_Symbol_381
  INTEGER(w2f__i4) OpenAD_Symbol_382
  INTEGER(w2f__i4) OpenAD_Symbol_383
  INTEGER(w2f__i4) OpenAD_Symbol_384
  INTEGER(w2f__i4) OpenAD_Symbol_385
  INTEGER(w2f__i4) OpenAD_Symbol_386
  INTEGER(w2f__i4) OpenAD_Symbol_387
  INTEGER(w2f__i4) OpenAD_Symbol_388
  REAL(w2f__8) OpenAD_lin_20
  REAL(w2f__8) OpenAD_lin_21
  type(active) :: OpenAD_prp_14(:)
  ALLOCATABLE OpenAD_prp_14
  REAL(w2f__8) OpenAD_tyc_2(:, :)
  ALLOCATABLE OpenAD_tyc_2
  REAL(w2f__8) OpenAD_tyc_3(:, :)
  ALLOCATABLE OpenAD_tyc_3
  type(active) :: OpenAD_tyc_4
  type(active) :: OpenAD_tyc_5
  type(active) :: OpenAD_tyc_6
  REAL(w2f__8) OpenAD_tyc_61(:, :)
  ALLOCATABLE OpenAD_tyc_61
  REAL(w2f__8) OpenAD_tyc_63(:, :)
  ALLOCATABLE OpenAD_tyc_63
  type(active) :: OpenAD_tyc_65
  type(active) :: OpenAD_tyc_66
  type(active) :: OpenAD_tyc_67


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  ADVCONST1 = (DT /(DX * 1.6D+01))
  ADVCONST2 = (DT /(DY * 1.6D+01))
  CONS1 = ((DT * 6.3113852D+07 * 2.10000000000000008882D00) / 3.65638D+06)
  CONS2 = (DT * 5.0D-01)
  CONS3 = ((DT * 6.3113852D+07) / 1.82819D+06)
  CONS4 = (DT / 2.10000000000000008882D00)
  C1(1 : INT(NZ)) = ((DT *((SIGMA(1 : NZ) * 9.1D+02 * 9.81000000000000049738D00)&
     & ** 4) * 2.0D00) / 2.925104D+07)

  F3 = (1.82819D+06 /(DT * 3.35D+05 * 9.1D+02))
  SLIDEF1 = ((DT * 9.81000000000000049738D00) / 2.009D+03)
  SLIDEF2 = ((DT * 8.9271000000000003638D+03) / 6.62695446000000014901D+07)
  DUPA(1) = 0.0D00
  DUPA(2) = 0.0D00
  DO UP = 3, NZ, 1
    DUPA(INT(UP)) = ((SIGMA(UP) - SIGMA(UP + (-1))) /((SIGMA(UP + (-2)) - SIGMA(&
     &UP)) *(SIGMA(UP + (-2)) - SIGMA(UP + (-1)))))

  END DO
  DUPB(1) = 0.0D00
  DUPB(2) = 0.0D00
  DO UP = 3, NZ, 1
    DUPB(INT(UP)) = ((SIGMA(UP) - SIGMA(UP + (-2))) /((SIGMA(UP + (-1)) - SIGMA(&
     &UP)) *(SIGMA(UP + (-1)) - SIGMA(UP + (-2)))))

  END DO
  DUPC(1) = ((SIGMA(2) - SIGMA(1)) * 5.0D-01)
  DO UP = 2, (NZ +(-1)), 1
    DUPC(INT(UP)) = ((SIGMA(UP + 1) - SIGMA(UP + (-1))) * 5.0D-01 )
  END DO
  DUPC(INT(NZ)) = ((SIGMA(NZ) - SIGMA(NZ + (-1))) * 5.0D-01)
  DUPS(1 : INT(NZ), 1 : 3) = 0.0D00
  DO UP = 2, (NZ +(-1)), 1
    DUPS(INT(UP), 1) = (1.0D00 /((SIGMA(UP) - SIGMA(UP + (-1))) * (SIGMA(UP + 1)&
     & - SIGMA(UP + (-1)))))

    DUPS(INT(UP), 2) = (1.0D00 /((SIGMA(UP + 1) - SIGMA(UP)) *( SIGMA(UP + 1) - &
     &SIGMA(UP + (-1)))))

    DUPS(INT(UP), 3) = (1.0D00 /(SIGMA(UP + 1) - SIGMA(UP + (-1)) ))
  END DO
  DUPN = (SIGMA(NZ) - SIGMA(NZ + (-1)))
  INITTEMP(1:INT(NZ),1:INT(NX),1:INT(NY))%v = 0.0D00
  INITADVT(1:INT(NZ),1:INT(NX),1:INT(NY))%v = 0.0D00
  CALL STAGVARB(THCK,STAGTHCK,NX,NY)
  CALL DF_FIELD_2D_STAGGERED(USRF,DX,DY,DUSRFDEW,DUSRFDNS)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_2,THCK)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_2,THCK)
  CALL FINDDISP(DISSIP,OpenAD_tyc_2,STAGTHCK,DUSRFDEW,DUSRFDNS,FLWA,NX,NY,C1,THK&
     &LIM)

  DO NS = 2,(NY+(-1)),1
    DO EW = 2,(NX+(-1)),1
      SUMTEMP(1:INT(NZ))%v = (UVEL(1:NZ,EW,NS)%v+UVEL(1:NZ,EW+(-1),NS)%v+UVEL(1:&
     &NZ,EW,NS+(-1))%v+UVEL(1:NZ,EW+(-1),NS+(-1))%v)

      HADV_U(1:INT(NZ),INT(EW),INT(NS))%v = (SUMTEMP(1:NZ)%v*ADVCONST1)
      SUMTEMP(1:INT(NZ))%v = (VVEL(1:NZ,EW,NS)%v+VVEL(1:NZ,EW+(-1),NS)%v+VVEL(1:&
     &NZ,EW,NS+(-1))%v+VVEL(1:NZ,EW+(-1),NS+(-1))%v)

      HADV_V(1:INT(NZ),INT(EW),INT(NS))%v = (SUMTEMP(1:NZ)%v*ADVCONST2)
    END DO
  END DO
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_3,THCK)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_3,THCK)
  CALL HADVALL(INITADVT,TEMP,OpenAD_tyc_3,THKLIM,HADV_U,HADV_V,NX,NY,NZ)
  ITER = 0
  TEMPRESID = ABS(2.00000000000000004163D-03)
  do while ((ITER.LE.100).AND.(TEMPRESID.GT.1.00000000000000002082D-03))
    TEMPRESID = 0.0D00
    DO NS = 2,(NY+(-1)),1
      DO EW = 2,(NX+(-1)),1
        IF (THCK(EW,NS)%v.GT.THKLIM) THEN
          WEFF(1:INT(NZ))%v = (WVEL(1:NZ,EW,NS)%v-WGRD(1:NZ,EW,NS)%v)
          OAD_CTMP1(1:INT(NZ)) = ABS(WEFF%v)
          CALL OAD_S_MAXVAL_D_1(OAD_CTMP1,OAD_CTMP0)
          IF (OAD_CTMP0.GT.5.0D00) THEN
            WEFF(1:INT(NZ))%v = 0.0D00
          ENDIF
          DO I1 = 1,5,1
            DO I2 = 1,NZ,1
              TEMPX(INT(I2),INT(I1))%v = TEMP(I2,EW+I1+(-3),NS)%v
              TEMPY(INT(I2),INT(I1))%v = TEMP(I2,EW,I1+NS+(-3))%v
            END DO
          END DO
          CALL HADVPNT(ITERADVT,DIAGADVT,TEMPX,TEMPY,HADV_U(1:NZ,EW,NS),HADV_V(1&
     &:NZ,EW,NS),NZ)

          CALL FINDVTRI(NZ,DUPS,DUPN,THCK(EW,NS),SUBD,DIAG,SUPD,DIAGADVT,WEFF,FL&
     &OATMASK(EW,NS),CONS1,CONS2)

          IF (ITER.eq.0) THEN
            CALL FINDVTRI_INIT(NZ,DUPN,INITADVT,DISSIP,INITTEMP,BHEATFLX,DUSRFDE&
     &W,DUSRFDNS,EW,NS,SUBD,DIAG,SUPD,WEFF,UBAS,VBAS,TEMP(1:NZ,EW,NS),THCK(EW,NS&
     &),FLOATMASK(EW,NS),CONS3,CONS4,SLIDEF1,SLIDEF2)

          ENDIF
          CALL FINDVTRI_RHS(NZ,INITTEMP(1:NZ,EW,NS),ARTM(EW,NS),ITERADVT,RHSD,FL&
     &OATMASK(EW,NS))

          PREVTEMP(1:INT(NZ)) = TEMP(1:NZ,EW,NS)%v
          CALL TRIDIAG(SUBD,DIAG,SUPD,TEMP(1:NZ,EW,NS),RHSD,NZ)
          CALL CORRPMPT(TEMP(1:NZ,EW,NS),THCK(EW,NS),BWAT(EW,NS),SIGMA,NZ)
          OAD_CTMP4(1:INT(NZ)) = ABS(TEMP(1:NZ,EW,NS)%v-PREVTEMP(1:NZ))
          CALL OAD_S_MAXVAL_D_1(OAD_CTMP4,OAD_CTMP3)
!!! requested inline of 'oad_convert' has no defn
          CALL oad_convert(OpenAD_tyc_4,TEMPRESID)
!!! requested inline of 'oad_convert' has no defn
          CALL oad_convert(OpenAD_tyc_5,OAD_CTMP3)
          CALL OAD_S_MAX_D(OpenAD_tyc_4,OpenAD_tyc_5,OpenAD_tyc_6)
!!! requested inline of 'oad_convert' has no defn
          CALL oad_convert(OAD_CTMP2,OpenAD_tyc_6)
          TEMPRESID = OAD_CTMP2
        ENDIF
      END DO
    END DO
    ITER = (ITER+1)
  END DO
  DO NS = 1,NY,1
    DO EW = 1,NX,1
      IF (THCK(EW,NS)%v.LT.THKLIM) THEN
        OAD_CTMP6%v = DBLE(ARTM(EW,NS)%v)
        CALL OAD_S_MIN_D(0.0D00,OAD_CTMP6,OAD_CTMP5)
        MINARTM%v = OAD_CTMP5%v
        DO I2 = 1,NZ,1
          TEMP(INT(I2),INT(EW),INT(NS))%v = MINARTM%v
        END DO
      ENDIF
    END DO
  END DO
  CALL CALCBMLT(NX,NY,NZ,SIGMA,DUPA,DUPB,DUPC,DISSIP,TEMP,THCK,STAGTHCK,DUSRFDEW&
     &,DUSRFDNS,UBAS,VBAS,BHEATFLX,BMLT,FLOATMASK,F3,THKLIM)

    end if
    if (our_rev_mode%tape) then
! taping
  ADVCONST1 = (DT/(DX*1.6D+01))
  ADVCONST2 = (DT/(DY*1.6D+01))
  CONS1 = ((DT*6.3113852D+07*2.10000000000000008882D00)/3.65638D+06)
  CONS2 = (DT*5.0D-01)
  CONS3 = ((DT*6.3113852D+07)/1.82819D+06)
  CONS4 = (DT/2.10000000000000008882D00)
  C1(1:INT(NZ)) = ((DT*((SIGMA(1:NZ)*9.1D+02*9.81000000000000049738D00)**4)*2.0D&
     &00)/2.925104D+07)

  F3 = (1.82819D+06/(DT*3.35D+05*9.1D+02))
  SLIDEF1 = ((DT*9.81000000000000049738D00)/2.009D+03)
  SLIDEF2 = ((DT*8.9271000000000003638D+03)/6.62695446000000014901D+07)
  DUPA(1) = 0.0D00
  DUPA(2) = 0.0D00
  OpenAD_Symbol_303 = 0_w2f__i8
  DO UP = 3,NZ,1
    DUPA(INT(UP)) = ((SIGMA(UP)-SIGMA(UP+(-1)))/((SIGMA(UP+(-2))-SIGMA(UP))*(SIG&
     &MA(UP+(-2))-SIGMA(UP+(-1)))))

    OpenAD_Symbol_303 = (INT(OpenAD_Symbol_303)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_303
  oad_it_ptr = oad_it_ptr+1
  DUPB(1) = 0.0D00
  DUPB(2) = 0.0D00
  OpenAD_Symbol_304 = 0_w2f__i8
  DO UP = 3,NZ,1
    DUPB(INT(UP)) = ((SIGMA(UP)-SIGMA(UP+(-2)))/((SIGMA(UP+(-1))-SIGMA(UP))*(SIG&
     &MA(UP+(-1))-SIGMA(UP+(-2)))))

    OpenAD_Symbol_304 = (INT(OpenAD_Symbol_304)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_304
  oad_it_ptr = oad_it_ptr+1
  DUPC(1) = ((SIGMA(2)-SIGMA(1))*5.0D-01)
  OpenAD_Symbol_305 = 0_w2f__i8
  DO UP = 2,(NZ+(-1)),1
    DUPC(INT(UP)) = ((SIGMA(UP+1)-SIGMA(UP+(-1)))*5.0D-01)
    OpenAD_Symbol_305 = (INT(OpenAD_Symbol_305)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_305
  oad_it_ptr = oad_it_ptr+1
  DUPC(INT(NZ)) = ((SIGMA(NZ)-SIGMA(NZ+(-1)))*5.0D-01)
  DUPS(1:INT(NZ),1:3) = 0.0D00
  OpenAD_Symbol_306 = 0_w2f__i8
  DO UP = 2,(NZ+(-1)),1
    DUPS(INT(UP),1) = (1.0D00/((SIGMA(UP)-SIGMA(UP+(-1)))*(SIGMA(UP+1)-SIGMA(UP+&
     &(-1)))))

    DUPS(INT(UP),2) = (1.0D00/((SIGMA(UP+1)-SIGMA(UP))*(SIGMA(UP+1)-SIGMA(UP+(-1&
     &)))))

    DUPS(INT(UP),3) = (1.0D00/(SIGMA(UP+1)-SIGMA(UP+(-1))))
    OpenAD_Symbol_306 = (INT(OpenAD_Symbol_306)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_306
  oad_it_ptr = oad_it_ptr+1
  DUPN = (SIGMA(NZ)-SIGMA(NZ+(-1)))
  INITTEMP(1:INT(NZ),1:INT(NX),1:INT(NY))%v = 0.0D00
  INITADVT(1:INT(NZ),1:INT(NX),1:INT(NY))%v = 0.0D00
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = NZ
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = NX
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = NY
  oad_it_ptr = oad_it_ptr+1
  CALL STAGVARB(THCK,STAGTHCK,NX,NY)
  CALL DF_FIELD_2D_STAGGERED(USRF,DX,DY,DUSRFDEW,DUSRFDNS)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_2,THCK)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_2,THCK)
  CALL FINDDISP(DISSIP,OpenAD_tyc_2,STAGTHCK,DUSRFDEW,DUSRFDNS,FLWA,NX,NY,C1,THK&
     &LIM)

  OpenAD_Symbol_307 = 0_w2f__i8
  DO NS = 2,(NY+(-1)),1
    OpenAD_Symbol_308 = 0_w2f__i8
    DO EW = 2,(NX+(-1)),1
      SUMTEMP(1:INT(NZ))%v = (UVEL(1:NZ,EW,NS)%v+UVEL(1:NZ,EW+(-1),NS)%v+UVEL(1:&
     &NZ,EW,NS+(-1))%v+UVEL(1:NZ,EW+(-1),NS+(-1))%v)

      OpenAD_Symbol_379 = (EW+(-1))
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = OpenAD_Symbol_379
      oad_it_ptr = oad_it_ptr+1
      OpenAD_Symbol_380 = (NS+(-1))
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = OpenAD_Symbol_380
      oad_it_ptr = oad_it_ptr+1
      OpenAD_Symbol_381 = (EW+(-1))
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = OpenAD_Symbol_381
      oad_it_ptr = oad_it_ptr+1
      OpenAD_Symbol_382 = (NS+(-1))
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = OpenAD_Symbol_382
      oad_it_ptr = oad_it_ptr+1
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = NZ
      oad_it_ptr = oad_it_ptr+1
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = EW
      oad_it_ptr = oad_it_ptr+1
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = NS
      oad_it_ptr = oad_it_ptr+1
      OpenAD_lin_20 = ADVCONST1
      HADV_U(1:INT(NZ),INT(EW),INT(NS))%v = (SUMTEMP(1:NZ)%v*ADVCONST1)
      SUMTEMP(1:INT(NZ))%v = (VVEL(1:NZ,EW,NS)%v+VVEL(1:NZ,EW+(-1),NS)%v+VVEL(1:&
     &NZ,EW,NS+(-1))%v+VVEL(1:NZ,EW+(-1),NS+(-1))%v)

      if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
      oad_dt(oad_dt_ptr) = OpenAD_lin_20
      oad_dt_ptr = oad_dt_ptr+1
      OpenAD_Symbol_383 = (EW+(-1))
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = OpenAD_Symbol_383
      oad_it_ptr = oad_it_ptr+1
      OpenAD_Symbol_384 = (NS+(-1))
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = OpenAD_Symbol_384
      oad_it_ptr = oad_it_ptr+1
      OpenAD_Symbol_385 = (EW+(-1))
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = OpenAD_Symbol_385
      oad_it_ptr = oad_it_ptr+1
      OpenAD_Symbol_386 = (NS+(-1))
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = OpenAD_Symbol_386
      oad_it_ptr = oad_it_ptr+1
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = NZ
      oad_it_ptr = oad_it_ptr+1
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = EW
      oad_it_ptr = oad_it_ptr+1
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = NS
      oad_it_ptr = oad_it_ptr+1
      OpenAD_lin_21 = ADVCONST2
      HADV_V(1:INT(NZ),INT(EW),INT(NS))%v = (SUMTEMP(1:NZ)%v*ADVCONST2)
      if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
      oad_dt(oad_dt_ptr) = OpenAD_lin_21
      oad_dt_ptr = oad_dt_ptr+1
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = NZ
      oad_it_ptr = oad_it_ptr+1
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = EW
      oad_it_ptr = oad_it_ptr+1
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = NS
      oad_it_ptr = oad_it_ptr+1
      OpenAD_Symbol_308 = (INT(OpenAD_Symbol_308)+INT(1_w2f__i8))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_308
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_307 = (INT(OpenAD_Symbol_307)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_307
  oad_it_ptr = oad_it_ptr+1
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_3,THCK)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_3,THCK)
  CALL HADVALL(INITADVT,TEMP,OpenAD_tyc_3,THKLIM,HADV_U,HADV_V,NX,NY,NZ)
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = NX
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = NY
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = NZ
  oad_it_ptr = oad_it_ptr+1
  ITER = 0
  TEMPRESID = ABS(2.00000000000000004163D-03)
  OpenAD_Symbol_309 = 0_w2f__i8
  do while ((ITER.LE.100).AND.(TEMPRESID.GT.1.00000000000000002082D-03))
    TEMPRESID = 0.0D00
    OpenAD_Symbol_310 = 0_w2f__i8
    DO NS = 2,(NY+(-1)),1
      OpenAD_Symbol_311 = 0_w2f__i8
      DO EW = 2,(NX+(-1)),1
        IF (THCK(EW,NS)%v.GT.THKLIM) THEN
          WEFF(1:INT(NZ))%v = (WVEL(1:NZ,EW,NS)%v-WGRD(1:NZ,EW,NS)%v)
          OAD_CTMP1(1:INT(NZ)) = ABS(WEFF%v)
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = NZ
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = EW
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = NS
          oad_it_ptr = oad_it_ptr+1
          CALL OAD_S_MAXVAL_D_1(OAD_CTMP1,OAD_CTMP0)
          IF (OAD_CTMP0.GT.5.0D00) THEN
            WEFF(1:INT(NZ))%v = 0.0D00
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = NZ
            oad_it_ptr = oad_it_ptr+1
            OpenAD_Symbol_312 = 1_w2f__i8
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = OpenAD_Symbol_312
            oad_it_ptr = oad_it_ptr+1
          ELSE
            OpenAD_Symbol_313 = 0_w2f__i8
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = OpenAD_Symbol_313
            oad_it_ptr = oad_it_ptr+1
          ENDIF
          OpenAD_Symbol_314 = 0_w2f__i8
          DO I1 = 1,5,1
            OpenAD_Symbol_315 = 0_w2f__i8
            DO I2 = 1,NZ,1
              TEMPX(INT(I2),INT(I1))%v = TEMP(I2,EW+I1+(-3),NS)%v
              TEMPY(INT(I2),INT(I1))%v = TEMP(I2,EW,I1+NS+(-3))%v
              OpenAD_Symbol_387 = (EW+I1+(-3))
              if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
              oad_it(oad_it_ptr) = OpenAD_Symbol_387
              oad_it_ptr = oad_it_ptr+1
              OpenAD_Symbol_388 = (I1+NS+(-3))
              if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
              oad_it(oad_it_ptr) = OpenAD_Symbol_388
              oad_it_ptr = oad_it_ptr+1
              if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
              oad_it(oad_it_ptr) = I2
              oad_it_ptr = oad_it_ptr+1
              if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
              oad_it(oad_it_ptr) = NS
              oad_it_ptr = oad_it_ptr+1
              if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
              oad_it(oad_it_ptr) = I1
              oad_it_ptr = oad_it_ptr+1
              if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
              oad_it(oad_it_ptr) = EW
              oad_it_ptr = oad_it_ptr+1
              OpenAD_Symbol_315 = (INT(OpenAD_Symbol_315)+INT(1_w2f__i8))
            END DO
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = OpenAD_Symbol_315
            oad_it_ptr = oad_it_ptr+1
            OpenAD_Symbol_314 = (INT(OpenAD_Symbol_314)+INT(1_w2f__i8))
          END DO
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_314
          oad_it_ptr = oad_it_ptr+1
          CALL HADVPNT(ITERADVT,DIAGADVT,TEMPX,TEMPY,HADV_U(1:NZ,EW,NS),HADV_V(1&
     &:NZ,EW,NS),NZ)

          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = NZ
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = EW
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = NS
          oad_it_ptr = oad_it_ptr+1
          CALL FINDVTRI(NZ,DUPS,DUPN,THCK(EW,NS),SUBD,DIAG,SUPD,DIAGADVT,WEFF,FL&
     &OATMASK(EW,NS),CONS1,CONS2)

          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = EW
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = NS
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = NZ
          oad_it_ptr = oad_it_ptr+1
          IF (ITER.eq.0) THEN
            CALL FINDVTRI_INIT(NZ,DUPN,INITADVT,DISSIP,INITTEMP,BHEATFLX,DUSRFDE&
     &W,DUSRFDNS,EW,NS,SUBD,DIAG,SUPD,WEFF,UBAS,VBAS,TEMP(1:NZ,EW,NS),THCK(EW,NS&
     &),FLOATMASK(EW,NS),CONS3,CONS4,SLIDEF1,SLIDEF2)

            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = NZ
            oad_it_ptr = oad_it_ptr+1
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = EW
            oad_it_ptr = oad_it_ptr+1
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = NS
            oad_it_ptr = oad_it_ptr+1
            OpenAD_Symbol_316 = 1_w2f__i8
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = OpenAD_Symbol_316
            oad_it_ptr = oad_it_ptr+1
          ELSE
            OpenAD_Symbol_317 = 0_w2f__i8
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = OpenAD_Symbol_317
            oad_it_ptr = oad_it_ptr+1
          ENDIF
          CALL FINDVTRI_RHS(NZ,INITTEMP(1:NZ,EW,NS),ARTM(EW,NS),ITERADVT,RHSD,FL&
     &OATMASK(EW,NS))

          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = NZ
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = EW
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = NS
          oad_it_ptr = oad_it_ptr+1
          PREVTEMP(1:INT(NZ)) = TEMP(1:NZ,EW,NS)%v
          CALL TRIDIAG(SUBD,DIAG,SUPD,TEMP(1:NZ,EW,NS),RHSD,NZ)
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = NZ
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = EW
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = NS
          oad_it_ptr = oad_it_ptr+1
          CALL CORRPMPT(TEMP(1:NZ,EW,NS),THCK(EW,NS),BWAT(EW,NS),SIGMA,NZ)
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = NZ
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = EW
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = NS
          oad_it_ptr = oad_it_ptr+1
          OAD_CTMP4(1:INT(NZ)) = ABS(TEMP(1:NZ,EW,NS)%v-PREVTEMP(1:NZ))
          CALL OAD_S_MAXVAL_D_1(OAD_CTMP4,OAD_CTMP3)
!!! requested inline of 'oad_convert' has no defn
          CALL oad_convert(OpenAD_tyc_4,TEMPRESID)
!!! requested inline of 'oad_convert' has no defn
          CALL oad_convert(OpenAD_tyc_5,OAD_CTMP3)
          CALL OAD_S_MAX_D(OpenAD_tyc_4,OpenAD_tyc_5,OpenAD_tyc_6)
!!! requested inline of 'oad_convert' has no defn
          CALL oad_convert(OAD_CTMP2,OpenAD_tyc_6)
          TEMPRESID = OAD_CTMP2
          OpenAD_Symbol_319 = 1_w2f__i8
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_319
          oad_it_ptr = oad_it_ptr+1
        ELSE
          OpenAD_Symbol_318 = 0_w2f__i8
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_318
          oad_it_ptr = oad_it_ptr+1
        ENDIF
        OpenAD_Symbol_311 = (INT(OpenAD_Symbol_311)+INT(1_w2f__i8))
      END DO
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = OpenAD_Symbol_311
      oad_it_ptr = oad_it_ptr+1
      OpenAD_Symbol_310 = (INT(OpenAD_Symbol_310)+INT(1_w2f__i8))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_310
    oad_it_ptr = oad_it_ptr+1
    ITER = (ITER+1)
    OpenAD_Symbol_309 = (INT(OpenAD_Symbol_309)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_309
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_320 = 0_w2f__i8
  DO NS = 1,NY,1
    OpenAD_Symbol_321 = 0_w2f__i8
    DO EW = 1,NX,1
      IF (THCK(EW,NS)%v.LT.THKLIM) THEN
        OAD_CTMP6%v = DBLE(ARTM(EW,NS)%v)
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        CALL OAD_S_MIN_D(0.0D00,OAD_CTMP6,OAD_CTMP5)
        MINARTM%v = OAD_CTMP5%v
        OpenAD_Symbol_322 = 0_w2f__i8
        DO I2 = 1,NZ,1
          TEMP(INT(I2),INT(EW),INT(NS))%v = MINARTM%v
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = I2
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = EW
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = NS
          oad_it_ptr = oad_it_ptr+1
          OpenAD_Symbol_322 = (INT(OpenAD_Symbol_322)+INT(1_w2f__i8))
        END DO
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_322
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_324 = 1_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_324
        oad_it_ptr = oad_it_ptr+1
      ELSE
        OpenAD_Symbol_323 = 0_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_323
        oad_it_ptr = oad_it_ptr+1
      ENDIF
      OpenAD_Symbol_321 = (INT(OpenAD_Symbol_321)+INT(1_w2f__i8))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_321
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_320 = (INT(OpenAD_Symbol_320)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_320
  oad_it_ptr = oad_it_ptr+1
  CALL CALCBMLT(NX,NY,NZ,SIGMA,DUPA,DUPB,DUPC,DISSIP,TEMP,THCK,STAGTHCK,DUSRFDEW&
     &,DUSRFDNS,UBAS,VBAS,BHEATFLX,BMLT,FLOATMASK,F3,THKLIM)

  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = NZ
  oad_it_ptr = oad_it_ptr+1
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  NZ = oad_it(oad_it_ptr)
  CALL CALCBMLT(NX,NY,NZ,SIGMA,DUPA,DUPB,DUPC,DISSIP,TEMP,THCK,STAGTHCK,DUSRFDEW&
     &,DUSRFDNS,UBAS,VBAS,BHEATFLX,BMLT,FLOATMASK,F3,THKLIM)

  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_271 = oad_it(oad_it_ptr)
  OpenAD_Symbol_272 = 1
  do while (INT(OpenAD_Symbol_272).LE.INT(OpenAD_Symbol_271))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_273 = oad_it(oad_it_ptr)
    OpenAD_Symbol_274 = 1
    do while (INT(OpenAD_Symbol_274).LE.INT(OpenAD_Symbol_273))
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_275 = oad_it(oad_it_ptr)
      IF (OpenAD_Symbol_275.ne.0) THEN
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_276 = oad_it(oad_it_ptr)
        OpenAD_Symbol_277 = 1
        do while (INT(OpenAD_Symbol_277).LE.INT(OpenAD_Symbol_276))
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1458 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1459 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1460 = oad_it(oad_it_ptr)
          MINARTM%d = MINARTM%d+TEMP(OpenAD_Symbol_1460,OpenAD_Symbol_1459,OpenA&
     &D_Symbol_1458)%d

          TEMP(OpenAD_Symbol_1460,OpenAD_Symbol_1459,OpenAD_Symbol_1458)%d = 0.0&
     &d0

          OpenAD_Symbol_277 = INT(OpenAD_Symbol_277)+1
        END DO
        OAD_CTMP5%d = OAD_CTMP5%d+MINARTM%d
        MINARTM%d = 0.0d0
        CALL OAD_S_MIN_D(0.0D00,OAD_CTMP6,OAD_CTMP5)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1456 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1457 = oad_it(oad_it_ptr)
        ARTM(OpenAD_Symbol_1457,OpenAD_Symbol_1456)%d = ARTM(OpenAD_Symbol_1457,&
     &OpenAD_Symbol_1456)%d+OAD_CTMP6%d

        OAD_CTMP6%d = 0.0d0
      ENDIF
      OpenAD_Symbol_274 = INT(OpenAD_Symbol_274)+1
    END DO
    OpenAD_Symbol_272 = INT(OpenAD_Symbol_272)+1
  END DO
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_278 = oad_it(oad_it_ptr)
  OpenAD_Symbol_279 = 1
  do while (INT(OpenAD_Symbol_279).LE.INT(OpenAD_Symbol_278))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_280 = oad_it(oad_it_ptr)
    OpenAD_Symbol_281 = 1
    do while (INT(OpenAD_Symbol_281).LE.INT(OpenAD_Symbol_280))
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_282 = oad_it(oad_it_ptr)
      OpenAD_Symbol_283 = 1
      do while (INT(OpenAD_Symbol_283).LE.INT(OpenAD_Symbol_282))
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_284 = oad_it(oad_it_ptr)
        IF (OpenAD_Symbol_284.ne.0) THEN
          CALL OAD_S_MAX_D(OpenAD_tyc_65,OpenAD_tyc_66,OpenAD_tyc_67)
          CALL OAD_S_MAXVAL_D_1(OAD_CTMP4,OAD_CTMP3)
          oad_it_ptr = oad_it_ptr-1
          NS = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          EW = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          NZ = oad_it(oad_it_ptr)
          CALL CORRPMPT(TEMP(1:NZ,EW,NS),THCK(EW,NS),BWAT(EW,NS),SIGMA,NZ)
          oad_it_ptr = oad_it_ptr-1
          NS = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          EW = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          NZ = oad_it(oad_it_ptr)
          CALL TRIDIAG(SUBD,DIAG,SUPD,TEMP(1:NZ,EW,NS),RHSD,NZ)
          oad_it_ptr = oad_it_ptr-1
          NS = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          EW = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          NZ = oad_it(oad_it_ptr)
          CALL FINDVTRI_RHS(NZ,INITTEMP(1:NZ,EW,NS),ARTM(EW,NS),ITERADVT,RHSD,FL&
     &OATMASK(EW,NS))

          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_285 = oad_it(oad_it_ptr)
          IF (OpenAD_Symbol_285.ne.0) THEN
            oad_it_ptr = oad_it_ptr-1
            NS = oad_it(oad_it_ptr)
            oad_it_ptr = oad_it_ptr-1
            EW = oad_it(oad_it_ptr)
            oad_it_ptr = oad_it_ptr-1
            NZ = oad_it(oad_it_ptr)
            CALL FINDVTRI_INIT(NZ,DUPN,INITADVT,DISSIP,INITTEMP,BHEATFLX,DUSRFDE&
     &W,DUSRFDNS,EW,NS,SUBD,DIAG,SUPD,WEFF,UBAS,VBAS,TEMP(1:NZ,EW,NS),THCK(EW,NS&
     &),FLOATMASK(EW,NS),CONS3,CONS4,SLIDEF1,SLIDEF2)

          ENDIF
          oad_it_ptr = oad_it_ptr-1
          NZ = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          NS = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          EW = oad_it(oad_it_ptr)
          CALL FINDVTRI(NZ,DUPS,DUPN,THCK(EW,NS),SUBD,DIAG,SUPD,DIAGADVT,WEFF,FL&
     &OATMASK(EW,NS),CONS1,CONS2)

          oad_it_ptr = oad_it_ptr-1
          NS = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          EW = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          NZ = oad_it(oad_it_ptr)
          CALL HADVPNT(ITERADVT,DIAGADVT,TEMPX,TEMPY,HADV_U(1:NZ,EW,NS),HADV_V(1&
     &:NZ,EW,NS),NZ)

          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_286 = oad_it(oad_it_ptr)
          OpenAD_Symbol_287 = 1
          do while (INT(OpenAD_Symbol_287).LE.INT(OpenAD_Symbol_286))
            oad_it_ptr = oad_it_ptr-1
            OpenAD_Symbol_288 = oad_it(oad_it_ptr)
            OpenAD_Symbol_289 = 1
            do while (INT(OpenAD_Symbol_289).LE.INT(OpenAD_Symbol_288))
              oad_it_ptr = oad_it_ptr-1
              OpenAD_Symbol_1450 = oad_it(oad_it_ptr)
              oad_it_ptr = oad_it_ptr-1
              OpenAD_Symbol_1451 = oad_it(oad_it_ptr)
              oad_it_ptr = oad_it_ptr-1
              OpenAD_Symbol_1452 = oad_it(oad_it_ptr)
              oad_it_ptr = oad_it_ptr-1
              OpenAD_Symbol_1453 = oad_it(oad_it_ptr)
              oad_it_ptr = oad_it_ptr-1
              OpenAD_Symbol_1454 = oad_it(oad_it_ptr)
              TEMP(OpenAD_Symbol_1453,OpenAD_Symbol_1450,OpenAD_Symbol_1454)%d =&
     & TEMP(OpenAD_Symbol_1453,OpenAD_Symbol_1450,OpenAD_Symbol_1454)%d+TEMPY(Op&
     &enAD_Symbol_1453,OpenAD_Symbol_1451)%d

              TEMPY(OpenAD_Symbol_1453,OpenAD_Symbol_1451)%d = 0.0d0
              oad_it_ptr = oad_it_ptr-1
              OpenAD_Symbol_1455 = oad_it(oad_it_ptr)
              TEMP(OpenAD_Symbol_1453,OpenAD_Symbol_1455,OpenAD_Symbol_1452)%d =&
     & TEMP(OpenAD_Symbol_1453,OpenAD_Symbol_1455,OpenAD_Symbol_1452)%d+TEMPX(Op&
     &enAD_Symbol_1453,OpenAD_Symbol_1451)%d

              TEMPX(OpenAD_Symbol_1453,OpenAD_Symbol_1451)%d = 0.0d0
              OpenAD_Symbol_289 = INT(OpenAD_Symbol_289)+1
            END DO
            OpenAD_Symbol_287 = INT(OpenAD_Symbol_287)+1
          END DO
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_290 = oad_it(oad_it_ptr)
          IF (OpenAD_Symbol_290.ne.0) THEN
            oad_it_ptr = oad_it_ptr-1
            OpenAD_Symbol_1449 = oad_it(oad_it_ptr)
            WEFF(1:OpenAD_Symbol_1449)%d = 0.0d0
          ENDIF
          CALL OAD_S_MAXVAL_D_1(OAD_CTMP1,OAD_CTMP0)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1446 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1447 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1448 = oad_it(oad_it_ptr)
          WGRD(1:OpenAD_Symbol_1448,OpenAD_Symbol_1447,OpenAD_Symbol_1446)%d = W&
     &GRD(1:OpenAD_Symbol_1448,OpenAD_Symbol_1447,OpenAD_Symbol_1446)%d-WEFF(1:O&
     &penAD_Symbol_1448)%d

          WVEL(1:OpenAD_Symbol_1448,OpenAD_Symbol_1447,OpenAD_Symbol_1446)%d = W&
     &VEL(1:OpenAD_Symbol_1448,OpenAD_Symbol_1447,OpenAD_Symbol_1446)%d+WEFF(1:O&
     &penAD_Symbol_1448)%d

          WEFF(1:OpenAD_Symbol_1448)%d = 0.0d0
        ENDIF
        OpenAD_Symbol_283 = INT(OpenAD_Symbol_283)+1
      END DO
      OpenAD_Symbol_281 = INT(OpenAD_Symbol_281)+1
    END DO
    OpenAD_Symbol_279 = INT(OpenAD_Symbol_279)+1
  END DO
  oad_it_ptr = oad_it_ptr-1
  NZ = oad_it(oad_it_ptr)
  oad_it_ptr = oad_it_ptr-1
  NY = oad_it(oad_it_ptr)
  oad_it_ptr = oad_it_ptr-1
  NX = oad_it(oad_it_ptr)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_63,THCK)
  CALL HADVALL(INITADVT,TEMP,OpenAD_tyc_63,THKLIM,HADV_U,HADV_V,NX,NY,NZ)
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_291 = oad_it(oad_it_ptr)
  OpenAD_Symbol_292 = 1
  do while (INT(OpenAD_Symbol_292).LE.INT(OpenAD_Symbol_291))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_293 = oad_it(oad_it_ptr)
    OpenAD_Symbol_294 = 1
    do while (INT(OpenAD_Symbol_294).LE.INT(OpenAD_Symbol_293))
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1427 = oad_it(oad_it_ptr)
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1428 = oad_it(oad_it_ptr)
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1429 = oad_it(oad_it_ptr)
      oad_dt_ptr = oad_dt_ptr-1
      OpenAD_Symbol_1430 = oad_dt(oad_dt_ptr)
      SUMTEMP(1:OpenAD_Symbol_1429)%d = SUMTEMP(1:OpenAD_Symbol_1429)%d+HADV_V(1&
     &:OpenAD_Symbol_1429,OpenAD_Symbol_1428,OpenAD_Symbol_1427)%d*(OpenAD_Symbo&
     &l_1430)

      HADV_V(1:OpenAD_Symbol_1429,OpenAD_Symbol_1428,OpenAD_Symbol_1427)%d = 0.0&
     &d0

      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1431 = oad_it(oad_it_ptr)
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1432 = oad_it(oad_it_ptr)
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1433 = oad_it(oad_it_ptr)
      oad_dt_ptr = oad_dt_ptr-1
      OpenAD_Symbol_1434 = oad_dt(oad_dt_ptr)
!!! requested inline of 'oad_AllocateMatching' has no defn
      CALL oad_AllocateMatching(OpenAD_prp_14,SUMTEMP(1:NZ))
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1435 = oad_it(oad_it_ptr)
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1436 = oad_it(oad_it_ptr)
      VVEL(1:OpenAD_Symbol_1433,OpenAD_Symbol_1436,OpenAD_Symbol_1435)%d = VVEL(&
     &1:OpenAD_Symbol_1433,OpenAD_Symbol_1436,OpenAD_Symbol_1435)%d+SUMTEMP(1:Op&
     &enAD_Symbol_1433)%d

      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1437 = oad_it(oad_it_ptr)
      VVEL(1:OpenAD_Symbol_1433,OpenAD_Symbol_1432,OpenAD_Symbol_1437)%d = VVEL(&
     &1:OpenAD_Symbol_1433,OpenAD_Symbol_1432,OpenAD_Symbol_1437)%d+SUMTEMP(1:Op&
     &enAD_Symbol_1433)%d

      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1438 = oad_it(oad_it_ptr)
      VVEL(1:OpenAD_Symbol_1433,OpenAD_Symbol_1438,OpenAD_Symbol_1431)%d = VVEL(&
     &1:OpenAD_Symbol_1433,OpenAD_Symbol_1438,OpenAD_Symbol_1431)%d+SUMTEMP(1:Op&
     &enAD_Symbol_1433)%d

      VVEL(1:OpenAD_Symbol_1433,OpenAD_Symbol_1432,OpenAD_Symbol_1431)%d = VVEL(&
     &1:OpenAD_Symbol_1433,OpenAD_Symbol_1432,OpenAD_Symbol_1431)%d+SUMTEMP(1:Op&
     &enAD_Symbol_1433)%d

      SUMTEMP(1:OpenAD_Symbol_1433)%d = 0.0d0
      OpenAD_prp_14%d = OpenAD_prp_14%d+HADV_U(1:OpenAD_Symbol_1433,OpenAD_Symbo&
     &l_1432,OpenAD_Symbol_1431)%d*(OpenAD_Symbol_1434)

      HADV_U(1:OpenAD_Symbol_1433,OpenAD_Symbol_1432,OpenAD_Symbol_1431)%d = 0.0&
     &d0

      SUMTEMP(1:OpenAD_Symbol_1433)%d = SUMTEMP(1:OpenAD_Symbol_1433)%d+OpenAD_p&
     &rp_14%d

      OpenAD_prp_14%d = 0.0d0
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1439 = oad_it(oad_it_ptr)
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1440 = oad_it(oad_it_ptr)
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1441 = oad_it(oad_it_ptr)
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1442 = oad_it(oad_it_ptr)
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1443 = oad_it(oad_it_ptr)
      UVEL(1:OpenAD_Symbol_1441,OpenAD_Symbol_1443,OpenAD_Symbol_1442)%d = UVEL(&
     &1:OpenAD_Symbol_1441,OpenAD_Symbol_1443,OpenAD_Symbol_1442)%d+SUMTEMP(1:Op&
     &enAD_Symbol_1441)%d

      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1444 = oad_it(oad_it_ptr)
      UVEL(1:OpenAD_Symbol_1441,OpenAD_Symbol_1440,OpenAD_Symbol_1444)%d = UVEL(&
     &1:OpenAD_Symbol_1441,OpenAD_Symbol_1440,OpenAD_Symbol_1444)%d+SUMTEMP(1:Op&
     &enAD_Symbol_1441)%d

      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1445 = oad_it(oad_it_ptr)
      UVEL(1:OpenAD_Symbol_1441,OpenAD_Symbol_1445,OpenAD_Symbol_1439)%d = UVEL(&
     &1:OpenAD_Symbol_1441,OpenAD_Symbol_1445,OpenAD_Symbol_1439)%d+SUMTEMP(1:Op&
     &enAD_Symbol_1441)%d

      UVEL(1:OpenAD_Symbol_1441,OpenAD_Symbol_1440,OpenAD_Symbol_1439)%d = UVEL(&
     &1:OpenAD_Symbol_1441,OpenAD_Symbol_1440,OpenAD_Symbol_1439)%d+SUMTEMP(1:Op&
     &enAD_Symbol_1441)%d

      SUMTEMP(1:OpenAD_Symbol_1441)%d = 0.0d0
      OpenAD_Symbol_294 = INT(OpenAD_Symbol_294)+1
    END DO
    OpenAD_Symbol_292 = INT(OpenAD_Symbol_292)+1
  END DO
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_61,THCK)
  CALL FINDDISP(DISSIP,OpenAD_tyc_61,STAGTHCK,DUSRFDEW,DUSRFDNS,FLWA,NX,NY,C1,TH&
     &KLIM)

  CALL DF_FIELD_2D_STAGGERED(USRF,DX,DY,DUSRFDEW,DUSRFDNS)
  CALL STAGVARB(THCK,STAGTHCK,NX,NY)
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1424 = oad_it(oad_it_ptr)
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1425 = oad_it(oad_it_ptr)
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1426 = oad_it(oad_it_ptr)
  INITADVT(1:OpenAD_Symbol_1426,1:OpenAD_Symbol_1425,1:OpenAD_Symbol_1424)%d = 0&
     &.0d0

  INITTEMP(1:OpenAD_Symbol_1426,1:OpenAD_Symbol_1425,1:OpenAD_Symbol_1424)%d = 0&
     &.0d0

  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_295 = oad_it(oad_it_ptr)
  OpenAD_Symbol_296 = 1
  do while (INT(OpenAD_Symbol_296).LE.INT(OpenAD_Symbol_295))
    OpenAD_Symbol_296 = INT(OpenAD_Symbol_296)+1
  END DO
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_297 = oad_it(oad_it_ptr)
  OpenAD_Symbol_298 = 1
  do while (INT(OpenAD_Symbol_298).LE.INT(OpenAD_Symbol_297))
    OpenAD_Symbol_298 = INT(OpenAD_Symbol_298)+1
  END DO
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_299 = oad_it(oad_it_ptr)
  OpenAD_Symbol_300 = 1
  do while (INT(OpenAD_Symbol_300).LE.INT(OpenAD_Symbol_299))
    OpenAD_Symbol_300 = INT(OpenAD_Symbol_300)+1
  END DO
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_301 = oad_it(oad_it_ptr)
  OpenAD_Symbol_302 = 1
  do while (INT(OpenAD_Symbol_302).LE.INT(OpenAD_Symbol_301))
    OpenAD_Symbol_302 = INT(OpenAD_Symbol_302)+1
  END DO
    end if
  end subroutine TSTEP_TEMPFULLSOLN
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE HADVPNT(ITERADVT, DIAGADVT, TEMPX, TEMPY, U, V, UPN)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  use glimmer_global ,only: DP
  use glimmer_global ,only: DP
  use glimmer_global ,only: DP
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_463
  INTEGER(w2f__i8) OpenAD_Symbol_464
  INTEGER(w2f__i8) OpenAD_Symbol_465
  INTEGER(w2f__i8) OpenAD_Symbol_466
  INTEGER(w2f__i8) OpenAD_Symbol_467
  INTEGER(w2f__i8) OpenAD_Symbol_468
  INTEGER(w2f__i8) OpenAD_Symbol_469
  INTEGER(w2f__i8) OpenAD_Symbol_470
  INTEGER(w2f__i8) OpenAD_Symbol_471
  INTEGER(w2f__i8) OpenAD_Symbol_472
  INTEGER(w2f__i8) OpenAD_Symbol_473
  INTEGER(w2f__i8) OpenAD_Symbol_474
  INTEGER(w2f__i8) OpenAD_Symbol_475
  INTEGER(w2f__i8) OpenAD_Symbol_476
  INTEGER(w2f__i8) OpenAD_Symbol_477
  INTEGER(w2f__i8) OpenAD_Symbol_478
  INTEGER(w2f__i8) OpenAD_Symbol_479
  INTEGER(w2f__i8) OpenAD_Symbol_480
  INTEGER(w2f__i8) OpenAD_Symbol_481
  INTEGER(w2f__i8) OpenAD_Symbol_482
  INTEGER(w2f__i8) OpenAD_Symbol_483
  INTEGER(w2f__i8) OpenAD_Symbol_484
  INTEGER(w2f__i8) OpenAD_Symbol_485
  INTEGER(w2f__i8) OpenAD_Symbol_486
  INTEGER(w2f__i8) OpenAD_Symbol_487
  INTEGER(w2f__i8) OpenAD_Symbol_488
  INTEGER(w2f__i8) OpenAD_Symbol_489
  INTEGER(w2f__i8) OpenAD_Symbol_490
  INTEGER(w2f__i8) OpenAD_Symbol_491
  INTEGER(w2f__i8) OpenAD_Symbol_492
  INTEGER(w2f__i8) OpenAD_Symbol_493
  INTEGER(w2f__i8) OpenAD_Symbol_494
  INTEGER(w2f__i8) OpenAD_Symbol_495
  INTEGER(w2f__i8) OpenAD_Symbol_496
  INTEGER(w2f__i8) OpenAD_Symbol_497
  INTEGER(w2f__i8) OpenAD_Symbol_498
  INTEGER(w2f__i8) OpenAD_Symbol_499
  INTEGER(w2f__i8) OpenAD_Symbol_500
  INTEGER(w2f__i8) OpenAD_Symbol_501
  INTEGER(w2f__i8) OpenAD_Symbol_502
  INTEGER(w2f__i8) OpenAD_Symbol_503
  INTEGER(w2f__i8) OpenAD_Symbol_504
  INTEGER(w2f__i8) OpenAD_Symbol_505
  INTEGER(w2f__i8) OpenAD_Symbol_506
  INTEGER(w2f__i8) OpenAD_Symbol_507
  INTEGER(w2f__i8) OpenAD_Symbol_508
  INTEGER(w2f__i8) OpenAD_Symbol_509
  INTEGER(w2f__i8) OpenAD_Symbol_510
!
!       **** Parameters and Result ****
!
  INTEGER(w2f__i4) UPN
  type(active) :: ITERADVT(1:UPN)
  type(active) :: DIAGADVT(1:UPN)
  type(active) :: TEMPX(1:UPN,1:5)
  type(active) :: TEMPY(1:UPN,1:5)
  type(active) :: U(1:UPN)
  type(active) :: V(1:UPN)
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) UP
  INTEGER(w2f__i4) OpenAD_Symbol_1490
  INTEGER(w2f__i4) OpenAD_Symbol_1491
  REAL(w2f__8) OpenAD_Symbol_1492
  REAL(w2f__8) OpenAD_Symbol_1493
  INTEGER(w2f__i4) OpenAD_Symbol_1494
  REAL(w2f__8) OpenAD_Symbol_1495
  REAL(w2f__8) OpenAD_Symbol_1496
  INTEGER(w2f__i4) OpenAD_Symbol_1497
  REAL(w2f__8) OpenAD_Symbol_1498
  REAL(w2f__8) OpenAD_Symbol_1499
  INTEGER(w2f__i4) OpenAD_Symbol_1500
  REAL(w2f__8) OpenAD_Symbol_1501
  REAL(w2f__8) OpenAD_Symbol_1502
  REAL(w2f__8) OpenAD_aux_18
  REAL(w2f__8) OpenAD_aux_19
  REAL(w2f__8) OpenAD_aux_20
  REAL(w2f__8) OpenAD_aux_21
  REAL(w2f__8) OpenAD_lin_31
  REAL(w2f__8) OpenAD_lin_32
  REAL(w2f__8) OpenAD_lin_33
  REAL(w2f__8) OpenAD_lin_34
  REAL(w2f__8) OpenAD_lin_35
  REAL(w2f__8) OpenAD_lin_36
  REAL(w2f__8) OpenAD_lin_37
  REAL(w2f__8) OpenAD_lin_38
  type(active) :: OpenAD_prp_17
  type(active) :: OpenAD_prp_18
  type(active) :: OpenAD_prp_19
  type(active) :: OpenAD_prp_20
  type(active) :: OpenAD_prp_21
  type(active) :: OpenAD_prp_22
  type(active) :: OpenAD_prp_23
  type(active) :: OpenAD_prp_24
!
!       **** Statements ****
!


    integer iaddr
    external iaddr

   if (our_rev_mode%plain) then
! original function
  ITERADVT(1:INT(UPN))%v = 0.0D00
  DIAGADVT(1:INT(UPN))%v = 0.0D00
  IF (U(1)%v.GT.0.0D00) THEN
    DO UP = 1,UPN,1
      ITERADVT(INT(UP))%v = (U(UP)%v*(TEMPX(UP,1)%v+TEMPX(UP,2)%v*(-4.0D00)))
      DIAGADVT(INT(UP))%v = (U(UP)%v*3.0D00)
    END DO
  ELSE
    IF (U(1)%v.LT.0.0D00) THEN
      DO UP = 1,UPN,1
        ITERADVT(INT(UP))%v = (U(UP)%v*(TEMPX(UP,4)%v*4.0D00-TEMPX(UP,5)%v))
        DIAGADVT(INT(UP))%v = (U(UP)%v*(-3.0D00))
      END DO
    ENDIF
  ENDIF
  IF (V(1)%v.GT.0.0D00) THEN
    DO UP = 1,UPN,1
      ITERADVT(INT(UP))%v = (ITERADVT(UP)%v+V(UP)%v*(TEMPY(UP,1)%v+TEMPY(UP,2)%v&
     &*(-4.0D00)))

      DIAGADVT(INT(UP))%v = (DIAGADVT(UP)%v+V(UP)%v*3.0D00)
    END DO
  ELSE
    IF (V(1)%v.LT.0.0D00) THEN
      DO UP = 1,UPN,1
        ITERADVT(INT(UP))%v = (ITERADVT(UP)%v+V(UP)%v*(TEMPY(UP,4)%v*4.0D00-TEMP&
     &Y(UP,5)%v))

        DIAGADVT(INT(UP))%v = (DIAGADVT(UP)%v-V(UP)%v*3.0D00)
      END DO
    ENDIF
  ENDIF
    end if
    if (our_rev_mode%tape) then
! taping
  ITERADVT(1:INT(UPN))%v = 0.0D00
  DIAGADVT(1:INT(UPN))%v = 0.0D00
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = UPN
  oad_it_ptr = oad_it_ptr+1
  IF (U(1)%v.GT.0.0D00) THEN
    OpenAD_Symbol_475 = 0_w2f__i8
    DO UP = 1,UPN,1
      OpenAD_aux_18 = (TEMPX(UP,1)%v+TEMPX(UP,2)%v*(-4.0D00))
      OpenAD_lin_31 = OpenAD_aux_18
      OpenAD_lin_32 = U(UP)%v
      ITERADVT(INT(UP))%v = (U(UP)%v*OpenAD_aux_18)
      DIAGADVT(INT(UP))%v = (U(UP)%v*3.0D00)
      if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
      oad_dt(oad_dt_ptr) = OpenAD_lin_31
      oad_dt_ptr = oad_dt_ptr+1
      if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
      oad_dt(oad_dt_ptr) = OpenAD_lin_32
      oad_dt_ptr = oad_dt_ptr+1
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = UP
      oad_it_ptr = oad_it_ptr+1
      OpenAD_Symbol_475 = (INT(OpenAD_Symbol_475)+INT(1_w2f__i8))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_475
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_479 = 1_w2f__i8
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_479
    oad_it_ptr = oad_it_ptr+1
  ELSE
    IF (U(1)%v.LT.0.0D00) THEN
      OpenAD_Symbol_476 = 0_w2f__i8
      DO UP = 1,UPN,1
        OpenAD_aux_21 = (TEMPX(UP,4)%v*4.0D00-TEMPX(UP,5)%v)
        OpenAD_lin_37 = OpenAD_aux_21
        OpenAD_lin_38 = U(UP)%v
        ITERADVT(INT(UP))%v = (U(UP)%v*OpenAD_aux_21)
        DIAGADVT(INT(UP))%v = (U(UP)%v*(-3.0D00))
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_lin_37
        oad_dt_ptr = oad_dt_ptr+1
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_lin_38
        oad_dt_ptr = oad_dt_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = UP
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_476 = (INT(OpenAD_Symbol_476)+INT(1_w2f__i8))
      END DO
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = OpenAD_Symbol_476
      oad_it_ptr = oad_it_ptr+1
      OpenAD_Symbol_478 = 1_w2f__i8
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = OpenAD_Symbol_478
      oad_it_ptr = oad_it_ptr+1
    ELSE
      OpenAD_Symbol_477 = 0_w2f__i8
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = OpenAD_Symbol_477
      oad_it_ptr = oad_it_ptr+1
    ENDIF
    OpenAD_Symbol_480 = 0_w2f__i8
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_480
    oad_it_ptr = oad_it_ptr+1
  ENDIF
  IF (V(1)%v.GT.0.0D00) THEN
    OpenAD_Symbol_481 = 0_w2f__i8
    DO UP = 1,UPN,1
      OpenAD_aux_19 = (TEMPY(UP,1)%v+TEMPY(UP,2)%v*(-4.0D00))
      OpenAD_lin_33 = OpenAD_aux_19
      OpenAD_lin_34 = V(UP)%v
      ITERADVT(INT(UP))%v = (ITERADVT(UP)%v+V(UP)%v*OpenAD_aux_19)
      DIAGADVT(INT(UP))%v = (DIAGADVT(UP)%v+V(UP)%v*3.0D00)
      if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
      oad_dt(oad_dt_ptr) = OpenAD_lin_33
      oad_dt_ptr = oad_dt_ptr+1
      if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
      oad_dt(oad_dt_ptr) = OpenAD_lin_34
      oad_dt_ptr = oad_dt_ptr+1
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = UP
      oad_it_ptr = oad_it_ptr+1
      OpenAD_Symbol_481 = (INT(OpenAD_Symbol_481)+INT(1_w2f__i8))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_481
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_485 = 1_w2f__i8
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_485
    oad_it_ptr = oad_it_ptr+1
  ELSE
    IF (V(1)%v.LT.0.0D00) THEN
      OpenAD_Symbol_482 = 0_w2f__i8
      DO UP = 1,UPN,1
        OpenAD_aux_20 = (TEMPY(UP,4)%v*4.0D00-TEMPY(UP,5)%v)
        OpenAD_lin_35 = OpenAD_aux_20
        OpenAD_lin_36 = V(UP)%v
        ITERADVT(INT(UP))%v = (ITERADVT(UP)%v+V(UP)%v*OpenAD_aux_20)
        DIAGADVT(INT(UP))%v = (DIAGADVT(UP)%v-V(UP)%v*3.0D00)
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_lin_35
        oad_dt_ptr = oad_dt_ptr+1
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_lin_36
        oad_dt_ptr = oad_dt_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = UP
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_482 = (INT(OpenAD_Symbol_482)+INT(1_w2f__i8))
      END DO
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = OpenAD_Symbol_482
      oad_it_ptr = oad_it_ptr+1
      OpenAD_Symbol_484 = 1_w2f__i8
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = OpenAD_Symbol_484
      oad_it_ptr = oad_it_ptr+1
    ELSE
      OpenAD_Symbol_483 = 0_w2f__i8
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = OpenAD_Symbol_483
      oad_it_ptr = oad_it_ptr+1
    ENDIF
    OpenAD_Symbol_486 = 0_w2f__i8
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_486
    oad_it_ptr = oad_it_ptr+1
  ENDIF
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_463 = oad_it(oad_it_ptr)
  IF (OpenAD_Symbol_463.ne.0) THEN
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_467 = oad_it(oad_it_ptr)
    OpenAD_Symbol_468 = 1
    do while (INT(OpenAD_Symbol_468).LE.INT(OpenAD_Symbol_467))
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1494 = oad_it(oad_it_ptr)
      oad_dt_ptr = oad_dt_ptr-1
      OpenAD_Symbol_1495 = oad_dt(oad_dt_ptr)
      oad_dt_ptr = oad_dt_ptr-1
      OpenAD_Symbol_1496 = oad_dt(oad_dt_ptr)
      OpenAD_prp_20%d = OpenAD_prp_20%d+ITERADVT(OpenAD_Symbol_1494)%d*(OpenAD_S&
     &ymbol_1495)

      V(OpenAD_Symbol_1494)%d = V(OpenAD_Symbol_1494)%d+ITERADVT(OpenAD_Symbol_1&
     &494)%d*(OpenAD_Symbol_1496)

      OpenAD_prp_18%d = OpenAD_prp_18%d+ITERADVT(OpenAD_Symbol_1494)%d
      ITERADVT(OpenAD_Symbol_1494)%d = 0.0d0
      V(OpenAD_Symbol_1494)%d = V(OpenAD_Symbol_1494)%d+DIAGADVT(OpenAD_Symbol_1&
     &494)%d*(3.0D00)

      OpenAD_prp_19%d = OpenAD_prp_19%d+DIAGADVT(OpenAD_Symbol_1494)%d
      DIAGADVT(OpenAD_Symbol_1494)%d = 0.0d0
      TEMPY(OpenAD_Symbol_1494,2)%d = TEMPY(OpenAD_Symbol_1494,2)%d+OpenAD_prp_2&
     &0%d*(-4.0D00)

      TEMPY(OpenAD_Symbol_1494,1)%d = TEMPY(OpenAD_Symbol_1494,1)%d+OpenAD_prp_2&
     &0%d

      OpenAD_prp_20%d = 0.0d0
      DIAGADVT(OpenAD_Symbol_1494)%d = DIAGADVT(OpenAD_Symbol_1494)%d+OpenAD_prp&
     &_19%d

      OpenAD_prp_19%d = 0.0d0
      ITERADVT(OpenAD_Symbol_1494)%d = ITERADVT(OpenAD_Symbol_1494)%d+OpenAD_prp&
     &_18%d

      OpenAD_prp_18%d = 0.0d0
      OpenAD_Symbol_468 = INT(OpenAD_Symbol_468)+1
    END DO
  ELSE
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_464 = oad_it(oad_it_ptr)
    IF (OpenAD_Symbol_464.ne.0) THEN
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_465 = oad_it(oad_it_ptr)
      OpenAD_Symbol_466 = 1
      do while (INT(OpenAD_Symbol_466).LE.INT(OpenAD_Symbol_465))
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1497 = oad_it(oad_it_ptr)
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1498 = oad_dt(oad_dt_ptr)
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1499 = oad_dt(oad_dt_ptr)
        OpenAD_prp_23%d = OpenAD_prp_23%d+ITERADVT(OpenAD_Symbol_1497)%d*(OpenAD&
     &_Symbol_1498)

        V(OpenAD_Symbol_1497)%d = V(OpenAD_Symbol_1497)%d+ITERADVT(OpenAD_Symbol&
     &_1497)%d*(OpenAD_Symbol_1499)

        OpenAD_prp_21%d = OpenAD_prp_21%d+ITERADVT(OpenAD_Symbol_1497)%d
        ITERADVT(OpenAD_Symbol_1497)%d = 0.0d0
        V(OpenAD_Symbol_1497)%d = V(OpenAD_Symbol_1497)%d+DIAGADVT(OpenAD_Symbol&
     &_1497)%d*(-3.0D00)

        OpenAD_prp_22%d = OpenAD_prp_22%d+DIAGADVT(OpenAD_Symbol_1497)%d
        DIAGADVT(OpenAD_Symbol_1497)%d = 0.0d0
        TEMPY(OpenAD_Symbol_1497,4)%d = TEMPY(OpenAD_Symbol_1497,4)%d+OpenAD_prp&
     &_23%d*(4.0D00)

        TEMPY(OpenAD_Symbol_1497,5)%d = TEMPY(OpenAD_Symbol_1497,5)%d-OpenAD_prp&
     &_23%d

        OpenAD_prp_23%d = 0.0d0
        DIAGADVT(OpenAD_Symbol_1497)%d = DIAGADVT(OpenAD_Symbol_1497)%d+OpenAD_p&
     &rp_22%d

        OpenAD_prp_22%d = 0.0d0
        ITERADVT(OpenAD_Symbol_1497)%d = ITERADVT(OpenAD_Symbol_1497)%d+OpenAD_p&
     &rp_21%d

        OpenAD_prp_21%d = 0.0d0
        OpenAD_Symbol_466 = INT(OpenAD_Symbol_466)+1
      END DO
    ENDIF
  ENDIF
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_469 = oad_it(oad_it_ptr)
  IF (OpenAD_Symbol_469.ne.0) THEN
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_473 = oad_it(oad_it_ptr)
    OpenAD_Symbol_474 = 1
    do while (INT(OpenAD_Symbol_474).LE.INT(OpenAD_Symbol_473))
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1491 = oad_it(oad_it_ptr)
      oad_dt_ptr = oad_dt_ptr-1
      OpenAD_Symbol_1492 = oad_dt(oad_dt_ptr)
      oad_dt_ptr = oad_dt_ptr-1
      OpenAD_Symbol_1493 = oad_dt(oad_dt_ptr)
      OpenAD_prp_17%d = OpenAD_prp_17%d+ITERADVT(OpenAD_Symbol_1491)%d*(OpenAD_S&
     &ymbol_1492)

      U(OpenAD_Symbol_1491)%d = U(OpenAD_Symbol_1491)%d+ITERADVT(OpenAD_Symbol_1&
     &491)%d*(OpenAD_Symbol_1493)

      ITERADVT(OpenAD_Symbol_1491)%d = 0.0d0
      TEMPX(OpenAD_Symbol_1491,2)%d = TEMPX(OpenAD_Symbol_1491,2)%d+OpenAD_prp_1&
     &7%d*(-4.0D00)

      TEMPX(OpenAD_Symbol_1491,1)%d = TEMPX(OpenAD_Symbol_1491,1)%d+OpenAD_prp_1&
     &7%d

      OpenAD_prp_17%d = 0.0d0
      U(OpenAD_Symbol_1491)%d = U(OpenAD_Symbol_1491)%d+DIAGADVT(OpenAD_Symbol_1&
     &491)%d*(3.0D00)

      DIAGADVT(OpenAD_Symbol_1491)%d = 0.0d0
      OpenAD_Symbol_474 = INT(OpenAD_Symbol_474)+1
    END DO
  ELSE
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_470 = oad_it(oad_it_ptr)
    IF (OpenAD_Symbol_470.ne.0) THEN
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_471 = oad_it(oad_it_ptr)
      OpenAD_Symbol_472 = 1
      do while (INT(OpenAD_Symbol_472).LE.INT(OpenAD_Symbol_471))
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1500 = oad_it(oad_it_ptr)
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1501 = oad_dt(oad_dt_ptr)
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1502 = oad_dt(oad_dt_ptr)
        OpenAD_prp_24%d = OpenAD_prp_24%d+ITERADVT(OpenAD_Symbol_1500)%d*(OpenAD&
     &_Symbol_1501)

        U(OpenAD_Symbol_1500)%d = U(OpenAD_Symbol_1500)%d+ITERADVT(OpenAD_Symbol&
     &_1500)%d*(OpenAD_Symbol_1502)

        ITERADVT(OpenAD_Symbol_1500)%d = 0.0d0
        TEMPX(OpenAD_Symbol_1500,4)%d = TEMPX(OpenAD_Symbol_1500,4)%d+OpenAD_prp&
     &_24%d*(4.0D00)

        TEMPX(OpenAD_Symbol_1500,5)%d = TEMPX(OpenAD_Symbol_1500,5)%d-OpenAD_prp&
     &_24%d

        OpenAD_prp_24%d = 0.0d0
        U(OpenAD_Symbol_1500)%d = U(OpenAD_Symbol_1500)%d+DIAGADVT(OpenAD_Symbol&
     &_1500)%d*(-3.0D00)

        DIAGADVT(OpenAD_Symbol_1500)%d = 0.0d0
        OpenAD_Symbol_472 = INT(OpenAD_Symbol_472)+1
      END DO
    ENDIF
  ENDIF
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1490 = oad_it(oad_it_ptr)
  DIAGADVT(1:OpenAD_Symbol_1490)%d = 0.0d0
  ITERADVT(1:OpenAD_Symbol_1490)%d = 0.0d0
    end if
  end subroutine HADVPNT
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE HADVALL(INITADVT, TEMP, THCK, THKLIM, HADV_U, HADV_V , EWN, NSN, UP&
     &N)

    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  use glimmer_global ,only: DP
  use glimmer_global ,only: DP
  use glimmer_global ,only: DP
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_431
  INTEGER(w2f__i8) OpenAD_Symbol_432
  INTEGER(w2f__i8) OpenAD_Symbol_433
  INTEGER(w2f__i8) OpenAD_Symbol_434
  INTEGER(w2f__i8) OpenAD_Symbol_435
  INTEGER(w2f__i8) OpenAD_Symbol_436
  INTEGER(w2f__i8) OpenAD_Symbol_437
  INTEGER(w2f__i8) OpenAD_Symbol_438
  INTEGER(w2f__i8) OpenAD_Symbol_439
  INTEGER(w2f__i8) OpenAD_Symbol_440
  INTEGER(w2f__i8) OpenAD_Symbol_441
  INTEGER(w2f__i8) OpenAD_Symbol_442
  INTEGER(w2f__i8) OpenAD_Symbol_443
  INTEGER(w2f__i8) OpenAD_Symbol_444
  INTEGER(w2f__i8) OpenAD_Symbol_445
  INTEGER(w2f__i8) OpenAD_Symbol_446
  INTEGER(w2f__i8) OpenAD_Symbol_447
  INTEGER(w2f__i8) OpenAD_Symbol_448
  INTEGER(w2f__i8) OpenAD_Symbol_449
  INTEGER(w2f__i8) OpenAD_Symbol_450
  INTEGER(w2f__i8) OpenAD_Symbol_451
  INTEGER(w2f__i8) OpenAD_Symbol_452
  INTEGER(w2f__i8) OpenAD_Symbol_453
  INTEGER(w2f__i8) OpenAD_Symbol_454
  INTEGER(w2f__i8) OpenAD_Symbol_455
  INTEGER(w2f__i8) OpenAD_Symbol_456
  INTEGER(w2f__i8) OpenAD_Symbol_457
  INTEGER(w2f__i8) OpenAD_Symbol_458
  INTEGER(w2f__i8) OpenAD_Symbol_459
  INTEGER(w2f__i8) OpenAD_Symbol_460
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) THKLIM
  INTEGER(w2f__i4) EWN
  INTEGER(w2f__i4) NSN
  INTEGER(w2f__i4) UPN
  type(active) :: INITADVT(1:UPN,1:EWN,1:NSN)
  type(active) :: TEMP(1:UPN,0:INT((EWN+1)),0:INT((NSN+1)))
  REAL(w2f__8) THCK(1 : EWN, 1 : NSN)
  type(active) :: HADV_U(1:UPN,1:EWN,1:NSN)
  type(active) :: HADV_V(1:UPN,1:EWN,1:NSN)
!
!       **** Local Variables and Functions ****
!
  REAL(w2f__8) DIAGADVT(1 : UPN)
  INTEGER(w2f__i4) EW
  INTEGER(w2f__i4) I1
  INTEGER(w2f__i4) I2
  INTEGER(w2f__i4) NS
  type(active) :: TEMPX(1:UPN,1:5)
  type(active) :: TEMPY(1:UPN,1:5)
  INTEGER(w2f__i4) OpenAD_Symbol_1481
  INTEGER(w2f__i4) OpenAD_Symbol_1482
  INTEGER(w2f__i4) OpenAD_Symbol_1483
  INTEGER(w2f__i4) OpenAD_Symbol_1484
  INTEGER(w2f__i4) OpenAD_Symbol_1485
  INTEGER(w2f__i4) OpenAD_Symbol_1486
  INTEGER(w2f__i4) OpenAD_Symbol_1487
  INTEGER(w2f__i4) OpenAD_Symbol_1488
  INTEGER(w2f__i4) OpenAD_Symbol_1489
  INTEGER(w2f__i4) OpenAD_Symbol_461
  INTEGER(w2f__i4) OpenAD_Symbol_462
  type(active) :: OpenAD_tyc_7(:)
  ALLOCATABLE OpenAD_tyc_7
  type(active) :: OpenAD_tyc_71(:)
  ALLOCATABLE OpenAD_tyc_71
!
!       **** Statements ****
!


    integer iaddr
    external iaddr

   if (our_rev_mode%plain) then
! original function
  INITADVT(1:INT(UPN),1:INT(EWN),1:INT(NSN))%v = 0.0D00
  DO NS = 2,(NSN+(-1)),1
    DO EW = 2,(EWN+(-1)),1
      IF (THCK(EW,NS).GT.THKLIM) THEN
        DO I1 = 1,5,1
          DO I2 = 1,UPN,1
            TEMPX(INT(I2),INT(I1))%v = TEMP(I2,EW+I1+(-3),NS)%v
            TEMPY(INT(I2),INT(I1))%v = TEMP(I2,EW,I1+NS+(-3))%v
          END DO
        END DO
!!! requested inline of 'oad_AllocateMatching' has no defn
        CALL oad_AllocateMatching(OpenAD_tyc_7,DIAGADVT)
        CALL HADVPNT(INITADVT(1:UPN,EW,NS),OpenAD_tyc_7,TEMPX,TEMPY,HADV_U(1:UPN&
     &,EW,NS),HADV_V(1:UPN,EW,NS),UPN)

!!! requested inline of 'oad_ShapeTest' has no defn
        CALL oad_ShapeTest(OpenAD_tyc_7,DIAGADVT)
!!! requested inline of 'oad_convert' has no defn
        CALL oad_convert(DIAGADVT,OpenAD_tyc_7)
      ENDIF
    END DO
  END DO
    end if
    if (our_rev_mode%tape) then
! taping
  INITADVT(1:INT(UPN),1:INT(EWN),1:INT(NSN))%v = 0.0D00
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = UPN
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = EWN
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = NSN
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_440 = 0_w2f__i8
  DO NS = 2,(NSN+(-1)),1
    OpenAD_Symbol_441 = 0_w2f__i8
    DO EW = 2,(EWN+(-1)),1
      IF (THCK(EW,NS).GT.THKLIM) THEN
        OpenAD_Symbol_442 = 0_w2f__i8
        DO I1 = 1,5,1
          OpenAD_Symbol_443 = 0_w2f__i8
          DO I2 = 1,UPN,1
            TEMPX(INT(I2),INT(I1))%v = TEMP(I2,EW+I1+(-3),NS)%v
            TEMPY(INT(I2),INT(I1))%v = TEMP(I2,EW,I1+NS+(-3))%v
            OpenAD_Symbol_461 = (EW+I1+(-3))
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = OpenAD_Symbol_461
            oad_it_ptr = oad_it_ptr+1
            OpenAD_Symbol_462 = (I1+NS+(-3))
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = OpenAD_Symbol_462
            oad_it_ptr = oad_it_ptr+1
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = I2
            oad_it_ptr = oad_it_ptr+1
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = NS
            oad_it_ptr = oad_it_ptr+1
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = I1
            oad_it_ptr = oad_it_ptr+1
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = EW
            oad_it_ptr = oad_it_ptr+1
            OpenAD_Symbol_443 = (INT(OpenAD_Symbol_443)+INT(1_w2f__i8))
          END DO
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_443
          oad_it_ptr = oad_it_ptr+1
          OpenAD_Symbol_442 = (INT(OpenAD_Symbol_442)+INT(1_w2f__i8))
        END DO
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_442
        oad_it_ptr = oad_it_ptr+1
!!! requested inline of 'oad_AllocateMatching' has no defn
        CALL oad_AllocateMatching(OpenAD_tyc_7,DIAGADVT)
        CALL HADVPNT(INITADVT(1:UPN,EW,NS),OpenAD_tyc_7,TEMPX,TEMPY,HADV_U(1:UPN&
     &,EW,NS),HADV_V(1:UPN,EW,NS),UPN)

!!! requested inline of 'oad_ShapeTest' has no defn
        CALL oad_ShapeTest(OpenAD_tyc_7,DIAGADVT)
!!! requested inline of 'oad_convert' has no defn
        CALL oad_convert(DIAGADVT,OpenAD_tyc_7)
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = UPN
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_444 = 1_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_444
        oad_it_ptr = oad_it_ptr+1
      ELSE
        OpenAD_Symbol_445 = 0_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_445
        oad_it_ptr = oad_it_ptr+1
      ENDIF
      OpenAD_Symbol_441 = (INT(OpenAD_Symbol_441)+INT(1_w2f__i8))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_441
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_440 = (INT(OpenAD_Symbol_440)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_440
  oad_it_ptr = oad_it_ptr+1
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_431 = oad_it(oad_it_ptr)
  OpenAD_Symbol_432 = 1
  do while (INT(OpenAD_Symbol_432).LE.INT(OpenAD_Symbol_431))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_433 = oad_it(oad_it_ptr)
    OpenAD_Symbol_434 = 1
    do while (INT(OpenAD_Symbol_434).LE.INT(OpenAD_Symbol_433))
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_435 = oad_it(oad_it_ptr)
      IF (OpenAD_Symbol_435.ne.0) THEN
        oad_it_ptr = oad_it_ptr-1
        NS = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        EW = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        UPN = oad_it(oad_it_ptr)
!!! requested inline of 'oad_AllocateMatching' has no defn
        CALL oad_AllocateMatching(OpenAD_tyc_71,DIAGADVT)
        CALL HADVPNT(INITADVT(1:UPN,EW,NS),OpenAD_tyc_71,TEMPX,TEMPY,HADV_U(1:UP&
     &N,EW,NS),HADV_V(1:UPN,EW,NS),UPN)

        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_436 = oad_it(oad_it_ptr)
        OpenAD_Symbol_437 = 1
        do while (INT(OpenAD_Symbol_437).LE.INT(OpenAD_Symbol_436))
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_438 = oad_it(oad_it_ptr)
          OpenAD_Symbol_439 = 1
          do while (INT(OpenAD_Symbol_439).LE.INT(OpenAD_Symbol_438))
            oad_it_ptr = oad_it_ptr-1
            OpenAD_Symbol_1484 = oad_it(oad_it_ptr)
            oad_it_ptr = oad_it_ptr-1
            OpenAD_Symbol_1485 = oad_it(oad_it_ptr)
            oad_it_ptr = oad_it_ptr-1
            OpenAD_Symbol_1486 = oad_it(oad_it_ptr)
            oad_it_ptr = oad_it_ptr-1
            OpenAD_Symbol_1487 = oad_it(oad_it_ptr)
            oad_it_ptr = oad_it_ptr-1
            OpenAD_Symbol_1488 = oad_it(oad_it_ptr)
            TEMP(OpenAD_Symbol_1487,OpenAD_Symbol_1484,OpenAD_Symbol_1488)%d = T&
     &EMP(OpenAD_Symbol_1487,OpenAD_Symbol_1484,OpenAD_Symbol_1488)%d+TEMPY(Open&
     &AD_Symbol_1487,OpenAD_Symbol_1485)%d

            TEMPY(OpenAD_Symbol_1487,OpenAD_Symbol_1485)%d = 0.0d0
            oad_it_ptr = oad_it_ptr-1
            OpenAD_Symbol_1489 = oad_it(oad_it_ptr)
            TEMP(OpenAD_Symbol_1487,OpenAD_Symbol_1489,OpenAD_Symbol_1486)%d = T&
     &EMP(OpenAD_Symbol_1487,OpenAD_Symbol_1489,OpenAD_Symbol_1486)%d+TEMPX(Open&
     &AD_Symbol_1487,OpenAD_Symbol_1485)%d

            TEMPX(OpenAD_Symbol_1487,OpenAD_Symbol_1485)%d = 0.0d0
            OpenAD_Symbol_439 = INT(OpenAD_Symbol_439)+1
          END DO
          OpenAD_Symbol_437 = INT(OpenAD_Symbol_437)+1
        END DO
      ENDIF
      OpenAD_Symbol_434 = INT(OpenAD_Symbol_434)+1
    END DO
    OpenAD_Symbol_432 = INT(OpenAD_Symbol_432)+1
  END DO
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1481 = oad_it(oad_it_ptr)
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1482 = oad_it(oad_it_ptr)
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1483 = oad_it(oad_it_ptr)
  INITADVT(1:OpenAD_Symbol_1483,1:OpenAD_Symbol_1482,1:OpenAD_Symbol_1481)%d = 0&
     &.0d0

    end if
  end subroutine HADVALL
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE FINDVTRI(NZ, DUPS, DUPN, THCK, SUBD, DIAG, SUPD, DIAGADVT, WEFF, FL&
     &OAT, CONS1, CONS2)

    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  use glimmer_global ,only: DP
  use glimmer_global ,only: DP
  use glimmer_global ,only: DP
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_511
  INTEGER(w2f__i8) OpenAD_Symbol_512
  INTEGER(w2f__i8) OpenAD_Symbol_513
  INTEGER(w2f__i8) OpenAD_Symbol_514
  INTEGER(w2f__i8) OpenAD_Symbol_515
  INTEGER(w2f__i8) OpenAD_Symbol_516
  INTEGER(w2f__i8) OpenAD_Symbol_517
  INTEGER(w2f__i8) OpenAD_Symbol_518
  INTEGER(w2f__i8) OpenAD_Symbol_519
  INTEGER(w2f__i8) OpenAD_Symbol_520
  INTEGER(w2f__i8) OpenAD_Symbol_521
  INTEGER(w2f__i8) OpenAD_Symbol_522
  INTEGER(w2f__i8) OpenAD_Symbol_523
  INTEGER(w2f__i8) OpenAD_Symbol_524
  INTEGER(w2f__i8) OpenAD_Symbol_525
  INTEGER(w2f__i8) OpenAD_Symbol_526
  INTEGER(w2f__i8) OpenAD_Symbol_527
  INTEGER(w2f__i8) OpenAD_Symbol_528
!
!       **** Parameters and Result ****
!
  INTEGER(w2f__i4) NZ
  REAL(w2f__8) DUPS(1 : NZ, 1 : 3)
  REAL(w2f__8) DUPN
  type(active) :: THCK
  type(active) :: SUBD(1:)
  type(active) :: DIAG(1:)
  type(active) :: SUPD(1:)
  type(active) :: DIAGADVT(1:)
  type(active) :: WEFF(1:)
  LOGICAL(w2f__i4) FLOAT
  REAL(w2f__8) CONS1
  REAL(w2f__8) CONS2
!
!       **** Local Variables and Functions ****
!
  type(active) :: ADV_FACTOR
  type(active) :: DIFF_FACTOR
  INTEGER(w2f__i4) I
  REAL(w2f__8) OpenAD_Symbol_1503
  REAL(w2f__8) OpenAD_Symbol_1504
  INTEGER(w2f__i4) OpenAD_Symbol_1505
  REAL(w2f__8) OpenAD_Symbol_1506
  REAL(w2f__8) OpenAD_Symbol_1507
  INTEGER(w2f__i4) OpenAD_Symbol_1508
  REAL(w2f__8) OpenAD_Symbol_1509
  REAL(w2f__8) OpenAD_Symbol_1510
  INTEGER(w2f__i4) OpenAD_Symbol_1511
  INTEGER(w2f__i4) OpenAD_Symbol_1512
  INTEGER(w2f__i4) OpenAD_Symbol_1513
  INTEGER(w2f__i4) OpenAD_Symbol_1514
  REAL(w2f__8) OpenAD_Symbol_1515
  REAL(w2f__8) OpenAD_acc_14
  REAL(w2f__8) OpenAD_acc_15
  REAL(w2f__8) OpenAD_acc_16
  REAL(w2f__8) OpenAD_acc_17
  REAL(w2f__8) OpenAD_acc_18
  REAL(w2f__8) OpenAD_acc_19
  REAL(w2f__8) OpenAD_aux_22
  REAL(w2f__8) OpenAD_aux_23
  REAL(w2f__8) OpenAD_aux_24
  REAL(w2f__8) OpenAD_aux_25
  REAL(w2f__8) OpenAD_lin_39
  REAL(w2f__8) OpenAD_lin_40
  REAL(w2f__8) OpenAD_lin_41
  REAL(w2f__8) OpenAD_lin_42
  REAL(w2f__8) OpenAD_lin_43
  REAL(w2f__8) OpenAD_lin_44
  REAL(w2f__8) OpenAD_lin_45
  REAL(w2f__8) OpenAD_lin_46
  REAL(w2f__8) OpenAD_lin_47
  type(active) :: OpenAD_prp_25
  type(active) :: OpenAD_prp_26
!
!       **** Statements ****
!


    integer iaddr
    external iaddr

   if (our_rev_mode%plain) then
! original function
  DIFF_FACTOR%v = (CONS1/(THCK%v**2))
  ADV_FACTOR%v = (CONS2/THCK%v)
  DO I = 2,(NZ+(-1)),1
    SUBD(INT(I))%v = (DUPS(I,3)*WEFF(I)%v*ADV_FACTOR%v)
    SUPD(INT(I))%v = (-(SUBD(I)%v+DUPS(I,2)*DIFF_FACTOR%v))
    SUBD(INT(I))%v = (SUBD(I)%v-DUPS(I,1)*DIFF_FACTOR%v)
  END DO
  DO I = 2,(NZ+(-1)),1
    DIAG(INT(I))%v = (DIAGADVT(I)%v+1.0D00-SUBD(I)%v-SUPD(I)%v)
  END DO
  SUPD(1)%v = 0.0D00
  SUBD(1)%v = 0.0D00
  DIAG(1)%v = 1.0D00
  IF(FLOAT) THEN
    SUPD(INT(NZ))%v = 0.0D00
    SUBD(INT(NZ))%v = 0.0D00
    DIAG(INT(NZ))%v = 1.0D00
  ELSE
    SUPD(INT(NZ))%v = 0.0D00
    SUBD(INT(NZ))%v = (-((DIFF_FACTOR%v*5.0D-01)/(DUPN**2)))
    DIAG(INT(NZ))%v = (DIAGADVT(NZ)%v+1.0D00-SUBD(NZ)%v)
  ENDIF
    end if
    if (our_rev_mode%tape) then
! taping
  OpenAD_aux_22 = (THCK%v**2)
  OpenAD_lin_40 = (2*(THCK%v**(2-INT(1_w2f__i8))))
  OpenAD_lin_39 = (-(CONS1/(OpenAD_aux_22*OpenAD_aux_22)))
  DIFF_FACTOR%v = (CONS1/OpenAD_aux_22)
  OpenAD_lin_41 = (-(CONS2/(THCK%v*THCK%v)))
  ADV_FACTOR%v = (CONS2/THCK%v)
  OpenAD_acc_14 = (OpenAD_lin_40*OpenAD_lin_39)
  if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
  oad_dt(oad_dt_ptr) = OpenAD_lin_41
  oad_dt_ptr = oad_dt_ptr+1
  if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
  oad_dt(oad_dt_ptr) = OpenAD_acc_14
  oad_dt_ptr = oad_dt_ptr+1
  OpenAD_Symbol_516 = 0_w2f__i8
  DO I = 2,(NZ+(-1)),1
    OpenAD_aux_23 = (WEFF(I)%v*ADV_FACTOR%v)
    OpenAD_lin_43 = ADV_FACTOR%v
    OpenAD_lin_44 = WEFF(I)%v
    OpenAD_lin_42 = DUPS(I,3)
    SUBD(INT(I))%v = (DUPS(I,3)*OpenAD_aux_23)
    OpenAD_acc_15 = (OpenAD_lin_43*OpenAD_lin_42)
    OpenAD_acc_16 = (OpenAD_lin_44*OpenAD_lin_42)
    if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
    oad_dt(oad_dt_ptr) = OpenAD_acc_15
    oad_dt_ptr = oad_dt_ptr+1
    if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
    oad_dt(oad_dt_ptr) = OpenAD_acc_16
    oad_dt_ptr = oad_dt_ptr+1
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = I
    oad_it_ptr = oad_it_ptr+1
    OpenAD_lin_45 = DUPS(I,2)
    SUPD(INT(I))%v = (-(SUBD(I)%v+DUPS(I,2)*DIFF_FACTOR%v))
    OpenAD_lin_46 = DUPS(I,1)
    SUBD(INT(I))%v = (SUBD(I)%v-DUPS(I,1)*DIFF_FACTOR%v)
    OpenAD_acc_17 = (OpenAD_lin_45*INT((-1_w2f__i8)))
    OpenAD_acc_18 = (OpenAD_lin_46*INT((-1_w2f__i8)))
    if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
    oad_dt(oad_dt_ptr) = OpenAD_acc_18
    oad_dt_ptr = oad_dt_ptr+1
    if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
    oad_dt(oad_dt_ptr) = OpenAD_acc_17
    oad_dt_ptr = oad_dt_ptr+1
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = I
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_516 = (INT(OpenAD_Symbol_516)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_516
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_517 = 0_w2f__i8
  DO I = 2,(NZ+(-1)),1
    DIAG(INT(I))%v = (DIAGADVT(I)%v+1.0D00-SUBD(I)%v-SUPD(I)%v)
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = I
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_517 = (INT(OpenAD_Symbol_517)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_517
  oad_it_ptr = oad_it_ptr+1
  SUPD(1)%v = 0.0D00
  SUBD(1)%v = 0.0D00
  DIAG(1)%v = 1.0D00
  IF(FLOAT) THEN
    SUPD(INT(NZ))%v = 0.0D00
    SUBD(INT(NZ))%v = 0.0D00
    DIAG(INT(NZ))%v = 1.0D00
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = NZ
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_518 = 1_w2f__i8
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_518
    oad_it_ptr = oad_it_ptr+1
  ELSE
    SUPD(INT(NZ))%v = 0.0D00
    OpenAD_aux_24 = (DIFF_FACTOR%v*5.0D-01)
    OpenAD_aux_25 = (DUPN**2)
    OpenAD_lin_47 = (INT(1_w2f__i8)/OpenAD_aux_25)
    SUBD(INT(NZ))%v = (-(OpenAD_aux_24/OpenAD_aux_25))
    OpenAD_acc_19 = (5.0D-01*OpenAD_lin_47*INT((-1_w2f__i8)))
    if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
    oad_dt(oad_dt_ptr) = OpenAD_acc_19
    oad_dt_ptr = oad_dt_ptr+1
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = NZ
    oad_it_ptr = oad_it_ptr+1
    DIAG(INT(NZ))%v = (DIAGADVT(NZ)%v+1.0D00-SUBD(NZ)%v)
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = NZ
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_519 = 0_w2f__i8
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_519
    oad_it_ptr = oad_it_ptr+1
  ENDIF
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_511 = oad_it(oad_it_ptr)
  IF (OpenAD_Symbol_511.ne.0) THEN
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1512 = oad_it(oad_it_ptr)
    DIAG(OpenAD_Symbol_1512)%d = 0.0d0
    SUBD(OpenAD_Symbol_1512)%d = 0.0d0
    SUPD(OpenAD_Symbol_1512)%d = 0.0d0
  ELSE
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1513 = oad_it(oad_it_ptr)
    SUBD(OpenAD_Symbol_1513)%d = SUBD(OpenAD_Symbol_1513)%d-DIAG(OpenAD_Symbol_1&
     &513)%d

    DIAGADVT(OpenAD_Symbol_1513)%d = DIAGADVT(OpenAD_Symbol_1513)%d+DIAG(OpenAD_&
     &Symbol_1513)%d

    DIAG(OpenAD_Symbol_1513)%d = 0.0d0
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1514 = oad_it(oad_it_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1515 = oad_dt(oad_dt_ptr)
    DIFF_FACTOR%d = DIFF_FACTOR%d+SUBD(OpenAD_Symbol_1514)%d*(OpenAD_Symbol_1515&
     &)

    SUBD(OpenAD_Symbol_1514)%d = 0.0d0
    SUPD(OpenAD_Symbol_1514)%d = 0.0d0
  ENDIF
  DIAG(1)%d = 0.0d0
  SUBD(1)%d = 0.0d0
  SUPD(1)%d = 0.0d0
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_512 = oad_it(oad_it_ptr)
  OpenAD_Symbol_513 = 1
  do while (INT(OpenAD_Symbol_513).LE.INT(OpenAD_Symbol_512))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1511 = oad_it(oad_it_ptr)
    SUBD(OpenAD_Symbol_1511)%d = SUBD(OpenAD_Symbol_1511)%d-DIAG(OpenAD_Symbol_1&
     &511)%d

    SUPD(OpenAD_Symbol_1511)%d = SUPD(OpenAD_Symbol_1511)%d-DIAG(OpenAD_Symbol_1&
     &511)%d

    DIAGADVT(OpenAD_Symbol_1511)%d = DIAGADVT(OpenAD_Symbol_1511)%d+DIAG(OpenAD_&
     &Symbol_1511)%d

    DIAG(OpenAD_Symbol_1511)%d = 0.0d0
    OpenAD_Symbol_513 = INT(OpenAD_Symbol_513)+1
  END DO
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_514 = oad_it(oad_it_ptr)
  OpenAD_Symbol_515 = 1
  do while (INT(OpenAD_Symbol_515).LE.INT(OpenAD_Symbol_514))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1505 = oad_it(oad_it_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1506 = oad_dt(oad_dt_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1507 = oad_dt(oad_dt_ptr)
    DIFF_FACTOR%d = DIFF_FACTOR%d+SUPD(OpenAD_Symbol_1505)%d*(OpenAD_Symbol_1506&
     &)

    OpenAD_prp_25%d = OpenAD_prp_25%d-SUPD(OpenAD_Symbol_1505)%d
    SUPD(OpenAD_Symbol_1505)%d = 0.0d0
    DIFF_FACTOR%d = DIFF_FACTOR%d+SUBD(OpenAD_Symbol_1505)%d*(OpenAD_Symbol_1507&
     &)

    OpenAD_prp_26%d = OpenAD_prp_26%d+SUBD(OpenAD_Symbol_1505)%d
    SUBD(OpenAD_Symbol_1505)%d = 0.0d0
    SUBD(OpenAD_Symbol_1505)%d = SUBD(OpenAD_Symbol_1505)%d+OpenAD_prp_26%d
    OpenAD_prp_26%d = 0.0d0
    SUBD(OpenAD_Symbol_1505)%d = SUBD(OpenAD_Symbol_1505)%d+OpenAD_prp_25%d
    OpenAD_prp_25%d = 0.0d0
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1508 = oad_it(oad_it_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1509 = oad_dt(oad_dt_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1510 = oad_dt(oad_dt_ptr)
    ADV_FACTOR%d = ADV_FACTOR%d+SUBD(OpenAD_Symbol_1508)%d*(OpenAD_Symbol_1509)
    WEFF(OpenAD_Symbol_1508)%d = WEFF(OpenAD_Symbol_1508)%d+SUBD(OpenAD_Symbol_1&
     &508)%d*(OpenAD_Symbol_1510)

    SUBD(OpenAD_Symbol_1508)%d = 0.0d0
    OpenAD_Symbol_515 = INT(OpenAD_Symbol_515)+1
  END DO
  oad_dt_ptr = oad_dt_ptr-1
  OpenAD_Symbol_1503 = oad_dt(oad_dt_ptr)
  oad_dt_ptr = oad_dt_ptr-1
  OpenAD_Symbol_1504 = oad_dt(oad_dt_ptr)
  THCK%d = THCK%d+DIFF_FACTOR%d*(OpenAD_Symbol_1503)
  DIFF_FACTOR%d = 0.0d0
  THCK%d = THCK%d+ADV_FACTOR%d*(OpenAD_Symbol_1504)
  ADV_FACTOR%d = 0.0d0
    end if
  end subroutine FINDVTRI
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE FINDVTRI_INIT(NZ, DUPN, INITADVT, DISSIP, INITTEMP, BHEATFLX, DUSRF&
     &DEW, DUSRFDNS, EW, NS, SUBD, DIAG, SUPD, WEFF, UBAS, VBAS, TEMP, THCK, FLO&
     &AT, CONS3, CONS4, SLIDEF1, SLIDEF2)

    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  use glimmer_global ,only: DP
  use glimmer_pmpt
  use glimmer_global ,only: DP
  use glimmer_pmpt
  use glimmer_global ,only: DP
  use glimmer_pmpt
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_529
  INTEGER(w2f__i8) OpenAD_Symbol_530
  INTEGER(w2f__i8) OpenAD_Symbol_531
  INTEGER(w2f__i8) OpenAD_Symbol_532
  INTEGER(w2f__i8) OpenAD_Symbol_533
  INTEGER(w2f__i8) OpenAD_Symbol_534
  INTEGER(w2f__i8) OpenAD_Symbol_535
  INTEGER(w2f__i8) OpenAD_Symbol_536
  INTEGER(w2f__i8) OpenAD_Symbol_537
  INTEGER(w2f__i8) OpenAD_Symbol_538
  INTEGER(w2f__i8) OpenAD_Symbol_539
  INTEGER(w2f__i8) OpenAD_Symbol_540
  INTEGER(w2f__i8) OpenAD_Symbol_541
  INTEGER(w2f__i8) OpenAD_Symbol_542
  INTEGER(w2f__i8) OpenAD_Symbol_543
  INTEGER(w2f__i8) OpenAD_Symbol_544
  INTEGER(w2f__i8) OpenAD_Symbol_545
  INTEGER(w2f__i8) OpenAD_Symbol_546
  INTEGER(w2f__i8) OpenAD_Symbol_547
  INTEGER(w2f__i8) OpenAD_Symbol_548
  INTEGER(w2f__i8) OpenAD_Symbol_549
  INTEGER(w2f__i8) OpenAD_Symbol_550
  INTEGER(w2f__i8) OpenAD_Symbol_551
  INTEGER(w2f__i8) OpenAD_Symbol_552
  INTEGER(w2f__i8) OpenAD_Symbol_553
  INTEGER(w2f__i8) OpenAD_Symbol_554
  INTEGER(w2f__i8) OpenAD_Symbol_555
  INTEGER(w2f__i8) OpenAD_Symbol_556
  INTEGER(w2f__i8) OpenAD_Symbol_557
  INTEGER(w2f__i8) OpenAD_Symbol_558
  INTEGER(w2f__i8) OpenAD_Symbol_559
  INTEGER(w2f__i8) OpenAD_Symbol_560
  INTEGER(w2f__i8) OpenAD_Symbol_561
  INTEGER(w2f__i8) OpenAD_Symbol_562
  INTEGER(w2f__i8) OpenAD_Symbol_563
  INTEGER(w2f__i8) OpenAD_Symbol_564
!
!       **** Parameters and Result ****
!
  INTEGER(w2f__i4) NZ
  REAL(w2f__8) DUPN
  type(active) :: INITADVT(1:,1:,1:)
  type(active) :: DISSIP(1:,1:,1:)
  type(active) :: INITTEMP(1:,1:,1:)
  REAL(w2f__8) BHEATFLX(1 :, 1 :)
  type(active) :: DUSRFDEW(1:,1:)
  type(active) :: DUSRFDNS(1:,1:)
  INTEGER(w2f__i4) EW
  INTEGER(w2f__i4) NS
  type(active) :: SUBD(1:)
  type(active) :: DIAG(1:)
  type(active) :: SUPD(1:)
  type(active) :: WEFF(1:)
  type(active) :: UBAS(1:,1:)
  type(active) :: VBAS(1:,1:)
  type(active) :: TEMP(1:)
  type(active) :: THCK
  LOGICAL(w2f__i4) FLOAT
  REAL(w2f__8) CONS3
  REAL(w2f__8) CONS4
  REAL(w2f__8) SLIDEF1
  REAL(w2f__8) SLIDEF2
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) EWP
  INTEGER(w2f__i4) NSP
  INTEGER(w2f__i4) SLIDE_COUNT
  type(active) :: SLTERM
  INTEGER(w2f__i4) UP
  INTEGER(w2f__i4) OpenAD_Symbol_1516
  INTEGER(w2f__i4) OpenAD_Symbol_1517
  INTEGER(w2f__i4) OpenAD_Symbol_1518
  REAL(w2f__8) OpenAD_Symbol_1519
  REAL(w2f__8) OpenAD_Symbol_1520
  REAL(w2f__8) OpenAD_Symbol_1521
  REAL(w2f__8) OpenAD_Symbol_1522
  REAL(w2f__8) OpenAD_Symbol_1523
  REAL(w2f__8) OpenAD_Symbol_1524
  INTEGER(w2f__i4) OpenAD_Symbol_1525
  INTEGER(w2f__i4) OpenAD_Symbol_1526
  INTEGER(w2f__i4) OpenAD_Symbol_1527
  INTEGER(w2f__i4) OpenAD_Symbol_1528
  REAL(w2f__8) OpenAD_Symbol_1529
  REAL(w2f__8) OpenAD_Symbol_1530
  REAL(w2f__8) OpenAD_Symbol_1531
  REAL(w2f__8) OpenAD_Symbol_1532
  INTEGER(w2f__i4) OpenAD_Symbol_1533
  INTEGER(w2f__i4) OpenAD_Symbol_1534
  INTEGER(w2f__i4) OpenAD_Symbol_1535
  REAL(w2f__8) OpenAD_Symbol_1536
  REAL(w2f__8) OpenAD_Symbol_1537
  REAL(w2f__8) OpenAD_Symbol_1538
  REAL(w2f__8) OpenAD_Symbol_1539
  REAL(w2f__8) OpenAD_Symbol_1540
  REAL(w2f__8) OpenAD_Symbol_1541
  REAL(w2f__8) OpenAD_Symbol_1542
  REAL(w2f__8) OpenAD_Symbol_1543
  REAL(w2f__8) OpenAD_Symbol_1544
  REAL(w2f__8) OpenAD_Symbol_1545
  INTEGER(w2f__i4) OpenAD_Symbol_1546
  INTEGER(w2f__i4) OpenAD_Symbol_565
  INTEGER(w2f__i4) OpenAD_Symbol_566
  INTEGER(w2f__i4) OpenAD_Symbol_567
  REAL(w2f__8) OpenAD_acc_20
  REAL(w2f__8) OpenAD_acc_21
  REAL(w2f__8) OpenAD_acc_22
  REAL(w2f__8) OpenAD_acc_23
  REAL(w2f__8) OpenAD_acc_24
  REAL(w2f__8) OpenAD_acc_25
  REAL(w2f__8) OpenAD_acc_26
  REAL(w2f__8) OpenAD_acc_27
  REAL(w2f__8) OpenAD_acc_28
  REAL(w2f__8) OpenAD_acc_29
  REAL(w2f__8) OpenAD_acc_30
  REAL(w2f__8) OpenAD_acc_31
  REAL(w2f__8) OpenAD_acc_32
  REAL(w2f__8) OpenAD_acc_33
  REAL(w2f__8) OpenAD_aux_26
  REAL(w2f__8) OpenAD_aux_27
  REAL(w2f__8) OpenAD_aux_28
  REAL(w2f__8) OpenAD_aux_29
  REAL(w2f__8) OpenAD_aux_30
  REAL(w2f__8) OpenAD_aux_31
  REAL(w2f__8) OpenAD_aux_32
  REAL(w2f__8) OpenAD_aux_33
  REAL(w2f__8) OpenAD_lin_48
  REAL(w2f__8) OpenAD_lin_49
  REAL(w2f__8) OpenAD_lin_50
  REAL(w2f__8) OpenAD_lin_51
  REAL(w2f__8) OpenAD_lin_52
  REAL(w2f__8) OpenAD_lin_53
  REAL(w2f__8) OpenAD_lin_54
  REAL(w2f__8) OpenAD_lin_55
  REAL(w2f__8) OpenAD_lin_56
  REAL(w2f__8) OpenAD_lin_57
  REAL(w2f__8) OpenAD_lin_58
  REAL(w2f__8) OpenAD_lin_59
  REAL(w2f__8) OpenAD_lin_60
  REAL(w2f__8) OpenAD_lin_61
  REAL(w2f__8) OpenAD_lin_62
  REAL(w2f__8) OpenAD_lin_63
  REAL(w2f__8) OpenAD_lin_64
  REAL(w2f__8) OpenAD_lin_65
  REAL(w2f__8) OpenAD_lin_66
  REAL(w2f__8) OpenAD_lin_67
  REAL(w2f__8) OpenAD_lin_68
  REAL(w2f__8) OpenAD_lin_69
  REAL(w2f__8) OpenAD_lin_70
  REAL(w2f__8) OpenAD_lin_71
  type(active) :: OpenAD_prp_27
  type(active) :: OpenAD_prp_28


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  DO UP = 2, (NZ +(-1)), 1
    INITTEMP(INT(UP),INT(EW),INT(NS))%v = (DISSIP(UP,EW,NS)%v+TEMP(UP)%v*(2.0D00&
     &-DIAG(UP)%v)-SUBD(UP)%v*TEMP(UP+(-1))%v-SUPD(UP)%v*TEMP(UP+1)%v-INITADVT(U&
     &P,EW,NS)%v)

  END DO
  IF(FLOAT) THEN
    CALL CALCPMPTB(INITTEMP(NZ,EW,NS),THCK)
  ELSE
    SLTERM%v = 0.0D00
    SLIDE_COUNT = 0
    DO NSP = NS+(-1),NS,1
      DO EWP = EW+(-1),EW,1
        IF ((ABS(UBAS(EWP,NSP)%v).GT.9.99999999999999954748D-07).OR.(ABS(VBAS(EW&
     &P,NSP)%v).GT.9.99999999999999954748D-07)) THEN

          SLIDE_COUNT = (SLIDE_COUNT+1)
          SLTERM%v = (SLTERM%v+DUSRFDEW(EWP,NSP)%v*UBAS(EWP,NSP)%v+DUSRFDNS(EWP,&
     &NSP)%v*VBAS(EWP,NSP)%v)

        ENDIF
      END DO
    END DO
    IF (SLIDE_COUNT.GE.4) THEN
      SLTERM%v = (SLTERM%v*2.5D-01)
    ELSE
      SLTERM%v = 0.0D00
    ENDIF
    INITTEMP(INT(NZ),INT(EW),INT(NS))%v = (DISSIP(NZ,EW,NS)%v+TEMP(NZ)%v*(2.0D00&
     &-DIAG(NZ)%v)-SUBD(NZ)%v*TEMP(NZ+(-1))%v-((BHEATFLX(EW,NS)*CONS3*5.0D-01)/(&
     &DUPN*THCK%v))-((SLIDEF1*SLTERM%v)/DUPN)-WEFF(NZ)%v*BHEATFLX(EW,NS)*CONS4-W&
     &EFF(NZ)%v*SLTERM%v*THCK%v*SLIDEF2-INITADVT(NZ,EW,NS)%v)

  ENDIF
    end if
    if (our_rev_mode%tape) then
! taping
  OpenAD_Symbol_538 = 0_w2f__i8
  DO UP = 2,(NZ+(-1)),1
    OpenAD_aux_26 = (2.0D00-DIAG(UP)%v)
    OpenAD_lin_48 = OpenAD_aux_26
    OpenAD_lin_49 = TEMP(UP)%v
    OpenAD_lin_50 = TEMP(UP+(-1))%v
    OpenAD_lin_51 = SUBD(UP)%v
    OpenAD_lin_52 = TEMP(UP+1)%v
    OpenAD_lin_53 = SUPD(UP)%v
    INITTEMP(INT(UP),INT(EW),INT(NS))%v = (DISSIP(UP,EW,NS)%v+TEMP(UP)%v*OpenAD_&
     &aux_26-SUBD(UP)%v*TEMP(UP+(-1))%v-SUPD(UP)%v*TEMP(UP+1)%v-INITADVT(UP,EW,N&
     &S)%v)

    OpenAD_acc_20 = (INT((-1_w2f__i8))*OpenAD_lin_49)
    OpenAD_acc_21 = (OpenAD_lin_50*INT((-1_w2f__i8)))
    OpenAD_acc_22 = (OpenAD_lin_51*INT((-1_w2f__i8)))
    OpenAD_acc_23 = (OpenAD_lin_52*INT((-1_w2f__i8)))
    OpenAD_acc_24 = (OpenAD_lin_53*INT((-1_w2f__i8)))
    if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
    oad_dt(oad_dt_ptr) = OpenAD_lin_48
    oad_dt_ptr = oad_dt_ptr+1
    if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
    oad_dt(oad_dt_ptr) = OpenAD_acc_20
    oad_dt_ptr = oad_dt_ptr+1
    if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
    oad_dt(oad_dt_ptr) = OpenAD_acc_21
    oad_dt_ptr = oad_dt_ptr+1
    if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
    oad_dt(oad_dt_ptr) = OpenAD_acc_22
    oad_dt_ptr = oad_dt_ptr+1
    OpenAD_Symbol_565 = (UP+(-1))
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_565
    oad_it_ptr = oad_it_ptr+1
    if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
    oad_dt(oad_dt_ptr) = OpenAD_acc_23
    oad_dt_ptr = oad_dt_ptr+1
    if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
    oad_dt(oad_dt_ptr) = OpenAD_acc_24
    oad_dt_ptr = oad_dt_ptr+1
    OpenAD_Symbol_566 = (UP+1)
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_566
    oad_it_ptr = oad_it_ptr+1
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = UP
    oad_it_ptr = oad_it_ptr+1
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = EW
    oad_it_ptr = oad_it_ptr+1
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = NS
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_538 = (INT(OpenAD_Symbol_538)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_538
  oad_it_ptr = oad_it_ptr+1
  IF(FLOAT) THEN
    CALL CALCPMPTB(INITTEMP(NZ,EW,NS),THCK)
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = NZ
    oad_it_ptr = oad_it_ptr+1
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = EW
    oad_it_ptr = oad_it_ptr+1
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = NS
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_545 = 1_w2f__i8
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_545
    oad_it_ptr = oad_it_ptr+1
  ELSE
    SLTERM%v = 0.0D00
    SLIDE_COUNT = 0
    OpenAD_Symbol_539 = 0_w2f__i8
    DO NSP = NS+(-1),NS,1
      OpenAD_Symbol_540 = 0_w2f__i8
      DO EWP = EW+(-1),EW,1
        IF ((ABS(UBAS(EWP,NSP)%v).GT.9.99999999999999954748D-07).OR.(ABS(VBAS(EW&
     &P,NSP)%v).GT.9.99999999999999954748D-07)) THEN

          SLIDE_COUNT = (SLIDE_COUNT+1)
          OpenAD_lin_54 = UBAS(EWP,NSP)%v
          OpenAD_lin_55 = DUSRFDEW(EWP,NSP)%v
          OpenAD_lin_56 = VBAS(EWP,NSP)%v
          OpenAD_lin_57 = DUSRFDNS(EWP,NSP)%v
          SLTERM%v = (SLTERM%v+DUSRFDEW(EWP,NSP)%v*UBAS(EWP,NSP)%v+DUSRFDNS(EWP,&
     &NSP)%v*VBAS(EWP,NSP)%v)

          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_lin_54
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_lin_55
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_lin_56
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_lin_57
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = EWP
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = NSP
          oad_it_ptr = oad_it_ptr+1
          OpenAD_Symbol_541 = 1_w2f__i8
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_541
          oad_it_ptr = oad_it_ptr+1
        ELSE
          OpenAD_Symbol_542 = 0_w2f__i8
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_542
          oad_it_ptr = oad_it_ptr+1
        ENDIF
        OpenAD_Symbol_540 = (INT(OpenAD_Symbol_540)+INT(1_w2f__i8))
      END DO
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = OpenAD_Symbol_540
      oad_it_ptr = oad_it_ptr+1
      OpenAD_Symbol_539 = (INT(OpenAD_Symbol_539)+INT(1_w2f__i8))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_539
    oad_it_ptr = oad_it_ptr+1
    IF (SLIDE_COUNT.GE.4) THEN
      SLTERM%v = (SLTERM%v*2.5D-01)
      OpenAD_Symbol_543 = 1_w2f__i8
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = OpenAD_Symbol_543
      oad_it_ptr = oad_it_ptr+1
    ELSE
      SLTERM%v = 0.0D00
      OpenAD_Symbol_544 = 0_w2f__i8
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = OpenAD_Symbol_544
      oad_it_ptr = oad_it_ptr+1
    ENDIF
    OpenAD_aux_27 = (2.0D00-DIAG(NZ)%v)
    OpenAD_aux_28 = (BHEATFLX(EW,NS)*CONS3*5.0D-01)
    OpenAD_aux_29 = (DUPN*THCK%v)
    OpenAD_aux_30 = (SLIDEF1*SLTERM%v)
    OpenAD_aux_31 = (BHEATFLX(EW,NS)*CONS4)
    OpenAD_aux_33 = (THCK%v*SLIDEF2)
    OpenAD_aux_32 = (SLTERM%v*OpenAD_aux_33)
    OpenAD_lin_58 = OpenAD_aux_27
    OpenAD_lin_59 = TEMP(NZ)%v
    OpenAD_lin_60 = TEMP(NZ+(-1))%v
    OpenAD_lin_61 = SUBD(NZ)%v
    OpenAD_lin_63 = DUPN
    OpenAD_lin_62 = (-(OpenAD_aux_28/(OpenAD_aux_29*OpenAD_aux_29)))
    OpenAD_lin_65 = SLIDEF1
    OpenAD_lin_64 = (INT(1_w2f__i8)/DUPN)
    OpenAD_lin_66 = OpenAD_aux_31
    OpenAD_lin_67 = OpenAD_aux_32
    OpenAD_lin_69 = OpenAD_aux_33
    OpenAD_lin_71 = SLIDEF2
    OpenAD_lin_70 = SLTERM%v
    OpenAD_lin_68 = WEFF(NZ)%v
    INITTEMP(INT(NZ),INT(EW),INT(NS))%v = (DISSIP(NZ,EW,NS)%v+TEMP(NZ)%v*OpenAD_&
     &aux_27-SUBD(NZ)%v*TEMP(NZ+(-1))%v-(OpenAD_aux_28/OpenAD_aux_29)-(OpenAD_au&
     &x_30/DUPN)-WEFF(NZ)%v*OpenAD_aux_31-WEFF(NZ)%v*OpenAD_aux_32-INITADVT(NZ,E&
     &W,NS)%v)

    OpenAD_acc_25 = (OpenAD_lin_66*INT((-1_w2f__i8)))
    OpenAD_acc_26 = (INT((-1_w2f__i8))*OpenAD_lin_59)
    OpenAD_acc_27 = (OpenAD_lin_60*INT((-1_w2f__i8)))
    OpenAD_acc_28 = (OpenAD_lin_61*INT((-1_w2f__i8)))
    OpenAD_acc_29 = (OpenAD_lin_63*OpenAD_lin_62*INT((-1_w2f__i8)))
    OpenAD_acc_30 = (OpenAD_lin_65*OpenAD_lin_64*INT((-1_w2f__i8)))
    OpenAD_acc_31 = (OpenAD_lin_67*INT((-1_w2f__i8)))
    OpenAD_acc_32 = (OpenAD_lin_69*OpenAD_lin_68*INT((-1_w2f__i8)))
    OpenAD_acc_33 = (OpenAD_lin_71*OpenAD_lin_70*OpenAD_lin_68*INT((-1_w2f__i8))&
     &)

    if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
    oad_dt(oad_dt_ptr) = OpenAD_acc_25
    oad_dt_ptr = oad_dt_ptr+1
    if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
    oad_dt(oad_dt_ptr) = OpenAD_lin_58
    oad_dt_ptr = oad_dt_ptr+1
    if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
    oad_dt(oad_dt_ptr) = OpenAD_acc_26
    oad_dt_ptr = oad_dt_ptr+1
    if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
    oad_dt(oad_dt_ptr) = OpenAD_acc_27
    oad_dt_ptr = oad_dt_ptr+1
    if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
    oad_dt(oad_dt_ptr) = OpenAD_acc_28
    oad_dt_ptr = oad_dt_ptr+1
    OpenAD_Symbol_567 = (NZ+(-1))
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_567
    oad_it_ptr = oad_it_ptr+1
    if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
    oad_dt(oad_dt_ptr) = OpenAD_acc_29
    oad_dt_ptr = oad_dt_ptr+1
    if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
    oad_dt(oad_dt_ptr) = OpenAD_acc_30
    oad_dt_ptr = oad_dt_ptr+1
    if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
    oad_dt(oad_dt_ptr) = OpenAD_acc_31
    oad_dt_ptr = oad_dt_ptr+1
    if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
    oad_dt(oad_dt_ptr) = OpenAD_acc_32
    oad_dt_ptr = oad_dt_ptr+1
    if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
    oad_dt(oad_dt_ptr) = OpenAD_acc_33
    oad_dt_ptr = oad_dt_ptr+1
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = NZ
    oad_it_ptr = oad_it_ptr+1
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = EW
    oad_it_ptr = oad_it_ptr+1
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = NS
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_546 = 0_w2f__i8
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_546
    oad_it_ptr = oad_it_ptr+1
  ENDIF
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_529 = oad_it(oad_it_ptr)
  IF (OpenAD_Symbol_529.ne.0) THEN
    oad_it_ptr = oad_it_ptr-1
    NS = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    EW = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    NZ = oad_it(oad_it_ptr)
    CALL CALCPMPTB(INITTEMP(NZ,EW,NS),THCK)
  ELSE
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1533 = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1534 = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1535 = oad_it(oad_it_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1536 = oad_dt(oad_dt_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1537 = oad_dt(oad_dt_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1538 = oad_dt(oad_dt_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1539 = oad_dt(oad_dt_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1540 = oad_dt(oad_dt_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1541 = oad_dt(oad_dt_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1542 = oad_dt(oad_dt_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1543 = oad_dt(oad_dt_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1544 = oad_dt(oad_dt_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1545 = oad_dt(oad_dt_ptr)
    THCK%d = THCK%d+INITTEMP(OpenAD_Symbol_1535,OpenAD_Symbol_1534,OpenAD_Symbol&
     &_1533)%d*(OpenAD_Symbol_1536)

    SLTERM%d = SLTERM%d+INITTEMP(OpenAD_Symbol_1535,OpenAD_Symbol_1534,OpenAD_Sy&
     &mbol_1533)%d*(OpenAD_Symbol_1537)

    WEFF(OpenAD_Symbol_1535)%d = WEFF(OpenAD_Symbol_1535)%d+INITTEMP(OpenAD_Symb&
     &ol_1535,OpenAD_Symbol_1534,OpenAD_Symbol_1533)%d*(OpenAD_Symbol_1538)

    SLTERM%d = SLTERM%d+INITTEMP(OpenAD_Symbol_1535,OpenAD_Symbol_1534,OpenAD_Sy&
     &mbol_1533)%d*(OpenAD_Symbol_1539)

    THCK%d = THCK%d+INITTEMP(OpenAD_Symbol_1535,OpenAD_Symbol_1534,OpenAD_Symbol&
     &_1533)%d*(OpenAD_Symbol_1540)

    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1546 = oad_it(oad_it_ptr)
    TEMP(OpenAD_Symbol_1546)%d = TEMP(OpenAD_Symbol_1546)%d+INITTEMP(OpenAD_Symb&
     &ol_1535,OpenAD_Symbol_1534,OpenAD_Symbol_1533)%d*(OpenAD_Symbol_1541)

    SUBD(OpenAD_Symbol_1535)%d = SUBD(OpenAD_Symbol_1535)%d+INITTEMP(OpenAD_Symb&
     &ol_1535,OpenAD_Symbol_1534,OpenAD_Symbol_1533)%d*(OpenAD_Symbol_1542)

    DIAG(OpenAD_Symbol_1535)%d = DIAG(OpenAD_Symbol_1535)%d+INITTEMP(OpenAD_Symb&
     &ol_1535,OpenAD_Symbol_1534,OpenAD_Symbol_1533)%d*(OpenAD_Symbol_1543)

    TEMP(OpenAD_Symbol_1535)%d = TEMP(OpenAD_Symbol_1535)%d+INITTEMP(OpenAD_Symb&
     &ol_1535,OpenAD_Symbol_1534,OpenAD_Symbol_1533)%d*(OpenAD_Symbol_1544)

    WEFF(OpenAD_Symbol_1535)%d = WEFF(OpenAD_Symbol_1535)%d+INITTEMP(OpenAD_Symb&
     &ol_1535,OpenAD_Symbol_1534,OpenAD_Symbol_1533)%d*(OpenAD_Symbol_1545)

    INITADVT(OpenAD_Symbol_1535,OpenAD_Symbol_1534,OpenAD_Symbol_1533)%d = INITA&
     &DVT(OpenAD_Symbol_1535,OpenAD_Symbol_1534,OpenAD_Symbol_1533)%d-INITTEMP(O&
     &penAD_Symbol_1535,OpenAD_Symbol_1534,OpenAD_Symbol_1533)%d

    DISSIP(OpenAD_Symbol_1535,OpenAD_Symbol_1534,OpenAD_Symbol_1533)%d = DISSIP(&
     &OpenAD_Symbol_1535,OpenAD_Symbol_1534,OpenAD_Symbol_1533)%d+INITTEMP(OpenA&
     &D_Symbol_1535,OpenAD_Symbol_1534,OpenAD_Symbol_1533)%d

    INITTEMP(OpenAD_Symbol_1535,OpenAD_Symbol_1534,OpenAD_Symbol_1533)%d = 0.0d0
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_530 = oad_it(oad_it_ptr)
    IF (OpenAD_Symbol_530.ne.0) THEN
      OpenAD_prp_28%d = OpenAD_prp_28%d+SLTERM%d*(2.5D-01)
      SLTERM%d = 0.0d0
      SLTERM%d = SLTERM%d+OpenAD_prp_28%d
      OpenAD_prp_28%d = 0.0d0
    ELSE
      SLTERM%d = 0.0d0
    ENDIF
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_531 = oad_it(oad_it_ptr)
    OpenAD_Symbol_532 = 1
    do while (INT(OpenAD_Symbol_532).LE.INT(OpenAD_Symbol_531))
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_533 = oad_it(oad_it_ptr)
      OpenAD_Symbol_534 = 1
      do while (INT(OpenAD_Symbol_534).LE.INT(OpenAD_Symbol_533))
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_535 = oad_it(oad_it_ptr)
        IF (OpenAD_Symbol_535.ne.0) THEN
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1527 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1528 = oad_it(oad_it_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1529 = oad_dt(oad_dt_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1530 = oad_dt(oad_dt_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1531 = oad_dt(oad_dt_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1532 = oad_dt(oad_dt_ptr)
          VBAS(OpenAD_Symbol_1528,OpenAD_Symbol_1527)%d = VBAS(OpenAD_Symbol_152&
     &8,OpenAD_Symbol_1527)%d+SLTERM%d*(OpenAD_Symbol_1529)

          DUSRFDNS(OpenAD_Symbol_1528,OpenAD_Symbol_1527)%d = DUSRFDNS(OpenAD_Sy&
     &mbol_1528,OpenAD_Symbol_1527)%d+SLTERM%d*(OpenAD_Symbol_1530)

          UBAS(OpenAD_Symbol_1528,OpenAD_Symbol_1527)%d = UBAS(OpenAD_Symbol_152&
     &8,OpenAD_Symbol_1527)%d+SLTERM%d*(OpenAD_Symbol_1531)

          DUSRFDEW(OpenAD_Symbol_1528,OpenAD_Symbol_1527)%d = DUSRFDEW(OpenAD_Sy&
     &mbol_1528,OpenAD_Symbol_1527)%d+SLTERM%d*(OpenAD_Symbol_1532)

          OpenAD_prp_27%d = OpenAD_prp_27%d+SLTERM%d
          SLTERM%d = 0.0d0
          SLTERM%d = SLTERM%d+OpenAD_prp_27%d
          OpenAD_prp_27%d = 0.0d0
        ENDIF
        OpenAD_Symbol_534 = INT(OpenAD_Symbol_534)+1
      END DO
      OpenAD_Symbol_532 = INT(OpenAD_Symbol_532)+1
    END DO
    SLTERM%d = 0.0d0
  ENDIF
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_536 = oad_it(oad_it_ptr)
  OpenAD_Symbol_537 = 1
  do while (INT(OpenAD_Symbol_537).LE.INT(OpenAD_Symbol_536))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1516 = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1517 = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1518 = oad_it(oad_it_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1519 = oad_dt(oad_dt_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1520 = oad_dt(oad_dt_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1521 = oad_dt(oad_dt_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1522 = oad_dt(oad_dt_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1523 = oad_dt(oad_dt_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1524 = oad_dt(oad_dt_ptr)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1525 = oad_it(oad_it_ptr)
    TEMP(OpenAD_Symbol_1525)%d = TEMP(OpenAD_Symbol_1525)%d+INITTEMP(OpenAD_Symb&
     &ol_1518,OpenAD_Symbol_1517,OpenAD_Symbol_1516)%d*(OpenAD_Symbol_1519)

    SUPD(OpenAD_Symbol_1518)%d = SUPD(OpenAD_Symbol_1518)%d+INITTEMP(OpenAD_Symb&
     &ol_1518,OpenAD_Symbol_1517,OpenAD_Symbol_1516)%d*(OpenAD_Symbol_1520)

    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1526 = oad_it(oad_it_ptr)
    TEMP(OpenAD_Symbol_1526)%d = TEMP(OpenAD_Symbol_1526)%d+INITTEMP(OpenAD_Symb&
     &ol_1518,OpenAD_Symbol_1517,OpenAD_Symbol_1516)%d*(OpenAD_Symbol_1521)

    SUBD(OpenAD_Symbol_1518)%d = SUBD(OpenAD_Symbol_1518)%d+INITTEMP(OpenAD_Symb&
     &ol_1518,OpenAD_Symbol_1517,OpenAD_Symbol_1516)%d*(OpenAD_Symbol_1522)

    DIAG(OpenAD_Symbol_1518)%d = DIAG(OpenAD_Symbol_1518)%d+INITTEMP(OpenAD_Symb&
     &ol_1518,OpenAD_Symbol_1517,OpenAD_Symbol_1516)%d*(OpenAD_Symbol_1523)

    TEMP(OpenAD_Symbol_1518)%d = TEMP(OpenAD_Symbol_1518)%d+INITTEMP(OpenAD_Symb&
     &ol_1518,OpenAD_Symbol_1517,OpenAD_Symbol_1516)%d*(OpenAD_Symbol_1524)

    INITADVT(OpenAD_Symbol_1518,OpenAD_Symbol_1517,OpenAD_Symbol_1516)%d = INITA&
     &DVT(OpenAD_Symbol_1518,OpenAD_Symbol_1517,OpenAD_Symbol_1516)%d-INITTEMP(O&
     &penAD_Symbol_1518,OpenAD_Symbol_1517,OpenAD_Symbol_1516)%d

    DISSIP(OpenAD_Symbol_1518,OpenAD_Symbol_1517,OpenAD_Symbol_1516)%d = DISSIP(&
     &OpenAD_Symbol_1518,OpenAD_Symbol_1517,OpenAD_Symbol_1516)%d+INITTEMP(OpenA&
     &D_Symbol_1518,OpenAD_Symbol_1517,OpenAD_Symbol_1516)%d

    INITTEMP(OpenAD_Symbol_1518,OpenAD_Symbol_1517,OpenAD_Symbol_1516)%d = 0.0d0
    OpenAD_Symbol_537 = INT(OpenAD_Symbol_537)+1
  END DO
    end if
  end subroutine FINDVTRI_INIT
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE FINDVTRI_RHS(NZ, INITTEMP, ARTM, ITERADVT, RHSD, FLOAT)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  use glimmer_global ,only: DP
  use glimmer_global ,only: DP
  use glimmer_global ,only: DP
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_568
  INTEGER(w2f__i8) OpenAD_Symbol_569
  INTEGER(w2f__i8) OpenAD_Symbol_570
  INTEGER(w2f__i8) OpenAD_Symbol_571
  INTEGER(w2f__i8) OpenAD_Symbol_572
  INTEGER(w2f__i8) OpenAD_Symbol_573
  INTEGER(w2f__i8) OpenAD_Symbol_574
  INTEGER(w2f__i8) OpenAD_Symbol_575
  INTEGER(w2f__i8) OpenAD_Symbol_576
  INTEGER(w2f__i8) OpenAD_Symbol_577
  INTEGER(w2f__i8) OpenAD_Symbol_578
  INTEGER(w2f__i8) OpenAD_Symbol_579
!
!       **** Parameters and Result ****
!
  INTEGER(w2f__i4) NZ
  type(active) :: INITTEMP(1:)
  type(active) :: ARTM
  type(active) :: ITERADVT(1:)
  type(active) :: RHSD(1:)
  LOGICAL(w2f__i4) FLOAT
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) UP
  INTEGER(w2f__i4) OpenAD_Symbol_1547
  INTEGER(w2f__i4) OpenAD_Symbol_1548
  INTEGER(w2f__i4) OpenAD_Symbol_1549
!
!       **** Statements ****
!


    integer iaddr
    external iaddr

   if (our_rev_mode%plain) then
! original function
  RHSD(1)%v = ARTM%v
  IF(FLOAT) THEN
    RHSD(INT(NZ))%v = INITTEMP(NZ)%v
  ELSE
    RHSD(INT(NZ))%v = (INITTEMP(NZ)%v-ITERADVT(NZ)%v)
  ENDIF
  DO UP = 2,(NZ+(-1)),1
    RHSD(INT(UP))%v = (INITTEMP(UP)%v-ITERADVT(UP)%v)
  END DO
    end if
    if (our_rev_mode%tape) then
! taping
  RHSD(1)%v = ARTM%v
  IF(FLOAT) THEN
    RHSD(INT(NZ))%v = INITTEMP(NZ)%v
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = NZ
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_571 = 1_w2f__i8
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_571
    oad_it_ptr = oad_it_ptr+1
  ELSE
    RHSD(INT(NZ))%v = (INITTEMP(NZ)%v-ITERADVT(NZ)%v)
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = NZ
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_572 = 0_w2f__i8
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_572
    oad_it_ptr = oad_it_ptr+1
  ENDIF
  OpenAD_Symbol_573 = 0_w2f__i8
  DO UP = 2,(NZ+(-1)),1
    RHSD(INT(UP))%v = (INITTEMP(UP)%v-ITERADVT(UP)%v)
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = UP
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_573 = (INT(OpenAD_Symbol_573)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_573
  oad_it_ptr = oad_it_ptr+1
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_568 = oad_it(oad_it_ptr)
  OpenAD_Symbol_569 = 1
  do while (INT(OpenAD_Symbol_569).LE.INT(OpenAD_Symbol_568))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1548 = oad_it(oad_it_ptr)
    ITERADVT(OpenAD_Symbol_1548)%d = ITERADVT(OpenAD_Symbol_1548)%d-RHSD(OpenAD_&
     &Symbol_1548)%d

    INITTEMP(OpenAD_Symbol_1548)%d = INITTEMP(OpenAD_Symbol_1548)%d+RHSD(OpenAD_&
     &Symbol_1548)%d

    RHSD(OpenAD_Symbol_1548)%d = 0.0d0
    OpenAD_Symbol_569 = INT(OpenAD_Symbol_569)+1
  END DO
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_570 = oad_it(oad_it_ptr)
  IF (OpenAD_Symbol_570.ne.0) THEN
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1547 = oad_it(oad_it_ptr)
    INITTEMP(OpenAD_Symbol_1547)%d = INITTEMP(OpenAD_Symbol_1547)%d+RHSD(OpenAD_&
     &Symbol_1547)%d

    RHSD(OpenAD_Symbol_1547)%d = 0.0d0
  ELSE
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1549 = oad_it(oad_it_ptr)
    ITERADVT(OpenAD_Symbol_1549)%d = ITERADVT(OpenAD_Symbol_1549)%d-RHSD(OpenAD_&
     &Symbol_1549)%d

    INITTEMP(OpenAD_Symbol_1549)%d = INITTEMP(OpenAD_Symbol_1549)%d+RHSD(OpenAD_&
     &Symbol_1549)%d

    RHSD(OpenAD_Symbol_1549)%d = 0.0d0
  ENDIF
  ARTM%d = ARTM%d+RHSD(1)%d
  RHSD(1)%d = 0.0d0
    end if
  end subroutine FINDVTRI_RHS
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE FINDDISP(DISSIP, THCK, STAGTHCK, DUSRFDEW, DUSRFDNS, FLWA, EWN, NSN&
     &, C1, THKLIM)

    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  use glimmer_global ,only: DP
  use physcon ,only: GN
  use glimmer_utils
  use glimmer_global ,only: DP
  use physcon ,only: GN
  use glimmer_utils
  use glimmer_global ,only: DP
  use physcon ,only: GN
  use glimmer_utils
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_389
  INTEGER(w2f__i8) OpenAD_Symbol_390
  INTEGER(w2f__i8) OpenAD_Symbol_391
  INTEGER(w2f__i8) OpenAD_Symbol_392
  INTEGER(w2f__i8) OpenAD_Symbol_393
  INTEGER(w2f__i8) OpenAD_Symbol_394
  INTEGER(w2f__i8) OpenAD_Symbol_395
  INTEGER(w2f__i8) OpenAD_Symbol_396
  INTEGER(w2f__i8) OpenAD_Symbol_397
  INTEGER(w2f__i8) OpenAD_Symbol_398
  INTEGER(w2f__i8) OpenAD_Symbol_399
  INTEGER(w2f__i8) OpenAD_Symbol_400
  INTEGER(w2f__i8) OpenAD_Symbol_401
  INTEGER(w2f__i8) OpenAD_Symbol_402
  INTEGER(w2f__i8) OpenAD_Symbol_403
  INTEGER(w2f__i8) OpenAD_Symbol_404
  INTEGER(w2f__i8) OpenAD_Symbol_405
  INTEGER(w2f__i8) OpenAD_Symbol_406
  INTEGER(w2f__i8) OpenAD_Symbol_407
  INTEGER(w2f__i8) OpenAD_Symbol_408
  INTEGER(w2f__i8) OpenAD_Symbol_409
  INTEGER(w2f__i8) OpenAD_Symbol_410
  INTEGER(w2f__i8) OpenAD_Symbol_411
  INTEGER(w2f__i8) OpenAD_Symbol_412
  INTEGER(w2f__i8) OpenAD_Symbol_413
  INTEGER(w2f__i8) OpenAD_Symbol_414
  INTEGER(w2f__i8) OpenAD_Symbol_415
  INTEGER(w2f__i8) OpenAD_Symbol_416
  INTEGER(w2f__i8) OpenAD_Symbol_417
  INTEGER(w2f__i8) OpenAD_Symbol_418
!
!       **** Parameters and Result ****
!
  type(active) :: DISSIP(1:,1:,1:)
  REAL(w2f__8) THCK(1 :, 1 :)
  type(active) :: STAGTHCK(1:,1:)
  type(active) :: DUSRFDEW(1:,1:)
  type(active) :: DUSRFDNS(1:,1:)
  type(active) :: FLWA(1:,1:,1:)
  INTEGER(w2f__i4) EWN
  INTEGER(w2f__i4) NSN
  REAL(w2f__8) C1(1 :)
  REAL(w2f__8) THKLIM
!
!       **** Local Variables and Functions ****
!
  type(active) :: C2
  INTEGER(w2f__i4) EW
  INTEGER(w2f__i4) NS
  INTEGER(w2f__i4) NZ
  INTEGER(w2f__i4) P1
  PARAMETER ( P1 = 4)
  type(active) :: SUM1
  type(active) :: SUM2
  type(active) :: SUM3
  INTEGER(w2f__i4) UP
  INTEGER(w2f__i4) OpenAD_Symbol_1461
  INTEGER(w2f__i4) OpenAD_Symbol_1462
  INTEGER(w2f__i4) OpenAD_Symbol_1463
  REAL(w2f__8) OpenAD_Symbol_1464
  REAL(w2f__8) OpenAD_Symbol_1465
  INTEGER(w2f__i4) OpenAD_Symbol_1466
  INTEGER(w2f__i4) OpenAD_Symbol_1467
  INTEGER(w2f__i4) OpenAD_Symbol_1468
  INTEGER(w2f__i4) OpenAD_Symbol_1469
  INTEGER(w2f__i4) OpenAD_Symbol_1470
  INTEGER(w2f__i4) OpenAD_Symbol_1471
  INTEGER(w2f__i4) OpenAD_Symbol_1472
  INTEGER(w2f__i4) OpenAD_Symbol_1473
  INTEGER(w2f__i4) OpenAD_Symbol_1474
  INTEGER(w2f__i4) OpenAD_Symbol_1475
  INTEGER(w2f__i4) OpenAD_Symbol_1476
  INTEGER(w2f__i4) OpenAD_Symbol_1477
  REAL(w2f__8) OpenAD_Symbol_1478
  REAL(w2f__8) OpenAD_Symbol_1479
  REAL(w2f__8) OpenAD_Symbol_1480
  INTEGER(w2f__i4) OpenAD_Symbol_419
  INTEGER(w2f__i4) OpenAD_Symbol_420
  INTEGER(w2f__i4) OpenAD_Symbol_421
  INTEGER(w2f__i4) OpenAD_Symbol_422
  INTEGER(w2f__i4) OpenAD_Symbol_423
  INTEGER(w2f__i4) OpenAD_Symbol_424
  INTEGER(w2f__i4) OpenAD_Symbol_425
  INTEGER(w2f__i4) OpenAD_Symbol_426
  INTEGER(w2f__i4) OpenAD_Symbol_427
  INTEGER(w2f__i4) OpenAD_Symbol_428
  INTEGER(w2f__i4) OpenAD_Symbol_429
  INTEGER(w2f__i4) OpenAD_Symbol_430
  REAL(w2f__8) OpenAD_acc_10
  REAL(w2f__8) OpenAD_acc_11
  REAL(w2f__8) OpenAD_acc_12
  REAL(w2f__8) OpenAD_acc_13
  REAL(w2f__8) OpenAD_acc_9
  REAL(w2f__8) OpenAD_aux_10
  REAL(w2f__8) OpenAD_aux_11
  REAL(w2f__8) OpenAD_aux_12
  REAL(w2f__8) OpenAD_aux_13
  REAL(w2f__8) OpenAD_aux_14
  REAL(w2f__8) OpenAD_aux_15
  REAL(w2f__8) OpenAD_aux_16
  REAL(w2f__8) OpenAD_aux_17
  REAL(w2f__8) OpenAD_lin_22
  REAL(w2f__8) OpenAD_lin_23
  REAL(w2f__8) OpenAD_lin_24
  REAL(w2f__8) OpenAD_lin_25
  REAL(w2f__8) OpenAD_lin_26
  REAL(w2f__8) OpenAD_lin_27
  REAL(w2f__8) OpenAD_lin_28
  REAL(w2f__8) OpenAD_lin_29
  REAL(w2f__8) OpenAD_lin_30
  type(active) :: OpenAD_prp_15
  type(active) :: OpenAD_prp_16
!
!       **** Statements ****
!


    integer iaddr
    external iaddr

   if (our_rev_mode%plain) then
! original function
  DISSIP(1:,1:,1:)%v = 0.0D00
  NZ = SIZE(DISSIP,1)
  DO NS = 2,(NSN+(-1)),1
    DO EW = 2,(EWN+(-1)),1
      IF (THCK(EW,NS).GT.THKLIM) THEN
        CALL MYSUM(STAGTHCK(EW+(-1):EW,NS+(-1):NS),SUM1,2,2)
        CALL MYSUM(DUSRFDEW(EW+(-1):EW,NS+(-1):NS),SUM2,2,2)
        CALL MYSUM(DUSRFDNS(EW+(-1):EW,NS+(-1):NS),SUM3,2,2)
        IF ((SUM2%v.eq.0.0D00).AND.(SUM3%v.eq.0.0D00)) THEN
          C2%v = 0.0D00
        ELSE
          C2%v = ((SQRT(((SUM2%v*2.5D-01)**2)+((SUM3%v*2.5D-01)**2))*SUM1%v*2.5D&
     &-01)**4)

        ENDIF
        DO UP = 1,NZ,1
          DISSIP(INT(UP),INT(EW),INT(NS))%v = (C1(UP)*C2%v*(FLWA(UP,EW+1,NS+(-1)&
     &)%v+FLWA(UP,EW+1,NS+1)%v+FLWA(UP,EW+(-1),NS+(-1))%v+FLWA(UP,EW+(-1),NS+1)%&
     &v+(FLWA(UP,EW,NS+1)%v+FLWA(UP,EW,NS+(-1))%v+FLWA(UP,EW+(-1),NS)%v+FLWA(UP,&
     &EW+1,NS)%v)*2.0D00+FLWA(UP,EW,NS)%v*4.0D00))

        END DO
      ENDIF
    END DO
  END DO
    end if
    if (our_rev_mode%tape) then
! taping
  DISSIP(1:,1:,1:)%v = 0.0D00
  NZ = SIZE(DISSIP,1)
  OpenAD_Symbol_397 = 0_w2f__i8
  DO NS = 2,(NSN+(-1)),1
    OpenAD_Symbol_398 = 0_w2f__i8
    DO EW = 2,(EWN+(-1)),1
      IF (THCK(EW,NS).GT.THKLIM) THEN
        CALL MYSUM(STAGTHCK(EW+(-1):EW,NS+(-1):NS),SUM1,2,2)
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        CALL MYSUM(DUSRFDEW(EW+(-1):EW,NS+(-1):NS),SUM2,2,2)
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        CALL MYSUM(DUSRFDNS(EW+(-1):EW,NS+(-1):NS),SUM3,2,2)
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        IF ((SUM2%v.eq.0.0D00).AND.(SUM3%v.eq.0.0D00)) THEN
          C2%v = 0.0D00
          OpenAD_Symbol_399 = 1_w2f__i8
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_399
          oad_it_ptr = oad_it_ptr+1
        ELSE
          OpenAD_aux_16 = (SUM2%v*2.5D-01)
          OpenAD_aux_17 = (SUM3%v*2.5D-01)
          OpenAD_aux_14 = SQRT((OpenAD_aux_16**2)+(OpenAD_aux_17**2))
          OpenAD_aux_15 = (SUM1%v*2.5D-01)
          OpenAD_aux_13 = (OpenAD_aux_14*OpenAD_aux_15)
          OpenAD_lin_29 = (2*(OpenAD_aux_16**(2-INT(1_w2f__i8))))
          OpenAD_lin_30 = (2*(OpenAD_aux_17**(2-INT(1_w2f__i8))))
          OpenAD_lin_28 = (5.0D-01/OpenAD_aux_14)
          OpenAD_lin_26 = OpenAD_aux_15
          OpenAD_lin_27 = OpenAD_aux_14
          OpenAD_lin_25 = (4*(OpenAD_aux_13**(4-INT(1_w2f__i8))))
          C2%v = (OpenAD_aux_13**4)
          OpenAD_acc_10 = (OpenAD_lin_28*OpenAD_lin_26*OpenAD_lin_25)
          OpenAD_acc_11 = (2.5D-01*OpenAD_lin_27*OpenAD_lin_25)
          OpenAD_acc_12 = (2.5D-01*OpenAD_lin_30*OpenAD_acc_10)
          OpenAD_acc_13 = (2.5D-01*OpenAD_lin_29*OpenAD_acc_10)
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_acc_11
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_acc_12
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_acc_13
          oad_dt_ptr = oad_dt_ptr+1
          OpenAD_Symbol_400 = 0_w2f__i8
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_400
          oad_it_ptr = oad_it_ptr+1
        ENDIF
        OpenAD_Symbol_401 = 0_w2f__i8
        DO UP = 1,NZ,1
          OpenAD_aux_10 = (C1(UP)*C2%v)
          OpenAD_aux_12 = (FLWA(UP,EW,NS+1)%v+FLWA(UP,EW,NS+(-1))%v+FLWA(UP,EW+(&
     &-1),NS)%v+FLWA(UP,EW+1,NS)%v)

          OpenAD_aux_11 = (FLWA(UP,EW+1,NS+(-1))%v+FLWA(UP,EW+1,NS+1)%v+FLWA(UP,&
     &EW+(-1),NS+(-1))%v+FLWA(UP,EW+(-1),NS+1)%v+OpenAD_aux_12*2.0D00+FLWA(UP,EW&
     &,NS)%v*4.0D00)

          OpenAD_lin_24 = C1(UP)
          OpenAD_lin_22 = OpenAD_aux_11
          OpenAD_lin_23 = OpenAD_aux_10
          DISSIP(INT(UP),INT(EW),INT(NS))%v = (OpenAD_aux_10*OpenAD_aux_11)
          OpenAD_acc_9 = (OpenAD_lin_24*OpenAD_lin_22)
          OpenAD_Symbol_419 = (NS+1)
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_419
          oad_it_ptr = oad_it_ptr+1
          OpenAD_Symbol_420 = (NS+(-1))
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_420
          oad_it_ptr = oad_it_ptr+1
          OpenAD_Symbol_421 = (EW+(-1))
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_421
          oad_it_ptr = oad_it_ptr+1
          OpenAD_Symbol_422 = (EW+1)
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_422
          oad_it_ptr = oad_it_ptr+1
          OpenAD_Symbol_423 = (EW+1)
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_423
          oad_it_ptr = oad_it_ptr+1
          OpenAD_Symbol_424 = (NS+(-1))
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_424
          oad_it_ptr = oad_it_ptr+1
          OpenAD_Symbol_425 = (EW+1)
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_425
          oad_it_ptr = oad_it_ptr+1
          OpenAD_Symbol_426 = (NS+1)
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_426
          oad_it_ptr = oad_it_ptr+1
          OpenAD_Symbol_427 = (EW+(-1))
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_427
          oad_it_ptr = oad_it_ptr+1
          OpenAD_Symbol_428 = (NS+(-1))
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_428
          oad_it_ptr = oad_it_ptr+1
          OpenAD_Symbol_429 = (EW+(-1))
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_429
          oad_it_ptr = oad_it_ptr+1
          OpenAD_Symbol_430 = (NS+1)
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_430
          oad_it_ptr = oad_it_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_lin_23
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_acc_9
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = UP
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = EW
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = NS
          oad_it_ptr = oad_it_ptr+1
          OpenAD_Symbol_401 = (INT(OpenAD_Symbol_401)+INT(1_w2f__i8))
        END DO
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_401
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_403 = 1_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_403
        oad_it_ptr = oad_it_ptr+1
      ELSE
        OpenAD_Symbol_402 = 0_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_402
        oad_it_ptr = oad_it_ptr+1
      ENDIF
      OpenAD_Symbol_398 = (INT(OpenAD_Symbol_398)+INT(1_w2f__i8))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_398
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_397 = (INT(OpenAD_Symbol_397)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_397
  oad_it_ptr = oad_it_ptr+1
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_389 = oad_it(oad_it_ptr)
  OpenAD_Symbol_390 = 1
  do while (INT(OpenAD_Symbol_390).LE.INT(OpenAD_Symbol_389))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_391 = oad_it(oad_it_ptr)
    OpenAD_Symbol_392 = 1
    do while (INT(OpenAD_Symbol_392).LE.INT(OpenAD_Symbol_391))
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_393 = oad_it(oad_it_ptr)
      IF (OpenAD_Symbol_393.ne.0) THEN
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_394 = oad_it(oad_it_ptr)
        OpenAD_Symbol_395 = 1
        do while (INT(OpenAD_Symbol_395).LE.INT(OpenAD_Symbol_394))
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1461 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1462 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1463 = oad_it(oad_it_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1464 = oad_dt(oad_dt_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1465 = oad_dt(oad_dt_ptr)
          C2%d = C2%d+DISSIP(OpenAD_Symbol_1463,OpenAD_Symbol_1462,OpenAD_Symbol&
     &_1461)%d*(OpenAD_Symbol_1464)

          OpenAD_prp_16%d = OpenAD_prp_16%d+DISSIP(OpenAD_Symbol_1463,OpenAD_Sym&
     &bol_1462,OpenAD_Symbol_1461)%d*(OpenAD_Symbol_1465)

          DISSIP(OpenAD_Symbol_1463,OpenAD_Symbol_1462,OpenAD_Symbol_1461)%d = 0&
     &.0d0

          OpenAD_prp_15%d = OpenAD_prp_15%d+OpenAD_prp_16%d*(2.0D00)
          FLWA(OpenAD_Symbol_1463,OpenAD_Symbol_1462,OpenAD_Symbol_1461)%d = FLW&
     &A(OpenAD_Symbol_1463,OpenAD_Symbol_1462,OpenAD_Symbol_1461)%d+OpenAD_prp_1&
     &6%d*(4.0D00)

          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1466 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1467 = oad_it(oad_it_ptr)
          FLWA(OpenAD_Symbol_1463,OpenAD_Symbol_1467,OpenAD_Symbol_1466)%d = FLW&
     &A(OpenAD_Symbol_1463,OpenAD_Symbol_1467,OpenAD_Symbol_1466)%d+OpenAD_prp_1&
     &6%d

          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1468 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1469 = oad_it(oad_it_ptr)
          FLWA(OpenAD_Symbol_1463,OpenAD_Symbol_1469,OpenAD_Symbol_1468)%d = FLW&
     &A(OpenAD_Symbol_1463,OpenAD_Symbol_1469,OpenAD_Symbol_1468)%d+OpenAD_prp_1&
     &6%d

          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1470 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1471 = oad_it(oad_it_ptr)
          FLWA(OpenAD_Symbol_1463,OpenAD_Symbol_1471,OpenAD_Symbol_1470)%d = FLW&
     &A(OpenAD_Symbol_1463,OpenAD_Symbol_1471,OpenAD_Symbol_1470)%d+OpenAD_prp_1&
     &6%d

          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1472 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1473 = oad_it(oad_it_ptr)
          FLWA(OpenAD_Symbol_1463,OpenAD_Symbol_1473,OpenAD_Symbol_1472)%d = FLW&
     &A(OpenAD_Symbol_1463,OpenAD_Symbol_1473,OpenAD_Symbol_1472)%d+OpenAD_prp_1&
     &6%d

          OpenAD_prp_16%d = 0.0d0
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1474 = oad_it(oad_it_ptr)
          FLWA(OpenAD_Symbol_1463,OpenAD_Symbol_1474,OpenAD_Symbol_1461)%d = FLW&
     &A(OpenAD_Symbol_1463,OpenAD_Symbol_1474,OpenAD_Symbol_1461)%d+OpenAD_prp_1&
     &5%d

          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1475 = oad_it(oad_it_ptr)
          FLWA(OpenAD_Symbol_1463,OpenAD_Symbol_1475,OpenAD_Symbol_1461)%d = FLW&
     &A(OpenAD_Symbol_1463,OpenAD_Symbol_1475,OpenAD_Symbol_1461)%d+OpenAD_prp_1&
     &5%d

          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1476 = oad_it(oad_it_ptr)
          FLWA(OpenAD_Symbol_1463,OpenAD_Symbol_1462,OpenAD_Symbol_1476)%d = FLW&
     &A(OpenAD_Symbol_1463,OpenAD_Symbol_1462,OpenAD_Symbol_1476)%d+OpenAD_prp_1&
     &5%d

          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1477 = oad_it(oad_it_ptr)
          FLWA(OpenAD_Symbol_1463,OpenAD_Symbol_1462,OpenAD_Symbol_1477)%d = FLW&
     &A(OpenAD_Symbol_1463,OpenAD_Symbol_1462,OpenAD_Symbol_1477)%d+OpenAD_prp_1&
     &5%d

          OpenAD_prp_15%d = 0.0d0
          OpenAD_Symbol_395 = INT(OpenAD_Symbol_395)+1
        END DO
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_396 = oad_it(oad_it_ptr)
        IF (OpenAD_Symbol_396.ne.0) THEN
          C2%d = 0.0d0
        ELSE
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1478 = oad_dt(oad_dt_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1479 = oad_dt(oad_dt_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1480 = oad_dt(oad_dt_ptr)
          SUM2%d = SUM2%d+C2%d*(OpenAD_Symbol_1478)
          SUM3%d = SUM3%d+C2%d*(OpenAD_Symbol_1479)
          SUM1%d = SUM1%d+C2%d*(OpenAD_Symbol_1480)
          C2%d = 0.0d0
        ENDIF
        oad_it_ptr = oad_it_ptr-1
        NS = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        EW = oad_it(oad_it_ptr)
        CALL MYSUM(DUSRFDNS(EW+(-1):EW,NS+(-1):NS),SUM3,2,2)
        oad_it_ptr = oad_it_ptr-1
        NS = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        EW = oad_it(oad_it_ptr)
        CALL MYSUM(DUSRFDEW(EW+(-1):EW,NS+(-1):NS),SUM2,2,2)
        oad_it_ptr = oad_it_ptr-1
        NS = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        EW = oad_it(oad_it_ptr)
        CALL MYSUM(STAGTHCK(EW+(-1):EW,NS+(-1):NS),SUM1,2,2)
      ENDIF
      OpenAD_Symbol_392 = INT(OpenAD_Symbol_392)+1
    END DO
    OpenAD_Symbol_390 = INT(OpenAD_Symbol_390)+1
  END DO
  DISSIP(1:,1:,1:)%d = 0.0d0
    end if
  end subroutine FINDDISP
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE CALCBMLT(NX, NY, NZ, SIGMA, DUPA, DUPB, DUPC, DISSIP , TEMP, THCK, &
     &STAGTHCK, DUSRFDEW, DUSRFDNS, UBAS, VBAS, BHEATFLX , BMLT, FLOATER, F3, TH&
     &KLIM)

    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  use glimmer_global ,only: DP
  use physcon ,only: CONI , GRAV , LHCI , RHOI , SCYR
  use glimmer_pmpt
  use glimmer_global ,only: DP
  use physcon ,only: CONI , GRAV , LHCI , RHOI , SCYR
  use glimmer_pmpt
  use glimmer_global ,only: DP
  use physcon ,only: CONI , GRAV , LHCI , RHOI , SCYR
  use glimmer_pmpt
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_580
  INTEGER(w2f__i8) OpenAD_Symbol_581
  INTEGER(w2f__i8) OpenAD_Symbol_582
  INTEGER(w2f__i8) OpenAD_Symbol_583
  INTEGER(w2f__i8) OpenAD_Symbol_584
  INTEGER(w2f__i8) OpenAD_Symbol_585
  INTEGER(w2f__i8) OpenAD_Symbol_586
  INTEGER(w2f__i8) OpenAD_Symbol_587
  INTEGER(w2f__i8) OpenAD_Symbol_588
  INTEGER(w2f__i8) OpenAD_Symbol_589
  INTEGER(w2f__i8) OpenAD_Symbol_590
  INTEGER(w2f__i8) OpenAD_Symbol_591
  INTEGER(w2f__i8) OpenAD_Symbol_592
  INTEGER(w2f__i8) OpenAD_Symbol_593
  INTEGER(w2f__i8) OpenAD_Symbol_594
  INTEGER(w2f__i8) OpenAD_Symbol_595
  INTEGER(w2f__i8) OpenAD_Symbol_596
  INTEGER(w2f__i8) OpenAD_Symbol_597
  INTEGER(w2f__i8) OpenAD_Symbol_598
  INTEGER(w2f__i8) OpenAD_Symbol_599
  INTEGER(w2f__i8) OpenAD_Symbol_600
  INTEGER(w2f__i8) OpenAD_Symbol_601
  INTEGER(w2f__i8) OpenAD_Symbol_602
  INTEGER(w2f__i8) OpenAD_Symbol_603
  INTEGER(w2f__i8) OpenAD_Symbol_604
  INTEGER(w2f__i8) OpenAD_Symbol_605
  INTEGER(w2f__i8) OpenAD_Symbol_606
  INTEGER(w2f__i8) OpenAD_Symbol_607
  INTEGER(w2f__i8) OpenAD_Symbol_608
  INTEGER(w2f__i8) OpenAD_Symbol_609
  INTEGER(w2f__i8) OpenAD_Symbol_610
  INTEGER(w2f__i8) OpenAD_Symbol_611
  INTEGER(w2f__i8) OpenAD_Symbol_612
  INTEGER(w2f__i8) OpenAD_Symbol_613
  INTEGER(w2f__i8) OpenAD_Symbol_614
  INTEGER(w2f__i8) OpenAD_Symbol_615
  INTEGER(w2f__i8) OpenAD_Symbol_616
  INTEGER(w2f__i8) OpenAD_Symbol_617
  INTEGER(w2f__i8) OpenAD_Symbol_618
  INTEGER(w2f__i8) OpenAD_Symbol_619
  INTEGER(w2f__i8) OpenAD_Symbol_620
  INTEGER(w2f__i8) OpenAD_Symbol_621
  INTEGER(w2f__i8) OpenAD_Symbol_622
  INTEGER(w2f__i8) OpenAD_Symbol_623
  INTEGER(w2f__i8) OpenAD_Symbol_624
  INTEGER(w2f__i8) OpenAD_Symbol_625
  INTEGER(w2f__i8) OpenAD_Symbol_626
  INTEGER(w2f__i8) OpenAD_Symbol_627
!
!       **** Parameters and Result ****
!
  INTEGER(w2f__i4) NX
  INTEGER(w2f__i4) NY
  INTEGER(w2f__i4) NZ
  REAL(w2f__8) SIGMA(1 : NZ)
  REAL(w2f__8) DUPA(1 : NZ)
  REAL(w2f__8) DUPB(1 : NZ)
  REAL(w2f__8) DUPC(1 : NZ)
  type(active) :: DISSIP(1:,1:,1:)
  type(active) :: TEMP(1:,0:,0:)
  type(active) :: THCK(1:,1:)
  type(active) :: STAGTHCK(1:,1:)
  type(active) :: DUSRFDEW(1:,1:)
  type(active) :: DUSRFDNS(1:,1:)
  type(active) :: UBAS(1:,1:)
  type(active) :: VBAS(1:,1:)
  REAL(w2f__8) BHEATFLX(1 :, 1 :)
  type(active) :: BMLT(1:,1:)
  LOGICAL(w2f__i4) FLOATER(1 :, 1 :)
  REAL(w2f__8) F3
  REAL(w2f__8) THKLIM
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) EW
  INTEGER(w2f__i4) EWP
  REAL(w2f__8) F1
  PARAMETER ( F1 = 2.17384105625717582999D-01)
  REAL(w2f__8) F2
  PARAMETER ( F2 = 1.03516240774151224713D-01)
  REAL(w2f__8) F4
  PARAMETER ( F4 = 7.32089552238805981111D-06)
  type(active) :: NEWMLT
  INTEGER(w2f__i4) NS
  INTEGER(w2f__i4) NSP
  type(active) :: OAD_CTMP0
  type(active) :: OAD_CTMP1
  type(active) :: PMPTEMP(1:NZ)
  type(active) :: SLTERM
  INTEGER(w2f__i4) UP
  INTEGER(w2f__i4) OpenAD_Symbol_1550
  INTEGER(w2f__i4) OpenAD_Symbol_1551
  REAL(w2f__8) OpenAD_Symbol_1552
  REAL(w2f__8) OpenAD_Symbol_1553
  REAL(w2f__8) OpenAD_Symbol_1554
  REAL(w2f__8) OpenAD_Symbol_1555
  REAL(w2f__8) OpenAD_Symbol_1556
  INTEGER(w2f__i4) OpenAD_Symbol_1557
  INTEGER(w2f__i4) OpenAD_Symbol_1558
  INTEGER(w2f__i4) OpenAD_Symbol_1559
  REAL(w2f__8) OpenAD_Symbol_1560
  REAL(w2f__8) OpenAD_Symbol_1561
  INTEGER(w2f__i4) OpenAD_Symbol_1562
  INTEGER(w2f__i4) OpenAD_Symbol_1563
  INTEGER(w2f__i4) OpenAD_Symbol_1564
  REAL(w2f__8) OpenAD_Symbol_1565
  REAL(w2f__8) OpenAD_Symbol_1566
  INTEGER(w2f__i4) OpenAD_Symbol_1567
  INTEGER(w2f__i4) OpenAD_Symbol_1568
  REAL(w2f__8) OpenAD_Symbol_1569
  REAL(w2f__8) OpenAD_Symbol_1570
  REAL(w2f__8) OpenAD_Symbol_1571
  INTEGER(w2f__i4) OpenAD_Symbol_1572
  INTEGER(w2f__i4) OpenAD_Symbol_1573
  INTEGER(w2f__i4) OpenAD_Symbol_1574
  INTEGER(w2f__i4) OpenAD_Symbol_1575
  INTEGER(w2f__i4) OpenAD_Symbol_1576
  INTEGER(w2f__i4) OpenAD_Symbol_1577
  INTEGER(w2f__i4) OpenAD_Symbol_1578
  INTEGER(w2f__i4) OpenAD_Symbol_1579
  REAL(w2f__8) OpenAD_Symbol_1580
  REAL(w2f__8) OpenAD_Symbol_1581
  REAL(w2f__8) OpenAD_Symbol_1582
  INTEGER(w2f__i4) OpenAD_Symbol_1583
  INTEGER(w2f__i4) OpenAD_Symbol_1584
  INTEGER(w2f__i4) OpenAD_Symbol_1585
  INTEGER(w2f__i4) OpenAD_Symbol_1586
  INTEGER(w2f__i4) OpenAD_Symbol_1587
  INTEGER(w2f__i4) OpenAD_Symbol_1588
  INTEGER(w2f__i4) OpenAD_Symbol_1589
  INTEGER(w2f__i4) OpenAD_Symbol_1590
  INTEGER(w2f__i4) OpenAD_Symbol_628
  INTEGER(w2f__i4) OpenAD_Symbol_629
  INTEGER(w2f__i4) OpenAD_Symbol_630
  INTEGER(w2f__i4) OpenAD_Symbol_631
  INTEGER(w2f__i4) OpenAD_Symbol_632
  INTEGER(w2f__i4) OpenAD_Symbol_633
  INTEGER(w2f__i4) OpenAD_Symbol_634
  INTEGER(w2f__i4) OpenAD_Symbol_635
  REAL(w2f__8) OpenAD_acc_34
  REAL(w2f__8) OpenAD_acc_35
  REAL(w2f__8) OpenAD_acc_36
  REAL(w2f__8) OpenAD_acc_37
  REAL(w2f__8) OpenAD_acc_38
  REAL(w2f__8) OpenAD_acc_39
  REAL(w2f__8) OpenAD_acc_40
  REAL(w2f__8) OpenAD_acc_41
  REAL(w2f__8) OpenAD_acc_42
  REAL(w2f__8) OpenAD_acc_43
  REAL(w2f__8) OpenAD_acc_44
  REAL(w2f__8) OpenAD_acc_45
  REAL(w2f__8) OpenAD_acc_46
  REAL(w2f__8) OpenAD_acc_47
  REAL(w2f__8) OpenAD_acc_48
  REAL(w2f__8) OpenAD_acc_49
  REAL(w2f__8) OpenAD_aux_34
  REAL(w2f__8) OpenAD_aux_35
  REAL(w2f__8) OpenAD_aux_36
  REAL(w2f__8) OpenAD_aux_37
  REAL(w2f__8) OpenAD_aux_38
  REAL(w2f__8) OpenAD_aux_39
  REAL(w2f__8) OpenAD_aux_40
  REAL(w2f__8) OpenAD_aux_41
  REAL(w2f__8) OpenAD_aux_42
  REAL(w2f__8) OpenAD_aux_43
  REAL(w2f__8) OpenAD_aux_44
  REAL(w2f__8) OpenAD_aux_45
  REAL(w2f__8) OpenAD_aux_46
  REAL(w2f__8) OpenAD_lin_72
  REAL(w2f__8) OpenAD_lin_73
  REAL(w2f__8) OpenAD_lin_74
  REAL(w2f__8) OpenAD_lin_75
  REAL(w2f__8) OpenAD_lin_76
  REAL(w2f__8) OpenAD_lin_77
  REAL(w2f__8) OpenAD_lin_78
  REAL(w2f__8) OpenAD_lin_79
  REAL(w2f__8) OpenAD_lin_80
  REAL(w2f__8) OpenAD_lin_81
  REAL(w2f__8) OpenAD_lin_82
  REAL(w2f__8) OpenAD_lin_83
  REAL(w2f__8) OpenAD_lin_84
  REAL(w2f__8) OpenAD_lin_85
  REAL(w2f__8) OpenAD_lin_86
  REAL(w2f__8) OpenAD_lin_87
  REAL(w2f__8) OpenAD_lin_88
  REAL(w2f__8) OpenAD_lin_89
  REAL(w2f__8) OpenAD_lin_90
  REAL(w2f__8) OpenAD_lin_91
  type(active) :: OpenAD_prp_29
  type(active) :: OpenAD_prp_30
  type(active) :: OpenAD_prp_31
  type(active) :: OpenAD_prp_32
  type(active) :: OpenAD_prp_33
  type(active) :: OpenAD_prp_34
  type(active) :: OpenAD_prp_35
  type(active) :: OpenAD_prp_36
  type(active) :: OpenAD_tyc_73
  type(active) :: OpenAD_tyc_8


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  DO NS = 2, (NY +(-1)), 1
    DO EW = 2, (NX +(-1)), 1
      IF ((THCK(EW,NS)%v.GT.THKLIM).AND.(.not. FLOATER(EW,NS))) THEN
        CALL CALCPMPT(PMPTEMP,THCK(EW,NS),SIGMA)
        IF (ABS(TEMP(NZ,EW,NS)%v-PMPTEMP(NZ)%v).LT.1.00000000000000002082D-03) T&
     &HEN

          SLTERM%v = 0.0D00
          DO NSP = NS+(-1),NS,1
            DO EWP = EW+(-1),EW,1
              SLTERM%v = (SLTERM%v-STAGTHCK(EWP,NSP)%v*(DUSRFDEW(EWP,NSP)%v*UBAS&
     &(EWP,NSP)%v+DUSRFDNS(EWP,NSP)%v*VBAS(EWP,NSP)%v))

            END DO
          END DO
          BMLT(INT(EW),INT(NS))%v = 0.0D00
          NEWMLT%v = (DISSIP(NZ,EW,NS)%v*THCK(EW,NS)%v*DUPC(NZ)*F3+SLTERM%v*7.32&
     &089552238805981111D-06-BHEATFLX(EW,NS)*1.03516240774151224713D-01)

          UP = (NZ+(-1))
          do while ((UP.GE.3).AND.(ABS(TEMP(UP,EW,NS)%v-PMPTEMP(UP)%v).LT.1.0000&
     &0000000000002082D-03))

            BMLT(INT(EW),INT(NS))%v = (BMLT(EW,NS)%v+NEWMLT%v)
            NEWMLT%v = (DISSIP(UP,EW,NS)%v*THCK(EW,NS)%v*DUPC(UP)*F3)
            UP = (UP+(-1))
          END DO
          UP = (UP+1)
          IF (NZ.eq.UP) THEN
            BMLT(INT(EW),INT(NS))%v = (NEWMLT%v-(((DUPA(UP)*(TEMP(UP+(-2),EW,NS)&
     &%v-PMPTEMP(UP+(-2))%v)+DUPB(UP)*(TEMP(UP+(-1),EW,NS)%v-PMPTEMP(UP+(-1))%v)&
     &)*2.17384105625717582999D-01)/THCK(EW,NS)%v))

          ELSE
            OAD_CTMP1%v = (NEWMLT%v-(((DUPA(UP)*(TEMP(UP+(-2),EW,NS)%v-PMPTEMP(U&
     &P+(-2))%v)+DUPB(UP)*(TEMP(UP+(-1),EW,NS)%v-PMPTEMP(UP+(-1))%v))*2.17384105&
     &625717582999D-01)/THCK(EW,NS)%v))

!!! requested inline of 'oad_convert' has no defn
            CALL oad_convert(OpenAD_tyc_8,0.0D00)
            CALL OAD_S_MAX_D(OpenAD_tyc_8,OAD_CTMP1,OAD_CTMP0)
            BMLT(INT(EW),INT(NS))%v = (BMLT(EW,NS)%v+OAD_CTMP0%v)
          ENDIF
        ELSE
          BMLT(INT(EW),INT(NS))%v = 0.0D00
        ENDIF
      ELSE
        BMLT(INT(EW),INT(NS))%v = 0.0D00
      ENDIF
    END DO
  END DO
    end if
    if (our_rev_mode%tape) then
! taping
  OpenAD_Symbol_593 = 0_w2f__i8
  DO NS = 2,(NY+(-1)),1
    OpenAD_Symbol_594 = 0_w2f__i8
    DO EW = 2,(NX+(-1)),1
      IF ((THCK(EW,NS)%v.GT.THKLIM).AND.(.not. FLOATER(EW,NS))) THEN
        CALL CALCPMPT(PMPTEMP,THCK(EW,NS),SIGMA)
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        IF (ABS(TEMP(NZ,EW,NS)%v-PMPTEMP(NZ)%v).LT.1.00000000000000002082D-03) T&
     &HEN

          SLTERM%v = 0.0D00
          OpenAD_Symbol_595 = 0_w2f__i8
          DO NSP = NS+(-1),NS,1
            OpenAD_Symbol_596 = 0_w2f__i8
            DO EWP = EW+(-1),EW,1
              OpenAD_aux_34 = (DUSRFDEW(EWP,NSP)%v*UBAS(EWP,NSP)%v+DUSRFDNS(EWP,&
     &NSP)%v*VBAS(EWP,NSP)%v)

              OpenAD_lin_72 = OpenAD_aux_34
              OpenAD_lin_74 = UBAS(EWP,NSP)%v
              OpenAD_lin_75 = DUSRFDEW(EWP,NSP)%v
              OpenAD_lin_76 = VBAS(EWP,NSP)%v
              OpenAD_lin_77 = DUSRFDNS(EWP,NSP)%v
              OpenAD_lin_73 = STAGTHCK(EWP,NSP)%v
              SLTERM%v = (SLTERM%v-STAGTHCK(EWP,NSP)%v*OpenAD_aux_34)
              OpenAD_acc_34 = (OpenAD_lin_73*INT((-1_w2f__i8)))
              OpenAD_acc_35 = (OpenAD_lin_72*INT((-1_w2f__i8)))
              OpenAD_acc_36 = (OpenAD_lin_76*OpenAD_acc_34)
              OpenAD_acc_37 = (OpenAD_lin_77*OpenAD_acc_34)
              OpenAD_acc_38 = (OpenAD_lin_74*OpenAD_acc_34)
              OpenAD_acc_39 = (OpenAD_lin_75*OpenAD_acc_34)
              if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
              oad_dt(oad_dt_ptr) = OpenAD_acc_35
              oad_dt_ptr = oad_dt_ptr+1
              if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
              oad_dt(oad_dt_ptr) = OpenAD_acc_36
              oad_dt_ptr = oad_dt_ptr+1
              if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
              oad_dt(oad_dt_ptr) = OpenAD_acc_37
              oad_dt_ptr = oad_dt_ptr+1
              if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
              oad_dt(oad_dt_ptr) = OpenAD_acc_38
              oad_dt_ptr = oad_dt_ptr+1
              if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
              oad_dt(oad_dt_ptr) = OpenAD_acc_39
              oad_dt_ptr = oad_dt_ptr+1
              if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
              oad_it(oad_it_ptr) = EWP
              oad_it_ptr = oad_it_ptr+1
              if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
              oad_it(oad_it_ptr) = NSP
              oad_it_ptr = oad_it_ptr+1
              OpenAD_Symbol_596 = (INT(OpenAD_Symbol_596)+INT(1_w2f__i8))
            END DO
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = OpenAD_Symbol_596
            oad_it_ptr = oad_it_ptr+1
            OpenAD_Symbol_595 = (INT(OpenAD_Symbol_595)+INT(1_w2f__i8))
          END DO
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_595
          oad_it_ptr = oad_it_ptr+1
          BMLT(INT(EW),INT(NS))%v = 0.0D00
          OpenAD_aux_36 = (DUPC(NZ)*F3)
          OpenAD_aux_35 = (THCK(EW,NS)%v*OpenAD_aux_36)
          OpenAD_lin_78 = OpenAD_aux_35
          OpenAD_lin_80 = OpenAD_aux_36
          OpenAD_lin_79 = DISSIP(NZ,EW,NS)%v
          NEWMLT%v = (DISSIP(NZ,EW,NS)%v*OpenAD_aux_35+SLTERM%v*7.32089552238805&
     &981111D-06-BHEATFLX(EW,NS)*1.03516240774151224713D-01)

          UP = (NZ+(-1))
          OpenAD_acc_40 = (OpenAD_lin_80*OpenAD_lin_79)
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_lin_78
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_acc_40
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = EW
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = NS
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = NZ
          oad_it_ptr = oad_it_ptr+1
          OpenAD_Symbol_597 = 0_w2f__i8
          do while ((UP.GE.3).AND.(ABS(TEMP(UP,EW,NS)%v-PMPTEMP(UP)%v).LT.1.0000&
     &0000000000002082D-03))

            BMLT(INT(EW),INT(NS))%v = (BMLT(EW,NS)%v+NEWMLT%v)
            OpenAD_aux_38 = (DUPC(UP)*F3)
            OpenAD_aux_37 = (THCK(EW,NS)%v*OpenAD_aux_38)
            OpenAD_lin_81 = OpenAD_aux_37
            OpenAD_lin_83 = OpenAD_aux_38
            OpenAD_lin_82 = DISSIP(UP,EW,NS)%v
            NEWMLT%v = (DISSIP(UP,EW,NS)%v*OpenAD_aux_37)
            OpenAD_acc_41 = (OpenAD_lin_83*OpenAD_lin_82)
            if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
            oad_dt(oad_dt_ptr) = OpenAD_lin_81
            oad_dt_ptr = oad_dt_ptr+1
            if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
            oad_dt(oad_dt_ptr) = OpenAD_acc_41
            oad_dt_ptr = oad_dt_ptr+1
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = EW
            oad_it_ptr = oad_it_ptr+1
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = NS
            oad_it_ptr = oad_it_ptr+1
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = UP
            oad_it_ptr = oad_it_ptr+1
            UP = (UP+(-1))
            OpenAD_Symbol_597 = (INT(OpenAD_Symbol_597)+INT(1_w2f__i8))
          END DO
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_597
          oad_it_ptr = oad_it_ptr+1
          UP = (UP+1)
          IF (NZ.eq.UP) THEN
            OpenAD_aux_41 = (TEMP(UP+(-2),EW,NS)%v-PMPTEMP(UP+(-2))%v)
            OpenAD_aux_42 = (TEMP(UP+(-1),EW,NS)%v-PMPTEMP(UP+(-1))%v)
            OpenAD_aux_40 = (DUPA(UP)*OpenAD_aux_41+DUPB(UP)*OpenAD_aux_42)
            OpenAD_aux_39 = (OpenAD_aux_40*2.17384105625717582999D-01)
            OpenAD_lin_86 = DUPA(UP)
            OpenAD_lin_87 = DUPB(UP)
            OpenAD_lin_84 = (INT(1_w2f__i8)/THCK(EW,NS)%v)
            OpenAD_lin_85 = (-(OpenAD_aux_39/(THCK(EW,NS)%v*THCK(EW,NS)%v)))
            BMLT(INT(EW),INT(NS))%v = (NEWMLT%v-(OpenAD_aux_39/THCK(EW,NS)%v))
            OpenAD_acc_42 = (2.17384105625717582999D-01*OpenAD_lin_84*INT((-1_w2&
     &f__i8)))

            OpenAD_acc_43 = (OpenAD_lin_85*INT((-1_w2f__i8)))
            OpenAD_acc_44 = (OpenAD_lin_87*OpenAD_acc_42)
            OpenAD_acc_45 = (OpenAD_lin_86*OpenAD_acc_42)
            OpenAD_Symbol_628 = (UP+(-2))
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = OpenAD_Symbol_628
            oad_it_ptr = oad_it_ptr+1
            OpenAD_Symbol_629 = (UP+(-2))
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = OpenAD_Symbol_629
            oad_it_ptr = oad_it_ptr+1
            OpenAD_Symbol_630 = (UP+(-1))
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = OpenAD_Symbol_630
            oad_it_ptr = oad_it_ptr+1
            OpenAD_Symbol_631 = (UP+(-1))
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = OpenAD_Symbol_631
            oad_it_ptr = oad_it_ptr+1
            if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
            oad_dt(oad_dt_ptr) = OpenAD_acc_43
            oad_dt_ptr = oad_dt_ptr+1
            if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
            oad_dt(oad_dt_ptr) = OpenAD_acc_44
            oad_dt_ptr = oad_dt_ptr+1
            if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
            oad_dt(oad_dt_ptr) = OpenAD_acc_45
            oad_dt_ptr = oad_dt_ptr+1
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = EW
            oad_it_ptr = oad_it_ptr+1
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = NS
            oad_it_ptr = oad_it_ptr+1
            OpenAD_Symbol_598 = 1_w2f__i8
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = OpenAD_Symbol_598
            oad_it_ptr = oad_it_ptr+1
          ELSE
            OpenAD_aux_45 = (TEMP(UP+(-2),EW,NS)%v-PMPTEMP(UP+(-2))%v)
            OpenAD_aux_46 = (TEMP(UP+(-1),EW,NS)%v-PMPTEMP(UP+(-1))%v)
            OpenAD_aux_44 = (DUPA(UP)*OpenAD_aux_45+DUPB(UP)*OpenAD_aux_46)
            OpenAD_aux_43 = (OpenAD_aux_44*2.17384105625717582999D-01)
            OpenAD_lin_90 = DUPA(UP)
            OpenAD_lin_91 = DUPB(UP)
            OpenAD_lin_88 = (INT(1_w2f__i8)/THCK(EW,NS)%v)
            OpenAD_lin_89 = (-(OpenAD_aux_43/(THCK(EW,NS)%v*THCK(EW,NS)%v)))
            OAD_CTMP1%v = (NEWMLT%v-(OpenAD_aux_43/THCK(EW,NS)%v))
            OpenAD_acc_46 = (2.17384105625717582999D-01*OpenAD_lin_88*INT((-1_w2&
     &f__i8)))

            OpenAD_acc_47 = (OpenAD_lin_89*INT((-1_w2f__i8)))
            OpenAD_acc_48 = (OpenAD_lin_91*OpenAD_acc_46)
            OpenAD_acc_49 = (OpenAD_lin_90*OpenAD_acc_46)
            OpenAD_Symbol_632 = (UP+(-2))
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = OpenAD_Symbol_632
            oad_it_ptr = oad_it_ptr+1
            OpenAD_Symbol_633 = (UP+(-2))
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = OpenAD_Symbol_633
            oad_it_ptr = oad_it_ptr+1
            OpenAD_Symbol_634 = (UP+(-1))
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = OpenAD_Symbol_634
            oad_it_ptr = oad_it_ptr+1
            OpenAD_Symbol_635 = (UP+(-1))
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = OpenAD_Symbol_635
            oad_it_ptr = oad_it_ptr+1
            if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
            oad_dt(oad_dt_ptr) = OpenAD_acc_47
            oad_dt_ptr = oad_dt_ptr+1
            if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
            oad_dt(oad_dt_ptr) = OpenAD_acc_48
            oad_dt_ptr = oad_dt_ptr+1
            if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
            oad_dt(oad_dt_ptr) = OpenAD_acc_49
            oad_dt_ptr = oad_dt_ptr+1
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = EW
            oad_it_ptr = oad_it_ptr+1
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = NS
            oad_it_ptr = oad_it_ptr+1
!!! requested inline of 'oad_convert' has no defn
            CALL oad_convert(OpenAD_tyc_8,0.0D00)
            CALL OAD_S_MAX_D(OpenAD_tyc_8,OAD_CTMP1,OAD_CTMP0)
            BMLT(INT(EW),INT(NS))%v = (BMLT(EW,NS)%v+OAD_CTMP0%v)
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = EW
            oad_it_ptr = oad_it_ptr+1
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = NS
            oad_it_ptr = oad_it_ptr+1
            OpenAD_Symbol_599 = 0_w2f__i8
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = OpenAD_Symbol_599
            oad_it_ptr = oad_it_ptr+1
          ENDIF
          OpenAD_Symbol_601 = 1_w2f__i8
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_601
          oad_it_ptr = oad_it_ptr+1
        ELSE
          BMLT(INT(EW),INT(NS))%v = 0.0D00
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = EW
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = NS
          oad_it_ptr = oad_it_ptr+1
          OpenAD_Symbol_600 = 0_w2f__i8
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_600
          oad_it_ptr = oad_it_ptr+1
        ENDIF
        OpenAD_Symbol_603 = 1_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_603
        oad_it_ptr = oad_it_ptr+1
      ELSE
        BMLT(INT(EW),INT(NS))%v = 0.0D00
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_602 = 0_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_602
        oad_it_ptr = oad_it_ptr+1
      ENDIF
      OpenAD_Symbol_594 = (INT(OpenAD_Symbol_594)+INT(1_w2f__i8))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_594
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_593 = (INT(OpenAD_Symbol_593)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_593
  oad_it_ptr = oad_it_ptr+1
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_580 = oad_it(oad_it_ptr)
  OpenAD_Symbol_581 = 1
  do while (INT(OpenAD_Symbol_581).LE.INT(OpenAD_Symbol_580))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_582 = oad_it(oad_it_ptr)
    OpenAD_Symbol_583 = 1
    do while (INT(OpenAD_Symbol_583).LE.INT(OpenAD_Symbol_582))
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_584 = oad_it(oad_it_ptr)
      IF (OpenAD_Symbol_584.ne.0) THEN
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_585 = oad_it(oad_it_ptr)
        IF (OpenAD_Symbol_585.ne.0) THEN
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_586 = oad_it(oad_it_ptr)
          IF (OpenAD_Symbol_586.ne.0) THEN
            oad_it_ptr = oad_it_ptr-1
            OpenAD_Symbol_1567 = oad_it(oad_it_ptr)
            oad_it_ptr = oad_it_ptr-1
            OpenAD_Symbol_1568 = oad_it(oad_it_ptr)
            oad_dt_ptr = oad_dt_ptr-1
            OpenAD_Symbol_1569 = oad_dt(oad_dt_ptr)
            oad_dt_ptr = oad_dt_ptr-1
            OpenAD_Symbol_1570 = oad_dt(oad_dt_ptr)
            oad_dt_ptr = oad_dt_ptr-1
            OpenAD_Symbol_1571 = oad_dt(oad_dt_ptr)
            OpenAD_prp_32%d = OpenAD_prp_32%d+BMLT(OpenAD_Symbol_1568,OpenAD_Sym&
     &bol_1567)%d*(OpenAD_Symbol_1569)

            OpenAD_prp_33%d = OpenAD_prp_33%d+BMLT(OpenAD_Symbol_1568,OpenAD_Sym&
     &bol_1567)%d*(OpenAD_Symbol_1570)

            THCK(OpenAD_Symbol_1568,OpenAD_Symbol_1567)%d = THCK(OpenAD_Symbol_1&
     &568,OpenAD_Symbol_1567)%d+BMLT(OpenAD_Symbol_1568,OpenAD_Symbol_1567)%d*(O&
     &penAD_Symbol_1571)

            NEWMLT%d = NEWMLT%d+BMLT(OpenAD_Symbol_1568,OpenAD_Symbol_1567)%d
            BMLT(OpenAD_Symbol_1568,OpenAD_Symbol_1567)%d = 0.0d0
            oad_it_ptr = oad_it_ptr-1
            OpenAD_Symbol_1572 = oad_it(oad_it_ptr)
            PMPTEMP(OpenAD_Symbol_1572)%d = PMPTEMP(OpenAD_Symbol_1572)%d-OpenAD&
     &_prp_33%d

            oad_it_ptr = oad_it_ptr-1
            OpenAD_Symbol_1573 = oad_it(oad_it_ptr)
            TEMP(OpenAD_Symbol_1573,OpenAD_Symbol_1568,OpenAD_Symbol_1567)%d = T&
     &EMP(OpenAD_Symbol_1573,OpenAD_Symbol_1568,OpenAD_Symbol_1567)%d+OpenAD_prp&
     &_33%d

            OpenAD_prp_33%d = 0.0d0
            oad_it_ptr = oad_it_ptr-1
            OpenAD_Symbol_1574 = oad_it(oad_it_ptr)
            PMPTEMP(OpenAD_Symbol_1574)%d = PMPTEMP(OpenAD_Symbol_1574)%d-OpenAD&
     &_prp_32%d

            oad_it_ptr = oad_it_ptr-1
            OpenAD_Symbol_1575 = oad_it(oad_it_ptr)
            TEMP(OpenAD_Symbol_1575,OpenAD_Symbol_1568,OpenAD_Symbol_1567)%d = T&
     &EMP(OpenAD_Symbol_1575,OpenAD_Symbol_1568,OpenAD_Symbol_1567)%d+OpenAD_prp&
     &_32%d

            OpenAD_prp_32%d = 0.0d0
          ELSE
            oad_it_ptr = oad_it_ptr-1
            OpenAD_Symbol_1576 = oad_it(oad_it_ptr)
            oad_it_ptr = oad_it_ptr-1
            OpenAD_Symbol_1577 = oad_it(oad_it_ptr)
            OAD_CTMP0%d = OAD_CTMP0%d+BMLT(OpenAD_Symbol_1577,OpenAD_Symbol_1576&
     &)%d

            OpenAD_prp_36%d = OpenAD_prp_36%d+BMLT(OpenAD_Symbol_1577,OpenAD_Sym&
     &bol_1576)%d

            BMLT(OpenAD_Symbol_1577,OpenAD_Symbol_1576)%d = 0.0d0
            BMLT(OpenAD_Symbol_1577,OpenAD_Symbol_1576)%d = BMLT(OpenAD_Symbol_1&
     &577,OpenAD_Symbol_1576)%d+OpenAD_prp_36%d

            OpenAD_prp_36%d = 0.0d0
            CALL OAD_S_MAX_D(OpenAD_tyc_73,OAD_CTMP1,OAD_CTMP0)
            oad_it_ptr = oad_it_ptr-1
            OpenAD_Symbol_1578 = oad_it(oad_it_ptr)
            oad_it_ptr = oad_it_ptr-1
            OpenAD_Symbol_1579 = oad_it(oad_it_ptr)
            oad_dt_ptr = oad_dt_ptr-1
            OpenAD_Symbol_1580 = oad_dt(oad_dt_ptr)
            oad_dt_ptr = oad_dt_ptr-1
            OpenAD_Symbol_1581 = oad_dt(oad_dt_ptr)
            oad_dt_ptr = oad_dt_ptr-1
            OpenAD_Symbol_1582 = oad_dt(oad_dt_ptr)
            OpenAD_prp_34%d = OpenAD_prp_34%d+OAD_CTMP1%d*(OpenAD_Symbol_1580)
            OpenAD_prp_35%d = OpenAD_prp_35%d+OAD_CTMP1%d*(OpenAD_Symbol_1581)
            THCK(OpenAD_Symbol_1579,OpenAD_Symbol_1578)%d = THCK(OpenAD_Symbol_1&
     &579,OpenAD_Symbol_1578)%d+OAD_CTMP1%d*(OpenAD_Symbol_1582)

            NEWMLT%d = NEWMLT%d+OAD_CTMP1%d
            OAD_CTMP1%d = 0.0d0
            oad_it_ptr = oad_it_ptr-1
            OpenAD_Symbol_1583 = oad_it(oad_it_ptr)
            PMPTEMP(OpenAD_Symbol_1583)%d = PMPTEMP(OpenAD_Symbol_1583)%d-OpenAD&
     &_prp_35%d

            oad_it_ptr = oad_it_ptr-1
            OpenAD_Symbol_1584 = oad_it(oad_it_ptr)
            TEMP(OpenAD_Symbol_1584,OpenAD_Symbol_1579,OpenAD_Symbol_1578)%d = T&
     &EMP(OpenAD_Symbol_1584,OpenAD_Symbol_1579,OpenAD_Symbol_1578)%d+OpenAD_prp&
     &_35%d

            OpenAD_prp_35%d = 0.0d0
            oad_it_ptr = oad_it_ptr-1
            OpenAD_Symbol_1585 = oad_it(oad_it_ptr)
            PMPTEMP(OpenAD_Symbol_1585)%d = PMPTEMP(OpenAD_Symbol_1585)%d-OpenAD&
     &_prp_34%d

            oad_it_ptr = oad_it_ptr-1
            OpenAD_Symbol_1586 = oad_it(oad_it_ptr)
            TEMP(OpenAD_Symbol_1586,OpenAD_Symbol_1579,OpenAD_Symbol_1578)%d = T&
     &EMP(OpenAD_Symbol_1586,OpenAD_Symbol_1579,OpenAD_Symbol_1578)%d+OpenAD_prp&
     &_34%d

            OpenAD_prp_34%d = 0.0d0
          ENDIF
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_587 = oad_it(oad_it_ptr)
          OpenAD_Symbol_588 = 1
          do while (INT(OpenAD_Symbol_588).LE.INT(OpenAD_Symbol_587))
            oad_it_ptr = oad_it_ptr-1
            OpenAD_Symbol_1562 = oad_it(oad_it_ptr)
            oad_it_ptr = oad_it_ptr-1
            OpenAD_Symbol_1563 = oad_it(oad_it_ptr)
            oad_it_ptr = oad_it_ptr-1
            OpenAD_Symbol_1564 = oad_it(oad_it_ptr)
            oad_dt_ptr = oad_dt_ptr-1
            OpenAD_Symbol_1565 = oad_dt(oad_dt_ptr)
            oad_dt_ptr = oad_dt_ptr-1
            OpenAD_Symbol_1566 = oad_dt(oad_dt_ptr)
            THCK(OpenAD_Symbol_1564,OpenAD_Symbol_1563)%d = THCK(OpenAD_Symbol_1&
     &564,OpenAD_Symbol_1563)%d+NEWMLT%d*(OpenAD_Symbol_1565)

            DISSIP(OpenAD_Symbol_1562,OpenAD_Symbol_1564,OpenAD_Symbol_1563)%d =&
     & DISSIP(OpenAD_Symbol_1562,OpenAD_Symbol_1564,OpenAD_Symbol_1563)%d+NEWMLT&
     &%d*(OpenAD_Symbol_1566)

            NEWMLT%d = 0.0d0
            OpenAD_prp_31%d = OpenAD_prp_31%d+BMLT(OpenAD_Symbol_1564,OpenAD_Sym&
     &bol_1563)%d

            OpenAD_prp_30%d = OpenAD_prp_30%d+BMLT(OpenAD_Symbol_1564,OpenAD_Sym&
     &bol_1563)%d

            BMLT(OpenAD_Symbol_1564,OpenAD_Symbol_1563)%d = 0.0d0
            NEWMLT%d = NEWMLT%d+OpenAD_prp_31%d
            OpenAD_prp_31%d = 0.0d0
            BMLT(OpenAD_Symbol_1564,OpenAD_Symbol_1563)%d = BMLT(OpenAD_Symbol_1&
     &564,OpenAD_Symbol_1563)%d+OpenAD_prp_30%d

            OpenAD_prp_30%d = 0.0d0
            OpenAD_Symbol_588 = INT(OpenAD_Symbol_588)+1
          END DO
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1557 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1558 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1559 = oad_it(oad_it_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1560 = oad_dt(oad_dt_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1561 = oad_dt(oad_dt_ptr)
          THCK(OpenAD_Symbol_1559,OpenAD_Symbol_1558)%d = THCK(OpenAD_Symbol_155&
     &9,OpenAD_Symbol_1558)%d+NEWMLT%d*(OpenAD_Symbol_1560)

          DISSIP(OpenAD_Symbol_1557,OpenAD_Symbol_1559,OpenAD_Symbol_1558)%d = D&
     &ISSIP(OpenAD_Symbol_1557,OpenAD_Symbol_1559,OpenAD_Symbol_1558)%d+NEWMLT%d&
     &*(OpenAD_Symbol_1561)

          SLTERM%d = SLTERM%d+NEWMLT%d*(7.32089552238805981111D-06)
          NEWMLT%d = 0.0d0
          BMLT(OpenAD_Symbol_1559,OpenAD_Symbol_1558)%d = 0.0d0
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_589 = oad_it(oad_it_ptr)
          OpenAD_Symbol_590 = 1
          do while (INT(OpenAD_Symbol_590).LE.INT(OpenAD_Symbol_589))
            oad_it_ptr = oad_it_ptr-1
            OpenAD_Symbol_591 = oad_it(oad_it_ptr)
            OpenAD_Symbol_592 = 1
            do while (INT(OpenAD_Symbol_592).LE.INT(OpenAD_Symbol_591))
              oad_it_ptr = oad_it_ptr-1
              OpenAD_Symbol_1550 = oad_it(oad_it_ptr)
              oad_it_ptr = oad_it_ptr-1
              OpenAD_Symbol_1551 = oad_it(oad_it_ptr)
              oad_dt_ptr = oad_dt_ptr-1
              OpenAD_Symbol_1552 = oad_dt(oad_dt_ptr)
              oad_dt_ptr = oad_dt_ptr-1
              OpenAD_Symbol_1553 = oad_dt(oad_dt_ptr)
              oad_dt_ptr = oad_dt_ptr-1
              OpenAD_Symbol_1554 = oad_dt(oad_dt_ptr)
              oad_dt_ptr = oad_dt_ptr-1
              OpenAD_Symbol_1555 = oad_dt(oad_dt_ptr)
              oad_dt_ptr = oad_dt_ptr-1
              OpenAD_Symbol_1556 = oad_dt(oad_dt_ptr)
              UBAS(OpenAD_Symbol_1551,OpenAD_Symbol_1550)%d = UBAS(OpenAD_Symbol&
     &_1551,OpenAD_Symbol_1550)%d+SLTERM%d*(OpenAD_Symbol_1552)

              DUSRFDEW(OpenAD_Symbol_1551,OpenAD_Symbol_1550)%d = DUSRFDEW(OpenA&
     &D_Symbol_1551,OpenAD_Symbol_1550)%d+SLTERM%d*(OpenAD_Symbol_1553)

              VBAS(OpenAD_Symbol_1551,OpenAD_Symbol_1550)%d = VBAS(OpenAD_Symbol&
     &_1551,OpenAD_Symbol_1550)%d+SLTERM%d*(OpenAD_Symbol_1554)

              DUSRFDNS(OpenAD_Symbol_1551,OpenAD_Symbol_1550)%d = DUSRFDNS(OpenA&
     &D_Symbol_1551,OpenAD_Symbol_1550)%d+SLTERM%d*(OpenAD_Symbol_1555)

              STAGTHCK(OpenAD_Symbol_1551,OpenAD_Symbol_1550)%d = STAGTHCK(OpenA&
     &D_Symbol_1551,OpenAD_Symbol_1550)%d+SLTERM%d*(OpenAD_Symbol_1556)

              OpenAD_prp_29%d = OpenAD_prp_29%d+SLTERM%d
              SLTERM%d = 0.0d0
              SLTERM%d = SLTERM%d+OpenAD_prp_29%d
              OpenAD_prp_29%d = 0.0d0
              OpenAD_Symbol_592 = INT(OpenAD_Symbol_592)+1
            END DO
            OpenAD_Symbol_590 = INT(OpenAD_Symbol_590)+1
          END DO
          SLTERM%d = 0.0d0
        ELSE
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1587 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1588 = oad_it(oad_it_ptr)
          BMLT(OpenAD_Symbol_1588,OpenAD_Symbol_1587)%d = 0.0d0
        ENDIF
        oad_it_ptr = oad_it_ptr-1
        NS = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        EW = oad_it(oad_it_ptr)
        CALL CALCPMPT(PMPTEMP,THCK(EW,NS),SIGMA)
      ELSE
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1589 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1590 = oad_it(oad_it_ptr)
        BMLT(OpenAD_Symbol_1590,OpenAD_Symbol_1589)%d = 0.0d0
      ENDIF
      OpenAD_Symbol_583 = INT(OpenAD_Symbol_583)+1
    END DO
    OpenAD_Symbol_581 = INT(OpenAD_Symbol_581)+1
  END DO
    end if
  end subroutine CALCBMLT
END

!$OPENAD XXX File_start [glide_velo.F90]
MODULE glide_velo
use OAD_active
use w2f__types
use oad_intrinsics
use glimmer_global ,only: DP
use physcon ,only: GN , GRAV , RHOI
IMPLICIT NONE
SAVE
!
!     **** Local Variables and Functions ****
!
REAL(w2f__8) C
PARAMETER ( C = -1.778571004123777771D+11)
INTEGER(w2f__i4) P2
PARAMETER ( P2 = 2)
INTEGER(w2f__i4) P3
PARAMETER ( P3 = 7)
INTEGER(w2f__i4) P4
PARAMETER ( P4 = 5)
!
!     **** Statements ****
!
CONTAINS
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE CALC_VELO(NX, NY, NZ, DX, DY, SIGMA, FLWA, DIFFU, USRF, THCK, UBAS,&
     & VBAS, UVEL, VVEL)

    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  use glimmer_utils
  use glimmer_global ,only: DP
  use glimmer_deriv ,only: DF_FIELD_2D_STAGGERED
  use glimmer_utils
  use glimmer_utils
  use glimmer_global ,only: DP
  use glimmer_deriv ,only: DF_FIELD_2D_STAGGERED
  use glimmer_utils
  use glimmer_utils
  use glimmer_global ,only: DP
  use glimmer_deriv ,only: DF_FIELD_2D_STAGGERED
  use glimmer_utils
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_636
  INTEGER(w2f__i8) OpenAD_Symbol_637
  INTEGER(w2f__i8) OpenAD_Symbol_638
  INTEGER(w2f__i8) OpenAD_Symbol_639
  INTEGER(w2f__i8) OpenAD_Symbol_640
  INTEGER(w2f__i8) OpenAD_Symbol_641
  INTEGER(w2f__i8) OpenAD_Symbol_642
  INTEGER(w2f__i8) OpenAD_Symbol_643
  INTEGER(w2f__i8) OpenAD_Symbol_644
  INTEGER(w2f__i8) OpenAD_Symbol_645
  INTEGER(w2f__i8) OpenAD_Symbol_646
  INTEGER(w2f__i8) OpenAD_Symbol_647
  INTEGER(w2f__i8) OpenAD_Symbol_648
  INTEGER(w2f__i8) OpenAD_Symbol_649
  INTEGER(w2f__i8) OpenAD_Symbol_650
  INTEGER(w2f__i8) OpenAD_Symbol_651
  INTEGER(w2f__i8) OpenAD_Symbol_652
  INTEGER(w2f__i8) OpenAD_Symbol_653
  INTEGER(w2f__i8) OpenAD_Symbol_654
  INTEGER(w2f__i8) OpenAD_Symbol_655
  INTEGER(w2f__i8) OpenAD_Symbol_656
  INTEGER(w2f__i8) OpenAD_Symbol_657
  INTEGER(w2f__i8) OpenAD_Symbol_658
  INTEGER(w2f__i8) OpenAD_Symbol_659
  INTEGER(w2f__i8) OpenAD_Symbol_660
  INTEGER(w2f__i8) OpenAD_Symbol_661
  INTEGER(w2f__i8) OpenAD_Symbol_662
  INTEGER(w2f__i8) OpenAD_Symbol_663
  INTEGER(w2f__i8) OpenAD_Symbol_664
  INTEGER(w2f__i8) OpenAD_Symbol_665
  INTEGER(w2f__i8) OpenAD_Symbol_666
  INTEGER(w2f__i8) OpenAD_Symbol_667
  INTEGER(w2f__i8) OpenAD_Symbol_668
  INTEGER(w2f__i8) OpenAD_Symbol_669
  INTEGER(w2f__i8) OpenAD_Symbol_670
  INTEGER(w2f__i8) OpenAD_Symbol_671
!
!       **** Parameters and Result ****
!
  INTEGER(w2f__i4) NX
  INTEGER(w2f__i4) NY
  INTEGER(w2f__i4) NZ
  REAL(w2f__8) DX
  REAL(w2f__8) DY
  REAL(w2f__8) SIGMA(1 : NZ)
  type(active) :: FLWA(1:NZ,1:NX,1:NY)
  type(active) :: DIFFU(1:INT((NX+(-1))),1:INT((NY+(-1))))
  type(active) :: USRF(1:NX,1:NY)
  type(active) :: THCK(1:NX,1:NY)
  type(active) :: UBAS(1:INT((NX+(-1))),1:INT((NY+(-1))))
  type(active) :: VBAS(1:INT((NX+(-1))),1:INT((NY+(-1))))
  type(active) :: UVEL(1:NZ,1:INT((NX+(-1))),1:INT((NY+(-1))))
  type(active) :: VVEL(1:NZ,1:INT((NX+(-1))),1:INT((NY+(-1))))
!
!       **** Local Variables and Functions ****
!
  type(active) :: CONST(1:3)
  REAL(w2f__8) DEPTH(1 : NZ)
  type(active) :: DINTFLWA(1:INT((NX+(-1))),1:INT((NY+(-1))))
  type(active) :: DUSRFDEW(1:INT((NX+(-1))),1:INT((NY+(-1))))
  type(active) :: DUSRFDNS(1:INT((NX+(-1))),1:INT((NY+(-1))))
  INTEGER(w2f__i4) EW
  type(active) :: FACTOR
  type(active) :: HRZFLWA(1:NZ)
  INTEGER(w2f__i4) NS
  type(active) :: STAGTHCK(1:INT((NX+(-1))),1:INT((NY+(-1))))
  type(active) :: SUMTEMP(1:NZ)
  INTEGER(w2f__i4) UP
  INTEGER(w2f__i4) OpenAD_Symbol_1591
  INTEGER(w2f__i4) OpenAD_Symbol_1592
  INTEGER(w2f__i4) OpenAD_Symbol_1593
  REAL(w2f__8) OpenAD_Symbol_1594
  REAL(w2f__8) OpenAD_Symbol_1595
  INTEGER(w2f__i4) OpenAD_Symbol_1596
  INTEGER(w2f__i4) OpenAD_Symbol_1597
  INTEGER(w2f__i4) OpenAD_Symbol_1598
  INTEGER(w2f__i4) OpenAD_Symbol_1599
  INTEGER(w2f__i4) OpenAD_Symbol_1600
  INTEGER(w2f__i4) OpenAD_Symbol_1601
  INTEGER(w2f__i4) OpenAD_Symbol_1602
  INTEGER(w2f__i4) OpenAD_Symbol_1603
  REAL(w2f__8) OpenAD_Symbol_1604
  REAL(w2f__8) OpenAD_Symbol_1605
  INTEGER(w2f__i4) OpenAD_Symbol_1606
  INTEGER(w2f__i4) OpenAD_Symbol_1607
  REAL(w2f__8) OpenAD_Symbol_1608
  REAL(w2f__8) OpenAD_Symbol_1609
  INTEGER(w2f__i4) OpenAD_Symbol_1610
  INTEGER(w2f__i4) OpenAD_Symbol_1611
  REAL(w2f__8) OpenAD_Symbol_1612
  REAL(w2f__8) OpenAD_Symbol_1613
  INTEGER(w2f__i4) OpenAD_Symbol_1614
  INTEGER(w2f__i4) OpenAD_Symbol_1615
  INTEGER(w2f__i4) OpenAD_Symbol_1616
  REAL(w2f__8) OpenAD_Symbol_1617
  REAL(w2f__8) OpenAD_Symbol_1618
  INTEGER(w2f__i4) OpenAD_Symbol_1619
  INTEGER(w2f__i4) OpenAD_Symbol_1620
  INTEGER(w2f__i4) OpenAD_Symbol_1621
  INTEGER(w2f__i4) OpenAD_Symbol_1622
  REAL(w2f__8) OpenAD_Symbol_1623
  REAL(w2f__8) OpenAD_Symbol_1624
  INTEGER(w2f__i4) OpenAD_Symbol_1625
  INTEGER(w2f__i4) OpenAD_Symbol_1626
  REAL(w2f__8) OpenAD_Symbol_1627
  INTEGER(w2f__i4) OpenAD_Symbol_1628
  INTEGER(w2f__i4) OpenAD_Symbol_1629
  INTEGER(w2f__i4) OpenAD_Symbol_1630
  INTEGER(w2f__i4) OpenAD_Symbol_1631
  INTEGER(w2f__i4) OpenAD_Symbol_672
  INTEGER(w2f__i4) OpenAD_Symbol_673
  INTEGER(w2f__i4) OpenAD_Symbol_674
  REAL(w2f__8) OpenAD_acc_50
  REAL(w2f__8) OpenAD_aux_47
  REAL(w2f__8) OpenAD_aux_48
  REAL(w2f__8) OpenAD_lin_100
  REAL(w2f__8) OpenAD_lin_101
  REAL(w2f__8) OpenAD_lin_102
  REAL(w2f__8) OpenAD_lin_103
  REAL(w2f__8) OpenAD_lin_104
  REAL(w2f__8) OpenAD_lin_92
  REAL(w2f__8) OpenAD_lin_93
  REAL(w2f__8) OpenAD_lin_94
  REAL(w2f__8) OpenAD_lin_95
  REAL(w2f__8) OpenAD_lin_96
  REAL(w2f__8) OpenAD_lin_97
  REAL(w2f__8) OpenAD_lin_98
  REAL(w2f__8) OpenAD_lin_99
  type(active) :: OpenAD_prp_37
  type(active) :: OpenAD_prp_38
  type(active) :: OpenAD_prp_39
  type(active) :: OpenAD_prp_40
  type(active) :: OpenAD_prp_41
  REAL(w2f__8) OpenAD_tyc_75(:, :)
  ALLOCATABLE OpenAD_tyc_75
  REAL(w2f__8) OpenAD_tyc_9(:, :)
  ALLOCATABLE OpenAD_tyc_9


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  DO UP = 1, (NZ +(-1)), 1
    DEPTH(INT(UP)) = ((((SIGMA(UP) + SIGMA(UP + 1)) * 5.0D-01) ** 3) *(SIGMA(UP &
     &+ 1) - SIGMA(UP)))

  END DO
  DEPTH(INT(NZ)) = 0.0D00
  CALL STAGVARB(THCK,STAGTHCK,NX,NY)
  CALL DF_FIELD_2D_STAGGERED(USRF,DX,DY,DUSRFDEW,DUSRFDNS)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_9,STAGTHCK)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_9,STAGTHCK)
  CALL INTEGRATE_FLWA(NX,NY,NZ,SIGMA,DEPTH,DINTFLWA,OpenAD_tyc_9,FLWA)
  CALL VELO_CALC_DIFFU(NX,NY,DINTFLWA,STAGTHCK,DUSRFDEW,DUSRFDNS,DIFFU)
  DO NS = 1,(NY+(-1)),1
    DO EW = 1,(NX+(-1)),1
      IF (STAGTHCK(EW,NS)%v.ne.0.0D00) THEN
        UVEL(INT(NZ),INT(EW),INT(NS))%v = UBAS(EW,NS)%v
        VVEL(INT(NZ),INT(EW),INT(NS))%v = VBAS(EW,NS)%v
        CALL HSUM4(FLWA(1:NZ,EW:EW+1,NS:NS+1),SUMTEMP,2,2,NZ)
        HRZFLWA(1:INT(NZ))%v = SUMTEMP(1:NZ)%v
        FACTOR%v = (DINTFLWA(EW,NS)%v*STAGTHCK(EW,NS)%v)
        IF ((FACTOR%v**2).ne.0.0D00) THEN
          CONST(2)%v = ((DIFFU(EW,NS)%v*(-1.778571004123777771D+11))/FACTOR%v)
          CONST(3)%v = (CONST(2)%v*DUSRFDNS(EW,NS)%v)
          CONST(2)%v = (CONST(2)%v*DUSRFDEW(EW,NS)%v)
        ELSE
          CONST(2:3)%v = 0.0D00
        ENDIF
        DO UP = NZ+(-1),1,(-1)
          CONST(1)%v = (DEPTH(UP)*(HRZFLWA(UP)%v+HRZFLWA(UP+1)%v))
          UVEL(INT(UP),INT(EW),INT(NS))%v = (UVEL(UP+1,EW,NS)%v+CONST(1)%v*CONST&
     &(2)%v)

          VVEL(INT(UP),INT(EW),INT(NS))%v = (VVEL(UP+1,EW,NS)%v+CONST(1)%v*CONST&
     &(3)%v)

        END DO
      ELSE
        UVEL(1:INT(NZ),INT(EW),INT(NS))%v = 0.0D00
        VVEL(1:INT(NZ),INT(EW),INT(NS))%v = 0.0D00
      ENDIF
    END DO
  END DO
    end if
    if (our_rev_mode%tape) then
! taping
  OpenAD_Symbol_646 = 0_w2f__i8
  DO UP = 1,(NZ+(-1)),1
    DEPTH(INT(UP)) = ((((SIGMA(UP)+SIGMA(UP+1))*5.0D-01)**3)*(SIGMA(UP+1)-SIGMA(&
     &UP)))

    OpenAD_Symbol_646 = (INT(OpenAD_Symbol_646)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_646
  oad_it_ptr = oad_it_ptr+1
  DEPTH(INT(NZ)) = 0.0D00
  CALL STAGVARB(THCK,STAGTHCK,NX,NY)
  CALL DF_FIELD_2D_STAGGERED(USRF,DX,DY,DUSRFDEW,DUSRFDNS)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_9,STAGTHCK)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_9,STAGTHCK)
  CALL INTEGRATE_FLWA(NX,NY,NZ,SIGMA,DEPTH,DINTFLWA,OpenAD_tyc_9,FLWA)
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = NX
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = NY
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = NZ
  oad_it_ptr = oad_it_ptr+1
  CALL VELO_CALC_DIFFU(NX,NY,DINTFLWA,STAGTHCK,DUSRFDEW,DUSRFDNS,DIFFU)
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = NX
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = NY
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_647 = 0_w2f__i8
  DO NS = 1,(NY+(-1)),1
    OpenAD_Symbol_648 = 0_w2f__i8
    DO EW = 1,(NX+(-1)),1
      IF (STAGTHCK(EW,NS)%v.ne.0.0D00) THEN
        UVEL(INT(NZ),INT(EW),INT(NS))%v = UBAS(EW,NS)%v
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NZ
        oad_it_ptr = oad_it_ptr+1
        VVEL(INT(NZ),INT(EW),INT(NS))%v = VBAS(EW,NS)%v
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NZ
        oad_it_ptr = oad_it_ptr+1
        CALL HSUM4(FLWA(1:NZ,EW:EW+1,NS:NS+1),SUMTEMP,2,2,NZ)
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NZ
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        HRZFLWA(1:INT(NZ))%v = SUMTEMP(1:NZ)%v
        OpenAD_lin_92 = STAGTHCK(EW,NS)%v
        OpenAD_lin_93 = DINTFLWA(EW,NS)%v
        FACTOR%v = (DINTFLWA(EW,NS)%v*STAGTHCK(EW,NS)%v)
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_lin_92
        oad_dt_ptr = oad_dt_ptr+1
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_lin_93
        oad_dt_ptr = oad_dt_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NZ
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        IF ((FACTOR%v**2).ne.0.0D00) THEN
          OpenAD_aux_47 = (DIFFU(EW,NS)%v*(-1.778571004123777771D+11))
          OpenAD_lin_94 = (INT(1_w2f__i8)/FACTOR%v)
          OpenAD_lin_95 = (-(OpenAD_aux_47/(FACTOR%v*FACTOR%v)))
          CONST(2)%v = (OpenAD_aux_47/FACTOR%v)
          OpenAD_acc_50 = ((-1.778571004123777771D+11)*OpenAD_lin_94)
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_lin_95
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_acc_50
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = EW
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = NS
          oad_it_ptr = oad_it_ptr+1
          OpenAD_lin_96 = DUSRFDNS(EW,NS)%v
          OpenAD_lin_97 = CONST(2)%v
          CONST(3)%v = (CONST(2)%v*DUSRFDNS(EW,NS)%v)
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_lin_96
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_lin_97
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = EW
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = NS
          oad_it_ptr = oad_it_ptr+1
          OpenAD_lin_98 = DUSRFDEW(EW,NS)%v
          OpenAD_lin_99 = CONST(2)%v
          CONST(2)%v = (CONST(2)%v*DUSRFDEW(EW,NS)%v)
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_lin_98
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_lin_99
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = EW
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = NS
          oad_it_ptr = oad_it_ptr+1
          OpenAD_Symbol_649 = 1_w2f__i8
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_649
          oad_it_ptr = oad_it_ptr+1
        ELSE
          CONST(2:3)%v = 0.0D00
          OpenAD_Symbol_650 = 0_w2f__i8
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_650
          oad_it_ptr = oad_it_ptr+1
        ENDIF
        OpenAD_Symbol_651 = 0_w2f__i8
        DO UP = NZ+(-1),1,(-1)
          OpenAD_aux_48 = (HRZFLWA(UP)%v+HRZFLWA(UP+1)%v)
          OpenAD_lin_100 = DEPTH(UP)
          CONST(1)%v = (DEPTH(UP)*OpenAD_aux_48)
          OpenAD_Symbol_672 = (UP+1)
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_672
          oad_it_ptr = oad_it_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_lin_100
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = UP
          oad_it_ptr = oad_it_ptr+1
          OpenAD_lin_101 = CONST(2)%v
          OpenAD_lin_102 = CONST(1)%v
          UVEL(INT(UP),INT(EW),INT(NS))%v = (UVEL(UP+1,EW,NS)%v+CONST(1)%v*CONST&
     &(2)%v)

          OpenAD_Symbol_673 = (UP+1)
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_673
          oad_it_ptr = oad_it_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_lin_101
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_lin_102
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = EW
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = NS
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = UP
          oad_it_ptr = oad_it_ptr+1
          OpenAD_lin_103 = CONST(3)%v
          OpenAD_lin_104 = CONST(1)%v
          VVEL(INT(UP),INT(EW),INT(NS))%v = (VVEL(UP+1,EW,NS)%v+CONST(1)%v*CONST&
     &(3)%v)

          OpenAD_Symbol_674 = (UP+1)
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_674
          oad_it_ptr = oad_it_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_lin_103
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_lin_104
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = EW
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = NS
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = UP
          oad_it_ptr = oad_it_ptr+1
          OpenAD_Symbol_651 = (INT(OpenAD_Symbol_651)+INT(1_w2f__i8))
        END DO
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_651
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_653 = 1_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_653
        oad_it_ptr = oad_it_ptr+1
      ELSE
        UVEL(1:INT(NZ),INT(EW),INT(NS))%v = 0.0D00
        VVEL(1:INT(NZ),INT(EW),INT(NS))%v = 0.0D00
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NZ
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_652 = 0_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_652
        oad_it_ptr = oad_it_ptr+1
      ENDIF
      OpenAD_Symbol_648 = (INT(OpenAD_Symbol_648)+INT(1_w2f__i8))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_648
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_647 = (INT(OpenAD_Symbol_647)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_647
  oad_it_ptr = oad_it_ptr+1
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_636 = oad_it(oad_it_ptr)
  OpenAD_Symbol_637 = 1
  do while (INT(OpenAD_Symbol_637).LE.INT(OpenAD_Symbol_636))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_638 = oad_it(oad_it_ptr)
    OpenAD_Symbol_639 = 1
    do while (INT(OpenAD_Symbol_639).LE.INT(OpenAD_Symbol_638))
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_640 = oad_it(oad_it_ptr)
      IF (OpenAD_Symbol_640.ne.0) THEN
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_641 = oad_it(oad_it_ptr)
        OpenAD_Symbol_642 = 1
        do while (INT(OpenAD_Symbol_642).LE.INT(OpenAD_Symbol_641))
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1614 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1615 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1616 = oad_it(oad_it_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1617 = oad_dt(oad_dt_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1618 = oad_dt(oad_dt_ptr)
          CONST(3)%d = CONST(3)%d+VVEL(OpenAD_Symbol_1614,OpenAD_Symbol_1616,Ope&
     &nAD_Symbol_1615)%d*(OpenAD_Symbol_1617)

          CONST(1)%d = CONST(1)%d+VVEL(OpenAD_Symbol_1614,OpenAD_Symbol_1616,Ope&
     &nAD_Symbol_1615)%d*(OpenAD_Symbol_1618)

          OpenAD_prp_41%d = OpenAD_prp_41%d+VVEL(OpenAD_Symbol_1614,OpenAD_Symbo&
     &l_1616,OpenAD_Symbol_1615)%d

          VVEL(OpenAD_Symbol_1614,OpenAD_Symbol_1616,OpenAD_Symbol_1615)%d = 0.0&
     &d0

          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1619 = oad_it(oad_it_ptr)
          VVEL(OpenAD_Symbol_1619,OpenAD_Symbol_1616,OpenAD_Symbol_1615)%d = VVE&
     &L(OpenAD_Symbol_1619,OpenAD_Symbol_1616,OpenAD_Symbol_1615)%d+OpenAD_prp_4&
     &1%d

          OpenAD_prp_41%d = 0.0d0
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1620 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1621 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1622 = oad_it(oad_it_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1623 = oad_dt(oad_dt_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1624 = oad_dt(oad_dt_ptr)
          CONST(2)%d = CONST(2)%d+UVEL(OpenAD_Symbol_1620,OpenAD_Symbol_1622,Ope&
     &nAD_Symbol_1621)%d*(OpenAD_Symbol_1623)

          CONST(1)%d = CONST(1)%d+UVEL(OpenAD_Symbol_1620,OpenAD_Symbol_1622,Ope&
     &nAD_Symbol_1621)%d*(OpenAD_Symbol_1624)

          OpenAD_prp_40%d = OpenAD_prp_40%d+UVEL(OpenAD_Symbol_1620,OpenAD_Symbo&
     &l_1622,OpenAD_Symbol_1621)%d

          UVEL(OpenAD_Symbol_1620,OpenAD_Symbol_1622,OpenAD_Symbol_1621)%d = 0.0&
     &d0

          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1625 = oad_it(oad_it_ptr)
          UVEL(OpenAD_Symbol_1625,OpenAD_Symbol_1622,OpenAD_Symbol_1621)%d = UVE&
     &L(OpenAD_Symbol_1625,OpenAD_Symbol_1622,OpenAD_Symbol_1621)%d+OpenAD_prp_4&
     &0%d

          OpenAD_prp_40%d = 0.0d0
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1626 = oad_it(oad_it_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1627 = oad_dt(oad_dt_ptr)
          OpenAD_prp_39%d = OpenAD_prp_39%d+CONST(1)%d*(OpenAD_Symbol_1627)
          CONST(1)%d = 0.0d0
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1628 = oad_it(oad_it_ptr)
          HRZFLWA(OpenAD_Symbol_1628)%d = HRZFLWA(OpenAD_Symbol_1628)%d+OpenAD_p&
     &rp_39%d

          HRZFLWA(OpenAD_Symbol_1626)%d = HRZFLWA(OpenAD_Symbol_1626)%d+OpenAD_p&
     &rp_39%d

          OpenAD_prp_39%d = 0.0d0
          OpenAD_Symbol_642 = INT(OpenAD_Symbol_642)+1
        END DO
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_643 = oad_it(oad_it_ptr)
        IF (OpenAD_Symbol_643.ne.0) THEN
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1602 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1603 = oad_it(oad_it_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1604 = oad_dt(oad_dt_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1605 = oad_dt(oad_dt_ptr)
          DUSRFDEW(OpenAD_Symbol_1603,OpenAD_Symbol_1602)%d = DUSRFDEW(OpenAD_Sy&
     &mbol_1603,OpenAD_Symbol_1602)%d+CONST(2)%d*(OpenAD_Symbol_1604)

          OpenAD_prp_38%d = OpenAD_prp_38%d+CONST(2)%d*(OpenAD_Symbol_1605)
          CONST(2)%d = 0.0d0
          CONST(2)%d = CONST(2)%d+OpenAD_prp_38%d
          OpenAD_prp_38%d = 0.0d0
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1606 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1607 = oad_it(oad_it_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1608 = oad_dt(oad_dt_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1609 = oad_dt(oad_dt_ptr)
          DUSRFDNS(OpenAD_Symbol_1607,OpenAD_Symbol_1606)%d = DUSRFDNS(OpenAD_Sy&
     &mbol_1607,OpenAD_Symbol_1606)%d+CONST(3)%d*(OpenAD_Symbol_1608)

          OpenAD_prp_37%d = OpenAD_prp_37%d+CONST(3)%d*(OpenAD_Symbol_1609)
          CONST(3)%d = 0.0d0
          CONST(2)%d = CONST(2)%d+OpenAD_prp_37%d
          OpenAD_prp_37%d = 0.0d0
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1610 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1611 = oad_it(oad_it_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1612 = oad_dt(oad_dt_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1613 = oad_dt(oad_dt_ptr)
          DIFFU(OpenAD_Symbol_1611,OpenAD_Symbol_1610)%d = DIFFU(OpenAD_Symbol_1&
     &611,OpenAD_Symbol_1610)%d+CONST(2)%d*(OpenAD_Symbol_1612)

          FACTOR%d = FACTOR%d+CONST(2)%d*(OpenAD_Symbol_1613)
          CONST(2)%d = 0.0d0
        ELSE
          CONST(2:3)%d = 0.0d0
        ENDIF
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1591 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1592 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1593 = oad_it(oad_it_ptr)
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1594 = oad_dt(oad_dt_ptr)
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1595 = oad_dt(oad_dt_ptr)
        STAGTHCK(OpenAD_Symbol_1592,OpenAD_Symbol_1591)%d = STAGTHCK(OpenAD_Symb&
     &ol_1592,OpenAD_Symbol_1591)%d+FACTOR%d*(OpenAD_Symbol_1594)

        DINTFLWA(OpenAD_Symbol_1592,OpenAD_Symbol_1591)%d = DINTFLWA(OpenAD_Symb&
     &ol_1592,OpenAD_Symbol_1591)%d+FACTOR%d*(OpenAD_Symbol_1595)

        FACTOR%d = 0.0d0
        SUMTEMP(1:OpenAD_Symbol_1593)%d = SUMTEMP(1:OpenAD_Symbol_1593)%d+HRZFLW&
     &A(1:OpenAD_Symbol_1593)%d

        HRZFLWA(1:OpenAD_Symbol_1593)%d = 0.0d0
        oad_it_ptr = oad_it_ptr-1
        NS = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        EW = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        NZ = oad_it(oad_it_ptr)
        CALL HSUM4(FLWA(1:NZ,EW:EW+1,NS:NS+1),SUMTEMP,2,2,NZ)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1596 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1597 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1598 = oad_it(oad_it_ptr)
        VBAS(OpenAD_Symbol_1598,OpenAD_Symbol_1597)%d = VBAS(OpenAD_Symbol_1598,&
     &OpenAD_Symbol_1597)%d+VVEL(OpenAD_Symbol_1596,OpenAD_Symbol_1598,OpenAD_Sy&
     &mbol_1597)%d

        VVEL(OpenAD_Symbol_1596,OpenAD_Symbol_1598,OpenAD_Symbol_1597)%d = 0.0d0
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1599 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1600 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1601 = oad_it(oad_it_ptr)
        UBAS(OpenAD_Symbol_1601,OpenAD_Symbol_1600)%d = UBAS(OpenAD_Symbol_1601,&
     &OpenAD_Symbol_1600)%d+UVEL(OpenAD_Symbol_1599,OpenAD_Symbol_1601,OpenAD_Sy&
     &mbol_1600)%d

        UVEL(OpenAD_Symbol_1599,OpenAD_Symbol_1601,OpenAD_Symbol_1600)%d = 0.0d0
      ELSE
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1629 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1630 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1631 = oad_it(oad_it_ptr)
        VVEL(1:OpenAD_Symbol_1631,OpenAD_Symbol_1630,OpenAD_Symbol_1629)%d = 0.0&
     &d0

        UVEL(1:OpenAD_Symbol_1631,OpenAD_Symbol_1630,OpenAD_Symbol_1629)%d = 0.0&
     &d0

      ENDIF
      OpenAD_Symbol_639 = INT(OpenAD_Symbol_639)+1
    END DO
    OpenAD_Symbol_637 = INT(OpenAD_Symbol_637)+1
  END DO
  oad_it_ptr = oad_it_ptr-1
  NY = oad_it(oad_it_ptr)
  oad_it_ptr = oad_it_ptr-1
  NX = oad_it(oad_it_ptr)
  CALL VELO_CALC_DIFFU(NX,NY,DINTFLWA,STAGTHCK,DUSRFDEW,DUSRFDNS,DIFFU)
  oad_it_ptr = oad_it_ptr-1
  NZ = oad_it(oad_it_ptr)
  oad_it_ptr = oad_it_ptr-1
  NY = oad_it(oad_it_ptr)
  oad_it_ptr = oad_it_ptr-1
  NX = oad_it(oad_it_ptr)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_75,STAGTHCK)
  CALL INTEGRATE_FLWA(NX,NY,NZ,SIGMA,DEPTH,DINTFLWA,OpenAD_tyc_75,FLWA)
  CALL DF_FIELD_2D_STAGGERED(USRF,DX,DY,DUSRFDEW,DUSRFDNS)
  CALL STAGVARB(THCK,STAGTHCK,NX,NY)
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_644 = oad_it(oad_it_ptr)
  OpenAD_Symbol_645 = 1
  do while (INT(OpenAD_Symbol_645).LE.INT(OpenAD_Symbol_644))
    OpenAD_Symbol_645 = INT(OpenAD_Symbol_645)+1
  END DO
    end if
  end subroutine CALC_VELO
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE VELO_CALC_DIFFU(NX, NY, DINTFLWA, STAGTHCK, DUSRFDEW , DUSRFDNS, DI&
     &FFU)

    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  use glimmer_global ,only: DP
  use glimmer_global ,only: DP
  use glimmer_global ,only: DP
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_707
  INTEGER(w2f__i8) OpenAD_Symbol_708
  INTEGER(w2f__i8) OpenAD_Symbol_709
  INTEGER(w2f__i8) OpenAD_Symbol_710
  INTEGER(w2f__i8) OpenAD_Symbol_711
  INTEGER(w2f__i8) OpenAD_Symbol_712
  INTEGER(w2f__i8) OpenAD_Symbol_713
  INTEGER(w2f__i8) OpenAD_Symbol_714
  INTEGER(w2f__i8) OpenAD_Symbol_715
  INTEGER(w2f__i8) OpenAD_Symbol_716
  INTEGER(w2f__i8) OpenAD_Symbol_717
  INTEGER(w2f__i8) OpenAD_Symbol_718
  INTEGER(w2f__i8) OpenAD_Symbol_719
  INTEGER(w2f__i8) OpenAD_Symbol_720
  INTEGER(w2f__i8) OpenAD_Symbol_721
  INTEGER(w2f__i8) OpenAD_Symbol_722
  INTEGER(w2f__i8) OpenAD_Symbol_723
  INTEGER(w2f__i8) OpenAD_Symbol_724
!
!       **** Parameters and Result ****
!
  INTEGER(w2f__i4) NX
  INTEGER(w2f__i4) NY
  type(active) :: DINTFLWA(1:INT((NX+(-1))),1:INT((NY+(-1))))
  type(active) :: STAGTHCK(1:INT((NX+(-1))),1:INT((NY+(-1))))
  type(active) :: DUSRFDEW(1:INT((NX+(-1))),1:INT((NY+(-1))))
  type(active) :: DUSRFDNS(1:INT((NX+(-1))),1:INT((NY+(-1))))
  type(active) :: DIFFU(1:INT((NX+(-1))),1:INT((NY+(-1))))
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  INTEGER(w2f__i4) OpenAD_Symbol_1641
  INTEGER(w2f__i4) OpenAD_Symbol_1642
  REAL(w2f__8) OpenAD_Symbol_1643
  REAL(w2f__8) OpenAD_Symbol_1644
  REAL(w2f__8) OpenAD_Symbol_1645
  REAL(w2f__8) OpenAD_Symbol_1646
  INTEGER(w2f__i4) OpenAD_Symbol_1647
  INTEGER(w2f__i4) OpenAD_Symbol_1648
  REAL(w2f__8) OpenAD_acc_51
  REAL(w2f__8) OpenAD_acc_52
  REAL(w2f__8) OpenAD_acc_53
  REAL(w2f__8) OpenAD_acc_54
  REAL(w2f__8) OpenAD_acc_55
  REAL(w2f__8) OpenAD_aux_50
  REAL(w2f__8) OpenAD_aux_51
  REAL(w2f__8) OpenAD_aux_52
  REAL(w2f__8) OpenAD_aux_53
  REAL(w2f__8) OpenAD_lin_106
  REAL(w2f__8) OpenAD_lin_107
  REAL(w2f__8) OpenAD_lin_108
  REAL(w2f__8) OpenAD_lin_109
  REAL(w2f__8) OpenAD_lin_110
  REAL(w2f__8) OpenAD_lin_111
  REAL(w2f__8) OpenAD_lin_112
  REAL(w2f__8) OpenAD_lin_113
  REAL(w2f__8) OpenAD_lin_114
  type(active) :: OpenAD_prp_44
  type(active) :: OpenAD_prp_45
  type(active) :: OpenAD_prp_46


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  DO I = 1, (NX +(-1)), 1
    DO J = 1, (NY +(-1)), 1
      IF ((DIFFU(I,J)%v.GT.0.0D00).AND.(((DUSRFDNS(I,J)%v**2).ne.0.0D00).AND.((S&
     &TAGTHCK(I,J)%v.ne.0.0D00).AND.((DUSRFDEW(I,J)%v**2).ne.0.0D00)))) THEN

        DIFFU(INT(I),INT(J))%v = ((SQRT((DUSRFDEW(I,J)%v**2)+(DUSRFDNS(I,J)%v**2&
     &))**2)*DINTFLWA(I,J)%v*(STAGTHCK(I,J)%v**5))

      ELSE
        DIFFU(INT(I),INT(J))%v = 0.0D00
      ENDIF
    END DO
  END DO
    end if
    if (our_rev_mode%tape) then
! taping
  OpenAD_Symbol_712 = 0_w2f__i8
  DO I = 1,(NX+(-1)),1
    OpenAD_Symbol_713 = 0_w2f__i8
    DO J = 1,(NY+(-1)),1
      IF ((DIFFU(I,J)%v.GT.0.0D00).AND.(((DUSRFDNS(I,J)%v**2).ne.0.0D00).AND.((S&
     &TAGTHCK(I,J)%v.ne.0.0D00).AND.((DUSRFDEW(I,J)%v**2).ne.0.0D00)))) THEN

        OpenAD_aux_52 = SQRT((DUSRFDEW(I,J)%v**2)+(DUSRFDNS(I,J)%v**2))
        OpenAD_aux_50 = (OpenAD_aux_52**2)
        OpenAD_aux_53 = (STAGTHCK(I,J)%v**5)
        OpenAD_aux_51 = (DINTFLWA(I,J)%v*OpenAD_aux_53)
        OpenAD_lin_110 = (2*(DUSRFDEW(I,J)%v**(2-INT(1_w2f__i8))))
        OpenAD_lin_111 = (2*(DUSRFDNS(I,J)%v**(2-INT(1_w2f__i8))))
        OpenAD_lin_109 = (5.0D-01/OpenAD_aux_52)
        OpenAD_lin_108 = (2*(OpenAD_aux_52**(2-INT(1_w2f__i8))))
        OpenAD_lin_106 = OpenAD_aux_51
        OpenAD_lin_112 = OpenAD_aux_53
        OpenAD_lin_114 = (5*(STAGTHCK(I,J)%v**(5-INT(1_w2f__i8))))
        OpenAD_lin_113 = DINTFLWA(I,J)%v
        OpenAD_lin_107 = OpenAD_aux_50
        DIFFU(INT(I),INT(J))%v = (OpenAD_aux_50*OpenAD_aux_51)
        OpenAD_acc_51 = (OpenAD_lin_109*OpenAD_lin_108*OpenAD_lin_106)
        OpenAD_acc_52 = (OpenAD_lin_112*OpenAD_lin_107)
        OpenAD_acc_53 = (OpenAD_lin_114*OpenAD_lin_113*OpenAD_lin_107)
        OpenAD_acc_54 = (OpenAD_lin_111*OpenAD_acc_51)
        OpenAD_acc_55 = (OpenAD_lin_110*OpenAD_acc_51)
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_acc_52
        oad_dt_ptr = oad_dt_ptr+1
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_acc_53
        oad_dt_ptr = oad_dt_ptr+1
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_acc_54
        oad_dt_ptr = oad_dt_ptr+1
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_acc_55
        oad_dt_ptr = oad_dt_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = I
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = J
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_714 = 1_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_714
        oad_it_ptr = oad_it_ptr+1
      ELSE
        DIFFU(INT(I),INT(J))%v = 0.0D00
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = I
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = J
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_715 = 0_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_715
        oad_it_ptr = oad_it_ptr+1
      ENDIF
      OpenAD_Symbol_713 = (INT(OpenAD_Symbol_713)+INT(1_w2f__i8))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_713
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_712 = (INT(OpenAD_Symbol_712)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_712
  oad_it_ptr = oad_it_ptr+1
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_707 = oad_it(oad_it_ptr)
  OpenAD_Symbol_708 = 1
  do while (INT(OpenAD_Symbol_708).LE.INT(OpenAD_Symbol_707))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_709 = oad_it(oad_it_ptr)
    OpenAD_Symbol_710 = 1
    do while (INT(OpenAD_Symbol_710).LE.INT(OpenAD_Symbol_709))
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_711 = oad_it(oad_it_ptr)
      IF (OpenAD_Symbol_711.ne.0) THEN
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1641 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1642 = oad_it(oad_it_ptr)
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1643 = oad_dt(oad_dt_ptr)
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1644 = oad_dt(oad_dt_ptr)
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1645 = oad_dt(oad_dt_ptr)
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1646 = oad_dt(oad_dt_ptr)
        OpenAD_prp_44%d = OpenAD_prp_44%d+DIFFU(OpenAD_Symbol_1642,OpenAD_Symbol&
     &_1641)%d*(OpenAD_Symbol_1643)

        OpenAD_prp_45%d = OpenAD_prp_45%d+DIFFU(OpenAD_Symbol_1642,OpenAD_Symbol&
     &_1641)%d*(OpenAD_Symbol_1644)

        OpenAD_prp_46%d = OpenAD_prp_46%d+DIFFU(OpenAD_Symbol_1642,OpenAD_Symbol&
     &_1641)%d*(OpenAD_Symbol_1645)

        DINTFLWA(OpenAD_Symbol_1642,OpenAD_Symbol_1641)%d = DINTFLWA(OpenAD_Symb&
     &ol_1642,OpenAD_Symbol_1641)%d+DIFFU(OpenAD_Symbol_1642,OpenAD_Symbol_1641)&
     &%d*(OpenAD_Symbol_1646)

        DIFFU(OpenAD_Symbol_1642,OpenAD_Symbol_1641)%d = 0.0d0
        STAGTHCK(OpenAD_Symbol_1642,OpenAD_Symbol_1641)%d = STAGTHCK(OpenAD_Symb&
     &ol_1642,OpenAD_Symbol_1641)%d+OpenAD_prp_46%d

        OpenAD_prp_46%d = 0.0d0
        DUSRFDNS(OpenAD_Symbol_1642,OpenAD_Symbol_1641)%d = DUSRFDNS(OpenAD_Symb&
     &ol_1642,OpenAD_Symbol_1641)%d+OpenAD_prp_45%d

        OpenAD_prp_45%d = 0.0d0
        DUSRFDEW(OpenAD_Symbol_1642,OpenAD_Symbol_1641)%d = DUSRFDEW(OpenAD_Symb&
     &ol_1642,OpenAD_Symbol_1641)%d+OpenAD_prp_44%d

        OpenAD_prp_44%d = 0.0d0
      ELSE
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1647 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1648 = oad_it(oad_it_ptr)
        DIFFU(OpenAD_Symbol_1648,OpenAD_Symbol_1647)%d = 0.0d0
      ENDIF
      OpenAD_Symbol_710 = INT(OpenAD_Symbol_710)+1
    END DO
    OpenAD_Symbol_708 = INT(OpenAD_Symbol_708)+1
  END DO
    end if
  end subroutine VELO_CALC_DIFFU
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE INTEGRATE_FLWA(NX, NY, NZ, SIGMA, DEPTH, DINTFLWA, STAGTHCK, FLWA)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  use glimmer_utils
  use glimmer_global ,only: DP
  use glimmer_utils
  use glimmer_global ,only: DP
  use glimmer_utils
  use glimmer_global ,only: DP
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_675
  INTEGER(w2f__i8) OpenAD_Symbol_676
  INTEGER(w2f__i8) OpenAD_Symbol_677
  INTEGER(w2f__i8) OpenAD_Symbol_678
  INTEGER(w2f__i8) OpenAD_Symbol_679
  INTEGER(w2f__i8) OpenAD_Symbol_680
  INTEGER(w2f__i8) OpenAD_Symbol_681
  INTEGER(w2f__i8) OpenAD_Symbol_682
  INTEGER(w2f__i8) OpenAD_Symbol_683
  INTEGER(w2f__i8) OpenAD_Symbol_684
  INTEGER(w2f__i8) OpenAD_Symbol_685
  INTEGER(w2f__i8) OpenAD_Symbol_686
  INTEGER(w2f__i8) OpenAD_Symbol_687
  INTEGER(w2f__i8) OpenAD_Symbol_688
  INTEGER(w2f__i8) OpenAD_Symbol_689
  INTEGER(w2f__i8) OpenAD_Symbol_690
  INTEGER(w2f__i8) OpenAD_Symbol_691
  INTEGER(w2f__i8) OpenAD_Symbol_692
  INTEGER(w2f__i8) OpenAD_Symbol_693
  INTEGER(w2f__i8) OpenAD_Symbol_694
  INTEGER(w2f__i8) OpenAD_Symbol_695
  INTEGER(w2f__i8) OpenAD_Symbol_696
  INTEGER(w2f__i8) OpenAD_Symbol_697
  INTEGER(w2f__i8) OpenAD_Symbol_698
  INTEGER(w2f__i8) OpenAD_Symbol_699
  INTEGER(w2f__i8) OpenAD_Symbol_700
  INTEGER(w2f__i8) OpenAD_Symbol_701
  INTEGER(w2f__i8) OpenAD_Symbol_702
  INTEGER(w2f__i8) OpenAD_Symbol_703
  INTEGER(w2f__i8) OpenAD_Symbol_704
!
!       **** Parameters and Result ****
!
  INTEGER(w2f__i4) NX
  INTEGER(w2f__i4) NY
  INTEGER(w2f__i4) NZ
  REAL(w2f__8) SIGMA(1 : NZ)
  REAL(w2f__8) DEPTH(1 : NZ)
  type(active) :: DINTFLWA(1:INT((NX+(-1))),1:INT((NY+(-1))))
  REAL(w2f__8) STAGTHCK(1 : INT((NX +(-1))), 1 : INT((NY +(-1))))
  type(active) :: FLWA(1:NZ,1:NX,1:NY)
!
!       **** Local Variables and Functions ****
!
  REAL(w2f__8) DUPS(1 : NZ)
  INTEGER(w2f__i4) EW
  type(active) :: HRZFLWA(1:NZ)
  type(active) :: INTFLWA(1:NZ)
  INTEGER(w2f__i4) NS
  type(active) :: OAD_CTMP0
  INTEGER(w2f__i4) UP
  INTEGER(w2f__i4) OpenAD_Symbol_1632
  INTEGER(w2f__i4) OpenAD_Symbol_1633
  REAL(w2f__8) OpenAD_Symbol_1634
  INTEGER(w2f__i4) OpenAD_Symbol_1635
  INTEGER(w2f__i4) OpenAD_Symbol_1636
  INTEGER(w2f__i4) OpenAD_Symbol_1637
  INTEGER(w2f__i4) OpenAD_Symbol_1638
  INTEGER(w2f__i4) OpenAD_Symbol_1639
  INTEGER(w2f__i4) OpenAD_Symbol_1640
  INTEGER(w2f__i4) OpenAD_Symbol_705
  INTEGER(w2f__i4) OpenAD_Symbol_706
  REAL(w2f__8) OpenAD_aux_49
  REAL(w2f__8) OpenAD_lin_105
  type(active) :: OpenAD_prp_42
  type(active) :: OpenAD_prp_43


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  DO UP = 1, (NZ +(-1)), 1
    DUPS(INT(UP)) = (SIGMA(UP + 1) - SIGMA(UP))
  END DO
  DUPS(INT(NZ)) = 0.0D00
  DO NS = 1, (NY +(-1)), 1
    DO EW = 1, (NX +(-1)), 1
      IF(STAGTHCK(EW, NS) .ne. 0.0D00) THEN
        CALL HSUM4(FLWA(1:NZ,EW:EW+1,NS:NS+1),HRZFLWA,2,2,NZ)
        INTFLWA(INT(NZ))%v = 0.0D00
        DO UP = NZ+(-1),1,(-1)
          INTFLWA(INT(UP))%v = (INTFLWA(UP+1)%v+DEPTH(UP)*(HRZFLWA(UP)%v+HRZFLWA&
     &(UP+1)%v))

        END DO
        CALL OAD_S_VERTINTG(DUPS,INTFLWA,OAD_CTMP0)
        DINTFLWA(INT(EW),INT(NS))%v = (OAD_CTMP0%v*(-1.778571004123777771D+11))
      ELSE
        DINTFLWA(INT(EW),INT(NS))%v = 0.0D00
      ENDIF
    END DO
  END DO
    end if
    if (our_rev_mode%tape) then
! taping
  OpenAD_Symbol_684 = 0_w2f__i8
  DO UP = 1,(NZ+(-1)),1
    DUPS(INT(UP)) = (SIGMA(UP+1)-SIGMA(UP))
    OpenAD_Symbol_684 = (INT(OpenAD_Symbol_684)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_684
  oad_it_ptr = oad_it_ptr+1
  DUPS(INT(NZ)) = 0.0D00
  OpenAD_Symbol_685 = 0_w2f__i8
  DO NS = 1,(NY+(-1)),1
    OpenAD_Symbol_686 = 0_w2f__i8
    DO EW = 1,(NX+(-1)),1
      IF (STAGTHCK(EW,NS).ne.0.0D00) THEN
        CALL HSUM4(FLWA(1:NZ,EW:EW+1,NS:NS+1),HRZFLWA,2,2,NZ)
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NZ
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        INTFLWA(INT(NZ))%v = 0.0D00
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NZ
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_687 = 0_w2f__i8
        DO UP = NZ+(-1),1,(-1)
          OpenAD_aux_49 = (HRZFLWA(UP)%v+HRZFLWA(UP+1)%v)
          OpenAD_lin_105 = DEPTH(UP)
          INTFLWA(INT(UP))%v = (INTFLWA(UP+1)%v+DEPTH(UP)*OpenAD_aux_49)
          OpenAD_Symbol_705 = (UP+1)
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_705
          oad_it_ptr = oad_it_ptr+1
          OpenAD_Symbol_706 = (UP+1)
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_706
          oad_it_ptr = oad_it_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_lin_105
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = UP
          oad_it_ptr = oad_it_ptr+1
          OpenAD_Symbol_687 = (INT(OpenAD_Symbol_687)+INT(1_w2f__i8))
        END DO
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_687
        oad_it_ptr = oad_it_ptr+1
        CALL OAD_S_VERTINTG(DUPS,INTFLWA,OAD_CTMP0)
        oad_chunk_size = size(DUPS,1)
        if (oad_dt_sz.lt.oad_dt_ptr+oad_chunk_size) call oad_dt_grow()
        oad_dt(oad_dt_ptr:oad_dt_ptr+oad_chunk_size-1) = DUPS
        oad_dt_ptr = oad_dt_ptr+oad_chunk_size
        DINTFLWA(INT(EW),INT(NS))%v = (OAD_CTMP0%v*(-1.778571004123777771D+11))
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_688 = 1_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_688
        oad_it_ptr = oad_it_ptr+1
      ELSE
        DINTFLWA(INT(EW),INT(NS))%v = 0.0D00
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_689 = 0_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_689
        oad_it_ptr = oad_it_ptr+1
      ENDIF
      OpenAD_Symbol_686 = (INT(OpenAD_Symbol_686)+INT(1_w2f__i8))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_686
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_685 = (INT(OpenAD_Symbol_685)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_685
  oad_it_ptr = oad_it_ptr+1
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_675 = oad_it(oad_it_ptr)
  OpenAD_Symbol_676 = 1
  do while (INT(OpenAD_Symbol_676).LE.INT(OpenAD_Symbol_675))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_677 = oad_it(oad_it_ptr)
    OpenAD_Symbol_678 = 1
    do while (INT(OpenAD_Symbol_678).LE.INT(OpenAD_Symbol_677))
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_679 = oad_it(oad_it_ptr)
      IF (OpenAD_Symbol_679.ne.0) THEN
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1637 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1638 = oad_it(oad_it_ptr)
        OAD_CTMP0%d = OAD_CTMP0%d+DINTFLWA(OpenAD_Symbol_1638,OpenAD_Symbol_1637&
     &)%d*(-1.778571004123777771D+11)

        DINTFLWA(OpenAD_Symbol_1638,OpenAD_Symbol_1637)%d = 0.0d0
        oad_chunk_size = size(DUPS,1)
        oad_dt_ptr = oad_dt_ptr-oad_chunk_size
        DUPS = oad_dt(oad_dt_ptr:oad_dt_ptr+oad_chunk_size-1)
        CALL OAD_S_VERTINTG(DUPS,INTFLWA,OAD_CTMP0)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_680 = oad_it(oad_it_ptr)
        OpenAD_Symbol_681 = 1
        do while (INT(OpenAD_Symbol_681).LE.INT(OpenAD_Symbol_680))
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1633 = oad_it(oad_it_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1634 = oad_dt(oad_dt_ptr)
          OpenAD_prp_43%d = OpenAD_prp_43%d+INTFLWA(OpenAD_Symbol_1633)%d*(OpenA&
     &D_Symbol_1634)

          OpenAD_prp_42%d = OpenAD_prp_42%d+INTFLWA(OpenAD_Symbol_1633)%d
          INTFLWA(OpenAD_Symbol_1633)%d = 0.0d0
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1635 = oad_it(oad_it_ptr)
          HRZFLWA(OpenAD_Symbol_1635)%d = HRZFLWA(OpenAD_Symbol_1635)%d+OpenAD_p&
     &rp_43%d

          HRZFLWA(OpenAD_Symbol_1633)%d = HRZFLWA(OpenAD_Symbol_1633)%d+OpenAD_p&
     &rp_43%d

          OpenAD_prp_43%d = 0.0d0
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1636 = oad_it(oad_it_ptr)
          INTFLWA(OpenAD_Symbol_1636)%d = INTFLWA(OpenAD_Symbol_1636)%d+OpenAD_p&
     &rp_42%d

          OpenAD_prp_42%d = 0.0d0
          OpenAD_Symbol_681 = INT(OpenAD_Symbol_681)+1
        END DO
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1632 = oad_it(oad_it_ptr)
        INTFLWA(OpenAD_Symbol_1632)%d = 0.0d0
        oad_it_ptr = oad_it_ptr-1
        NS = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        EW = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        NZ = oad_it(oad_it_ptr)
        CALL HSUM4(FLWA(1:NZ,EW:EW+1,NS:NS+1),HRZFLWA,2,2,NZ)
      ELSE
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1639 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1640 = oad_it(oad_it_ptr)
        DINTFLWA(OpenAD_Symbol_1640,OpenAD_Symbol_1639)%d = 0.0d0
      ENDIF
      OpenAD_Symbol_678 = INT(OpenAD_Symbol_678)+1
    END DO
    OpenAD_Symbol_676 = INT(OpenAD_Symbol_676)+1
  END DO
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_682 = oad_it(oad_it_ptr)
  OpenAD_Symbol_683 = 1
  do while (INT(OpenAD_Symbol_683).LE.INT(OpenAD_Symbol_682))
    OpenAD_Symbol_683 = INT(OpenAD_Symbol_683)+1
  END DO
    end if
  end subroutine INTEGRATE_FLWA
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE CALCVERTICALVELOCITY(NX, NY, NZ, DX, DY, SIGMA, WHICHWVEL, THKLIM, &
     &THCK, USRF, DTHCKDTM, DUSRFDTM, BMLT, ACAB, UVEL, VVEL, WVEL, WGRD)

    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  use glimmer_deriv ,only: DF_FIELD_2D_STAGGERED
  use glimmer_utils ,only: STAGVARB
  use glimmer_deriv ,only: DF_FIELD_2D_STAGGERED
  use glimmer_utils ,only: STAGVARB
  use glimmer_deriv ,only: DF_FIELD_2D_STAGGERED
  use glimmer_utils ,only: STAGVARB
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_725
  INTEGER(w2f__i8) OpenAD_Symbol_726
  INTEGER(w2f__i8) OpenAD_Symbol_727
  INTEGER(w2f__i8) OpenAD_Symbol_728
  INTEGER(w2f__i8) OpenAD_Symbol_729
  INTEGER(w2f__i8) OpenAD_Symbol_730
!
!       **** Parameters and Result ****
!
  INTEGER(w2f__i4) NX
  INTEGER(w2f__i4) NY
  INTEGER(w2f__i4) NZ
  REAL(w2f__8) DX
  REAL(w2f__8) DY
  REAL(w2f__8) SIGMA(1 : NZ)
  INTEGER(w2f__i4) WHICHWVEL
  REAL(w2f__8) THKLIM
  type(active) :: THCK(1:NX,1:NY)
  type(active) :: USRF(1:NX,1:NY)
  type(active) :: DTHCKDTM(1:NX,1:NY)
  type(active) :: DUSRFDTM(1:NX,1:NY)
  type(active) :: BMLT(1:NX,1:NY)
  type(active) :: ACAB(1:NX,1:NY)
  type(active) :: UVEL(1:NZ,1:INT((NX+(-1))),1:INT((NY+(-1))))
  type(active) :: VVEL(1:NZ,1:INT((NX+(-1))),1:INT((NY+(-1))))
  type(active) :: WVEL(1:NZ,1:NX,1:NY)
  type(active) :: WGRD(1:NZ,1:NX,1:NY)
!
!       **** Local Variables and Functions ****
!
  type(active) :: DTHCKDEW(1:INT((NX+(-1))),1:INT((NY+(-1))))
  type(active) :: DTHCKDNS(1:INT((NX+(-1))),1:INT((NY+(-1))))
  type(active) :: DUSRFDEW(1:INT((NX+(-1))),1:INT((NY+(-1))))
  type(active) :: DUSRFDNS(1:INT((NX+(-1))),1:INT((NY+(-1))))
  type(active) :: STAGTHCK(1:INT((NX+(-1))),1:INT((NY+(-1))))
  REAL(w2f__8) OpenAD_tyc_10(:, :)
  ALLOCATABLE OpenAD_tyc_10
  REAL(w2f__8) OpenAD_tyc_11(:, :)
  ALLOCATABLE OpenAD_tyc_11
  REAL(w2f__8) OpenAD_tyc_12(:, :)
  ALLOCATABLE OpenAD_tyc_12
  REAL(w2f__8) OpenAD_tyc_77(:, :)
  ALLOCATABLE OpenAD_tyc_77
  REAL(w2f__8) OpenAD_tyc_79(:, :)
  ALLOCATABLE OpenAD_tyc_79
  REAL(w2f__8) OpenAD_tyc_81(:, :)
  ALLOCATABLE OpenAD_tyc_81


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  CALL STAGVARB(THCK,STAGTHCK,NX,NY)
  CALL DF_FIELD_2D_STAGGERED(USRF,DX,DY,DUSRFDEW,DUSRFDNS)
  CALL DF_FIELD_2D_STAGGERED(THCK,DX,DY,DTHCKDEW,DTHCKDNS)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_10,THCK)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_10,THCK)
  CALL GRIDWVEL(NX,NY,NZ,SIGMA,THKLIM,UVEL,VVEL,DUSRFDEW,DUSRFDNS,DTHCKDEW,DTHCK&
     &DNS,DUSRFDTM,DTHCKDTM,OpenAD_tyc_10,WGRD)

!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_11,THCK)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_11,THCK)
  CALL WVELINTG(NX,NY,NZ,DX,DY,SIGMA,THKLIM,STAGTHCK,DUSRFDEW,DUSRFDNS,DTHCKDEW,&
     &DTHCKDNS,UVEL,VVEL,WGRD(NZ,1:NX,1:NY),OpenAD_tyc_11,BMLT,WVEL)

  IF (WHICHWVEL.eq.1) THEN
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_12,THCK)
!!! requested inline of 'oad_convert' has no defn
    CALL oad_convert(OpenAD_tyc_12,THCK)
    CALL CHCKWVEL(NX,NY,NZ,SIGMA,THKLIM,DUSRFDEW,DUSRFDNS,DUSRFDTM,UVEL(1,1:NX+(&
     &-1),1:NY+(-1)),VVEL(1,1:NX+(-1),1:NY+(-1)),WVEL,OpenAD_tyc_12,ACAB)

  ENDIF
    end if
    if (our_rev_mode%tape) then
! taping
  CALL STAGVARB(THCK,STAGTHCK,NX,NY)
  CALL DF_FIELD_2D_STAGGERED(USRF,DX,DY,DUSRFDEW,DUSRFDNS)
  CALL DF_FIELD_2D_STAGGERED(THCK,DX,DY,DTHCKDEW,DTHCKDNS)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_10,THCK)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_10,THCK)
  CALL GRIDWVEL(NX,NY,NZ,SIGMA,THKLIM,UVEL,VVEL,DUSRFDEW,DUSRFDNS,DTHCKDEW,DTHCK&
     &DNS,DUSRFDTM,DTHCKDTM,OpenAD_tyc_10,WGRD)

  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = NZ
  oad_it_ptr = oad_it_ptr+1
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_11,THCK)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_11,THCK)
  CALL WVELINTG(NX,NY,NZ,DX,DY,SIGMA,THKLIM,STAGTHCK,DUSRFDEW,DUSRFDNS,DTHCKDEW,&
     &DTHCKDNS,UVEL,VVEL,WGRD(NZ,1:NX,1:NY),OpenAD_tyc_11,BMLT,WVEL)

  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = NZ
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = NX
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = NY
  oad_it_ptr = oad_it_ptr+1
  IF (WHICHWVEL.eq.1) THEN
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_12,THCK)
!!! requested inline of 'oad_convert' has no defn
    CALL oad_convert(OpenAD_tyc_12,THCK)
    CALL CHCKWVEL(NX,NY,NZ,SIGMA,THKLIM,DUSRFDEW,DUSRFDNS,DUSRFDTM,UVEL(1,1:NX+(&
     &-1),1:NY+(-1)),VVEL(1,1:NX+(-1),1:NY+(-1)),WVEL,OpenAD_tyc_12,ACAB)

    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = NX
    oad_it_ptr = oad_it_ptr+1
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = NY
    oad_it_ptr = oad_it_ptr+1
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = NZ
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_726 = 1_w2f__i8
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_726
    oad_it_ptr = oad_it_ptr+1
  ELSE
    OpenAD_Symbol_727 = 0_w2f__i8
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_727
    oad_it_ptr = oad_it_ptr+1
  ENDIF
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_725 = oad_it(oad_it_ptr)
  IF (OpenAD_Symbol_725.ne.0) THEN
    oad_it_ptr = oad_it_ptr-1
    NZ = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    NY = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    NX = oad_it(oad_it_ptr)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_81,THCK)
    CALL CHCKWVEL(NX,NY,NZ,SIGMA,THKLIM,DUSRFDEW,DUSRFDNS,DUSRFDTM,UVEL(1,1:NX+(&
     &-1),1:NY+(-1)),VVEL(1,1:NX+(-1),1:NY+(-1)),WVEL,OpenAD_tyc_81,ACAB)

  ENDIF
  oad_it_ptr = oad_it_ptr-1
  NY = oad_it(oad_it_ptr)
  oad_it_ptr = oad_it_ptr-1
  NX = oad_it(oad_it_ptr)
  oad_it_ptr = oad_it_ptr-1
  NZ = oad_it(oad_it_ptr)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_77,THCK)
  CALL WVELINTG(NX,NY,NZ,DX,DY,SIGMA,THKLIM,STAGTHCK,DUSRFDEW,DUSRFDNS,DTHCKDEW,&
     &DTHCKDNS,UVEL,VVEL,WGRD(NZ,1:NX,1:NY),OpenAD_tyc_77,BMLT,WVEL)

  oad_it_ptr = oad_it_ptr-1
  NZ = oad_it(oad_it_ptr)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_79,THCK)
  CALL GRIDWVEL(NX,NY,NZ,SIGMA,THKLIM,UVEL,VVEL,DUSRFDEW,DUSRFDNS,DTHCKDEW,DTHCK&
     &DNS,DUSRFDTM,DTHCKDTM,OpenAD_tyc_79,WGRD)

  CALL DF_FIELD_2D_STAGGERED(THCK,DX,DY,DTHCKDEW,DTHCKDNS)
  CALL DF_FIELD_2D_STAGGERED(USRF,DX,DY,DUSRFDEW,DUSRFDNS)
  CALL STAGVARB(THCK,STAGTHCK,NX,NY)
    end if
  end subroutine CALCVERTICALVELOCITY
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE GRIDWVEL(NX, NY, NZ, SIGMA, THKLIM, UVEL, VVEL, DUSRFDEW, DUSRFDNS,&
     & DTHCKDEW, DTHCKDNS, DUSRFDTM, DTHCKDTM, THCK, WGRD)

    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  use glimmer_utils
  use glimmer_utils
  use glimmer_utils
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_731
  INTEGER(w2f__i8) OpenAD_Symbol_732
  INTEGER(w2f__i8) OpenAD_Symbol_733
  INTEGER(w2f__i8) OpenAD_Symbol_734
  INTEGER(w2f__i8) OpenAD_Symbol_735
  INTEGER(w2f__i8) OpenAD_Symbol_736
  INTEGER(w2f__i8) OpenAD_Symbol_737
  INTEGER(w2f__i8) OpenAD_Symbol_738
  INTEGER(w2f__i8) OpenAD_Symbol_739
  INTEGER(w2f__i8) OpenAD_Symbol_740
  INTEGER(w2f__i8) OpenAD_Symbol_741
  INTEGER(w2f__i8) OpenAD_Symbol_742
  INTEGER(w2f__i8) OpenAD_Symbol_743
  INTEGER(w2f__i8) OpenAD_Symbol_744
  INTEGER(w2f__i8) OpenAD_Symbol_745
  INTEGER(w2f__i8) OpenAD_Symbol_746
  INTEGER(w2f__i8) OpenAD_Symbol_747
  INTEGER(w2f__i8) OpenAD_Symbol_748
  INTEGER(w2f__i8) OpenAD_Symbol_749
  INTEGER(w2f__i8) OpenAD_Symbol_750
  INTEGER(w2f__i8) OpenAD_Symbol_751
  INTEGER(w2f__i8) OpenAD_Symbol_752
  INTEGER(w2f__i8) OpenAD_Symbol_753
  INTEGER(w2f__i8) OpenAD_Symbol_754
  INTEGER(w2f__i8) OpenAD_Symbol_755
  INTEGER(w2f__i8) OpenAD_Symbol_756
  INTEGER(w2f__i8) OpenAD_Symbol_757
  INTEGER(w2f__i8) OpenAD_Symbol_758
  INTEGER(w2f__i8) OpenAD_Symbol_759
  INTEGER(w2f__i8) OpenAD_Symbol_760
!
!       **** Parameters and Result ****
!
  INTEGER(w2f__i4) NX
  INTEGER(w2f__i4) NY
  INTEGER(w2f__i4) NZ
  REAL(w2f__8) SIGMA(1 : NZ)
  REAL(w2f__8) THKLIM
  type(active) :: UVEL(1:,1:,1:)
  type(active) :: VVEL(1:,1:,1:)
  type(active) :: DUSRFDEW(1:,1:)
  type(active) :: DUSRFDNS(1:,1:)
  type(active) :: DTHCKDEW(1:,1:)
  type(active) :: DTHCKDNS(1:,1:)
  type(active) :: DUSRFDTM(1:,1:)
  type(active) :: DTHCKDTM(1:,1:)
  REAL(w2f__8) THCK(1 :, 1 :)
  type(active) :: WGRD(1:,1:,1:)
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) EW
  INTEGER(w2f__i4) NS
  type(active) :: ST1(1:NZ)
  type(active) :: ST2(1:NZ)
  type(active) :: SUM1
  type(active) :: SUM2
  type(active) :: SUM3
  type(active) :: SUM4
  INTEGER(w2f__i4) UP
  INTEGER(w2f__i4) OpenAD_Symbol_1649
  INTEGER(w2f__i4) OpenAD_Symbol_1650
  INTEGER(w2f__i4) OpenAD_Symbol_1651
  REAL(w2f__8) OpenAD_Symbol_1652
  REAL(w2f__8) OpenAD_Symbol_1653
  REAL(w2f__8) OpenAD_Symbol_1654
  REAL(w2f__8) OpenAD_Symbol_1655
  REAL(w2f__8) OpenAD_Symbol_1656
  REAL(w2f__8) OpenAD_Symbol_1657
  REAL(w2f__8) OpenAD_Symbol_1658
  INTEGER(w2f__i4) OpenAD_Symbol_1659
  INTEGER(w2f__i4) OpenAD_Symbol_1660
  INTEGER(w2f__i4) OpenAD_Symbol_1661
  REAL(w2f__8) OpenAD_acc_56
  REAL(w2f__8) OpenAD_acc_57
  REAL(w2f__8) OpenAD_acc_58
  REAL(w2f__8) OpenAD_acc_59
  REAL(w2f__8) OpenAD_acc_60
  REAL(w2f__8) OpenAD_acc_61
  REAL(w2f__8) OpenAD_acc_62
  REAL(w2f__8) OpenAD_aux_54
  REAL(w2f__8) OpenAD_aux_55
  REAL(w2f__8) OpenAD_aux_56
  REAL(w2f__8) OpenAD_lin_115
  REAL(w2f__8) OpenAD_lin_116
  REAL(w2f__8) OpenAD_lin_117
  REAL(w2f__8) OpenAD_lin_118
  REAL(w2f__8) OpenAD_lin_119
  REAL(w2f__8) OpenAD_lin_120
  REAL(w2f__8) OpenAD_lin_121
  type(active) :: OpenAD_prp_47
  type(active) :: OpenAD_prp_48


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  DO NS = 2, (NY +(-1)), 1
    DO EW = 2, (NX +(-1)), 1
      IF(THCK(EW, NS) .GT. THKLIM) THEN
        CALL HSUM4(UVEL(1:,EW+(-1):EW,NS+(-1):NS),ST1,2,2,NZ)
        CALL HSUM4(VVEL(1:,EW+(-1):EW,NS+(-1):NS),ST2,2,2,NZ)
        CALL MYSUM(DUSRFDEW(EW+(-1):EW,NS+(-1):NS),SUM1,2,2)
        CALL MYSUM(DTHCKDEW(EW+(-1):EW,NS+(-1):NS),SUM2,2,2)
        CALL MYSUM(DUSRFDNS(EW+(-1):EW,NS+(-1):NS),SUM3,2,2)
        CALL MYSUM(DTHCKDNS(EW+(-1):EW,NS+(-1):NS),SUM4,2,2)
        DO UP = 1,NZ,1
          WGRD(INT(UP),INT(EW),INT(NS))%v = ((ST1(UP)%v*(SUM1%v-SIGMA(UP)*SUM2%v&
     &)+ST2(UP)%v*(SUM3%v-SIGMA(UP)*SUM4%v))*6.25D-02+DUSRFDTM(EW,NS)%v-SIGMA(UP&
     &)*DTHCKDTM(EW,NS)%v)

        END DO
      ELSE
        DO UP = 1,NZ,1
          WGRD(INT(UP),INT(EW),INT(NS))%v = 0.0D00
        END DO
      ENDIF
    END DO
  END DO
    end if
    if (our_rev_mode%tape) then
! taping
  OpenAD_Symbol_740 = 0_w2f__i8
  DO NS = 2,(NY+(-1)),1
    OpenAD_Symbol_741 = 0_w2f__i8
    DO EW = 2,(NX+(-1)),1
      IF (THCK(EW,NS).GT.THKLIM) THEN
        CALL HSUM4(UVEL(1:,EW+(-1):EW,NS+(-1):NS),ST1,2,2,NZ)
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NZ
        oad_it_ptr = oad_it_ptr+1
        CALL HSUM4(VVEL(1:,EW+(-1):EW,NS+(-1):NS),ST2,2,2,NZ)
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NZ
        oad_it_ptr = oad_it_ptr+1
        CALL MYSUM(DUSRFDEW(EW+(-1):EW,NS+(-1):NS),SUM1,2,2)
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        CALL MYSUM(DTHCKDEW(EW+(-1):EW,NS+(-1):NS),SUM2,2,2)
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        CALL MYSUM(DUSRFDNS(EW+(-1):EW,NS+(-1):NS),SUM3,2,2)
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        CALL MYSUM(DTHCKDNS(EW+(-1):EW,NS+(-1):NS),SUM4,2,2)
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_742 = 0_w2f__i8
        DO UP = 1,NZ,1
          OpenAD_aux_55 = (SUM1%v-SIGMA(UP)*SUM2%v)
          OpenAD_aux_56 = (SUM3%v-SIGMA(UP)*SUM4%v)
          OpenAD_aux_54 = (ST1(UP)%v*OpenAD_aux_55+ST2(UP)%v*OpenAD_aux_56)
          OpenAD_lin_115 = OpenAD_aux_55
          OpenAD_lin_117 = SIGMA(UP)
          OpenAD_lin_116 = ST1(UP)%v
          OpenAD_lin_118 = OpenAD_aux_56
          OpenAD_lin_120 = SIGMA(UP)
          OpenAD_lin_119 = ST2(UP)%v
          OpenAD_lin_121 = SIGMA(UP)
          WGRD(INT(UP),INT(EW),INT(NS))%v = (OpenAD_aux_54*6.25D-02+DUSRFDTM(EW,&
     &NS)%v-SIGMA(UP)*DTHCKDTM(EW,NS)%v)

          OpenAD_acc_56 = (OpenAD_lin_118*6.25D-02)
          OpenAD_acc_57 = (OpenAD_lin_119*6.25D-02)
          OpenAD_acc_58 = (OpenAD_lin_115*6.25D-02)
          OpenAD_acc_59 = (OpenAD_lin_116*6.25D-02)
          OpenAD_acc_60 = (OpenAD_lin_117*INT((-1_w2f__i8)))
          OpenAD_acc_61 = (OpenAD_lin_120*INT((-1_w2f__i8)))
          OpenAD_acc_62 = (OpenAD_lin_121*INT((-1_w2f__i8)))
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_acc_60
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_acc_61
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_acc_56
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_acc_57
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_acc_58
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_acc_59
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_acc_62
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = EW
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = NS
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = UP
          oad_it_ptr = oad_it_ptr+1
          OpenAD_Symbol_742 = (INT(OpenAD_Symbol_742)+INT(1_w2f__i8))
        END DO
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_742
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_744 = 1_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_744
        oad_it_ptr = oad_it_ptr+1
      ELSE
        OpenAD_Symbol_743 = 0_w2f__i8
        DO UP = 1,NZ,1
          WGRD(INT(UP),INT(EW),INT(NS))%v = 0.0D00
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = UP
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = EW
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = NS
          oad_it_ptr = oad_it_ptr+1
          OpenAD_Symbol_743 = (INT(OpenAD_Symbol_743)+INT(1_w2f__i8))
        END DO
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_743
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_745 = 0_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_745
        oad_it_ptr = oad_it_ptr+1
      ENDIF
      OpenAD_Symbol_741 = (INT(OpenAD_Symbol_741)+INT(1_w2f__i8))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_741
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_740 = (INT(OpenAD_Symbol_740)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_740
  oad_it_ptr = oad_it_ptr+1
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_731 = oad_it(oad_it_ptr)
  OpenAD_Symbol_732 = 1
  do while (INT(OpenAD_Symbol_732).LE.INT(OpenAD_Symbol_731))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_733 = oad_it(oad_it_ptr)
    OpenAD_Symbol_734 = 1
    do while (INT(OpenAD_Symbol_734).LE.INT(OpenAD_Symbol_733))
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_735 = oad_it(oad_it_ptr)
      IF (OpenAD_Symbol_735.ne.0) THEN
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_738 = oad_it(oad_it_ptr)
        OpenAD_Symbol_739 = 1
        do while (INT(OpenAD_Symbol_739).LE.INT(OpenAD_Symbol_738))
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1649 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1650 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1651 = oad_it(oad_it_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1652 = oad_dt(oad_dt_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1653 = oad_dt(oad_dt_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1654 = oad_dt(oad_dt_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1655 = oad_dt(oad_dt_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1656 = oad_dt(oad_dt_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1657 = oad_dt(oad_dt_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1658 = oad_dt(oad_dt_ptr)
          DTHCKDTM(OpenAD_Symbol_1651,OpenAD_Symbol_1650)%d = DTHCKDTM(OpenAD_Sy&
     &mbol_1651,OpenAD_Symbol_1650)%d+WGRD(OpenAD_Symbol_1649,OpenAD_Symbol_1651&
     &,OpenAD_Symbol_1650)%d*(OpenAD_Symbol_1652)

          OpenAD_prp_47%d = OpenAD_prp_47%d+WGRD(OpenAD_Symbol_1649,OpenAD_Symbo&
     &l_1651,OpenAD_Symbol_1650)%d*(OpenAD_Symbol_1653)

          ST1(OpenAD_Symbol_1649)%d = ST1(OpenAD_Symbol_1649)%d+WGRD(OpenAD_Symb&
     &ol_1649,OpenAD_Symbol_1651,OpenAD_Symbol_1650)%d*(OpenAD_Symbol_1654)

          OpenAD_prp_48%d = OpenAD_prp_48%d+WGRD(OpenAD_Symbol_1649,OpenAD_Symbo&
     &l_1651,OpenAD_Symbol_1650)%d*(OpenAD_Symbol_1655)

          ST2(OpenAD_Symbol_1649)%d = ST2(OpenAD_Symbol_1649)%d+WGRD(OpenAD_Symb&
     &ol_1649,OpenAD_Symbol_1651,OpenAD_Symbol_1650)%d*(OpenAD_Symbol_1656)

          DUSRFDTM(OpenAD_Symbol_1651,OpenAD_Symbol_1650)%d = DUSRFDTM(OpenAD_Sy&
     &mbol_1651,OpenAD_Symbol_1650)%d+WGRD(OpenAD_Symbol_1649,OpenAD_Symbol_1651&
     &,OpenAD_Symbol_1650)%d

          WGRD(OpenAD_Symbol_1649,OpenAD_Symbol_1651,OpenAD_Symbol_1650)%d = 0.0&
     &d0

          SUM4%d = SUM4%d+OpenAD_prp_48%d*(OpenAD_Symbol_1657)
          SUM3%d = SUM3%d+OpenAD_prp_48%d
          OpenAD_prp_48%d = 0.0d0
          SUM2%d = SUM2%d+OpenAD_prp_47%d*(OpenAD_Symbol_1658)
          SUM1%d = SUM1%d+OpenAD_prp_47%d
          OpenAD_prp_47%d = 0.0d0
          OpenAD_Symbol_739 = INT(OpenAD_Symbol_739)+1
        END DO
        oad_it_ptr = oad_it_ptr-1
        NS = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        EW = oad_it(oad_it_ptr)
        CALL MYSUM(DTHCKDNS(EW+(-1):EW,NS+(-1):NS),SUM4,2,2)
        oad_it_ptr = oad_it_ptr-1
        NS = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        EW = oad_it(oad_it_ptr)
        CALL MYSUM(DUSRFDNS(EW+(-1):EW,NS+(-1):NS),SUM3,2,2)
        oad_it_ptr = oad_it_ptr-1
        NS = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        EW = oad_it(oad_it_ptr)
        CALL MYSUM(DTHCKDEW(EW+(-1):EW,NS+(-1):NS),SUM2,2,2)
        oad_it_ptr = oad_it_ptr-1
        NS = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        EW = oad_it(oad_it_ptr)
        CALL MYSUM(DUSRFDEW(EW+(-1):EW,NS+(-1):NS),SUM1,2,2)
        oad_it_ptr = oad_it_ptr-1
        NZ = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        NS = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        EW = oad_it(oad_it_ptr)
        CALL HSUM4(VVEL(1:,EW+(-1):EW,NS+(-1):NS),ST2,2,2,NZ)
        oad_it_ptr = oad_it_ptr-1
        NZ = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        NS = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        EW = oad_it(oad_it_ptr)
        CALL HSUM4(UVEL(1:,EW+(-1):EW,NS+(-1):NS),ST1,2,2,NZ)
      ELSE
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_736 = oad_it(oad_it_ptr)
        OpenAD_Symbol_737 = 1
        do while (INT(OpenAD_Symbol_737).LE.INT(OpenAD_Symbol_736))
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1659 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1660 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1661 = oad_it(oad_it_ptr)
          WGRD(OpenAD_Symbol_1661,OpenAD_Symbol_1660,OpenAD_Symbol_1659)%d = 0.0&
     &d0

          OpenAD_Symbol_737 = INT(OpenAD_Symbol_737)+1
        END DO
      ENDIF
      OpenAD_Symbol_734 = INT(OpenAD_Symbol_734)+1
    END DO
    OpenAD_Symbol_732 = INT(OpenAD_Symbol_732)+1
  END DO
    end if
  end subroutine GRIDWVEL
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE WVELINTG(NX, NY, NZ, DX, DY, SIGMA, THKLIM, STAGTHCK , DUSRFDEW, DU&
     &SRFDNS, DTHCKDEW, DTHCKDNS, UVEL, VVEL, WGRD, THCK , BMLT, WVEL)

    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  use glimmer_utils
  use glimmer_utils
  use glimmer_utils
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_761
  INTEGER(w2f__i8) OpenAD_Symbol_762
  INTEGER(w2f__i8) OpenAD_Symbol_763
  INTEGER(w2f__i8) OpenAD_Symbol_764
  INTEGER(w2f__i8) OpenAD_Symbol_765
  INTEGER(w2f__i8) OpenAD_Symbol_766
  INTEGER(w2f__i8) OpenAD_Symbol_767
  INTEGER(w2f__i8) OpenAD_Symbol_768
  INTEGER(w2f__i8) OpenAD_Symbol_769
  INTEGER(w2f__i8) OpenAD_Symbol_770
  INTEGER(w2f__i8) OpenAD_Symbol_771
  INTEGER(w2f__i8) OpenAD_Symbol_772
  INTEGER(w2f__i8) OpenAD_Symbol_773
  INTEGER(w2f__i8) OpenAD_Symbol_774
  INTEGER(w2f__i8) OpenAD_Symbol_775
  INTEGER(w2f__i8) OpenAD_Symbol_776
  INTEGER(w2f__i8) OpenAD_Symbol_777
  INTEGER(w2f__i8) OpenAD_Symbol_778
  INTEGER(w2f__i8) OpenAD_Symbol_779
  INTEGER(w2f__i8) OpenAD_Symbol_780
  INTEGER(w2f__i8) OpenAD_Symbol_781
  INTEGER(w2f__i8) OpenAD_Symbol_782
  INTEGER(w2f__i8) OpenAD_Symbol_783
  INTEGER(w2f__i8) OpenAD_Symbol_784
  INTEGER(w2f__i8) OpenAD_Symbol_785
  INTEGER(w2f__i8) OpenAD_Symbol_786
  INTEGER(w2f__i8) OpenAD_Symbol_787
  INTEGER(w2f__i8) OpenAD_Symbol_788
  INTEGER(w2f__i8) OpenAD_Symbol_789
  INTEGER(w2f__i8) OpenAD_Symbol_790
  INTEGER(w2f__i8) OpenAD_Symbol_791
  INTEGER(w2f__i8) OpenAD_Symbol_792
  INTEGER(w2f__i8) OpenAD_Symbol_793
  INTEGER(w2f__i8) OpenAD_Symbol_794
  INTEGER(w2f__i8) OpenAD_Symbol_795
  INTEGER(w2f__i8) OpenAD_Symbol_796
!
!       **** Parameters and Result ****
!
  INTEGER(w2f__i4) NX
  INTEGER(w2f__i4) NY
  INTEGER(w2f__i4) NZ
  REAL(w2f__8) DX
  REAL(w2f__8) DY
  REAL(w2f__8) SIGMA(1 : NZ)
  REAL(w2f__8) THKLIM
  type(active) :: STAGTHCK(1:,1:)
  type(active) :: DUSRFDEW(1:,1:)
  type(active) :: DUSRFDNS(1:,1:)
  type(active) :: DTHCKDEW(1:,1:)
  type(active) :: DTHCKDNS(1:,1:)
  type(active) :: UVEL(1:,1:,1:)
  type(active) :: VVEL(1:,1:,1:)
  type(active) :: WGRD(1:,1:)
  REAL(w2f__8) THCK(1 :, 1 :)
  type(active) :: BMLT(1:,1:)
  type(active) :: WVEL(1:,1:,1:)
!
!       **** Local Variables and Functions ****
!
  type(active) :: CONS(1:6)
  REAL(w2f__8) DEPTHW(1 : NZ)
  REAL(w2f__8) DEW16
  REAL(w2f__8) DNS16
  REAL(w2f__8) DUPSW(1 : NZ)
  INTEGER(w2f__i4) EW
  INTEGER(w2f__i4) NS
  type(active) :: SUM1
  type(active) :: SUM2
  type(active) :: SUM3
  type(active) :: SUM4
  type(active) :: SUVEL(1:NZ)
  type(active) :: SVVEL(1:NZ)
  INTEGER(w2f__i4) UP
  REAL(w2f__8) OpenAD_Symbol_1662
  REAL(w2f__8) OpenAD_Symbol_1663
  INTEGER(w2f__i4) OpenAD_Symbol_1664
  INTEGER(w2f__i4) OpenAD_Symbol_1665
  INTEGER(w2f__i4) OpenAD_Symbol_1666
  INTEGER(w2f__i4) OpenAD_Symbol_1667
  INTEGER(w2f__i4) OpenAD_Symbol_1668
  INTEGER(w2f__i4) OpenAD_Symbol_1669
  REAL(w2f__8) OpenAD_Symbol_1670
  REAL(w2f__8) OpenAD_Symbol_1671
  REAL(w2f__8) OpenAD_Symbol_1672
  REAL(w2f__8) OpenAD_Symbol_1673
  REAL(w2f__8) OpenAD_Symbol_1674
  REAL(w2f__8) OpenAD_Symbol_1675
  REAL(w2f__8) OpenAD_Symbol_1676
  REAL(w2f__8) OpenAD_Symbol_1677
  REAL(w2f__8) OpenAD_Symbol_1678
  REAL(w2f__8) OpenAD_Symbol_1679
  INTEGER(w2f__i4) OpenAD_Symbol_1680
  INTEGER(w2f__i4) OpenAD_Symbol_1681
  INTEGER(w2f__i4) OpenAD_Symbol_1682
  INTEGER(w2f__i4) OpenAD_Symbol_1683
  INTEGER(w2f__i4) OpenAD_Symbol_1684
  INTEGER(w2f__i4) OpenAD_Symbol_797
  INTEGER(w2f__i4) OpenAD_Symbol_798
  INTEGER(w2f__i4) OpenAD_Symbol_799
  REAL(w2f__8) OpenAD_acc_63
  REAL(w2f__8) OpenAD_acc_64
  REAL(w2f__8) OpenAD_acc_65
  REAL(w2f__8) OpenAD_acc_66
  REAL(w2f__8) OpenAD_acc_67
  REAL(w2f__8) OpenAD_acc_68
  REAL(w2f__8) OpenAD_acc_69
  REAL(w2f__8) OpenAD_acc_70
  REAL(w2f__8) OpenAD_acc_71
  REAL(w2f__8) OpenAD_acc_72
  REAL(w2f__8) OpenAD_aux_57
  REAL(w2f__8) OpenAD_aux_58
  REAL(w2f__8) OpenAD_aux_59
  REAL(w2f__8) OpenAD_aux_60
  REAL(w2f__8) OpenAD_aux_61
  REAL(w2f__8) OpenAD_aux_62
  REAL(w2f__8) OpenAD_aux_63
  REAL(w2f__8) OpenAD_aux_64
  REAL(w2f__8) OpenAD_lin_122
  REAL(w2f__8) OpenAD_lin_123
  REAL(w2f__8) OpenAD_lin_124
  REAL(w2f__8) OpenAD_lin_125
  REAL(w2f__8) OpenAD_lin_126
  REAL(w2f__8) OpenAD_lin_127
  REAL(w2f__8) OpenAD_lin_128
  REAL(w2f__8) OpenAD_lin_129
  REAL(w2f__8) OpenAD_lin_130
  REAL(w2f__8) OpenAD_lin_131
  REAL(w2f__8) OpenAD_lin_132
  REAL(w2f__8) OpenAD_lin_133
  REAL(w2f__8) OpenAD_lin_134
  REAL(w2f__8) OpenAD_lin_135
  type(active) :: OpenAD_prp_49
  type(active) :: OpenAD_prp_50
  type(active) :: OpenAD_prp_51
  type(active) :: OpenAD_prp_52
  type(active) :: OpenAD_prp_53
  type(active) :: OpenAD_prp_54
  type(active) :: OpenAD_prp_55
  type(active) :: OpenAD_prp_56
  type(active) :: OpenAD_prp_57
  type(active) :: OpenAD_prp_58
  type(active) :: OpenAD_prp_59
  type(active) :: OpenAD_prp_60
  type(active) :: OpenAD_prp_61


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  DO UP = 1, (NZ +(-1)), 1
    DUPSW(INT(UP)) = (SIGMA(UP + 1) - SIGMA(UP))
  END DO
  DUPSW(INT(NZ)) = 0.0D00
  DO UP = 1, (NZ +(-1)), 1
    DEPTHW(INT(UP)) = ((SIGMA(UP) + SIGMA(UP + 1)) * 5.0D-01)
  END DO
  DEPTHW(INT(NZ)) = 0.0D00
  DEW16 = (1.0D00 /(DX * 1.6D+01))
  DNS16 = (1.0D00 /(DY * 1.6D+01))
  DO NS = 2, (NY +(-1)), 1
    DO EW = 2, (NX +(-1)), 1
      IF(THCK(EW, NS) .GT. THKLIM) THEN
        WVEL(INT(NZ),INT(EW),INT(NS))%v = (WGRD(EW,NS)%v-BMLT(EW,NS)%v)
        CALL MYSUM(DUSRFDEW(EW+(-1):EW,NS+(-1):NS),CONS(1),2,2)
        CONS(1)%v = (CONS(1)%v*6.25D-02)
        CALL MYSUM(DTHCKDEW(EW+(-1):EW,NS+(-1):NS),CONS(2),2,2)
        CONS(2)%v = (CONS(2)%v*6.25D-02)
        CALL MYSUM(DUSRFDNS(EW+(-1):EW,NS+(-1):NS),CONS(3),2,2)
        CONS(3)%v = (CONS(3)%v*6.25D-02)
        CALL MYSUM(DTHCKDNS(EW+(-1):EW,NS+(-1):NS),CONS(4),2,2)
        CONS(4)%v = (CONS(4)%v*6.25D-02)
        CALL MYSUM(STAGTHCK(EW+(-1):EW,NS+(-1):NS),CONS(5),2,2)
        CONS(6)%v = (CONS(5)%v*DNS16)
        CONS(5)%v = (CONS(5)%v*DEW16)
        CALL HSUM4(UVEL(1:,EW+(-1):EW,NS+(-1):NS),SUVEL,2,2,NZ)
        CALL HSUM4(VVEL(1:,EW+(-1):EW,NS+(-1):NS),SVVEL,2,2,NZ)
        DO UP = NZ+(-1),1,(-1)
          CALL MYSUM3D(UVEL(UP:UP+1,EW,NS+(-1):NS),SUM1,1,2,2)
          CALL MYSUM3D(UVEL(UP:UP+1,EW+(-1),NS+(-1):NS),SUM2,1,2,2)
          CALL MYSUM3D(VVEL(UP:UP+1,EW+(-1):EW,NS),SUM3,2,1,2)
          CALL MYSUM3D(VVEL(UP:UP+1,EW+(-1):EW,NS+(-1)),SUM4,2,1,2)
          WVEL(INT(UP),INT(EW),INT(NS))%v = (WVEL(UP+1,EW,NS)%v-CONS(5)%v*DUPSW(&
     &UP)*(SUM1%v-SUM2%v)-CONS(6)%v*DUPSW(UP)*(SUM3%v-SUM4%v)-(CONS(1)%v-CONS(2)&
     &%v*DEPTHW(UP))*(SUVEL(UP+1)%v-SUVEL(UP)%v)-(CONS(3)%v-CONS(4)%v*DEPTHW(UP)&
     &)*(SVVEL(UP+1)%v-SVVEL(UP)%v))

        END DO
      ELSE
        WVEL(1:,INT(EW),INT(NS))%v = 0.0D00
      ENDIF
    END DO
  END DO
    end if
    if (our_rev_mode%tape) then
! taping
  OpenAD_Symbol_772 = 0_w2f__i8
  DO UP = 1,(NZ+(-1)),1
    DUPSW(INT(UP)) = (SIGMA(UP+1)-SIGMA(UP))
    OpenAD_Symbol_772 = (INT(OpenAD_Symbol_772)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_772
  oad_it_ptr = oad_it_ptr+1
  DUPSW(INT(NZ)) = 0.0D00
  OpenAD_Symbol_773 = 0_w2f__i8
  DO UP = 1,(NZ+(-1)),1
    DEPTHW(INT(UP)) = ((SIGMA(UP)+SIGMA(UP+1))*5.0D-01)
    OpenAD_Symbol_773 = (INT(OpenAD_Symbol_773)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_773
  oad_it_ptr = oad_it_ptr+1
  DEPTHW(INT(NZ)) = 0.0D00
  DEW16 = (1.0D00/(DX*1.6D+01))
  DNS16 = (1.0D00/(DY*1.6D+01))
  OpenAD_Symbol_774 = 0_w2f__i8
  DO NS = 2,(NY+(-1)),1
    OpenAD_Symbol_775 = 0_w2f__i8
    DO EW = 2,(NX+(-1)),1
      IF (THCK(EW,NS).GT.THKLIM) THEN
        WVEL(INT(NZ),INT(EW),INT(NS))%v = (WGRD(EW,NS)%v-BMLT(EW,NS)%v)
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NZ
        oad_it_ptr = oad_it_ptr+1
        CALL MYSUM(DUSRFDEW(EW+(-1):EW,NS+(-1):NS),CONS(1),2,2)
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        CONS(1)%v = (CONS(1)%v*6.25D-02)
        CALL MYSUM(DTHCKDEW(EW+(-1):EW,NS+(-1):NS),CONS(2),2,2)
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        CONS(2)%v = (CONS(2)%v*6.25D-02)
        CALL MYSUM(DUSRFDNS(EW+(-1):EW,NS+(-1):NS),CONS(3),2,2)
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        CONS(3)%v = (CONS(3)%v*6.25D-02)
        CALL MYSUM(DTHCKDNS(EW+(-1):EW,NS+(-1):NS),CONS(4),2,2)
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        CONS(4)%v = (CONS(4)%v*6.25D-02)
        CALL MYSUM(STAGTHCK(EW+(-1):EW,NS+(-1):NS),CONS(5),2,2)
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        OpenAD_lin_122 = DNS16
        CONS(6)%v = (CONS(5)%v*DNS16)
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_lin_122
        oad_dt_ptr = oad_dt_ptr+1
        OpenAD_lin_123 = DEW16
        CONS(5)%v = (CONS(5)%v*DEW16)
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_lin_123
        oad_dt_ptr = oad_dt_ptr+1
        CALL HSUM4(UVEL(1:,EW+(-1):EW,NS+(-1):NS),SUVEL,2,2,NZ)
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NZ
        oad_it_ptr = oad_it_ptr+1
        CALL HSUM4(VVEL(1:,EW+(-1):EW,NS+(-1):NS),SVVEL,2,2,NZ)
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NZ
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_776 = 0_w2f__i8
        DO UP = NZ+(-1),1,(-1)
          CALL MYSUM3D(UVEL(UP:UP+1,EW,NS+(-1):NS),SUM1,1,2,2)
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = UP
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = EW
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = NS
          oad_it_ptr = oad_it_ptr+1
          CALL MYSUM3D(UVEL(UP:UP+1,EW+(-1),NS+(-1):NS),SUM2,1,2,2)
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = UP
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = EW
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = NS
          oad_it_ptr = oad_it_ptr+1
          CALL MYSUM3D(VVEL(UP:UP+1,EW+(-1):EW,NS),SUM3,2,1,2)
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = UP
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = EW
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = NS
          oad_it_ptr = oad_it_ptr+1
          CALL MYSUM3D(VVEL(UP:UP+1,EW+(-1):EW,NS+(-1)),SUM4,2,1,2)
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = UP
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = EW
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = NS
          oad_it_ptr = oad_it_ptr+1
          OpenAD_aux_57 = (CONS(5)%v*DUPSW(UP))
          OpenAD_aux_58 = (SUM1%v-SUM2%v)
          OpenAD_aux_59 = (CONS(6)%v*DUPSW(UP))
          OpenAD_aux_60 = (SUM3%v-SUM4%v)
          OpenAD_aux_61 = (CONS(1)%v-CONS(2)%v*DEPTHW(UP))
          OpenAD_aux_62 = (SUVEL(UP+1)%v-SUVEL(UP)%v)
          OpenAD_aux_63 = (CONS(3)%v-CONS(4)%v*DEPTHW(UP))
          OpenAD_aux_64 = (SVVEL(UP+1)%v-SVVEL(UP)%v)
          OpenAD_lin_126 = DUPSW(UP)
          OpenAD_lin_124 = OpenAD_aux_58
          OpenAD_lin_125 = OpenAD_aux_57
          OpenAD_lin_129 = DUPSW(UP)
          OpenAD_lin_127 = OpenAD_aux_60
          OpenAD_lin_128 = OpenAD_aux_59
          OpenAD_lin_132 = DEPTHW(UP)
          OpenAD_lin_130 = OpenAD_aux_62
          OpenAD_lin_131 = OpenAD_aux_61
          OpenAD_lin_135 = DEPTHW(UP)
          OpenAD_lin_133 = OpenAD_aux_64
          OpenAD_lin_134 = OpenAD_aux_63
          WVEL(INT(UP),INT(EW),INT(NS))%v = (WVEL(UP+1,EW,NS)%v-OpenAD_aux_57*Op&
     &enAD_aux_58-OpenAD_aux_59*OpenAD_aux_60-OpenAD_aux_61*OpenAD_aux_62-OpenAD&
     &_aux_63*OpenAD_aux_64)

          OpenAD_acc_63 = (OpenAD_lin_132*INT((-1_w2f__i8)))
          OpenAD_acc_64 = (OpenAD_lin_135*INT((-1_w2f__i8)))
          OpenAD_acc_65 = (OpenAD_lin_125*INT((-1_w2f__i8)))
          OpenAD_acc_66 = (OpenAD_lin_126*OpenAD_lin_124*INT((-1_w2f__i8)))
          OpenAD_acc_67 = (OpenAD_lin_128*INT((-1_w2f__i8)))
          OpenAD_acc_68 = (OpenAD_lin_129*OpenAD_lin_127*INT((-1_w2f__i8)))
          OpenAD_acc_69 = (OpenAD_lin_130*INT((-1_w2f__i8)))
          OpenAD_acc_70 = (OpenAD_lin_131*INT((-1_w2f__i8)))
          OpenAD_acc_71 = (OpenAD_lin_133*INT((-1_w2f__i8)))
          OpenAD_acc_72 = (OpenAD_lin_134*INT((-1_w2f__i8)))
          OpenAD_Symbol_797 = (UP+1)
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_797
          oad_it_ptr = oad_it_ptr+1
          OpenAD_Symbol_798 = (UP+1)
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_798
          oad_it_ptr = oad_it_ptr+1
          OpenAD_Symbol_799 = (UP+1)
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_799
          oad_it_ptr = oad_it_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_acc_63
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_acc_64
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_acc_65
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_acc_66
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_acc_67
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_acc_68
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_acc_69
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_acc_70
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_acc_71
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_acc_72
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = EW
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = NS
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = UP
          oad_it_ptr = oad_it_ptr+1
          OpenAD_Symbol_776 = (INT(OpenAD_Symbol_776)+INT(1_w2f__i8))
        END DO
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_776
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_778 = 1_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_778
        oad_it_ptr = oad_it_ptr+1
      ELSE
        WVEL(1:,INT(EW),INT(NS))%v = 0.0D00
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_777 = 0_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_777
        oad_it_ptr = oad_it_ptr+1
      ENDIF
      OpenAD_Symbol_775 = (INT(OpenAD_Symbol_775)+INT(1_w2f__i8))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_775
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_774 = (INT(OpenAD_Symbol_774)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_774
  oad_it_ptr = oad_it_ptr+1
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_761 = oad_it(oad_it_ptr)
  OpenAD_Symbol_762 = 1
  do while (INT(OpenAD_Symbol_762).LE.INT(OpenAD_Symbol_761))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_763 = oad_it(oad_it_ptr)
    OpenAD_Symbol_764 = 1
    do while (INT(OpenAD_Symbol_764).LE.INT(OpenAD_Symbol_763))
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_765 = oad_it(oad_it_ptr)
      IF (OpenAD_Symbol_765.ne.0) THEN
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_766 = oad_it(oad_it_ptr)
        OpenAD_Symbol_767 = 1
        do while (INT(OpenAD_Symbol_767).LE.INT(OpenAD_Symbol_766))
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1667 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1668 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1669 = oad_it(oad_it_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1670 = oad_dt(oad_dt_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1671 = oad_dt(oad_dt_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1672 = oad_dt(oad_dt_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1673 = oad_dt(oad_dt_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1674 = oad_dt(oad_dt_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1675 = oad_dt(oad_dt_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1676 = oad_dt(oad_dt_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1677 = oad_dt(oad_dt_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1678 = oad_dt(oad_dt_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1679 = oad_dt(oad_dt_ptr)
          OpenAD_prp_59%d = OpenAD_prp_59%d+WVEL(OpenAD_Symbol_1667,OpenAD_Symbo&
     &l_1669,OpenAD_Symbol_1668)%d*(OpenAD_Symbol_1670)

          OpenAD_prp_61%d = OpenAD_prp_61%d+WVEL(OpenAD_Symbol_1667,OpenAD_Symbo&
     &l_1669,OpenAD_Symbol_1668)%d*(OpenAD_Symbol_1671)

          OpenAD_prp_58%d = OpenAD_prp_58%d+WVEL(OpenAD_Symbol_1667,OpenAD_Symbo&
     &l_1669,OpenAD_Symbol_1668)%d*(OpenAD_Symbol_1672)

          OpenAD_prp_60%d = OpenAD_prp_60%d+WVEL(OpenAD_Symbol_1667,OpenAD_Symbo&
     &l_1669,OpenAD_Symbol_1668)%d*(OpenAD_Symbol_1673)

          CONS(6)%d = CONS(6)%d+WVEL(OpenAD_Symbol_1667,OpenAD_Symbol_1669,OpenA&
     &D_Symbol_1668)%d*(OpenAD_Symbol_1674)

          OpenAD_prp_57%d = OpenAD_prp_57%d+WVEL(OpenAD_Symbol_1667,OpenAD_Symbo&
     &l_1669,OpenAD_Symbol_1668)%d*(OpenAD_Symbol_1675)

          CONS(5)%d = CONS(5)%d+WVEL(OpenAD_Symbol_1667,OpenAD_Symbol_1669,OpenA&
     &D_Symbol_1668)%d*(OpenAD_Symbol_1676)

          OpenAD_prp_56%d = OpenAD_prp_56%d+WVEL(OpenAD_Symbol_1667,OpenAD_Symbo&
     &l_1669,OpenAD_Symbol_1668)%d*(OpenAD_Symbol_1677)

          OpenAD_prp_55%d = OpenAD_prp_55%d+WVEL(OpenAD_Symbol_1667,OpenAD_Symbo&
     &l_1669,OpenAD_Symbol_1668)%d

          WVEL(OpenAD_Symbol_1667,OpenAD_Symbol_1669,OpenAD_Symbol_1668)%d = 0.0&
     &d0

          CONS(4)%d = CONS(4)%d+OpenAD_prp_61%d*(OpenAD_Symbol_1678)
          CONS(3)%d = CONS(3)%d+OpenAD_prp_61%d
          OpenAD_prp_61%d = 0.0d0
          CONS(2)%d = CONS(2)%d+OpenAD_prp_60%d*(OpenAD_Symbol_1679)
          CONS(1)%d = CONS(1)%d+OpenAD_prp_60%d
          OpenAD_prp_60%d = 0.0d0
          SVVEL(OpenAD_Symbol_1667)%d = SVVEL(OpenAD_Symbol_1667)%d-OpenAD_prp_5&
     &9%d

          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1680 = oad_it(oad_it_ptr)
          SVVEL(OpenAD_Symbol_1680)%d = SVVEL(OpenAD_Symbol_1680)%d+OpenAD_prp_5&
     &9%d

          OpenAD_prp_59%d = 0.0d0
          SUVEL(OpenAD_Symbol_1667)%d = SUVEL(OpenAD_Symbol_1667)%d-OpenAD_prp_5&
     &8%d

          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1681 = oad_it(oad_it_ptr)
          SUVEL(OpenAD_Symbol_1681)%d = SUVEL(OpenAD_Symbol_1681)%d+OpenAD_prp_5&
     &8%d

          OpenAD_prp_58%d = 0.0d0
          SUM4%d = SUM4%d-OpenAD_prp_57%d
          SUM3%d = SUM3%d+OpenAD_prp_57%d
          OpenAD_prp_57%d = 0.0d0
          SUM2%d = SUM2%d-OpenAD_prp_56%d
          SUM1%d = SUM1%d+OpenAD_prp_56%d
          OpenAD_prp_56%d = 0.0d0
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1682 = oad_it(oad_it_ptr)
          WVEL(OpenAD_Symbol_1682,OpenAD_Symbol_1669,OpenAD_Symbol_1668)%d = WVE&
     &L(OpenAD_Symbol_1682,OpenAD_Symbol_1669,OpenAD_Symbol_1668)%d+OpenAD_prp_5&
     &5%d

          OpenAD_prp_55%d = 0.0d0
          oad_it_ptr = oad_it_ptr-1
          NS = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          EW = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          UP = oad_it(oad_it_ptr)
          CALL MYSUM3D(VVEL(UP:UP+1,EW+(-1):EW,NS+(-1)),SUM4,2,1,2)
          oad_it_ptr = oad_it_ptr-1
          NS = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          EW = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          UP = oad_it(oad_it_ptr)
          CALL MYSUM3D(VVEL(UP:UP+1,EW+(-1):EW,NS),SUM3,2,1,2)
          oad_it_ptr = oad_it_ptr-1
          NS = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          EW = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          UP = oad_it(oad_it_ptr)
          CALL MYSUM3D(UVEL(UP:UP+1,EW+(-1),NS+(-1):NS),SUM2,1,2,2)
          oad_it_ptr = oad_it_ptr-1
          NS = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          EW = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          UP = oad_it(oad_it_ptr)
          CALL MYSUM3D(UVEL(UP:UP+1,EW,NS+(-1):NS),SUM1,1,2,2)
          OpenAD_Symbol_767 = INT(OpenAD_Symbol_767)+1
        END DO
        oad_it_ptr = oad_it_ptr-1
        NZ = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        NS = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        EW = oad_it(oad_it_ptr)
        CALL HSUM4(VVEL(1:,EW+(-1):EW,NS+(-1):NS),SVVEL,2,2,NZ)
        oad_it_ptr = oad_it_ptr-1
        NZ = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        NS = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        EW = oad_it(oad_it_ptr)
        CALL HSUM4(UVEL(1:,EW+(-1):EW,NS+(-1):NS),SUVEL,2,2,NZ)
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1662 = oad_dt(oad_dt_ptr)
        OpenAD_prp_54%d = OpenAD_prp_54%d+CONS(5)%d*(OpenAD_Symbol_1662)
        CONS(5)%d = 0.0d0
        CONS(5)%d = CONS(5)%d+OpenAD_prp_54%d
        OpenAD_prp_54%d = 0.0d0
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1663 = oad_dt(oad_dt_ptr)
        OpenAD_prp_53%d = OpenAD_prp_53%d+CONS(6)%d*(OpenAD_Symbol_1663)
        CONS(6)%d = 0.0d0
        CONS(5)%d = CONS(5)%d+OpenAD_prp_53%d
        OpenAD_prp_53%d = 0.0d0
        oad_it_ptr = oad_it_ptr-1
        NS = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        EW = oad_it(oad_it_ptr)
        CALL MYSUM(STAGTHCK(EW+(-1):EW,NS+(-1):NS),CONS(5),2,2)
        OpenAD_prp_52%d = OpenAD_prp_52%d+CONS(4)%d*(6.25D-02)
        CONS(4)%d = 0.0d0
        CONS(4)%d = CONS(4)%d+OpenAD_prp_52%d
        OpenAD_prp_52%d = 0.0d0
        oad_it_ptr = oad_it_ptr-1
        NS = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        EW = oad_it(oad_it_ptr)
        CALL MYSUM(DTHCKDNS(EW+(-1):EW,NS+(-1):NS),CONS(4),2,2)
        OpenAD_prp_51%d = OpenAD_prp_51%d+CONS(3)%d*(6.25D-02)
        CONS(3)%d = 0.0d0
        CONS(3)%d = CONS(3)%d+OpenAD_prp_51%d
        OpenAD_prp_51%d = 0.0d0
        oad_it_ptr = oad_it_ptr-1
        NS = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        EW = oad_it(oad_it_ptr)
        CALL MYSUM(DUSRFDNS(EW+(-1):EW,NS+(-1):NS),CONS(3),2,2)
        OpenAD_prp_50%d = OpenAD_prp_50%d+CONS(2)%d*(6.25D-02)
        CONS(2)%d = 0.0d0
        CONS(2)%d = CONS(2)%d+OpenAD_prp_50%d
        OpenAD_prp_50%d = 0.0d0
        oad_it_ptr = oad_it_ptr-1
        NS = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        EW = oad_it(oad_it_ptr)
        CALL MYSUM(DTHCKDEW(EW+(-1):EW,NS+(-1):NS),CONS(2),2,2)
        OpenAD_prp_49%d = OpenAD_prp_49%d+CONS(1)%d*(6.25D-02)
        CONS(1)%d = 0.0d0
        CONS(1)%d = CONS(1)%d+OpenAD_prp_49%d
        OpenAD_prp_49%d = 0.0d0
        oad_it_ptr = oad_it_ptr-1
        NS = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        EW = oad_it(oad_it_ptr)
        CALL MYSUM(DUSRFDEW(EW+(-1):EW,NS+(-1):NS),CONS(1),2,2)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1664 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1665 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1666 = oad_it(oad_it_ptr)
        BMLT(OpenAD_Symbol_1666,OpenAD_Symbol_1665)%d = BMLT(OpenAD_Symbol_1666,&
     &OpenAD_Symbol_1665)%d-WVEL(OpenAD_Symbol_1664,OpenAD_Symbol_1666,OpenAD_Sy&
     &mbol_1665)%d

        WGRD(OpenAD_Symbol_1666,OpenAD_Symbol_1665)%d = WGRD(OpenAD_Symbol_1666,&
     &OpenAD_Symbol_1665)%d+WVEL(OpenAD_Symbol_1664,OpenAD_Symbol_1666,OpenAD_Sy&
     &mbol_1665)%d

        WVEL(OpenAD_Symbol_1664,OpenAD_Symbol_1666,OpenAD_Symbol_1665)%d = 0.0d0
      ELSE
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1683 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1684 = oad_it(oad_it_ptr)
        WVEL(1:,OpenAD_Symbol_1684,OpenAD_Symbol_1683)%d = 0.0d0
      ENDIF
      OpenAD_Symbol_764 = INT(OpenAD_Symbol_764)+1
    END DO
    OpenAD_Symbol_762 = INT(OpenAD_Symbol_762)+1
  END DO
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_768 = oad_it(oad_it_ptr)
  OpenAD_Symbol_769 = 1
  do while (INT(OpenAD_Symbol_769).LE.INT(OpenAD_Symbol_768))
    OpenAD_Symbol_769 = INT(OpenAD_Symbol_769)+1
  END DO
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_770 = oad_it(oad_it_ptr)
  OpenAD_Symbol_771 = 1
  do while (INT(OpenAD_Symbol_771).LE.INT(OpenAD_Symbol_770))
    OpenAD_Symbol_771 = INT(OpenAD_Symbol_771)+1
  END DO
    end if
  end subroutine WVELINTG
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE WVEL_EW(WGRD, WVEL)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) WGRD(1 :, 1 :, 1 :)
  REAL(w2f__8) WVEL(1 :, 1 :, 1 :)
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) EWN


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  EWN = SIZE(WGRD, 2)
  WGRD(1 : , 1, 1 : ) = WGRD(1 : , EWN + (-1), 1 : )
  WGRD(1 : , INT(EWN), 1 : ) = WGRD(1 : , 2, 1 : )
  WVEL(1 : , 1, 1 : ) = WVEL(1 : , EWN + (-1), 1 : )
  WVEL(1 : , INT(EWN), 1 : ) = WVEL(1 : , 2, 1 : )
    end if
    if (our_rev_mode%tape) then
! taping
  EWN = SIZE(WGRD, 2)
  WGRD(1 : , 1, 1 : ) = WGRD(1 : , EWN + (-1), 1 : )
  WGRD(1 : , INT(EWN), 1 : ) = WGRD(1 : , 2, 1 : )
  WVEL(1 : , 1, 1 : ) = WVEL(1 : , EWN + (-1), 1 : )
  WVEL(1 : , INT(EWN), 1 : ) = WVEL(1 : , 2, 1 : )
    end if
    if (our_rev_mode%adjoint) then
! adjoint
    end if
  end subroutine WVEL_EW
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE CHCKWVEL(NX, NY, NZ, SIGMA, THKLIM, DUSRFDEW, DUSRFDNS, DUSRFDTM, U&
     &VEL, VVEL, WVEL, THCK, ACAB)

    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  use glimmer_utils
  use glimmer_utils
  use glimmer_utils
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_800
  INTEGER(w2f__i8) OpenAD_Symbol_801
  INTEGER(w2f__i8) OpenAD_Symbol_802
  INTEGER(w2f__i8) OpenAD_Symbol_803
  INTEGER(w2f__i8) OpenAD_Symbol_804
  INTEGER(w2f__i8) OpenAD_Symbol_805
  INTEGER(w2f__i8) OpenAD_Symbol_806
  INTEGER(w2f__i8) OpenAD_Symbol_807
  INTEGER(w2f__i8) OpenAD_Symbol_808
  INTEGER(w2f__i8) OpenAD_Symbol_809
  INTEGER(w2f__i8) OpenAD_Symbol_810
  INTEGER(w2f__i8) OpenAD_Symbol_811
  INTEGER(w2f__i8) OpenAD_Symbol_812
  INTEGER(w2f__i8) OpenAD_Symbol_813
  INTEGER(w2f__i8) OpenAD_Symbol_814
  INTEGER(w2f__i8) OpenAD_Symbol_815
  INTEGER(w2f__i8) OpenAD_Symbol_816
  INTEGER(w2f__i8) OpenAD_Symbol_817
  INTEGER(w2f__i8) OpenAD_Symbol_818
  INTEGER(w2f__i8) OpenAD_Symbol_819
  INTEGER(w2f__i8) OpenAD_Symbol_820
  INTEGER(w2f__i8) OpenAD_Symbol_821
  INTEGER(w2f__i8) OpenAD_Symbol_822
  INTEGER(w2f__i8) OpenAD_Symbol_823
!
!       **** Parameters and Result ****
!
  INTEGER(w2f__i4) NX
  INTEGER(w2f__i4) NY
  INTEGER(w2f__i4) NZ
  REAL(w2f__8) SIGMA(1 : NZ)
  REAL(w2f__8) THKLIM
  type(active) :: DUSRFDEW(1:INT((NX+(-1))),1:INT((NY+(-1))))
  type(active) :: DUSRFDNS(1:INT((NX+(-1))),1:INT((NY+(-1))))
  type(active) :: DUSRFDTM(1:NX,1:NY)
  type(active) :: UVEL(1:INT((NX+(-1))),1:INT((NY+(-1))))
  type(active) :: VVEL(1:INT((NX+(-1))),1:INT((NY+(-1))))
  type(active) :: WVEL(1:NZ,1:NX,1:NY)
  REAL(w2f__8) THCK(1 : NX, 1 : NY)
  type(active) :: ACAB(1:NX,1:NY)
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) EW
  INTEGER(w2f__i4) NS
  type(active) :: SUM1
  type(active) :: SUM2
  type(active) :: SUM3
  type(active) :: SUM4
  type(active) :: TEMPCOEF
  INTEGER(w2f__i4) UP
  type(active) :: WCHK
  INTEGER(w2f__i4) OpenAD_Symbol_1685
  INTEGER(w2f__i4) OpenAD_Symbol_1686
  REAL(w2f__8) OpenAD_Symbol_1687
  REAL(w2f__8) OpenAD_Symbol_1688
  REAL(w2f__8) OpenAD_Symbol_1689
  REAL(w2f__8) OpenAD_Symbol_1690
  INTEGER(w2f__i4) OpenAD_Symbol_1691
  INTEGER(w2f__i4) OpenAD_Symbol_1692
  INTEGER(w2f__i4) OpenAD_Symbol_1693
  REAL(w2f__8) OpenAD_Symbol_1694
  REAL(w2f__8) OpenAD_acc_73
  REAL(w2f__8) OpenAD_acc_74
  REAL(w2f__8) OpenAD_acc_75
  REAL(w2f__8) OpenAD_acc_76
  REAL(w2f__8) OpenAD_aux_65
  REAL(w2f__8) OpenAD_aux_66
  REAL(w2f__8) OpenAD_lin_136
  REAL(w2f__8) OpenAD_lin_137
  REAL(w2f__8) OpenAD_lin_138
  REAL(w2f__8) OpenAD_lin_139
  REAL(w2f__8) OpenAD_lin_140
  type(active) :: OpenAD_prp_62


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  DO NS = 2, (NY +(-1)), 1
    DO EW = 2, (NX +(-1)), 1
      IF ((THCK(EW,NS).GT.THKLIM).AND.(WVEL(1,EW,NS)%v.ne.0.0D00)) THEN
        CALL MYSUM(UVEL(EW+(-1):EW,NS+(-1):NS),SUM1,2,2)
        CALL MYSUM(DUSRFDEW(EW+(-1):EW,NS+(-1):NS),SUM2,2,2)
        CALL MYSUM(VVEL(EW+(-1):EW,NS+(-1):NS),SUM3,2,2)
        CALL MYSUM(DUSRFDNS(EW+(-1):EW,NS+(-1):NS),SUM4,2,2)
        WCHK%v = ((SUM1%v*SUM2%v+SUM3%v*SUM4%v)*6.25D-02+DUSRFDTM(EW,NS)%v-ACAB(&
     &EW,NS)%v)

        TEMPCOEF%v = (WCHK%v-WVEL(1,EW,NS)%v)
        DO UP = 1,NZ,1
          WVEL(INT(UP),INT(EW),INT(NS))%v = (WVEL(UP,EW,NS)%v+TEMPCOEF%v*(1.0D00&
     &-SIGMA(UP)))

        END DO
      ENDIF
    END DO
  END DO
    end if
    if (our_rev_mode%tape) then
! taping
  OpenAD_Symbol_807 = 0_w2f__i8
  DO NS = 2,(NY+(-1)),1
    OpenAD_Symbol_808 = 0_w2f__i8
    DO EW = 2,(NX+(-1)),1
      IF ((THCK(EW,NS).GT.THKLIM).AND.(WVEL(1,EW,NS)%v.ne.0.0D00)) THEN
        CALL MYSUM(UVEL(EW+(-1):EW,NS+(-1):NS),SUM1,2,2)
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        CALL MYSUM(DUSRFDEW(EW+(-1):EW,NS+(-1):NS),SUM2,2,2)
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        CALL MYSUM(VVEL(EW+(-1):EW,NS+(-1):NS),SUM3,2,2)
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        CALL MYSUM(DUSRFDNS(EW+(-1):EW,NS+(-1):NS),SUM4,2,2)
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        OpenAD_aux_65 = (SUM1%v*SUM2%v+SUM3%v*SUM4%v)
        OpenAD_lin_136 = SUM2%v
        OpenAD_lin_137 = SUM1%v
        OpenAD_lin_138 = SUM4%v
        OpenAD_lin_139 = SUM3%v
        WCHK%v = (OpenAD_aux_65*6.25D-02+DUSRFDTM(EW,NS)%v-ACAB(EW,NS)%v)
        TEMPCOEF%v = (WCHK%v-WVEL(1,EW,NS)%v)
        OpenAD_acc_73 = (OpenAD_lin_138*6.25D-02)
        OpenAD_acc_74 = (OpenAD_lin_139*6.25D-02)
        OpenAD_acc_75 = (OpenAD_lin_136*6.25D-02)
        OpenAD_acc_76 = (OpenAD_lin_137*6.25D-02)
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_acc_73
        oad_dt_ptr = oad_dt_ptr+1
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_acc_74
        oad_dt_ptr = oad_dt_ptr+1
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_acc_75
        oad_dt_ptr = oad_dt_ptr+1
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_acc_76
        oad_dt_ptr = oad_dt_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_809 = 0_w2f__i8
        DO UP = 1,NZ,1
          OpenAD_aux_66 = (1.0D00-SIGMA(UP))
          OpenAD_lin_140 = OpenAD_aux_66
          WVEL(INT(UP),INT(EW),INT(NS))%v = (WVEL(UP,EW,NS)%v+TEMPCOEF%v*OpenAD_&
     &aux_66)

          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_lin_140
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = UP
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = EW
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = NS
          oad_it_ptr = oad_it_ptr+1
          OpenAD_Symbol_809 = (INT(OpenAD_Symbol_809)+INT(1_w2f__i8))
        END DO
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_809
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_811 = 1_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_811
        oad_it_ptr = oad_it_ptr+1
      ELSE
        OpenAD_Symbol_810 = 0_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_810
        oad_it_ptr = oad_it_ptr+1
      ENDIF
      OpenAD_Symbol_808 = (INT(OpenAD_Symbol_808)+INT(1_w2f__i8))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_808
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_807 = (INT(OpenAD_Symbol_807)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_807
  oad_it_ptr = oad_it_ptr+1
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_800 = oad_it(oad_it_ptr)
  OpenAD_Symbol_801 = 1
  do while (INT(OpenAD_Symbol_801).LE.INT(OpenAD_Symbol_800))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_802 = oad_it(oad_it_ptr)
    OpenAD_Symbol_803 = 1
    do while (INT(OpenAD_Symbol_803).LE.INT(OpenAD_Symbol_802))
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_804 = oad_it(oad_it_ptr)
      IF (OpenAD_Symbol_804.ne.0) THEN
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_805 = oad_it(oad_it_ptr)
        OpenAD_Symbol_806 = 1
        do while (INT(OpenAD_Symbol_806).LE.INT(OpenAD_Symbol_805))
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1691 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1692 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1693 = oad_it(oad_it_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1694 = oad_dt(oad_dt_ptr)
          TEMPCOEF%d = TEMPCOEF%d+WVEL(OpenAD_Symbol_1693,OpenAD_Symbol_1692,Ope&
     &nAD_Symbol_1691)%d*(OpenAD_Symbol_1694)

          OpenAD_prp_62%d = OpenAD_prp_62%d+WVEL(OpenAD_Symbol_1693,OpenAD_Symbo&
     &l_1692,OpenAD_Symbol_1691)%d

          WVEL(OpenAD_Symbol_1693,OpenAD_Symbol_1692,OpenAD_Symbol_1691)%d = 0.0&
     &d0

          WVEL(OpenAD_Symbol_1693,OpenAD_Symbol_1692,OpenAD_Symbol_1691)%d = WVE&
     &L(OpenAD_Symbol_1693,OpenAD_Symbol_1692,OpenAD_Symbol_1691)%d+OpenAD_prp_6&
     &2%d

          OpenAD_prp_62%d = 0.0d0
          OpenAD_Symbol_806 = INT(OpenAD_Symbol_806)+1
        END DO
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1685 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1686 = oad_it(oad_it_ptr)
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1687 = oad_dt(oad_dt_ptr)
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1688 = oad_dt(oad_dt_ptr)
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1689 = oad_dt(oad_dt_ptr)
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1690 = oad_dt(oad_dt_ptr)
        SUM2%d = SUM2%d+TEMPCOEF%d*(OpenAD_Symbol_1687)
        SUM1%d = SUM1%d+TEMPCOEF%d*(OpenAD_Symbol_1688)
        SUM4%d = SUM4%d+TEMPCOEF%d*(OpenAD_Symbol_1689)
        SUM3%d = SUM3%d+TEMPCOEF%d*(OpenAD_Symbol_1690)
        ACAB(OpenAD_Symbol_1686,OpenAD_Symbol_1685)%d = ACAB(OpenAD_Symbol_1686,&
     &OpenAD_Symbol_1685)%d-TEMPCOEF%d

        WVEL(1,OpenAD_Symbol_1686,OpenAD_Symbol_1685)%d = WVEL(1,OpenAD_Symbol_1&
     &686,OpenAD_Symbol_1685)%d-TEMPCOEF%d

        DUSRFDTM(OpenAD_Symbol_1686,OpenAD_Symbol_1685)%d = DUSRFDTM(OpenAD_Symb&
     &ol_1686,OpenAD_Symbol_1685)%d+TEMPCOEF%d

        TEMPCOEF%d = 0.0d0
        oad_it_ptr = oad_it_ptr-1
        NS = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        EW = oad_it(oad_it_ptr)
        CALL MYSUM(DUSRFDNS(EW+(-1):EW,NS+(-1):NS),SUM4,2,2)
        oad_it_ptr = oad_it_ptr-1
        NS = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        EW = oad_it(oad_it_ptr)
        CALL MYSUM(VVEL(EW+(-1):EW,NS+(-1):NS),SUM3,2,2)
        oad_it_ptr = oad_it_ptr-1
        NS = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        EW = oad_it(oad_it_ptr)
        CALL MYSUM(DUSRFDEW(EW+(-1):EW,NS+(-1):NS),SUM2,2,2)
        oad_it_ptr = oad_it_ptr-1
        NS = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        EW = oad_it(oad_it_ptr)
        CALL MYSUM(UVEL(EW+(-1):EW,NS+(-1):NS),SUM1,2,2)
      ENDIF
      OpenAD_Symbol_803 = INT(OpenAD_Symbol_803)+1
    END DO
    OpenAD_Symbol_801 = INT(OpenAD_Symbol_801)+1
  END DO
    end if
  end subroutine CHCKWVEL
END

!$OPENAD XXX File_start [glimmer_pdd.F90]
MODULE glimmer_pdd
use OAD_active
use w2f__types
use oad_intrinsics
use glimmer_global ,only: DP
IMPLICIT NONE
SAVE
!
!     **** Global Variables & Derived Type Definitions ****
!
INTEGER(w2f__i4) PDD_DX
INTEGER(w2f__i4) PDD_DY
INTEGER(w2f__i4) PDD_IX
INTEGER(w2f__i4) PDD_IY
REAL(w2f__8) PDD_PDDFAC_ICE
REAL(w2f__8) PDD_PDDFAC_SNOW
REAL(w2f__8) PDD_WMAX
REAL(w2f__8) BD_DD_SIGMA
REAL(w2f__8) BD_MEAN_ANNUAL_TEMP
REAL(w2f__8) BD_MEAN_JULY_TEMP
REAL(w2f__8) PDD_DD_SIGMA
REAL(w2f__8) BD_T_A_PRIME
REAL(w2f__8) PDD_DAILYTEMP
REAL(w2f__8) PDD_DTMJ
REAL(w2f__8) PDD_TMA
REAL(w2f__8) PDD_TMJ
!
!     **** Local Variables and Functions ****
!
INTEGER(w2f__i4) PDD_NX
PARAMETER ( PDD_NX = 31)
INTEGER(w2f__i4) PDD_NY
PARAMETER ( PDD_NY = 71)
!
!     **** Initializers ****
!
DATA PDD_DX / 1 /
DATA PDD_DY / 1 /
DATA PDD_IX / 0 /
DATA PDD_IY / -50 /
DATA PDD_PDDFAC_ICE / 1.40000000000000002914D-02 /
DATA PDD_PDDFAC_SNOW / 3.00000000000000006245D-03 /
DATA PDD_WMAX / 5.99999999999999977796D-01 /
DATA PDD_DD_SIGMA / 5.0D00 /
DATA PDD_DAILYTEMP / 0.0D00 /
DATA PDD_DTMJ / 0.0D00 /
DATA PDD_TMA / 0.0D00 /
DATA PDD_TMJ / 0.0D00 /
!
!     **** Statements ****
!
CONTAINS
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE GLIMMER_PDD_INIT(PDDTAB)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) PDDTAB(1 : 31, 1 : 71)


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  CALL PDDTABGRN(PDDTAB)
    end if
    if (our_rev_mode%tape) then
! taping
  CALL PDDTABGRN(PDDTAB)
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  CALL PDDTABGRN(PDDTAB)
    end if
  end subroutine GLIMMER_PDD_INIT
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE GLIMMER_PDD_MBAL(PDDTAB, ARTM, ARNG, PRCP, ABLT, ACAB, LANDSEA)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_836
  INTEGER(w2f__i8) OpenAD_Symbol_837
  INTEGER(w2f__i8) OpenAD_Symbol_838
  INTEGER(w2f__i8) OpenAD_Symbol_839
  INTEGER(w2f__i8) OpenAD_Symbol_840
  INTEGER(w2f__i8) OpenAD_Symbol_841
  INTEGER(w2f__i8) OpenAD_Symbol_842
  INTEGER(w2f__i8) OpenAD_Symbol_843
  INTEGER(w2f__i8) OpenAD_Symbol_844
  INTEGER(w2f__i8) OpenAD_Symbol_845
  INTEGER(w2f__i8) OpenAD_Symbol_846
  INTEGER(w2f__i8) OpenAD_Symbol_847
  INTEGER(w2f__i8) OpenAD_Symbol_848
  INTEGER(w2f__i8) OpenAD_Symbol_849
  INTEGER(w2f__i8) OpenAD_Symbol_850
  INTEGER(w2f__i8) OpenAD_Symbol_851
  INTEGER(w2f__i8) OpenAD_Symbol_852
  INTEGER(w2f__i8) OpenAD_Symbol_853
  INTEGER(w2f__i8) OpenAD_Symbol_854
  INTEGER(w2f__i8) OpenAD_Symbol_855
  INTEGER(w2f__i8) OpenAD_Symbol_856
  INTEGER(w2f__i8) OpenAD_Symbol_857
  INTEGER(w2f__i8) OpenAD_Symbol_858
  INTEGER(w2f__i8) OpenAD_Symbol_859
  INTEGER(w2f__i8) OpenAD_Symbol_860
  INTEGER(w2f__i8) OpenAD_Symbol_861
  INTEGER(w2f__i8) OpenAD_Symbol_862
  INTEGER(w2f__i8) OpenAD_Symbol_863
  INTEGER(w2f__i8) OpenAD_Symbol_864
  INTEGER(w2f__i8) OpenAD_Symbol_865
  INTEGER(w2f__i8) OpenAD_Symbol_866
  INTEGER(w2f__i8) OpenAD_Symbol_867
  INTEGER(w2f__i8) OpenAD_Symbol_868
  INTEGER(w2f__i8) OpenAD_Symbol_869
  INTEGER(w2f__i8) OpenAD_Symbol_870
  INTEGER(w2f__i8) OpenAD_Symbol_871
  INTEGER(w2f__i8) OpenAD_Symbol_872
  INTEGER(w2f__i8) OpenAD_Symbol_873
  INTEGER(w2f__i8) OpenAD_Symbol_874
  INTEGER(w2f__i8) OpenAD_Symbol_875
  INTEGER(w2f__i8) OpenAD_Symbol_876
  INTEGER(w2f__i8) OpenAD_Symbol_877
  INTEGER(w2f__i8) OpenAD_Symbol_878
  INTEGER(w2f__i8) OpenAD_Symbol_879
  INTEGER(w2f__i8) OpenAD_Symbol_880
  INTEGER(w2f__i8) OpenAD_Symbol_881
  INTEGER(w2f__i8) OpenAD_Symbol_882
  INTEGER(w2f__i8) OpenAD_Symbol_883
  INTEGER(w2f__i8) OpenAD_Symbol_884
  INTEGER(w2f__i8) OpenAD_Symbol_885
  INTEGER(w2f__i8) OpenAD_Symbol_886
  INTEGER(w2f__i8) OpenAD_Symbol_887
  INTEGER(w2f__i8) OpenAD_Symbol_888
  INTEGER(w2f__i8) OpenAD_Symbol_889
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) PDDTAB(1 : 31, 1 : 71)
  type(active) :: ARTM(1:,1:)
  type(active) :: ARNG(1:,1:)
  type(active) :: PRCP(1:,1:)
  type(active) :: ABLT(1:,1:)
  type(active) :: ACAB(1:,1:)
  LOGICAL(w2f__i4) LANDSEA(1 :, 1 :)
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) EW
  INTEGER(w2f__i4) EWN
  INTEGER(w2f__i4) JX
  INTEGER(w2f__i4) JY
  INTEGER(w2f__i4) KX
  INTEGER(w2f__i4) KY
  INTEGER(w2f__i4) NS
  INTEGER(w2f__i4) NSN
  type(active) :: PABLT
  type(active) :: PDD
  type(active) :: TX
  type(active) :: TY
  type(active) :: WFRAC
  INTEGER(w2f__i4) OpenAD_Symbol_1695
  INTEGER(w2f__i4) OpenAD_Symbol_1696
  REAL(w2f__8) OpenAD_Symbol_1697
  REAL(w2f__8) OpenAD_Symbol_1698
  REAL(w2f__8) OpenAD_Symbol_1699
  REAL(w2f__8) OpenAD_Symbol_1700
  REAL(w2f__8) OpenAD_Symbol_1701
  REAL(w2f__8) OpenAD_Symbol_1702
  REAL(w2f__8) OpenAD_Symbol_1703
  REAL(w2f__8) OpenAD_Symbol_1704
  REAL(w2f__8) OpenAD_Symbol_1705
  REAL(w2f__8) OpenAD_Symbol_1706
  INTEGER(w2f__i4) OpenAD_Symbol_1707
  INTEGER(w2f__i4) OpenAD_Symbol_1708
  INTEGER(w2f__i4) OpenAD_Symbol_1709
  INTEGER(w2f__i4) OpenAD_Symbol_1710
  INTEGER(w2f__i4) OpenAD_Symbol_1711
  INTEGER(w2f__i4) OpenAD_Symbol_1712
  INTEGER(w2f__i4) OpenAD_Symbol_1713
  INTEGER(w2f__i4) OpenAD_Symbol_1714
  REAL(w2f__8) OpenAD_Symbol_1715
  REAL(w2f__8) OpenAD_Symbol_1716
  INTEGER(w2f__i4) OpenAD_Symbol_1717
  INTEGER(w2f__i4) OpenAD_Symbol_1718
  INTEGER(w2f__i4) OpenAD_Symbol_1719
  INTEGER(w2f__i4) OpenAD_Symbol_1720
  INTEGER(w2f__i4) OpenAD_Symbol_1721
  INTEGER(w2f__i4) OpenAD_Symbol_1722
  REAL(w2f__8) OpenAD_acc_77
  REAL(w2f__8) OpenAD_acc_78
  REAL(w2f__8) OpenAD_acc_79
  REAL(w2f__8) OpenAD_acc_80
  REAL(w2f__8) OpenAD_acc_81
  REAL(w2f__8) OpenAD_acc_82
  REAL(w2f__8) OpenAD_acc_83
  REAL(w2f__8) OpenAD_aux_67
  REAL(w2f__8) OpenAD_aux_68
  REAL(w2f__8) OpenAD_aux_69
  REAL(w2f__8) OpenAD_aux_70
  REAL(w2f__8) OpenAD_aux_71
  REAL(w2f__8) OpenAD_aux_72
  REAL(w2f__8) OpenAD_aux_73
  REAL(w2f__8) OpenAD_aux_74
  REAL(w2f__8) OpenAD_aux_75
  REAL(w2f__8) OpenAD_lin_141
  REAL(w2f__8) OpenAD_lin_142
  REAL(w2f__8) OpenAD_lin_143
  REAL(w2f__8) OpenAD_lin_144
  REAL(w2f__8) OpenAD_lin_145
  REAL(w2f__8) OpenAD_lin_146
  REAL(w2f__8) OpenAD_lin_147
  REAL(w2f__8) OpenAD_lin_148
  REAL(w2f__8) OpenAD_lin_149
  REAL(w2f__8) OpenAD_lin_150
  REAL(w2f__8) OpenAD_lin_151
  REAL(w2f__8) OpenAD_lin_152
  REAL(w2f__8) OpenAD_lin_153
  REAL(w2f__8) OpenAD_lin_154
  REAL(w2f__8) OpenAD_lin_155
  REAL(w2f__8) OpenAD_lin_156
  type(active) :: OpenAD_prp_63


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  EWN = SIZE(ARTM, 1)
  NSN = SIZE(ARTM, 2)
  DO NS = 1, NSN, 1
    DO EW = 1, EWN, 1
      IF(LANDSEA(EW, NS)) THEN
        KY = INT((ARTM(EW,NS)%v-REAL(PDD_IY,8))/REAL(PDD_DY,8))
        KX = INT((ARNG(EW,NS)%v-REAL(PDD_IX,8))/REAL(PDD_DX,8))
        IF (KX.LT.0) THEN
          TX%v = 0.0D00
          JX = 2
          KX = 1
        ELSE
          IF (KX.GT.29) THEN
            TX%v = 1.0D00
            JX = 31
            KX = 30
          ELSE
            TX%v = (ARNG(EW,NS)%v-REAL(KX*PDD_DX,8)-REAL(PDD_IX,8))
            JX = (KX+2)
            KX = (KX+1)
          ENDIF
        ENDIF
        IF (KY.LT.0) THEN
          TY%v = 0.0D00
          JY = 2
          KY = 1
        ELSE
          IF (KY.GT.69) THEN
            TY%v = 1.0D00
            JY = 71
            KY = 70
          ELSE
            TY%v = (ARTM(EW,NS)%v-REAL(KY*PDD_DY,8)-REAL(PDD_IY,8))
            JY = (KY+2)
            KY = (KY+1)
          ENDIF
        ENDIF
        PDD%v = (PDDTAB(JX,KY)*TX%v*(1.0D00-TY%v)+PDDTAB(KX,KY)*(1.0D00-TX%v)*(1&
     &.0D00-TY%v)+TY%v*PDDTAB(JX,JY)*TX%v+TY%v*PDDTAB(KX,JY)*(1.0D00-TX%v))

        WFRAC%v = (PRCP(EW,NS)%v*PDD_WMAX)
        PABLT%v = (PDD%v*PDD_PDDFAC_SNOW)
        IF (PABLT%v.LE.WFRAC%v) THEN
          ABLT(INT(EW),INT(NS))%v = 0.0D00
        ELSE
          IF ((PRCP(EW,NS)%v.GE.PABLT%v).AND.(PABLT%v.GT.WFRAC%v)) THEN
            ABLT(INT(EW),INT(NS))%v = (PABLT%v-WFRAC%v)
          ELSE
            ABLT(INT(EW),INT(NS))%v = (PDD_PDDFAC_ICE*(PDD%v-(PRCP(EW,NS)%v/PDD_&
     &PDDFAC_SNOW))+PRCP(EW,NS)%v-WFRAC%v)

          ENDIF
        ENDIF
        ACAB(INT(EW),INT(NS))%v = (PRCP(EW,NS)%v-ABLT(EW,NS)%v)
      ELSE
        ABLT(INT(EW),INT(NS))%v = PRCP(EW,NS)%v
        ACAB(INT(EW),INT(NS))%v = 0.0D00
      ENDIF
    END DO
  END DO
    end if
    if (our_rev_mode%tape) then
! taping
  EWN = SIZE(ARTM,1)
  NSN = SIZE(ARTM,2)
  OpenAD_Symbol_847 = 0_w2f__i8
  DO NS = 1,NSN,1
    OpenAD_Symbol_848 = 0_w2f__i8
    DO EW = 1,EWN,1
      IF (LANDSEA(EW,NS)) THEN
        KY = INT((ARTM(EW,NS)%v-REAL(PDD_IY,8))/REAL(PDD_DY,8))
        KX = INT((ARNG(EW,NS)%v-REAL(PDD_IX,8))/REAL(PDD_DX,8))
        IF (KX.LT.0) THEN
          TX%v = 0.0D00
          JX = 2
          KX = 1
          OpenAD_Symbol_851 = 1_w2f__i8
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_851
          oad_it_ptr = oad_it_ptr+1
        ELSE
          IF (KX.GT.29) THEN
            TX%v = 1.0D00
            JX = 31
            KX = 30
            OpenAD_Symbol_849 = 1_w2f__i8
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = OpenAD_Symbol_849
            oad_it_ptr = oad_it_ptr+1
          ELSE
            TX%v = (ARNG(EW,NS)%v-REAL(KX*PDD_DX,8)-REAL(PDD_IX,8))
            JX = (KX+2)
            KX = (KX+1)
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = EW
            oad_it_ptr = oad_it_ptr+1
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = NS
            oad_it_ptr = oad_it_ptr+1
            OpenAD_Symbol_850 = 0_w2f__i8
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = OpenAD_Symbol_850
            oad_it_ptr = oad_it_ptr+1
          ENDIF
          OpenAD_Symbol_852 = 0_w2f__i8
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_852
          oad_it_ptr = oad_it_ptr+1
        ENDIF
        IF (KY.LT.0) THEN
          TY%v = 0.0D00
          JY = 2
          KY = 1
          OpenAD_Symbol_855 = 1_w2f__i8
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_855
          oad_it_ptr = oad_it_ptr+1
        ELSE
          IF (KY.GT.69) THEN
            TY%v = 1.0D00
            JY = 71
            KY = 70
            OpenAD_Symbol_853 = 1_w2f__i8
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = OpenAD_Symbol_853
            oad_it_ptr = oad_it_ptr+1
          ELSE
            TY%v = (ARTM(EW,NS)%v-REAL(KY*PDD_DY,8)-REAL(PDD_IY,8))
            JY = (KY+2)
            KY = (KY+1)
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = EW
            oad_it_ptr = oad_it_ptr+1
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = NS
            oad_it_ptr = oad_it_ptr+1
            OpenAD_Symbol_854 = 0_w2f__i8
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = OpenAD_Symbol_854
            oad_it_ptr = oad_it_ptr+1
          ENDIF
          OpenAD_Symbol_856 = 0_w2f__i8
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_856
          oad_it_ptr = oad_it_ptr+1
        ENDIF
        OpenAD_aux_67 = (PDDTAB(JX,KY)*TX%v)
        OpenAD_aux_68 = (1.0D00-TY%v)
        OpenAD_aux_71 = (1.0D00-TX%v)
        OpenAD_aux_69 = (PDDTAB(KX,KY)*OpenAD_aux_71)
        OpenAD_aux_70 = (1.0D00-TY%v)
        OpenAD_aux_72 = (PDDTAB(JX,JY)*TX%v)
        OpenAD_aux_74 = (1.0D00-TX%v)
        OpenAD_aux_73 = (PDDTAB(KX,JY)*OpenAD_aux_74)
        OpenAD_lin_143 = PDDTAB(JX,KY)
        OpenAD_lin_141 = OpenAD_aux_68
        OpenAD_lin_142 = OpenAD_aux_67
        OpenAD_lin_146 = PDDTAB(KX,KY)
        OpenAD_lin_144 = OpenAD_aux_70
        OpenAD_lin_145 = OpenAD_aux_69
        OpenAD_lin_147 = OpenAD_aux_72
        OpenAD_lin_149 = PDDTAB(JX,JY)
        OpenAD_lin_148 = TY%v
        OpenAD_lin_150 = OpenAD_aux_73
        OpenAD_lin_152 = PDDTAB(KX,JY)
        OpenAD_lin_151 = TY%v
        PDD%v = (OpenAD_aux_67*OpenAD_aux_68+OpenAD_aux_69*OpenAD_aux_70+TY%v*Op&
     &enAD_aux_72+TY%v*OpenAD_aux_73)

        OpenAD_lin_153 = PDD_WMAX
        WFRAC%v = (PRCP(EW,NS)%v*PDD_WMAX)
        OpenAD_lin_154 = PDD_PDDFAC_SNOW
        PABLT%v = (PDD%v*PDD_PDDFAC_SNOW)
        OpenAD_acc_77 = (OpenAD_lin_143*OpenAD_lin_141)
        OpenAD_acc_78 = (INT((-1_w2f__i8))*OpenAD_lin_142)
        OpenAD_acc_79 = (INT((-1_w2f__i8))*OpenAD_lin_146*OpenAD_lin_144)
        OpenAD_acc_80 = (INT((-1_w2f__i8))*OpenAD_lin_145)
        OpenAD_acc_81 = (OpenAD_lin_149*OpenAD_lin_148)
        OpenAD_acc_82 = (INT((-1_w2f__i8))*OpenAD_lin_152*OpenAD_lin_151)
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_lin_153
        oad_dt_ptr = oad_dt_ptr+1
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_acc_77
        oad_dt_ptr = oad_dt_ptr+1
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_acc_78
        oad_dt_ptr = oad_dt_ptr+1
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_acc_79
        oad_dt_ptr = oad_dt_ptr+1
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_acc_80
        oad_dt_ptr = oad_dt_ptr+1
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_lin_147
        oad_dt_ptr = oad_dt_ptr+1
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_acc_81
        oad_dt_ptr = oad_dt_ptr+1
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_lin_150
        oad_dt_ptr = oad_dt_ptr+1
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_acc_82
        oad_dt_ptr = oad_dt_ptr+1
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_lin_154
        oad_dt_ptr = oad_dt_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        IF (PABLT%v.LE.WFRAC%v) THEN
          ABLT(INT(EW),INT(NS))%v = 0.0D00
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = EW
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = NS
          oad_it_ptr = oad_it_ptr+1
          OpenAD_Symbol_859 = 1_w2f__i8
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_859
          oad_it_ptr = oad_it_ptr+1
        ELSE
          IF ((PRCP(EW,NS)%v.GE.PABLT%v).AND.(PABLT%v.GT.WFRAC%v)) THEN
            ABLT(INT(EW),INT(NS))%v = (PABLT%v-WFRAC%v)
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = EW
            oad_it_ptr = oad_it_ptr+1
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = NS
            oad_it_ptr = oad_it_ptr+1
            OpenAD_Symbol_857 = 1_w2f__i8
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = OpenAD_Symbol_857
            oad_it_ptr = oad_it_ptr+1
          ELSE
            OpenAD_aux_75 = (PDD%v-(PRCP(EW,NS)%v/PDD_PDDFAC_SNOW))
            OpenAD_lin_156 = (INT(1_w2f__i8)/PDD_PDDFAC_SNOW)
            OpenAD_lin_155 = PDD_PDDFAC_ICE
            ABLT(INT(EW),INT(NS))%v = (PDD_PDDFAC_ICE*OpenAD_aux_75+PRCP(EW,NS)%&
     &v-WFRAC%v)

            OpenAD_acc_83 = (OpenAD_lin_156*INT((-1_w2f__i8)))
            if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
            oad_dt(oad_dt_ptr) = OpenAD_acc_83
            oad_dt_ptr = oad_dt_ptr+1
            if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
            oad_dt(oad_dt_ptr) = OpenAD_lin_155
            oad_dt_ptr = oad_dt_ptr+1
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = EW
            oad_it_ptr = oad_it_ptr+1
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = NS
            oad_it_ptr = oad_it_ptr+1
            OpenAD_Symbol_858 = 0_w2f__i8
            if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
            oad_it(oad_it_ptr) = OpenAD_Symbol_858
            oad_it_ptr = oad_it_ptr+1
          ENDIF
          OpenAD_Symbol_860 = 0_w2f__i8
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_860
          oad_it_ptr = oad_it_ptr+1
        ENDIF
        ACAB(INT(EW),INT(NS))%v = (PRCP(EW,NS)%v-ABLT(EW,NS)%v)
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_862 = 1_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_862
        oad_it_ptr = oad_it_ptr+1
      ELSE
        ABLT(INT(EW),INT(NS))%v = PRCP(EW,NS)%v
        ACAB(INT(EW),INT(NS))%v = 0.0D00
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_861 = 0_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_861
        oad_it_ptr = oad_it_ptr+1
      ENDIF
      OpenAD_Symbol_848 = (INT(OpenAD_Symbol_848)+INT(1_w2f__i8))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_848
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_847 = (INT(OpenAD_Symbol_847)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_847
  oad_it_ptr = oad_it_ptr+1
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_836 = oad_it(oad_it_ptr)
  OpenAD_Symbol_837 = 1
  do while (INT(OpenAD_Symbol_837).LE.INT(OpenAD_Symbol_836))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_838 = oad_it(oad_it_ptr)
    OpenAD_Symbol_839 = 1
    do while (INT(OpenAD_Symbol_839).LE.INT(OpenAD_Symbol_838))
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_840 = oad_it(oad_it_ptr)
      IF (OpenAD_Symbol_840.ne.0) THEN
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1709 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1710 = oad_it(oad_it_ptr)
        ABLT(OpenAD_Symbol_1710,OpenAD_Symbol_1709)%d = ABLT(OpenAD_Symbol_1710,&
     &OpenAD_Symbol_1709)%d-ACAB(OpenAD_Symbol_1710,OpenAD_Symbol_1709)%d

        PRCP(OpenAD_Symbol_1710,OpenAD_Symbol_1709)%d = PRCP(OpenAD_Symbol_1710,&
     &OpenAD_Symbol_1709)%d+ACAB(OpenAD_Symbol_1710,OpenAD_Symbol_1709)%d

        ACAB(OpenAD_Symbol_1710,OpenAD_Symbol_1709)%d = 0.0d0
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_841 = oad_it(oad_it_ptr)
        IF (OpenAD_Symbol_841.ne.0) THEN
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1707 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1708 = oad_it(oad_it_ptr)
          ABLT(OpenAD_Symbol_1708,OpenAD_Symbol_1707)%d = 0.0d0
        ELSE
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_842 = oad_it(oad_it_ptr)
          IF (OpenAD_Symbol_842.ne.0) THEN
            oad_it_ptr = oad_it_ptr-1
            OpenAD_Symbol_1711 = oad_it(oad_it_ptr)
            oad_it_ptr = oad_it_ptr-1
            OpenAD_Symbol_1712 = oad_it(oad_it_ptr)
            WFRAC%d = WFRAC%d-ABLT(OpenAD_Symbol_1712,OpenAD_Symbol_1711)%d
            PABLT%d = PABLT%d+ABLT(OpenAD_Symbol_1712,OpenAD_Symbol_1711)%d
            ABLT(OpenAD_Symbol_1712,OpenAD_Symbol_1711)%d = 0.0d0
          ELSE
            oad_it_ptr = oad_it_ptr-1
            OpenAD_Symbol_1713 = oad_it(oad_it_ptr)
            oad_it_ptr = oad_it_ptr-1
            OpenAD_Symbol_1714 = oad_it(oad_it_ptr)
            oad_dt_ptr = oad_dt_ptr-1
            OpenAD_Symbol_1715 = oad_dt(oad_dt_ptr)
            oad_dt_ptr = oad_dt_ptr-1
            OpenAD_Symbol_1716 = oad_dt(oad_dt_ptr)
            OpenAD_prp_63%d = OpenAD_prp_63%d+ABLT(OpenAD_Symbol_1714,OpenAD_Sym&
     &bol_1713)%d*(OpenAD_Symbol_1715)

            WFRAC%d = WFRAC%d-ABLT(OpenAD_Symbol_1714,OpenAD_Symbol_1713)%d
            PRCP(OpenAD_Symbol_1714,OpenAD_Symbol_1713)%d = PRCP(OpenAD_Symbol_1&
     &714,OpenAD_Symbol_1713)%d+ABLT(OpenAD_Symbol_1714,OpenAD_Symbol_1713)%d

            ABLT(OpenAD_Symbol_1714,OpenAD_Symbol_1713)%d = 0.0d0
            PRCP(OpenAD_Symbol_1714,OpenAD_Symbol_1713)%d = PRCP(OpenAD_Symbol_1&
     &714,OpenAD_Symbol_1713)%d+OpenAD_prp_63%d*(OpenAD_Symbol_1716)

            PDD%d = PDD%d+OpenAD_prp_63%d
            OpenAD_prp_63%d = 0.0d0
          ENDIF
        ENDIF
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1695 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1696 = oad_it(oad_it_ptr)
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1697 = oad_dt(oad_dt_ptr)
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1698 = oad_dt(oad_dt_ptr)
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1699 = oad_dt(oad_dt_ptr)
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1700 = oad_dt(oad_dt_ptr)
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1701 = oad_dt(oad_dt_ptr)
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1702 = oad_dt(oad_dt_ptr)
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1703 = oad_dt(oad_dt_ptr)
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1704 = oad_dt(oad_dt_ptr)
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1705 = oad_dt(oad_dt_ptr)
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1706 = oad_dt(oad_dt_ptr)
        PDD%d = PDD%d+PABLT%d*(OpenAD_Symbol_1697)
        PABLT%d = 0.0d0
        TX%d = TX%d+PDD%d*(OpenAD_Symbol_1698)
        TY%d = TY%d+PDD%d*(OpenAD_Symbol_1699)
        TX%d = TX%d+PDD%d*(OpenAD_Symbol_1700)
        TY%d = TY%d+PDD%d*(OpenAD_Symbol_1701)
        TY%d = TY%d+PDD%d*(OpenAD_Symbol_1702)
        TX%d = TX%d+PDD%d*(OpenAD_Symbol_1703)
        TY%d = TY%d+PDD%d*(OpenAD_Symbol_1704)
        TX%d = TX%d+PDD%d*(OpenAD_Symbol_1705)
        PDD%d = 0.0d0
        PRCP(OpenAD_Symbol_1696,OpenAD_Symbol_1695)%d = PRCP(OpenAD_Symbol_1696,&
     &OpenAD_Symbol_1695)%d+WFRAC%d*(OpenAD_Symbol_1706)

        WFRAC%d = 0.0d0
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_843 = oad_it(oad_it_ptr)
        IF (OpenAD_Symbol_843.ne.0) THEN
          TY%d = 0.0d0
        ELSE
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_844 = oad_it(oad_it_ptr)
          IF (OpenAD_Symbol_844.ne.0) THEN
            TY%d = 0.0d0
          ELSE
            oad_it_ptr = oad_it_ptr-1
            OpenAD_Symbol_1717 = oad_it(oad_it_ptr)
            oad_it_ptr = oad_it_ptr-1
            OpenAD_Symbol_1718 = oad_it(oad_it_ptr)
            ARTM(OpenAD_Symbol_1718,OpenAD_Symbol_1717)%d = ARTM(OpenAD_Symbol_1&
     &718,OpenAD_Symbol_1717)%d+TY%d

            TY%d = 0.0d0
          ENDIF
        ENDIF
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_845 = oad_it(oad_it_ptr)
        IF (OpenAD_Symbol_845.ne.0) THEN
          TX%d = 0.0d0
        ELSE
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_846 = oad_it(oad_it_ptr)
          IF (OpenAD_Symbol_846.ne.0) THEN
            TX%d = 0.0d0
          ELSE
            oad_it_ptr = oad_it_ptr-1
            OpenAD_Symbol_1719 = oad_it(oad_it_ptr)
            oad_it_ptr = oad_it_ptr-1
            OpenAD_Symbol_1720 = oad_it(oad_it_ptr)
            ARNG(OpenAD_Symbol_1720,OpenAD_Symbol_1719)%d = ARNG(OpenAD_Symbol_1&
     &720,OpenAD_Symbol_1719)%d+TX%d

            TX%d = 0.0d0
          ENDIF
        ENDIF
      ELSE
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1721 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1722 = oad_it(oad_it_ptr)
        PRCP(OpenAD_Symbol_1722,OpenAD_Symbol_1721)%d = PRCP(OpenAD_Symbol_1722,&
     &OpenAD_Symbol_1721)%d+ABLT(OpenAD_Symbol_1722,OpenAD_Symbol_1721)%d

        ABLT(OpenAD_Symbol_1722,OpenAD_Symbol_1721)%d = 0.0d0
        ACAB(OpenAD_Symbol_1722,OpenAD_Symbol_1721)%d = 0.0d0
      ENDIF
      OpenAD_Symbol_839 = INT(OpenAD_Symbol_839)+1
    END DO
    OpenAD_Symbol_837 = INT(OpenAD_Symbol_837)+1
  END DO
    end if
  end subroutine GLIMMER_PDD_MBAL
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE PDDTABGRN(PDDTAB)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  use glimmer_global ,only: DP
  use glimmer_global ,only: DP
  use glimmer_global ,only: DP
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_824
  INTEGER(w2f__i8) OpenAD_Symbol_825
  INTEGER(w2f__i8) OpenAD_Symbol_826
  INTEGER(w2f__i8) OpenAD_Symbol_827
  INTEGER(w2f__i8) OpenAD_Symbol_828
  INTEGER(w2f__i8) OpenAD_Symbol_829
  INTEGER(w2f__i8) OpenAD_Symbol_830
  INTEGER(w2f__i8) OpenAD_Symbol_831
  INTEGER(w2f__i8) OpenAD_Symbol_832
  INTEGER(w2f__i8) OpenAD_Symbol_833
  INTEGER(w2f__i8) OpenAD_Symbol_834
  INTEGER(w2f__i8) OpenAD_Symbol_835
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) PDDTAB(1 : 31, 1 : 71)
!
!       **** Local Variables and Functions ****
!
  REAL(w2f__8) DTMJ
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  INTEGER(w2f__i4) KX
  INTEGER(w2f__i4) KY
  INTEGER(w2f__i4) OAD_CTMP0
  REAL(w2f__8) OAD_CTMP1
  REAL(w2f__8) OAD_CTMP2
  INTEGER(w2f__i4) OAD_CTMP3
  REAL(w2f__8) OAD_CTMP4
  REAL(w2f__8) OAD_CTMP5
  REAL(w2f__8) OAD_CTMP6
  REAL(w2f__8) TMA
  REAL(w2f__8) TWOPI
  PARAMETER ( TWOPI = 6.28319999999999989626D00)


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  DO J = 0, 70, 1
    TMA = REAL(PDD_IY + J * PDD_DY, 8)
    OAD_CTMP1 = REAL(PDD_IY, 8)
    OAD_CTMP2 = REAL(PDD_DY, 8)
    CALL OAD_S_FINDGRID(TMA,OAD_CTMP1,OAD_CTMP2,OAD_CTMP0)
    KY = OAD_CTMP0
    DO I = 0, 30, 1
      DTMJ = REAL(PDD_IX + I * PDD_DX, 8)
      BD_MEAN_JULY_TEMP = (DTMJ + TMA)
      OAD_CTMP4 = REAL(PDD_IX, 8)
      OAD_CTMP5 = REAL(PDD_DX, 8)
      CALL OAD_S_FINDGRID(DTMJ,OAD_CTMP4,OAD_CTMP5,OAD_CTMP3)
      KX = OAD_CTMP3
      BD_MEAN_ANNUAL_TEMP = TMA
      BD_DD_SIGMA = PDD_DD_SIGMA
      CALL OAD_S_ROMBERG_INT_OUTER(0.0D00,6.28319999999999989626D00,OAD_CTMP6)
      PDDTAB(INT(KX), INT(KY)) = (OAD_CTMP6 *(1.0D00 /( BD_DD_SIGMA * SQRT(6.283&
     &19999999999989626D00))))

      PDDTAB(INT(KX), INT(KY)) = ((PDDTAB(KX, KY) * 3.65D+02) / 6.28319999999999&
     &989626D00)

    END DO
  END DO
    end if
    if (our_rev_mode%tape) then
! taping
  OpenAD_Symbol_828 = 0_w2f__i8
  DO J = 0, 70, 1
    TMA = REAL(PDD_IY + J * PDD_DY, 8)
    OAD_CTMP1 = REAL(PDD_IY, 8)
    OAD_CTMP2 = REAL(PDD_DY, 8)
    CALL OAD_S_FINDGRID(TMA,OAD_CTMP1,OAD_CTMP2,OAD_CTMP0)
    KY = OAD_CTMP0
    OpenAD_Symbol_829 = 0_w2f__i8
    DO I = 0, 30, 1
      DTMJ = REAL(PDD_IX + I * PDD_DX, 8)
      BD_MEAN_JULY_TEMP = (DTMJ + TMA)
      OAD_CTMP4 = REAL(PDD_IX, 8)
      OAD_CTMP5 = REAL(PDD_DX, 8)
      CALL OAD_S_FINDGRID(DTMJ,OAD_CTMP4,OAD_CTMP5,OAD_CTMP3)
      KX = OAD_CTMP3
      BD_MEAN_ANNUAL_TEMP = TMA
      BD_DD_SIGMA = PDD_DD_SIGMA
      CALL OAD_S_ROMBERG_INT_OUTER(0.0D00,6.28319999999999989626D00,OAD_CTMP6)
      PDDTAB(INT(KX), INT(KY)) = (OAD_CTMP6 *(1.0D00 /( BD_DD_SIGMA * SQRT(6.283&
     &19999999999989626D00))))

      PDDTAB(INT(KX), INT(KY)) = ((PDDTAB(KX, KY) * 3.65D+02) / 6.28319999999999&
     &989626D00)

      OpenAD_Symbol_829 = (INT(OpenAD_Symbol_829) + INT(1_w2f__i8 ))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_829
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_828 = (INT(OpenAD_Symbol_828) + INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_828
  oad_it_ptr = oad_it_ptr+1
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_824 = oad_it(oad_it_ptr)
  OpenAD_Symbol_825 = 1
  DO WHILE(INT(OpenAD_Symbol_825) .LE. INT(OpenAD_Symbol_824))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_826 = oad_it(oad_it_ptr)
    OpenAD_Symbol_827 = 1
    DO WHILE(INT(OpenAD_Symbol_827) .LE. INT(OpenAD_Symbol_826))
      CALL OAD_S_ROMBERG_INT_OUTER(0.0D00,6.28319999999999989626D00,OAD_CTMP6)
      CALL OAD_S_FINDGRID(DTMJ,OAD_CTMP4,OAD_CTMP5,OAD_CTMP3)
      OpenAD_Symbol_827 = INT(OpenAD_Symbol_827) + 1
    END DO
    CALL OAD_S_FINDGRID(TMA,OAD_CTMP1,OAD_CTMP2,OAD_CTMP0)
    OpenAD_Symbol_825 = INT(OpenAD_Symbol_825) + 1
  END DO
    end if
  end subroutine PDDTABGRN

  Function ROMBERG_INT_OUTER(LGR, RGR)
  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) LGR
  INTENT(IN) LGR
  REAL(w2f__8) RGR
  INTENT(IN) RGR
  REAL(w2f__8) ROMBERG_INT_OUTER
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) F
  INTEGER(w2f__i4) H
  INTEGER(w2f__i4) J
  REAL(w2f__8) L
  REAL(w2f__8) M
  INTEGER(w2f__i4) N
  INTEGER(w2f__i4) ORD
  PARAMETER ( ORD = 6)
  REAL(w2f__8) T(1 : 7)
  REAL(w2f__8) U
!
!       **** Statements ****
!
  L = RGR - LGR
  T(1) = ((INNER_INTEGRAL(LGR)+INNER_INTEGRAL(RGR))*5.0D-01)
  N = 1
  DO H = 1, 6, 1
    U = 0.0D00
    M = L /(REAL(N, 8) * 2.0D00)
    DO J = 1, (N * 2 +(-1)), 2
      U = U+INNER_INTEGRAL((LGR+M*real(J,8)))
    END DO
    T(H+1) = ((T(H)+(U/real(N,8)))*5.0D-01)
    F = 1
    DO J = H, 1, (-1)
      F = F * 4
      T(J) = (T(J+1)+((T(J+1)-T(J))/real(F+(-1),8)))
    END DO
    N = N * 2
  END DO
  ROMBERG_INT_OUTER = T(1)*L
  RETURN
  END FUNCTION

  Function INNER_INTEGRAL(DAY)
  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) DAY
  INTENT(IN) DAY
  REAL(w2f__8) INNER_INTEGRAL
!
!       **** Local Variables and Functions ****
!
  REAL(w2f__8) UPPER_LIMIT
!
!       **** Statements ****
!
  BD_T_A_PRIME = BD_MEAN_ANNUAL_TEMP + COS(DAY) *( BD_MEAN_JULY_TEMP - BD_MEAN_A&
     &NNUAL_TEMP)

  UPPER_LIMIT = BD_T_A_PRIME + BD_DD_SIGMA * 2.5D00
  IF(UPPER_LIMIT .LE. 0.0D00) THEN
    INNER_INTEGRAL = 0.0D00
  ELSE
    INNER_INTEGRAL = ROMBERG_INT_INNER((0.0D00),UPPER_LIMIT)
  ENDIF
  RETURN
  END FUNCTION

  Function ROMBERG_INT_INNER(LGR, RGR)
  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) LGR
  INTENT(IN) LGR
  REAL(w2f__8) RGR
  INTENT(IN) RGR
  REAL(w2f__8) ROMBERG_INT_INNER
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) F
  INTEGER(w2f__i4) H
  INTEGER(w2f__i4) J
  REAL(w2f__8) L
  REAL(w2f__8) M
  INTEGER(w2f__i4) N
  INTEGER(w2f__i4) ORD
  PARAMETER ( ORD = 6)
  REAL(w2f__8) T(1 : 7)
  REAL(w2f__8) U
!
!       **** Statements ****
!
  L = RGR - LGR
  T(1) = ((LGR*exp(-(((LGR-BD_T_A_PRIME)**2)/((BD_DD_SIGMA**2)*2.0D00)))+RGR*exp&
     &(-(((RGR-BD_T_A_PRIME)**2)/((BD_DD_SIGMA**2)*2.0D00))))*5.0D-01)

  N = 1
  DO H = 1, 6, 1
    U = 0.0D00
    M = L / REAL(N * 2, 8)
    DO J = 1, (N * 2 +(-1)), 2
      U = U + EXP(-((((LGR + M * REAL(J, 8)) - BD_T_A_PRIME) ** 2 ) /((BD_DD_SIG&
     &MA ** 2) * 2.0D00))) *(LGR + M * REAL(J, 8))

    END DO
    T(H+1) = ((T(H)+(U/real(N,8)))*5.0D-01)
    F = 1
    DO J = H, 1, (-1)
      F = F * 4
      T(J) = (T(J+1)+((T(J+1)-T(J))/real(F+(-1),8)))
    END DO
    N = N * 2
  END DO
  ROMBERG_INT_INNER = T(1)*L
  RETURN
  END FUNCTION

  Function FINDGRID(RIN, INIT, STEP)
  use w2f__types
  use glimmer_global ,only: DP
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) RIN
  INTENT(IN) RIN
  REAL(w2f__8) INIT
  INTENT(IN) INIT
  REAL(w2f__8) STEP
  INTENT(IN) STEP
  INTEGER(w2f__i4) FINDGRID
!
!       **** Statements ****
!
  FINDGRID = INT((RIN - INIT) / STEP) + 1
  RETURN
  END FUNCTION
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_ROMBERG_INT_OUTER(LGR, RGR, ROMBERG_INT_OUTER)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_890
  INTEGER(w2f__i8) OpenAD_Symbol_891
  INTEGER(w2f__i8) OpenAD_Symbol_892
  INTEGER(w2f__i8) OpenAD_Symbol_893
  INTEGER(w2f__i8) OpenAD_Symbol_894
  INTEGER(w2f__i8) OpenAD_Symbol_895
  INTEGER(w2f__i8) OpenAD_Symbol_896
  INTEGER(w2f__i8) OpenAD_Symbol_897
  INTEGER(w2f__i8) OpenAD_Symbol_898
  INTEGER(w2f__i8) OpenAD_Symbol_899
  INTEGER(w2f__i8) OpenAD_Symbol_900
  INTEGER(w2f__i8) OpenAD_Symbol_901
  INTEGER(w2f__i8) OpenAD_Symbol_902
  INTEGER(w2f__i8) OpenAD_Symbol_903
  INTEGER(w2f__i8) OpenAD_Symbol_904
  INTEGER(w2f__i8) OpenAD_Symbol_905
  INTEGER(w2f__i8) OpenAD_Symbol_906
  INTEGER(w2f__i8) OpenAD_Symbol_907
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) LGR
  REAL(w2f__8) RGR
  REAL(w2f__8) ROMBERG_INT_OUTER
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) F
  INTEGER(w2f__i4) H
  INTEGER(w2f__i4) J
  REAL(w2f__8) L
  REAL(w2f__8) M
  INTEGER(w2f__i4) N
  REAL(w2f__8) OAD_CTMP0
  REAL(w2f__8) OAD_CTMP1
  REAL(w2f__8) OAD_CTMP2
  REAL(w2f__8) OAD_CTMP3
  INTEGER(w2f__i4) ORD
  PARAMETER ( ORD = 6)
  REAL(w2f__8) T(1 : 7)
  REAL(w2f__8) U


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  L = (RGR - LGR)
  CALL OAD_S_INNER_INTEGRAL(LGR,OAD_CTMP0)
  CALL OAD_S_INNER_INTEGRAL(RGR,OAD_CTMP1)
  T(1) = ((OAD_CTMP0 + OAD_CTMP1) * 5.0D-01)
  N = 1
  DO H = 1, 6, 1
    U = 0.0D00
    M = (L /(REAL(N, 8) * 2.0D00))
    DO J = 1, (N * 2 +(-1)), 2
      OAD_CTMP3 = (LGR + M * REAL(J, 8))
      CALL OAD_S_INNER_INTEGRAL(OAD_CTMP3,OAD_CTMP2)
      U = (OAD_CTMP2 + U)
    END DO
    T(H + 1) = ((T(H) +(U / REAL(N, 8))) * 5.0D-01)
    F = 1
    DO J = H, 1, (-1)
      F = (F * 4)
      T(INT(J)) = (T(J + 1) +((T(J + 1) - T(J)) / REAL(F +(-1), 8 )))
    END DO
    N = (N * 2)
  END DO
  ROMBERG_INT_OUTER = (T(1) * L)
    end if
    if (our_rev_mode%tape) then
! taping
  L = (RGR - LGR)
  CALL OAD_S_INNER_INTEGRAL(LGR,OAD_CTMP0)
  CALL OAD_S_INNER_INTEGRAL(RGR,OAD_CTMP1)
  T(1) = ((OAD_CTMP0 + OAD_CTMP1) * 5.0D-01)
  N = 1
  OpenAD_Symbol_896 = 0_w2f__i8
  DO H = 1, 6, 1
    U = 0.0D00
    M = (L /(REAL(N, 8) * 2.0D00))
    OpenAD_Symbol_897 = 0_w2f__i8
    DO J = 1, (N * 2 +(-1)), 2
      OAD_CTMP3 = (LGR + M * REAL(J, 8))
      CALL OAD_S_INNER_INTEGRAL(OAD_CTMP3,OAD_CTMP2)
      U = (OAD_CTMP2 + U)
      OpenAD_Symbol_897 = (INT(OpenAD_Symbol_897) + INT(1_w2f__i8 ))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_897
    oad_it_ptr = oad_it_ptr+1
    T(H + 1) = ((T(H) +(U / REAL(N, 8))) * 5.0D-01)
    F = 1
    OpenAD_Symbol_898 = 0_w2f__i8
    DO J = H, 1, (-1)
      F = (F * 4)
      T(INT(J)) = (T(J + 1) +((T(J + 1) - T(J)) / REAL(F +(-1), 8 )))
      OpenAD_Symbol_898 = (INT(OpenAD_Symbol_898) + INT(1_w2f__i8 ))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_898
    oad_it_ptr = oad_it_ptr+1
    N = (N * 2)
    OpenAD_Symbol_896 = (INT(OpenAD_Symbol_896) + INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_896
  oad_it_ptr = oad_it_ptr+1
  ROMBERG_INT_OUTER = (T(1) * L)
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_890 = oad_it(oad_it_ptr)
  OpenAD_Symbol_891 = 1
  DO WHILE(INT(OpenAD_Symbol_891) .LE. INT(OpenAD_Symbol_890))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_892 = oad_it(oad_it_ptr)
    OpenAD_Symbol_893 = 1
    DO WHILE(INT(OpenAD_Symbol_893) .LE. INT(OpenAD_Symbol_892))
      OpenAD_Symbol_893 = INT(OpenAD_Symbol_893) + 1
    END DO
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_894 = oad_it(oad_it_ptr)
    OpenAD_Symbol_895 = 1
    DO WHILE(INT(OpenAD_Symbol_895) .LE. INT(OpenAD_Symbol_894))
      CALL OAD_S_INNER_INTEGRAL(OAD_CTMP3,OAD_CTMP2)
      OpenAD_Symbol_895 = INT(OpenAD_Symbol_895) + 1
    END DO
    OpenAD_Symbol_891 = INT(OpenAD_Symbol_891) + 1
  END DO
  CALL OAD_S_INNER_INTEGRAL(RGR,OAD_CTMP1)
  CALL OAD_S_INNER_INTEGRAL(LGR,OAD_CTMP0)
    end if
  end subroutine OAD_S_ROMBERG_INT_OUTER
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_INNER_INTEGRAL(DAY, INNER_INTEGRAL)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_908
  INTEGER(w2f__i8) OpenAD_Symbol_909
  INTEGER(w2f__i8) OpenAD_Symbol_910
  INTEGER(w2f__i8) OpenAD_Symbol_911
  INTEGER(w2f__i8) OpenAD_Symbol_912
  INTEGER(w2f__i8) OpenAD_Symbol_913
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) DAY
  REAL(w2f__8) INNER_INTEGRAL
!
!       **** Local Variables and Functions ****
!
  REAL(w2f__8) OAD_CTMP0
  REAL(w2f__8) UPPER_LIMIT


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  BD_T_A_PRIME = (BD_MEAN_ANNUAL_TEMP + COS(DAY) *( BD_MEAN_JULY_TEMP - BD_MEAN_&
     &ANNUAL_TEMP))

  UPPER_LIMIT = (BD_T_A_PRIME + BD_DD_SIGMA * 2.5D00)
  IF(UPPER_LIMIT .LE. 0.0D00) THEN
    INNER_INTEGRAL = 0.0D00
  ELSE
    CALL OAD_S_ROMBERG_INT_INNER(0.0D00,UPPER_LIMIT,OAD_CTMP0)
    INNER_INTEGRAL = OAD_CTMP0
  ENDIF
    end if
    if (our_rev_mode%tape) then
! taping
  BD_T_A_PRIME = (BD_MEAN_ANNUAL_TEMP + COS(DAY) *( BD_MEAN_JULY_TEMP - BD_MEAN_&
     &ANNUAL_TEMP))

  UPPER_LIMIT = (BD_T_A_PRIME + BD_DD_SIGMA * 2.5D00)
  IF(UPPER_LIMIT .LE. 0.0D00) THEN
    INNER_INTEGRAL = 0.0D00
    OpenAD_Symbol_909 = 1_w2f__i8
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_909
    oad_it_ptr = oad_it_ptr+1
  ELSE
    CALL OAD_S_ROMBERG_INT_INNER(0.0D00,UPPER_LIMIT,OAD_CTMP0)
    INNER_INTEGRAL = OAD_CTMP0
    OpenAD_Symbol_910 = 0_w2f__i8
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_910
    oad_it_ptr = oad_it_ptr+1
  ENDIF
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_908 = oad_it(oad_it_ptr)
  IF(OpenAD_Symbol_908 .ne. 0) THEN
  ELSE
    CALL OAD_S_ROMBERG_INT_INNER(0.0D00,UPPER_LIMIT,OAD_CTMP0)
  ENDIF
    end if
  end subroutine OAD_S_INNER_INTEGRAL
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_ROMBERG_INT_INNER(LGR, RGR, ROMBERG_INT_INNER)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_914
  INTEGER(w2f__i8) OpenAD_Symbol_915
  INTEGER(w2f__i8) OpenAD_Symbol_916
  INTEGER(w2f__i8) OpenAD_Symbol_917
  INTEGER(w2f__i8) OpenAD_Symbol_918
  INTEGER(w2f__i8) OpenAD_Symbol_919
  INTEGER(w2f__i8) OpenAD_Symbol_920
  INTEGER(w2f__i8) OpenAD_Symbol_921
  INTEGER(w2f__i8) OpenAD_Symbol_922
  INTEGER(w2f__i8) OpenAD_Symbol_923
  INTEGER(w2f__i8) OpenAD_Symbol_924
  INTEGER(w2f__i8) OpenAD_Symbol_925
  INTEGER(w2f__i8) OpenAD_Symbol_926
  INTEGER(w2f__i8) OpenAD_Symbol_927
  INTEGER(w2f__i8) OpenAD_Symbol_928
  INTEGER(w2f__i8) OpenAD_Symbol_929
  INTEGER(w2f__i8) OpenAD_Symbol_930
  INTEGER(w2f__i8) OpenAD_Symbol_931
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) LGR
  REAL(w2f__8) RGR
  REAL(w2f__8) ROMBERG_INT_INNER
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) F
  INTEGER(w2f__i4) H
  INTEGER(w2f__i4) J
  REAL(w2f__8) L
  REAL(w2f__8) M
  INTEGER(w2f__i4) N
  INTEGER(w2f__i4) ORD
  PARAMETER ( ORD = 6)
  REAL(w2f__8) T(1 : 7)
  REAL(w2f__8) U


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  L = (RGR - LGR)
  T(1) = ((LGR * EXP(-(((LGR - BD_T_A_PRIME) ** 2) /((BD_DD_SIGMA ** 2) * 2.0D00&
     &))) + RGR * EXP(-(((RGR - BD_T_A_PRIME) ** 2) /(( BD_DD_SIGMA ** 2) * 2.0D&
     &00)))) * 5.0D-01)

  N = 1
  DO H = 1, 6, 1
    U = 0.0D00
    M = (L / REAL(N * 2, 8))
    DO J = 1, (N * 2 +(-1)), 2
      U = (U + EXP(-(((LGR + M * REAL(J, 8) - BD_T_A_PRIME) ** 2) /((BD_DD_SIGMA&
     & ** 2) * 2.0D00))) *(LGR + M * REAL(J, 8)))

    END DO
    T(H + 1) = ((T(H) +(U / REAL(N, 8))) * 5.0D-01)
    F = 1
    DO J = H, 1, (-1)
      F = (F * 4)
      T(INT(J)) = (T(J + 1) +((T(J + 1) - T(J)) / REAL(F +(-1), 8 )))
    END DO
    N = (N * 2)
  END DO
  ROMBERG_INT_INNER = (T(1) * L)
    end if
    if (our_rev_mode%tape) then
! taping
  L = (RGR - LGR)
  T(1) = ((LGR * EXP(-(((LGR - BD_T_A_PRIME) ** 2) /((BD_DD_SIGMA ** 2) * 2.0D00&
     &))) + RGR * EXP(-(((RGR - BD_T_A_PRIME) ** 2) /(( BD_DD_SIGMA ** 2) * 2.0D&
     &00)))) * 5.0D-01)

  N = 1
  OpenAD_Symbol_920 = 0_w2f__i8
  DO H = 1, 6, 1
    U = 0.0D00
    M = (L / REAL(N * 2, 8))
    OpenAD_Symbol_921 = 0_w2f__i8
    DO J = 1, (N * 2 +(-1)), 2
      U = (U + EXP(-(((LGR + M * REAL(J, 8) - BD_T_A_PRIME) ** 2) /((BD_DD_SIGMA&
     & ** 2) * 2.0D00))) *(LGR + M * REAL(J, 8)))

      OpenAD_Symbol_921 = (INT(OpenAD_Symbol_921) + INT(1_w2f__i8 ))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_921
    oad_it_ptr = oad_it_ptr+1
    T(H + 1) = ((T(H) +(U / REAL(N, 8))) * 5.0D-01)
    F = 1
    OpenAD_Symbol_922 = 0_w2f__i8
    DO J = H, 1, (-1)
      F = (F * 4)
      T(INT(J)) = (T(J + 1) +((T(J + 1) - T(J)) / REAL(F +(-1), 8 )))
      OpenAD_Symbol_922 = (INT(OpenAD_Symbol_922) + INT(1_w2f__i8 ))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_922
    oad_it_ptr = oad_it_ptr+1
    N = (N * 2)
    OpenAD_Symbol_920 = (INT(OpenAD_Symbol_920) + INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_920
  oad_it_ptr = oad_it_ptr+1
  ROMBERG_INT_INNER = (T(1) * L)
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_914 = oad_it(oad_it_ptr)
  OpenAD_Symbol_915 = 1
  DO WHILE(INT(OpenAD_Symbol_915) .LE. INT(OpenAD_Symbol_914))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_916 = oad_it(oad_it_ptr)
    OpenAD_Symbol_917 = 1
    DO WHILE(INT(OpenAD_Symbol_917) .LE. INT(OpenAD_Symbol_916))
      OpenAD_Symbol_917 = INT(OpenAD_Symbol_917) + 1
    END DO
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_918 = oad_it(oad_it_ptr)
    OpenAD_Symbol_919 = 1
    DO WHILE(INT(OpenAD_Symbol_919) .LE. INT(OpenAD_Symbol_918))
      OpenAD_Symbol_919 = INT(OpenAD_Symbol_919) + 1
    END DO
    OpenAD_Symbol_915 = INT(OpenAD_Symbol_915) + 1
  END DO
    end if
  end subroutine OAD_S_ROMBERG_INT_INNER
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE OAD_S_FINDGRID(RIN, INIT, STEP, FINDGRID)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  use glimmer_global ,only: DP
  use glimmer_global ,only: DP
  use glimmer_global ,only: DP
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) RIN
  REAL(w2f__8) INIT
  REAL(w2f__8) STEP
  INTEGER(w2f__i4) FINDGRID


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  FINDGRID = (INT((RIN - INIT) / STEP) + 1)
    end if
    if (our_rev_mode%tape) then
! taping
  FINDGRID = (INT((RIN - INIT) / STEP) + 1)
    end if
    if (our_rev_mode%adjoint) then
! adjoint
    end if
  end subroutine OAD_S_FINDGRID
END

!$OPENAD XXX File_start [adism_core.F90]
MODULE adism_core
use OAD_active
use w2f__types
use oad_intrinsics
use glimmer_utils
use glimmer_deriv
use physcon ,only: GN , GRAV , PMLT , RHOI
IMPLICIT NONE
SAVE
!
!     **** Local Variables and Functions ****
!
REAL(w2f__8) CONST
PARAMETER ( CONST = 2.84571360659804443359D+11)
REAL(w2f__8) ST
PARAMETER ( ST = 1.67000005603767931461D-04)
REAL(w2f__8) XS
PARAMETER ( XS = 7.5D+05)
REAL(w2f__8) YS
PARAMETER ( YS = 7.5D+05)
!
!     **** Statements ****
!
CONTAINS
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE CALCGLENA(N, M, NZ, GA, TSTAR, ENFAC)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  use physcon ,only: ACTENH , ACTENL , ARRMLH , ARRMLL , GASCON
  use physcon ,only: ACTENH , ACTENL , ARRMLH , ARRMLL , GASCON
  use physcon ,only: ACTENH , ACTENL , ARRMLH , ARRMLL , GASCON
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_932
  INTEGER(w2f__i8) OpenAD_Symbol_933
  INTEGER(w2f__i8) OpenAD_Symbol_934
  INTEGER(w2f__i8) OpenAD_Symbol_935
  INTEGER(w2f__i8) OpenAD_Symbol_936
  INTEGER(w2f__i8) OpenAD_Symbol_937
  INTEGER(w2f__i8) OpenAD_Symbol_938
  INTEGER(w2f__i8) OpenAD_Symbol_939
  INTEGER(w2f__i8) OpenAD_Symbol_940
  INTEGER(w2f__i8) OpenAD_Symbol_941
  INTEGER(w2f__i8) OpenAD_Symbol_942
  INTEGER(w2f__i8) OpenAD_Symbol_943
  INTEGER(w2f__i8) OpenAD_Symbol_944
  INTEGER(w2f__i8) OpenAD_Symbol_945
  INTEGER(w2f__i8) OpenAD_Symbol_946
  INTEGER(w2f__i8) OpenAD_Symbol_947
  INTEGER(w2f__i8) OpenAD_Symbol_948
  INTEGER(w2f__i8) OpenAD_Symbol_949
  INTEGER(w2f__i8) OpenAD_Symbol_950
  INTEGER(w2f__i8) OpenAD_Symbol_951
  INTEGER(w2f__i8) OpenAD_Symbol_952
  INTEGER(w2f__i8) OpenAD_Symbol_953
  INTEGER(w2f__i8) OpenAD_Symbol_954
  INTEGER(w2f__i8) OpenAD_Symbol_955
!
!       **** Parameters and Result ****
!
  INTEGER(w2f__i4) N
  INTEGER(w2f__i4) M
  INTEGER(w2f__i4) NZ
  type(active) :: GA(1:NZ,1:N,1:M)
  type(active) :: TSTAR(1:NZ,0:INT((N+1)),0:INT((M+1)))
  REAL(w2f__8) ENFAC
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  INTEGER(w2f__i4) UP
  INTEGER(w2f__i4) OpenAD_Symbol_1723
  INTEGER(w2f__i4) OpenAD_Symbol_1724
  INTEGER(w2f__i4) OpenAD_Symbol_1725
  INTEGER(w2f__i4) OpenAD_Symbol_1726
  INTEGER(w2f__i4) OpenAD_Symbol_1727
  INTEGER(w2f__i4) OpenAD_Symbol_1728
  REAL(w2f__8) OpenAD_Symbol_1729
  INTEGER(w2f__i4) OpenAD_Symbol_1730
  INTEGER(w2f__i4) OpenAD_Symbol_1731
  INTEGER(w2f__i4) OpenAD_Symbol_1732
  REAL(w2f__8) OpenAD_Symbol_1733
  REAL(w2f__8) OpenAD_acc_84
  REAL(w2f__8) OpenAD_acc_85
  REAL(w2f__8) OpenAD_aux_76
  REAL(w2f__8) OpenAD_aux_77
  REAL(w2f__8) OpenAD_aux_78
  REAL(w2f__8) OpenAD_aux_79
  REAL(w2f__8) OpenAD_aux_80
  REAL(w2f__8) OpenAD_aux_81
  REAL(w2f__8) OpenAD_aux_82
  REAL(w2f__8) OpenAD_aux_83
  REAL(w2f__8) OpenAD_lin_157
  REAL(w2f__8) OpenAD_lin_158
  REAL(w2f__8) OpenAD_lin_159
  REAL(w2f__8) OpenAD_lin_160
  REAL(w2f__8) OpenAD_lin_161
  REAL(w2f__8) OpenAD_lin_162
!
!       **** Statements ****
!


    integer iaddr
    external iaddr

   if (our_rev_mode%plain) then
! original function
  GA(1:INT(NZ),1:INT(N),1:INT(M))%v = 0.0
  DO I = 1,N,1
    DO J = 1,M,1
      DO UP = 1,NZ,1
        IF (TSTAR(UP,I,J)%v.GE.(-1.0D+01)) THEN
          GA(INT(UP),INT(I),INT(J))%v = (EXP(-(1.39D+05/((TSTAR(UP,I,J)%v+2.7314&
     &9993896484375D+02)*8.31400000000000005684D00)))*ENFAC*1.733D+03)

        ELSE
          GA(INT(UP),INT(I),INT(J))%v = (EXP(-(6.0D+04/((TSTAR(UP,I,J)%v+2.73149&
     &993896484375D+02)*8.31400000000000005684D00)))*ENFAC*3.612999999999999852D&
     &-13)

        ENDIF
      END DO
    END DO
  END DO
    end if
    if (our_rev_mode%tape) then
! taping
  GA(1:INT(NZ),1:INT(N),1:INT(M))%v = 0.0
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = NZ
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = N
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = M
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_939 = 0_w2f__i8
  DO I = 1,N,1
    OpenAD_Symbol_940 = 0_w2f__i8
    DO J = 1,M,1
      OpenAD_Symbol_941 = 0_w2f__i8
      DO UP = 1,NZ,1
        IF (TSTAR(UP,I,J)%v.GE.(-1.0D+01)) THEN
          OpenAD_aux_79 = (TSTAR(UP,I,J)%v+2.73149993896484375D+02)
          OpenAD_aux_78 = (OpenAD_aux_79*8.31400000000000005684D00)
          OpenAD_aux_76 = EXP(-(1.39D+05/OpenAD_aux_78))
          OpenAD_aux_77 = (ENFAC*1.733D+03)
          OpenAD_lin_159 = (-(1.39D+05/(OpenAD_aux_78*OpenAD_aux_78)))
          OpenAD_lin_158 = OpenAD_aux_76
          OpenAD_lin_157 = OpenAD_aux_77
          GA(INT(UP),INT(I),INT(J))%v = (OpenAD_aux_76*OpenAD_aux_77)
          OpenAD_acc_84 = (8.31400000000000005684D00*OpenAD_lin_159*INT((-1_w2f_&
     &_i8))*OpenAD_lin_158*OpenAD_lin_157)

          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_acc_84
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = UP
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = I
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = J
          oad_it_ptr = oad_it_ptr+1
          OpenAD_Symbol_942 = 1_w2f__i8
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_942
          oad_it_ptr = oad_it_ptr+1
        ELSE
          OpenAD_aux_83 = (TSTAR(UP,I,J)%v+2.73149993896484375D+02)
          OpenAD_aux_82 = (OpenAD_aux_83*8.31400000000000005684D00)
          OpenAD_aux_80 = EXP(-(6.0D+04/OpenAD_aux_82))
          OpenAD_aux_81 = (ENFAC*3.612999999999999852D-13)
          OpenAD_lin_162 = (-(6.0D+04/(OpenAD_aux_82*OpenAD_aux_82)))
          OpenAD_lin_161 = OpenAD_aux_80
          OpenAD_lin_160 = OpenAD_aux_81
          GA(INT(UP),INT(I),INT(J))%v = (OpenAD_aux_80*OpenAD_aux_81)
          OpenAD_acc_85 = (8.31400000000000005684D00*OpenAD_lin_162*INT((-1_w2f_&
     &_i8))*OpenAD_lin_161*OpenAD_lin_160)

          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_acc_85
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = UP
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = I
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = J
          oad_it_ptr = oad_it_ptr+1
          OpenAD_Symbol_943 = 0_w2f__i8
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_943
          oad_it_ptr = oad_it_ptr+1
        ENDIF
        OpenAD_Symbol_941 = (INT(OpenAD_Symbol_941)+INT(1_w2f__i8))
      END DO
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = OpenAD_Symbol_941
      oad_it_ptr = oad_it_ptr+1
      OpenAD_Symbol_940 = (INT(OpenAD_Symbol_940)+INT(1_w2f__i8))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_940
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_939 = (INT(OpenAD_Symbol_939)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_939
  oad_it_ptr = oad_it_ptr+1
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_932 = oad_it(oad_it_ptr)
  OpenAD_Symbol_933 = 1
  do while (INT(OpenAD_Symbol_933).LE.INT(OpenAD_Symbol_932))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_934 = oad_it(oad_it_ptr)
    OpenAD_Symbol_935 = 1
    do while (INT(OpenAD_Symbol_935).LE.INT(OpenAD_Symbol_934))
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_936 = oad_it(oad_it_ptr)
      OpenAD_Symbol_937 = 1
      do while (INT(OpenAD_Symbol_937).LE.INT(OpenAD_Symbol_936))
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_938 = oad_it(oad_it_ptr)
        IF (OpenAD_Symbol_938.ne.0) THEN
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1726 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1727 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1728 = oad_it(oad_it_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1729 = oad_dt(oad_dt_ptr)
          TSTAR(OpenAD_Symbol_1728,OpenAD_Symbol_1727,OpenAD_Symbol_1726)%d = TS&
     &TAR(OpenAD_Symbol_1728,OpenAD_Symbol_1727,OpenAD_Symbol_1726)%d+GA(OpenAD_&
     &Symbol_1728,OpenAD_Symbol_1727,OpenAD_Symbol_1726)%d*(OpenAD_Symbol_1729)

          GA(OpenAD_Symbol_1728,OpenAD_Symbol_1727,OpenAD_Symbol_1726)%d = 0.0d0
        ELSE
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1730 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1731 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1732 = oad_it(oad_it_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1733 = oad_dt(oad_dt_ptr)
          TSTAR(OpenAD_Symbol_1732,OpenAD_Symbol_1731,OpenAD_Symbol_1730)%d = TS&
     &TAR(OpenAD_Symbol_1732,OpenAD_Symbol_1731,OpenAD_Symbol_1730)%d+GA(OpenAD_&
     &Symbol_1732,OpenAD_Symbol_1731,OpenAD_Symbol_1730)%d*(OpenAD_Symbol_1733)

          GA(OpenAD_Symbol_1732,OpenAD_Symbol_1731,OpenAD_Symbol_1730)%d = 0.0d0
        ENDIF
        OpenAD_Symbol_937 = INT(OpenAD_Symbol_937)+1
      END DO
      OpenAD_Symbol_935 = INT(OpenAD_Symbol_935)+1
    END DO
    OpenAD_Symbol_933 = INT(OpenAD_Symbol_933)+1
  END DO
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1723 = oad_it(oad_it_ptr)
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1724 = oad_it(oad_it_ptr)
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1725 = oad_it(oad_it_ptr)
  GA(1:OpenAD_Symbol_1725,1:OpenAD_Symbol_1724,1:OpenAD_Symbol_1723)%d = 0.0d0
    end if
  end subroutine CALCGLENA
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE CALCTSTAR(N, M, NZ, TSTAR, TEMP, THCK, SIGMA)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  use physcon ,only: GRAV , PMLT , RHOI
  use physcon ,only: GRAV , PMLT , RHOI
  use physcon ,only: GRAV , PMLT , RHOI
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_956
  INTEGER(w2f__i8) OpenAD_Symbol_957
  INTEGER(w2f__i8) OpenAD_Symbol_958
  INTEGER(w2f__i8) OpenAD_Symbol_959
  INTEGER(w2f__i8) OpenAD_Symbol_960
  INTEGER(w2f__i8) OpenAD_Symbol_961
  INTEGER(w2f__i8) OpenAD_Symbol_962
  INTEGER(w2f__i8) OpenAD_Symbol_963
  INTEGER(w2f__i8) OpenAD_Symbol_964
  INTEGER(w2f__i8) OpenAD_Symbol_965
  INTEGER(w2f__i8) OpenAD_Symbol_966
  INTEGER(w2f__i8) OpenAD_Symbol_967
  INTEGER(w2f__i8) OpenAD_Symbol_968
  INTEGER(w2f__i8) OpenAD_Symbol_969
  INTEGER(w2f__i8) OpenAD_Symbol_970
  INTEGER(w2f__i8) OpenAD_Symbol_971
  INTEGER(w2f__i8) OpenAD_Symbol_972
  INTEGER(w2f__i8) OpenAD_Symbol_973
  INTEGER(w2f__i8) OpenAD_Symbol_974
  INTEGER(w2f__i8) OpenAD_Symbol_975
  INTEGER(w2f__i8) OpenAD_Symbol_976
  INTEGER(w2f__i8) OpenAD_Symbol_977
  INTEGER(w2f__i8) OpenAD_Symbol_978
  INTEGER(w2f__i8) OpenAD_Symbol_979
!
!       **** Parameters and Result ****
!
  INTEGER(w2f__i4) N
  INTEGER(w2f__i4) M
  INTEGER(w2f__i4) NZ
  type(active) :: TSTAR(1:NZ,0:INT((N+1)),0:INT((M+1)))
  type(active) :: TEMP(1:NZ,0:INT((N+1)),0:INT((M+1)))
  type(active) :: THCK(1:N,1:M)
  REAL(w2f__8) SIGMA(1 : NZ)
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) EW
  INTEGER(w2f__i4) NS
  INTEGER(w2f__i4) UP
  INTEGER(w2f__i4) OpenAD_Symbol_1734
  INTEGER(w2f__i4) OpenAD_Symbol_1735
  INTEGER(w2f__i4) OpenAD_Symbol_1736
  INTEGER(w2f__i4) OpenAD_Symbol_1737
  INTEGER(w2f__i4) OpenAD_Symbol_1738
  INTEGER(w2f__i4) OpenAD_Symbol_1739
  INTEGER(w2f__i4) OpenAD_Symbol_1740
  INTEGER(w2f__i4) OpenAD_Symbol_1741
  INTEGER(w2f__i4) OpenAD_Symbol_1742
  INTEGER(w2f__i4) OpenAD_Symbol_1743
  REAL(w2f__8) OpenAD_Symbol_1744
  INTEGER(w2f__i4) OpenAD_Symbol_1745
  INTEGER(w2f__i4) OpenAD_Symbol_1746
  INTEGER(w2f__i4) OpenAD_Symbol_1747
  INTEGER(w2f__i4) OpenAD_Symbol_980
  INTEGER(w2f__i4) OpenAD_Symbol_981
  INTEGER(w2f__i4) OpenAD_Symbol_982
  INTEGER(w2f__i4) OpenAD_Symbol_983
  INTEGER(w2f__i4) OpenAD_Symbol_984
  INTEGER(w2f__i4) OpenAD_Symbol_985
  REAL(w2f__8) OpenAD_acc_86
  REAL(w2f__8) OpenAD_aux_84
  REAL(w2f__8) OpenAD_lin_163
!
!       **** Statements ****
!


    integer iaddr
    external iaddr

   if (our_rev_mode%plain) then
! original function
  TSTAR(1:INT(NZ),0,0:M+1)%v = 0.0
  TSTAR(1:INT(NZ),N+1,0:M+1)%v = 0.0
  TSTAR(1:INT(NZ),0:N+1,0)%v = 0.0
  TSTAR(1:INT(NZ),0:N+1,M+1)%v = 0.0
  DO UP = 1,NZ,1
    DO EW = 1,N,1
      DO NS = 1,M,1
        TSTAR(INT(UP),INT(EW),INT(NS))%v = (TEMP(UP,EW,NS)%v+SIGMA(UP)*THCK(EW,N&
     &S)%v*8.69999457599999982335D-04)

        IF (TSTAR(UP,EW,NS)%v.GT.0.0D00) THEN
          TSTAR(INT(UP),INT(EW),INT(NS))%v = 0.0
        ENDIF
      END DO
    END DO
  END DO
    end if
    if (our_rev_mode%tape) then
! taping
  TSTAR(1:INT(NZ),0,0:M+1)%v = 0.0
  TSTAR(1:INT(NZ),N+1,0:M+1)%v = 0.0
  TSTAR(1:INT(NZ),0:N+1,0)%v = 0.0
  TSTAR(1:INT(NZ),0:N+1,M+1)%v = 0.0
  OpenAD_Symbol_980 = (M+1)
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_980
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_981 = (N+1)
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_981
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_982 = (M+1)
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_982
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_983 = (N+1)
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_983
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_984 = (N+1)
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_984
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_985 = (M+1)
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_985
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = NZ
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_963 = 0_w2f__i8
  DO UP = 1,NZ,1
    OpenAD_Symbol_964 = 0_w2f__i8
    DO EW = 1,N,1
      OpenAD_Symbol_965 = 0_w2f__i8
      DO NS = 1,M,1
        OpenAD_aux_84 = (THCK(EW,NS)%v*8.69999457599999982335D-04)
        OpenAD_lin_163 = SIGMA(UP)
        TSTAR(INT(UP),INT(EW),INT(NS))%v = (TEMP(UP,EW,NS)%v+SIGMA(UP)*OpenAD_au&
     &x_84)

        OpenAD_acc_86 = (8.69999457599999982335D-04*OpenAD_lin_163)
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_acc_86
        oad_dt_ptr = oad_dt_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = UP
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        IF (TSTAR(UP,EW,NS)%v.GT.0.0D00) THEN
          TSTAR(INT(UP),INT(EW),INT(NS))%v = 0.0
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = UP
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = EW
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = NS
          oad_it_ptr = oad_it_ptr+1
          OpenAD_Symbol_966 = 1_w2f__i8
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_966
          oad_it_ptr = oad_it_ptr+1
        ELSE
          OpenAD_Symbol_967 = 0_w2f__i8
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_967
          oad_it_ptr = oad_it_ptr+1
        ENDIF
        OpenAD_Symbol_965 = (INT(OpenAD_Symbol_965)+INT(1_w2f__i8))
      END DO
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = OpenAD_Symbol_965
      oad_it_ptr = oad_it_ptr+1
      OpenAD_Symbol_964 = (INT(OpenAD_Symbol_964)+INT(1_w2f__i8))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_964
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_963 = (INT(OpenAD_Symbol_963)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_963
  oad_it_ptr = oad_it_ptr+1
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_956 = oad_it(oad_it_ptr)
  OpenAD_Symbol_957 = 1
  do while (INT(OpenAD_Symbol_957).LE.INT(OpenAD_Symbol_956))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_958 = oad_it(oad_it_ptr)
    OpenAD_Symbol_959 = 1
    do while (INT(OpenAD_Symbol_959).LE.INT(OpenAD_Symbol_958))
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_960 = oad_it(oad_it_ptr)
      OpenAD_Symbol_961 = 1
      do while (INT(OpenAD_Symbol_961).LE.INT(OpenAD_Symbol_960))
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_962 = oad_it(oad_it_ptr)
        IF (OpenAD_Symbol_962.ne.0) THEN
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1745 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1746 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1747 = oad_it(oad_it_ptr)
          TSTAR(OpenAD_Symbol_1747,OpenAD_Symbol_1746,OpenAD_Symbol_1745)%d = 0.&
     &0d0

        ENDIF
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1741 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1742 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1743 = oad_it(oad_it_ptr)
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1744 = oad_dt(oad_dt_ptr)
        THCK(OpenAD_Symbol_1742,OpenAD_Symbol_1741)%d = THCK(OpenAD_Symbol_1742,&
     &OpenAD_Symbol_1741)%d+TSTAR(OpenAD_Symbol_1743,OpenAD_Symbol_1742,OpenAD_S&
     &ymbol_1741)%d*(OpenAD_Symbol_1744)

        TEMP(OpenAD_Symbol_1743,OpenAD_Symbol_1742,OpenAD_Symbol_1741)%d = TEMP(&
     &OpenAD_Symbol_1743,OpenAD_Symbol_1742,OpenAD_Symbol_1741)%d+TSTAR(OpenAD_S&
     &ymbol_1743,OpenAD_Symbol_1742,OpenAD_Symbol_1741)%d

        TSTAR(OpenAD_Symbol_1743,OpenAD_Symbol_1742,OpenAD_Symbol_1741)%d = 0.0d&
     &0

        OpenAD_Symbol_961 = INT(OpenAD_Symbol_961)+1
      END DO
      OpenAD_Symbol_959 = INT(OpenAD_Symbol_959)+1
    END DO
    OpenAD_Symbol_957 = INT(OpenAD_Symbol_957)+1
  END DO
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1734 = oad_it(oad_it_ptr)
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1735 = oad_it(oad_it_ptr)
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1736 = oad_it(oad_it_ptr)
  TSTAR(1:OpenAD_Symbol_1734,0:OpenAD_Symbol_1736,OpenAD_Symbol_1735)%d = 0.0d0
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1737 = oad_it(oad_it_ptr)
  TSTAR(1:OpenAD_Symbol_1734,0:OpenAD_Symbol_1737,0)%d = 0.0d0
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1738 = oad_it(oad_it_ptr)
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1739 = oad_it(oad_it_ptr)
  TSTAR(1:OpenAD_Symbol_1734,OpenAD_Symbol_1739,0:OpenAD_Symbol_1738)%d = 0.0d0
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1740 = oad_it(oad_it_ptr)
  TSTAR(1:OpenAD_Symbol_1734,0,0:OpenAD_Symbol_1740)%d = 0.0d0
    end if
  end subroutine CALCTSTAR
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE CALCDIFF(N, M, DIFF, USRF, THCK, GAINTEGRAL, BSLIDE, SLIDE_MASK, DX&
     &, DY)

    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  use physcon ,only: GRAV , RHOI
  use physcon ,only: GRAV , RHOI
  use physcon ,only: GRAV , RHOI
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_1000
  INTEGER(w2f__i8) OpenAD_Symbol_1001
  INTEGER(w2f__i8) OpenAD_Symbol_1002
  INTEGER(w2f__i8) OpenAD_Symbol_1003
  INTEGER(w2f__i8) OpenAD_Symbol_1004
  INTEGER(w2f__i8) OpenAD_Symbol_1005
  INTEGER(w2f__i8) OpenAD_Symbol_1006
  INTEGER(w2f__i8) OpenAD_Symbol_1007
  INTEGER(w2f__i8) OpenAD_Symbol_1008
  INTEGER(w2f__i8) OpenAD_Symbol_1009
  INTEGER(w2f__i8) OpenAD_Symbol_986
  INTEGER(w2f__i8) OpenAD_Symbol_987
  INTEGER(w2f__i8) OpenAD_Symbol_988
  INTEGER(w2f__i8) OpenAD_Symbol_989
  INTEGER(w2f__i8) OpenAD_Symbol_990
  INTEGER(w2f__i8) OpenAD_Symbol_991
  INTEGER(w2f__i8) OpenAD_Symbol_992
  INTEGER(w2f__i8) OpenAD_Symbol_993
  INTEGER(w2f__i8) OpenAD_Symbol_994
  INTEGER(w2f__i8) OpenAD_Symbol_995
  INTEGER(w2f__i8) OpenAD_Symbol_996
  INTEGER(w2f__i8) OpenAD_Symbol_997
  INTEGER(w2f__i8) OpenAD_Symbol_998
  INTEGER(w2f__i8) OpenAD_Symbol_999
!
!       **** Parameters and Result ****
!
  INTEGER(w2f__i4) N
  INTEGER(w2f__i4) M
  type(active) :: DIFF(1:N,1:M)
  type(active) :: USRF(1:N,1:M)
  type(active) :: THCK(1:N,1:M)
  type(active) :: GAINTEGRAL(1:N,1:M)
  REAL(w2f__8) BSLIDE(1 : N, 1 : M)
  LOGICAL(w2f__i4) SLIDE_MASK(1 : N, 1 : M)
  REAL(w2f__8) DX
  REAL(w2f__8) DY
!
!       **** Local Variables and Functions ****
!
  type(active) :: DSDX
  type(active) :: DSDY
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  INTEGER(w2f__i4) OpenAD_Symbol_1010
  INTEGER(w2f__i4) OpenAD_Symbol_1011
  INTEGER(w2f__i4) OpenAD_Symbol_1012
  INTEGER(w2f__i4) OpenAD_Symbol_1013
  INTEGER(w2f__i4) OpenAD_Symbol_1748
  INTEGER(w2f__i4) OpenAD_Symbol_1749
  INTEGER(w2f__i4) OpenAD_Symbol_1750
  INTEGER(w2f__i4) OpenAD_Symbol_1751
  REAL(w2f__8) OpenAD_Symbol_1752
  REAL(w2f__8) OpenAD_Symbol_1753
  INTEGER(w2f__i4) OpenAD_Symbol_1754
  INTEGER(w2f__i4) OpenAD_Symbol_1755
  INTEGER(w2f__i4) OpenAD_Symbol_1756
  INTEGER(w2f__i4) OpenAD_Symbol_1757
  INTEGER(w2f__i4) OpenAD_Symbol_1758
  INTEGER(w2f__i4) OpenAD_Symbol_1759
  REAL(w2f__8) OpenAD_Symbol_1760
  REAL(w2f__8) OpenAD_Symbol_1761
  REAL(w2f__8) OpenAD_Symbol_1762
  REAL(w2f__8) OpenAD_Symbol_1763
  REAL(w2f__8) OpenAD_Symbol_1764
  INTEGER(w2f__i4) OpenAD_Symbol_1765
  INTEGER(w2f__i4) OpenAD_Symbol_1766
  REAL(w2f__8) OpenAD_Symbol_1767
  REAL(w2f__8) OpenAD_Symbol_1768
  REAL(w2f__8) OpenAD_Symbol_1769
  REAL(w2f__8) OpenAD_Symbol_1770
  INTEGER(w2f__i4) OpenAD_Symbol_1771
  INTEGER(w2f__i4) OpenAD_Symbol_1772
  REAL(w2f__8) OpenAD_acc_87
  REAL(w2f__8) OpenAD_acc_88
  REAL(w2f__8) OpenAD_acc_89
  REAL(w2f__8) OpenAD_acc_90
  REAL(w2f__8) OpenAD_acc_91
  REAL(w2f__8) OpenAD_acc_92
  REAL(w2f__8) OpenAD_acc_93
  REAL(w2f__8) OpenAD_acc_94
  REAL(w2f__8) OpenAD_acc_95
  REAL(w2f__8) OpenAD_acc_96
  REAL(w2f__8) OpenAD_acc_97
  REAL(w2f__8) OpenAD_aux_85
  REAL(w2f__8) OpenAD_aux_86
  REAL(w2f__8) OpenAD_aux_87
  REAL(w2f__8) OpenAD_aux_88
  REAL(w2f__8) OpenAD_aux_89
  REAL(w2f__8) OpenAD_aux_90
  REAL(w2f__8) OpenAD_aux_91
  REAL(w2f__8) OpenAD_aux_92
  REAL(w2f__8) OpenAD_aux_93
  REAL(w2f__8) OpenAD_aux_94
  REAL(w2f__8) OpenAD_aux_95
  REAL(w2f__8) OpenAD_aux_96
  REAL(w2f__8) OpenAD_aux_97
  REAL(w2f__8) OpenAD_aux_98
  REAL(w2f__8) OpenAD_lin_164
  REAL(w2f__8) OpenAD_lin_165
  REAL(w2f__8) OpenAD_lin_166
  REAL(w2f__8) OpenAD_lin_167
  REAL(w2f__8) OpenAD_lin_168
  REAL(w2f__8) OpenAD_lin_169
  REAL(w2f__8) OpenAD_lin_170
  REAL(w2f__8) OpenAD_lin_171
  REAL(w2f__8) OpenAD_lin_172
  REAL(w2f__8) OpenAD_lin_173
  REAL(w2f__8) OpenAD_lin_174
  REAL(w2f__8) OpenAD_lin_175
  REAL(w2f__8) OpenAD_lin_176
  REAL(w2f__8) OpenAD_lin_177
  REAL(w2f__8) OpenAD_lin_178
  REAL(w2f__8) OpenAD_lin_179
  REAL(w2f__8) OpenAD_lin_180
  REAL(w2f__8) OpenAD_lin_181
  REAL(w2f__8) OpenAD_lin_182
  REAL(w2f__8) OpenAD_lin_183
  type(active) :: OpenAD_prp_64
  type(active) :: OpenAD_prp_65
  type(active) :: OpenAD_prp_66
  type(active) :: OpenAD_prp_67
  type(active) :: OpenAD_prp_68
!
!       **** Statements ****
!


    integer iaddr
    external iaddr

   if (our_rev_mode%plain) then
! original function
  DIFF(1,1:INT(M))%v = 0.0D00
  DIFF(INT(N),1:INT(M))%v = 0.0D00
  DIFF(1:INT(N),1)%v = 0.0D00
  DIFF(1:INT(N),INT(M))%v = 0.0D00
  DO I = 2,(N+(-1)),1
    DO J = 2,(M+(-1)),1
      IF (THCK(I,J)%v.GT.0.0D00) THEN
        DSDX%v = ((USRF(I+1,J)%v-USRF(I+(-1),J)%v)/(DX*2.0D00))
        DSDY%v = ((USRF(I,J+1)%v-USRF(I,J+(-1))%v)/(DY*2.0D00))
        IF (SLIDE_MASK(I,J)) THEN
          DIFF(INT(I),INT(J))%v = ((THCK(I,J)%v**2.0D00)*BSLIDE(I,J)*9.1D+02*9.8&
     &1000000000000049738D00+(THCK(I,J)%v**5)*GAINTEGRAL(I,J)%v*(((DSDX%v**2)+(D&
     &SDY%v**2))**1.0D00))

        ELSE
          DIFF(INT(I),INT(J))%v = ((THCK(I,J)%v**5)*GAINTEGRAL(I,J)%v*(((DSDX%v*&
     &*2)+(DSDY%v**2))**1.0D00))

        ENDIF
      ELSE
        DIFF(INT(I),INT(J))%v = 0.0
      ENDIF
    END DO
  END DO
    end if
    if (our_rev_mode%tape) then
! taping
  DIFF(1,1:INT(M))%v = 0.0D00
  DIFF(INT(N),1:INT(M))%v = 0.0D00
  DIFF(1:INT(N),1)%v = 0.0D00
  DIFF(1:INT(N),INT(M))%v = 0.0D00
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = M
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = N
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_992 = 0_w2f__i8
  DO I = 2,(N+(-1)),1
    OpenAD_Symbol_993 = 0_w2f__i8
    DO J = 2,(M+(-1)),1
      IF (THCK(I,J)%v.GT.0.0D00) THEN
        OpenAD_aux_85 = (USRF(I+1,J)%v-USRF(I+(-1),J)%v)
        OpenAD_aux_86 = (DX*2.0D00)
        OpenAD_lin_164 = (INT(1_w2f__i8)/OpenAD_aux_86)
        DSDX%v = (OpenAD_aux_85/OpenAD_aux_86)
        OpenAD_aux_87 = (USRF(I,J+1)%v-USRF(I,J+(-1))%v)
        OpenAD_aux_88 = (DY*2.0D00)
        OpenAD_lin_165 = (INT(1_w2f__i8)/OpenAD_aux_88)
        DSDY%v = (OpenAD_aux_87/OpenAD_aux_88)
        OpenAD_Symbol_1010 = (I+1)
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_1010
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_1011 = (I+(-1))
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_1011
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_1012 = (J+1)
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_1012
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_1013 = (J+(-1))
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_1013
        oad_it_ptr = oad_it_ptr+1
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_lin_164
        oad_dt_ptr = oad_dt_ptr+1
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_lin_165
        oad_dt_ptr = oad_dt_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = J
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = I
        oad_it_ptr = oad_it_ptr+1
        IF (SLIDE_MASK(I,J)) THEN
          OpenAD_aux_89 = (THCK(I,J)%v**2.0D00)
          OpenAD_aux_90 = (BSLIDE(I,J)*9.1D+02*9.81000000000000049738D00)
          OpenAD_aux_91 = (THCK(I,J)%v**5)
          OpenAD_aux_94 = ((DSDX%v**2)+(DSDY%v**2))
          OpenAD_aux_93 = (OpenAD_aux_94**1.0D00)
          OpenAD_aux_92 = (GAINTEGRAL(I,J)%v*OpenAD_aux_93)
          OpenAD_lin_167 = (2.0D00*(THCK(I,J)%v**(2.0D00-INT(1_w2f__i8))))
          OpenAD_lin_166 = OpenAD_aux_90
          OpenAD_lin_170 = (5*(THCK(I,J)%v**(5-INT(1_w2f__i8))))
          OpenAD_lin_168 = OpenAD_aux_92
          OpenAD_lin_171 = OpenAD_aux_93
          OpenAD_lin_174 = (2*(DSDX%v**(2-INT(1_w2f__i8))))
          OpenAD_lin_175 = (2*(DSDY%v**(2-INT(1_w2f__i8))))
          OpenAD_lin_173 = (1.0D00*(OpenAD_aux_94**(1.0D00-INT(1_w2f__i8))))
          OpenAD_lin_172 = GAINTEGRAL(I,J)%v
          OpenAD_lin_169 = OpenAD_aux_91
          DIFF(INT(I),INT(J))%v = (OpenAD_aux_89*OpenAD_aux_90+OpenAD_aux_91*Ope&
     &nAD_aux_92)

          OpenAD_acc_87 = (OpenAD_lin_173*OpenAD_lin_172*OpenAD_lin_169)
          OpenAD_acc_88 = (OpenAD_lin_171*OpenAD_lin_169)
          OpenAD_acc_89 = (OpenAD_lin_170*OpenAD_lin_168)
          OpenAD_acc_90 = (OpenAD_lin_175*OpenAD_acc_87)
          OpenAD_acc_91 = (OpenAD_lin_174*OpenAD_acc_87)
          OpenAD_acc_92 = (OpenAD_lin_167*OpenAD_lin_166)
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_acc_88
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_acc_89
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_acc_90
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_acc_91
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_acc_92
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = I
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = J
          oad_it_ptr = oad_it_ptr+1
          OpenAD_Symbol_994 = 1_w2f__i8
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_994
          oad_it_ptr = oad_it_ptr+1
        ELSE
          OpenAD_aux_95 = (THCK(I,J)%v**5)
          OpenAD_aux_98 = ((DSDX%v**2)+(DSDY%v**2))
          OpenAD_aux_97 = (OpenAD_aux_98**1.0D00)
          OpenAD_aux_96 = (GAINTEGRAL(I,J)%v*OpenAD_aux_97)
          OpenAD_lin_178 = (5*(THCK(I,J)%v**(5-INT(1_w2f__i8))))
          OpenAD_lin_176 = OpenAD_aux_96
          OpenAD_lin_179 = OpenAD_aux_97
          OpenAD_lin_182 = (2*(DSDX%v**(2-INT(1_w2f__i8))))
          OpenAD_lin_183 = (2*(DSDY%v**(2-INT(1_w2f__i8))))
          OpenAD_lin_181 = (1.0D00*(OpenAD_aux_98**(1.0D00-INT(1_w2f__i8))))
          OpenAD_lin_180 = GAINTEGRAL(I,J)%v
          OpenAD_lin_177 = OpenAD_aux_95
          DIFF(INT(I),INT(J))%v = (OpenAD_aux_95*OpenAD_aux_96)
          OpenAD_acc_93 = (OpenAD_lin_178*OpenAD_lin_176)
          OpenAD_acc_94 = (OpenAD_lin_179*OpenAD_lin_177)
          OpenAD_acc_95 = (OpenAD_lin_181*OpenAD_lin_180*OpenAD_lin_177)
          OpenAD_acc_96 = (OpenAD_lin_183*OpenAD_acc_95)
          OpenAD_acc_97 = (OpenAD_lin_182*OpenAD_acc_95)
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_acc_93
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_acc_94
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_acc_96
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_acc_97
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = I
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = J
          oad_it_ptr = oad_it_ptr+1
          OpenAD_Symbol_995 = 0_w2f__i8
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_995
          oad_it_ptr = oad_it_ptr+1
        ENDIF
        OpenAD_Symbol_997 = 1_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_997
        oad_it_ptr = oad_it_ptr+1
      ELSE
        DIFF(INT(I),INT(J))%v = 0.0
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = I
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = J
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_996 = 0_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_996
        oad_it_ptr = oad_it_ptr+1
      ENDIF
      OpenAD_Symbol_993 = (INT(OpenAD_Symbol_993)+INT(1_w2f__i8))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_993
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_992 = (INT(OpenAD_Symbol_992)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_992
  oad_it_ptr = oad_it_ptr+1
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_986 = oad_it(oad_it_ptr)
  OpenAD_Symbol_987 = 1
  do while (INT(OpenAD_Symbol_987).LE.INT(OpenAD_Symbol_986))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_988 = oad_it(oad_it_ptr)
    OpenAD_Symbol_989 = 1
    do while (INT(OpenAD_Symbol_989).LE.INT(OpenAD_Symbol_988))
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_990 = oad_it(oad_it_ptr)
      IF (OpenAD_Symbol_990.ne.0) THEN
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_991 = oad_it(oad_it_ptr)
        IF (OpenAD_Symbol_991.ne.0) THEN
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1758 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1759 = oad_it(oad_it_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1760 = oad_dt(oad_dt_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1761 = oad_dt(oad_dt_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1762 = oad_dt(oad_dt_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1763 = oad_dt(oad_dt_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1764 = oad_dt(oad_dt_ptr)
          OpenAD_prp_66%d = OpenAD_prp_66%d+DIFF(OpenAD_Symbol_1759,OpenAD_Symbo&
     &l_1758)%d*(OpenAD_Symbol_1760)

          DSDX%d = DSDX%d+DIFF(OpenAD_Symbol_1759,OpenAD_Symbol_1758)%d*(OpenAD_&
     &Symbol_1761)

          DSDY%d = DSDY%d+DIFF(OpenAD_Symbol_1759,OpenAD_Symbol_1758)%d*(OpenAD_&
     &Symbol_1762)

          OpenAD_prp_67%d = OpenAD_prp_67%d+DIFF(OpenAD_Symbol_1759,OpenAD_Symbo&
     &l_1758)%d*(OpenAD_Symbol_1763)

          GAINTEGRAL(OpenAD_Symbol_1759,OpenAD_Symbol_1758)%d = GAINTEGRAL(OpenA&
     &D_Symbol_1759,OpenAD_Symbol_1758)%d+DIFF(OpenAD_Symbol_1759,OpenAD_Symbol_&
     &1758)%d*(OpenAD_Symbol_1764)

          DIFF(OpenAD_Symbol_1759,OpenAD_Symbol_1758)%d = 0.0d0
          THCK(OpenAD_Symbol_1759,OpenAD_Symbol_1758)%d = THCK(OpenAD_Symbol_175&
     &9,OpenAD_Symbol_1758)%d+OpenAD_prp_67%d

          OpenAD_prp_67%d = 0.0d0
          THCK(OpenAD_Symbol_1759,OpenAD_Symbol_1758)%d = THCK(OpenAD_Symbol_175&
     &9,OpenAD_Symbol_1758)%d+OpenAD_prp_66%d

          OpenAD_prp_66%d = 0.0d0
        ELSE
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1765 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1766 = oad_it(oad_it_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1767 = oad_dt(oad_dt_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1768 = oad_dt(oad_dt_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1769 = oad_dt(oad_dt_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1770 = oad_dt(oad_dt_ptr)
          DSDX%d = DSDX%d+DIFF(OpenAD_Symbol_1766,OpenAD_Symbol_1765)%d*(OpenAD_&
     &Symbol_1767)

          DSDY%d = DSDY%d+DIFF(OpenAD_Symbol_1766,OpenAD_Symbol_1765)%d*(OpenAD_&
     &Symbol_1768)

          GAINTEGRAL(OpenAD_Symbol_1766,OpenAD_Symbol_1765)%d = GAINTEGRAL(OpenA&
     &D_Symbol_1766,OpenAD_Symbol_1765)%d+DIFF(OpenAD_Symbol_1766,OpenAD_Symbol_&
     &1765)%d*(OpenAD_Symbol_1769)

          OpenAD_prp_68%d = OpenAD_prp_68%d+DIFF(OpenAD_Symbol_1766,OpenAD_Symbo&
     &l_1765)%d*(OpenAD_Symbol_1770)

          DIFF(OpenAD_Symbol_1766,OpenAD_Symbol_1765)%d = 0.0d0
          THCK(OpenAD_Symbol_1766,OpenAD_Symbol_1765)%d = THCK(OpenAD_Symbol_176&
     &6,OpenAD_Symbol_1765)%d+OpenAD_prp_68%d

          OpenAD_prp_68%d = 0.0d0
        ENDIF
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1750 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1751 = oad_it(oad_it_ptr)
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1752 = oad_dt(oad_dt_ptr)
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1753 = oad_dt(oad_dt_ptr)
        OpenAD_prp_65%d = OpenAD_prp_65%d+DSDY%d*(OpenAD_Symbol_1752)
        DSDY%d = 0.0d0
        OpenAD_prp_64%d = OpenAD_prp_64%d+DSDX%d*(OpenAD_Symbol_1753)
        DSDX%d = 0.0d0
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1754 = oad_it(oad_it_ptr)
        USRF(OpenAD_Symbol_1750,OpenAD_Symbol_1754)%d = USRF(OpenAD_Symbol_1750,&
     &OpenAD_Symbol_1754)%d-OpenAD_prp_65%d

        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1755 = oad_it(oad_it_ptr)
        USRF(OpenAD_Symbol_1750,OpenAD_Symbol_1755)%d = USRF(OpenAD_Symbol_1750,&
     &OpenAD_Symbol_1755)%d+OpenAD_prp_65%d

        OpenAD_prp_65%d = 0.0d0
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1756 = oad_it(oad_it_ptr)
        USRF(OpenAD_Symbol_1756,OpenAD_Symbol_1751)%d = USRF(OpenAD_Symbol_1756,&
     &OpenAD_Symbol_1751)%d-OpenAD_prp_64%d

        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1757 = oad_it(oad_it_ptr)
        USRF(OpenAD_Symbol_1757,OpenAD_Symbol_1751)%d = USRF(OpenAD_Symbol_1757,&
     &OpenAD_Symbol_1751)%d+OpenAD_prp_64%d

        OpenAD_prp_64%d = 0.0d0
      ELSE
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1771 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1772 = oad_it(oad_it_ptr)
        DIFF(OpenAD_Symbol_1772,OpenAD_Symbol_1771)%d = 0.0d0
      ENDIF
      OpenAD_Symbol_989 = INT(OpenAD_Symbol_989)+1
    END DO
    OpenAD_Symbol_987 = INT(OpenAD_Symbol_987)+1
  END DO
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1748 = oad_it(oad_it_ptr)
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1749 = oad_it(oad_it_ptr)
  DIFF(1:OpenAD_Symbol_1748,OpenAD_Symbol_1749)%d = 0.0d0
  DIFF(1:OpenAD_Symbol_1748,1)%d = 0.0d0
  DIFF(OpenAD_Symbol_1748,1:OpenAD_Symbol_1749)%d = 0.0d0
  DIFF(1,1:OpenAD_Symbol_1749)%d = 0.0d0
    end if
  end subroutine CALCDIFF
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE CALC_GAINTEGRAL(NX, NY, NZ, SIGMA, DEPTH, DINTFLWA, THCK, FLWA)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  use glimmer_utils ,only: OAD_S_VERTINTG , VERTINTG
  use glimmer_global ,only: DP
  use physcon ,only: GRAV , RHOI
  use glimmer_utils ,only: OAD_S_VERTINTG , VERTINTG
  use glimmer_global ,only: DP
  use physcon ,only: GRAV , RHOI
  use glimmer_utils ,only: OAD_S_VERTINTG , VERTINTG
  use glimmer_global ,only: DP
  use physcon ,only: GRAV , RHOI
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_1014
  INTEGER(w2f__i8) OpenAD_Symbol_1015
  INTEGER(w2f__i8) OpenAD_Symbol_1016
  INTEGER(w2f__i8) OpenAD_Symbol_1017
  INTEGER(w2f__i8) OpenAD_Symbol_1018
  INTEGER(w2f__i8) OpenAD_Symbol_1019
  INTEGER(w2f__i8) OpenAD_Symbol_1020
  INTEGER(w2f__i8) OpenAD_Symbol_1021
  INTEGER(w2f__i8) OpenAD_Symbol_1022
  INTEGER(w2f__i8) OpenAD_Symbol_1023
  INTEGER(w2f__i8) OpenAD_Symbol_1024
  INTEGER(w2f__i8) OpenAD_Symbol_1025
  INTEGER(w2f__i8) OpenAD_Symbol_1026
  INTEGER(w2f__i8) OpenAD_Symbol_1027
  INTEGER(w2f__i8) OpenAD_Symbol_1028
  INTEGER(w2f__i8) OpenAD_Symbol_1029
  INTEGER(w2f__i8) OpenAD_Symbol_1030
  INTEGER(w2f__i8) OpenAD_Symbol_1031
  INTEGER(w2f__i8) OpenAD_Symbol_1032
  INTEGER(w2f__i8) OpenAD_Symbol_1033
  INTEGER(w2f__i8) OpenAD_Symbol_1034
  INTEGER(w2f__i8) OpenAD_Symbol_1035
  INTEGER(w2f__i8) OpenAD_Symbol_1036
  INTEGER(w2f__i8) OpenAD_Symbol_1037
  INTEGER(w2f__i8) OpenAD_Symbol_1038
  INTEGER(w2f__i8) OpenAD_Symbol_1039
  INTEGER(w2f__i8) OpenAD_Symbol_1040
  INTEGER(w2f__i8) OpenAD_Symbol_1041
  INTEGER(w2f__i8) OpenAD_Symbol_1042
  INTEGER(w2f__i8) OpenAD_Symbol_1043
!
!       **** Parameters and Result ****
!
  INTEGER(w2f__i4) NX
  INTEGER(w2f__i4) NY
  INTEGER(w2f__i4) NZ
  REAL(w2f__8) SIGMA(1 : NZ)
  REAL(w2f__8) DEPTH(1 : NZ)
  type(active) :: DINTFLWA(1:NX,1:NY)
  REAL(w2f__8) THCK(1 : NX, 1 : NY)
  type(active) :: FLWA(1:NZ,1:NX,1:NY)
!
!       **** Local Variables and Functions ****
!
  REAL(w2f__8) C
  PARAMETER ( C = 7.11428401649511108398D+11)
  REAL(w2f__8) DUPS(1 : NZ)
  INTEGER(w2f__i4) EW
  type(active) :: INTFLWA(1:NZ)
  INTEGER(w2f__i4) NS
  type(active) :: OAD_CTMP0
  INTEGER(w2f__i4) UP
  INTEGER(w2f__i4) OpenAD_Symbol_1044
  INTEGER(w2f__i4) OpenAD_Symbol_1045
  INTEGER(w2f__i4) OpenAD_Symbol_1773
  INTEGER(w2f__i4) OpenAD_Symbol_1774
  INTEGER(w2f__i4) OpenAD_Symbol_1775
  INTEGER(w2f__i4) OpenAD_Symbol_1776
  REAL(w2f__8) OpenAD_Symbol_1777
  INTEGER(w2f__i4) OpenAD_Symbol_1778
  INTEGER(w2f__i4) OpenAD_Symbol_1779
  INTEGER(w2f__i4) OpenAD_Symbol_1780
  INTEGER(w2f__i4) OpenAD_Symbol_1781
  INTEGER(w2f__i4) OpenAD_Symbol_1782
  INTEGER(w2f__i4) OpenAD_Symbol_1783
  REAL(w2f__8) OpenAD_aux_99
  REAL(w2f__8) OpenAD_lin_184
  type(active) :: OpenAD_prp_69
  type(active) :: OpenAD_prp_70


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  DO UP = 1, (NZ +(-1)), 1
    DUPS(INT(UP)) = (SIGMA(UP + 1) - SIGMA(UP))
  END DO
  DUPS(INT(NZ)) = 0.0D00
  DO NS = 1, NY, 1
    DO EW = 1, NX, 1
      IF(THCK(EW, NS) .ne. 0.0D00) THEN
        INTFLWA(INT(NZ))%v = 0.0D00
        DO UP = NZ+(-1),1,(-1)
          INTFLWA(INT(UP))%v = (INTFLWA(UP+1)%v+DEPTH(UP)*(FLWA(UP,EW,NS)%v+FLWA&
     &(UP+1,EW,NS)%v))

        END DO
        CALL OAD_S_VERTINTG(DUPS,INTFLWA,OAD_CTMP0)
        DINTFLWA(INT(EW),INT(NS))%v = (OAD_CTMP0%v*7.11428401649511108398D+11)
      ELSE
        DINTFLWA(INT(EW),INT(NS))%v = 0.0D00
      ENDIF
    END DO
  END DO
    end if
    if (our_rev_mode%tape) then
! taping
  OpenAD_Symbol_1023 = 0_w2f__i8
  DO UP = 1,(NZ+(-1)),1
    DUPS(INT(UP)) = (SIGMA(UP+1)-SIGMA(UP))
    OpenAD_Symbol_1023 = (INT(OpenAD_Symbol_1023)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1023
  oad_it_ptr = oad_it_ptr+1
  DUPS(INT(NZ)) = 0.0D00
  OpenAD_Symbol_1024 = 0_w2f__i8
  DO NS = 1,NY,1
    OpenAD_Symbol_1025 = 0_w2f__i8
    DO EW = 1,NX,1
      IF (THCK(EW,NS).ne.0.0D00) THEN
        INTFLWA(INT(NZ))%v = 0.0D00
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NZ
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_1026 = 0_w2f__i8
        DO UP = NZ+(-1),1,(-1)
          OpenAD_aux_99 = (FLWA(UP,EW,NS)%v+FLWA(UP+1,EW,NS)%v)
          OpenAD_lin_184 = DEPTH(UP)
          INTFLWA(INT(UP))%v = (INTFLWA(UP+1)%v+DEPTH(UP)*OpenAD_aux_99)
          OpenAD_Symbol_1044 = (UP+1)
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_1044
          oad_it_ptr = oad_it_ptr+1
          OpenAD_Symbol_1045 = (UP+1)
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_1045
          oad_it_ptr = oad_it_ptr+1
          if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
          oad_dt(oad_dt_ptr) = OpenAD_lin_184
          oad_dt_ptr = oad_dt_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = UP
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = EW
          oad_it_ptr = oad_it_ptr+1
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = NS
          oad_it_ptr = oad_it_ptr+1
          OpenAD_Symbol_1026 = (INT(OpenAD_Symbol_1026)+INT(1_w2f__i8))
        END DO
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_1026
        oad_it_ptr = oad_it_ptr+1
        CALL OAD_S_VERTINTG(DUPS,INTFLWA,OAD_CTMP0)
        oad_chunk_size = size(DUPS,1)
        if (oad_dt_sz.lt.oad_dt_ptr+oad_chunk_size) call oad_dt_grow()
        oad_dt(oad_dt_ptr:oad_dt_ptr+oad_chunk_size-1) = DUPS
        oad_dt_ptr = oad_dt_ptr+oad_chunk_size
        DINTFLWA(INT(EW),INT(NS))%v = (OAD_CTMP0%v*7.11428401649511108398D+11)
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_1027 = 1_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_1027
        oad_it_ptr = oad_it_ptr+1
      ELSE
        DINTFLWA(INT(EW),INT(NS))%v = 0.0D00
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = EW
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NS
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_1028 = 0_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_1028
        oad_it_ptr = oad_it_ptr+1
      ENDIF
      OpenAD_Symbol_1025 = (INT(OpenAD_Symbol_1025)+INT(1_w2f__i8))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_1025
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_1024 = (INT(OpenAD_Symbol_1024)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1024
  oad_it_ptr = oad_it_ptr+1
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1014 = oad_it(oad_it_ptr)
  OpenAD_Symbol_1015 = 1
  do while (INT(OpenAD_Symbol_1015).LE.INT(OpenAD_Symbol_1014))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1016 = oad_it(oad_it_ptr)
    OpenAD_Symbol_1017 = 1
    do while (INT(OpenAD_Symbol_1017).LE.INT(OpenAD_Symbol_1016))
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1018 = oad_it(oad_it_ptr)
      IF (OpenAD_Symbol_1018.ne.0) THEN
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1780 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1781 = oad_it(oad_it_ptr)
        OAD_CTMP0%d = OAD_CTMP0%d+DINTFLWA(OpenAD_Symbol_1781,OpenAD_Symbol_1780&
     &)%d*(7.11428401649511108398D+11)

        DINTFLWA(OpenAD_Symbol_1781,OpenAD_Symbol_1780)%d = 0.0d0
        oad_chunk_size = size(DUPS,1)
        oad_dt_ptr = oad_dt_ptr-oad_chunk_size
        DUPS = oad_dt(oad_dt_ptr:oad_dt_ptr+oad_chunk_size-1)
        CALL OAD_S_VERTINTG(DUPS,INTFLWA,OAD_CTMP0)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1019 = oad_it(oad_it_ptr)
        OpenAD_Symbol_1020 = 1
        do while (INT(OpenAD_Symbol_1020).LE.INT(OpenAD_Symbol_1019))
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1774 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1775 = oad_it(oad_it_ptr)
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1776 = oad_it(oad_it_ptr)
          oad_dt_ptr = oad_dt_ptr-1
          OpenAD_Symbol_1777 = oad_dt(oad_dt_ptr)
          OpenAD_prp_70%d = OpenAD_prp_70%d+INTFLWA(OpenAD_Symbol_1776)%d*(OpenA&
     &D_Symbol_1777)

          OpenAD_prp_69%d = OpenAD_prp_69%d+INTFLWA(OpenAD_Symbol_1776)%d
          INTFLWA(OpenAD_Symbol_1776)%d = 0.0d0
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1778 = oad_it(oad_it_ptr)
          FLWA(OpenAD_Symbol_1778,OpenAD_Symbol_1775,OpenAD_Symbol_1774)%d = FLW&
     &A(OpenAD_Symbol_1778,OpenAD_Symbol_1775,OpenAD_Symbol_1774)%d+OpenAD_prp_7&
     &0%d

          FLWA(OpenAD_Symbol_1776,OpenAD_Symbol_1775,OpenAD_Symbol_1774)%d = FLW&
     &A(OpenAD_Symbol_1776,OpenAD_Symbol_1775,OpenAD_Symbol_1774)%d+OpenAD_prp_7&
     &0%d

          OpenAD_prp_70%d = 0.0d0
          oad_it_ptr = oad_it_ptr-1
          OpenAD_Symbol_1779 = oad_it(oad_it_ptr)
          INTFLWA(OpenAD_Symbol_1779)%d = INTFLWA(OpenAD_Symbol_1779)%d+OpenAD_p&
     &rp_69%d

          OpenAD_prp_69%d = 0.0d0
          OpenAD_Symbol_1020 = INT(OpenAD_Symbol_1020)+1
        END DO
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1773 = oad_it(oad_it_ptr)
        INTFLWA(OpenAD_Symbol_1773)%d = 0.0d0
      ELSE
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1782 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1783 = oad_it(oad_it_ptr)
        DINTFLWA(OpenAD_Symbol_1783,OpenAD_Symbol_1782)%d = 0.0d0
      ENDIF
      OpenAD_Symbol_1017 = INT(OpenAD_Symbol_1017)+1
    END DO
    OpenAD_Symbol_1015 = INT(OpenAD_Symbol_1015)+1
  END DO
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1021 = oad_it(oad_it_ptr)
  OpenAD_Symbol_1022 = 1
  do while (INT(OpenAD_Symbol_1022).LE.INT(OpenAD_Symbol_1021))
    OpenAD_Symbol_1022 = INT(OpenAD_Symbol_1022)+1
  END DO
    end if
  end subroutine CALC_GAINTEGRAL
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE CALCTHCK(N, M, DIFF, USRF, NMSB, MASK, DX, DY, DT, TOTPTS, NEW_USRF&
     &)

    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  INTEGER(w2f__i4) N
  INTEGER(w2f__i4) M
  type(active) :: DIFF(1:N,1:M)
  type(active) :: USRF(1:N,1:M)
  type(active) :: NMSB(1:N,1:M)
  INTEGER(w2f__i4) MASK(1 : N, 1 : M)
  REAL(w2f__8) DX
  REAL(w2f__8) DY
  REAL(w2f__8) DT
  INTEGER(w2f__i4) TOTPTS
  type(active) :: NEW_USRF(1:N,1:M)
!
!       **** Local Variables and Functions ****
!
  EXTERNAL adism_umf
  type(active) :: B(1:TOTPTS)
  INTEGER(w2f__i4) MATCOL(1 : INT((TOTPTS * 5)))
  INTEGER(w2f__i4) MATCOUNT
  INTEGER(w2f__i4) MATROW(1 : INT((TOTPTS * 5)))
  type(active) :: MATVAL(1:INT((TOTPTS*5)))
  type(active) :: X(1:TOTPTS)
  INTEGER(w2f__i4) OpenAD_Symbol_1046
  INTEGER(w2f__i4) OpenAD_Symbol_1784
  INTEGER(w2f__i4) OpenAD_Symbol_1785


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  MATCOUNT = 0
  MATROW(1 : INT(TOTPTS * 5)) = 0
  MATCOL(1 : INT(TOTPTS * 5)) = 0
  MATVAL(1:TOTPTS*5)%v = 0.0
  X(1:INT(TOTPTS))%v = 0.0
  CALL FINDSUMS(N,M,TOTPTS,DIFF,MASK,NMSB,USRF,B,MATVAL,MATCOL,MATROW,MATCOUNT,D&
     &X,DY,DT)

  CALL adism_umf(TOTPTS,MATCOUNT,MATROW,MATCOL,MATVAL,B,X)
  CALL FILLARRAY(TOTPTS,USRF,X,NEW_USRF,MASK,N,M)
    end if
    if (our_rev_mode%tape) then
! taping
  MATCOUNT = 0
  MATVAL(1:TOTPTS*5)%v = 0.0
  X(1:INT(TOTPTS))%v = 0.0
  OpenAD_Symbol_1046 = (TOTPTS*5)
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1046
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = TOTPTS
  oad_it_ptr = oad_it_ptr+1
  MATROW(1:INT(TOTPTS*5)) = 0
  MATCOL(1:INT(TOTPTS*5)) = 0
  CALL FINDSUMS(N,M,TOTPTS,DIFF,MASK,NMSB,USRF,B,MATVAL,MATCOL,MATROW,MATCOUNT,D&
     &X,DY,DT)

  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = N
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = M
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = TOTPTS
  oad_it_ptr = oad_it_ptr+1
  CALL adism_umf(TOTPTS,MATCOUNT,MATROW,MATCOL,MATVAL,B,X)
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = TOTPTS
  oad_it_ptr = oad_it_ptr+1
  CALL FILLARRAY(TOTPTS,USRF,X,NEW_USRF,MASK,N,M)
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = TOTPTS
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = N
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = M
  oad_it_ptr = oad_it_ptr+1
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  M = oad_it(oad_it_ptr)
  oad_it_ptr = oad_it_ptr-1
  N = oad_it(oad_it_ptr)
  oad_it_ptr = oad_it_ptr-1
  TOTPTS = oad_it(oad_it_ptr)
  CALL FILLARRAY(TOTPTS,USRF,X,NEW_USRF,MASK,N,M)
  oad_it_ptr = oad_it_ptr-1
  TOTPTS = oad_it(oad_it_ptr)
  CALL adism_umf(TOTPTS,MATCOUNT,MATROW,MATCOL,MATVAL,B,X)
  oad_it_ptr = oad_it_ptr-1
  TOTPTS = oad_it(oad_it_ptr)
  oad_it_ptr = oad_it_ptr-1
  M = oad_it(oad_it_ptr)
  oad_it_ptr = oad_it_ptr-1
  N = oad_it(oad_it_ptr)
  CALL FINDSUMS(N,M,TOTPTS,DIFF,MASK,NMSB,USRF,B,MATVAL,MATCOL,MATROW,MATCOUNT,D&
     &X,DY,DT)

  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1784 = oad_it(oad_it_ptr)
  X(1:OpenAD_Symbol_1784)%d = 0.0d0
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1785 = oad_it(oad_it_ptr)
  MATVAL(1:OpenAD_Symbol_1785)%d = 0.0d0
    end if
  end subroutine CALCTHCK
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE FINDSUMS(N, M, TOTPTS, DIFF, MASK, NMSB, USRF, B, MATVAL, MATCOL, M&
     &ATROW, MATCOUNT, DX, DY, DT)

    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_1047
  INTEGER(w2f__i8) OpenAD_Symbol_1048
  INTEGER(w2f__i8) OpenAD_Symbol_1049
  INTEGER(w2f__i8) OpenAD_Symbol_1050
  INTEGER(w2f__i8) OpenAD_Symbol_1051
  INTEGER(w2f__i8) OpenAD_Symbol_1052
  INTEGER(w2f__i8) OpenAD_Symbol_1053
  INTEGER(w2f__i8) OpenAD_Symbol_1054
  INTEGER(w2f__i8) OpenAD_Symbol_1055
  INTEGER(w2f__i8) OpenAD_Symbol_1056
  INTEGER(w2f__i8) OpenAD_Symbol_1057
  INTEGER(w2f__i8) OpenAD_Symbol_1058
  INTEGER(w2f__i8) OpenAD_Symbol_1059
  INTEGER(w2f__i8) OpenAD_Symbol_1060
  INTEGER(w2f__i8) OpenAD_Symbol_1061
  INTEGER(w2f__i8) OpenAD_Symbol_1062
  INTEGER(w2f__i8) OpenAD_Symbol_1063
  INTEGER(w2f__i8) OpenAD_Symbol_1064
!
!       **** Parameters and Result ****
!
  INTEGER(w2f__i4) N
  INTEGER(w2f__i4) M
  INTEGER(w2f__i4) TOTPTS
  type(active) :: DIFF(1:N,1:M)
  INTEGER(w2f__i4) MASK(1 : N, 1 : M)
  type(active) :: NMSB(1:N,1:M)
  type(active) :: USRF(1:N,1:M)
  type(active) :: B(1:TOTPTS)
  type(active) :: MATVAL(1:INT((TOTPTS*5)))
  INTEGER(w2f__i4) MATCOL(1 : INT((TOTPTS * 5)))
  INTEGER(w2f__i4) MATROW(1 : INT((TOTPTS * 5)))
  INTEGER(w2f__i4) MATCOUNT
  REAL(w2f__8) DX
  REAL(w2f__8) DY
  REAL(w2f__8) DT
!
!       **** Local Variables and Functions ****
!
  REAL(w2f__8) FEW
  REAL(w2f__8) FNS
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  type(active) :: XM
  type(active) :: XP
  type(active) :: YM
  type(active) :: YP
  INTEGER(w2f__i4) OpenAD_Symbol_1065
  INTEGER(w2f__i4) OpenAD_Symbol_1066
  INTEGER(w2f__i4) OpenAD_Symbol_1067
  INTEGER(w2f__i4) OpenAD_Symbol_1068
  INTEGER(w2f__i4) OpenAD_Symbol_1786
  INTEGER(w2f__i4) OpenAD_Symbol_1787
  INTEGER(w2f__i4) OpenAD_Symbol_1788
  INTEGER(w2f__i4) OpenAD_Symbol_1789
  REAL(w2f__8) OpenAD_Symbol_1790
  INTEGER(w2f__i4) OpenAD_Symbol_1791
  INTEGER(w2f__i4) OpenAD_Symbol_1792
  REAL(w2f__8) OpenAD_Symbol_1793
  INTEGER(w2f__i4) OpenAD_Symbol_1794
  INTEGER(w2f__i4) OpenAD_Symbol_1795
  INTEGER(w2f__i4) OpenAD_Symbol_1796
  REAL(w2f__8) OpenAD_Symbol_1797
  INTEGER(w2f__i4) OpenAD_Symbol_1798
  INTEGER(w2f__i4) OpenAD_Symbol_1799
  INTEGER(w2f__i4) OpenAD_Symbol_1800
  REAL(w2f__8) OpenAD_Symbol_1801
  INTEGER(w2f__i4) OpenAD_Symbol_1802
  INTEGER(w2f__i4) OpenAD_Symbol_1803
  INTEGER(w2f__i4) OpenAD_Symbol_1804
  REAL(w2f__8) OpenAD_Symbol_1805
  INTEGER(w2f__i4) OpenAD_Symbol_1806
  REAL(w2f__8) OpenAD_aux_100
  REAL(w2f__8) OpenAD_aux_101
  REAL(w2f__8) OpenAD_aux_102
  REAL(w2f__8) OpenAD_aux_103
  REAL(w2f__8) OpenAD_lin_185
  REAL(w2f__8) OpenAD_lin_186
  REAL(w2f__8) OpenAD_lin_187
  REAL(w2f__8) OpenAD_lin_188
  REAL(w2f__8) OpenAD_lin_189
  type(active) :: OpenAD_prp_71
  type(active) :: OpenAD_prp_72
  type(active) :: OpenAD_prp_73
  type(active) :: OpenAD_prp_74


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  FEW = (-((DT * 5.0D-01) /(DX * DX)))
  FNS = (-((DT * 5.0D-01) /(DY * DY)))
  DO I = 2, (N +(-1)), 1
    DO J = 2, (M +(-1)), 1
      IF(MASK(I, J) .GT. 0) THEN
        XP%v = (FEW*(DIFF(I,J)%v+DIFF(I+1,J)%v))
        XM%v = (FEW*(DIFF(I,J)%v+DIFF(I+(-1),J)%v))
        YP%v = (FNS*(DIFF(I,J)%v+DIFF(I,J+1)%v))
        YM%v = (FNS*(DIFF(I,J)%v+DIFF(I,J+(-1))%v))
        CALL GENERATE_ROW(N,M,TOTPTS,I,J,MASK,XP,XM,YP,YM,MATVAL,MATCOL,MATROW,M&
     &ATCOUNT)

        B(MASK(INT(I),INT(J)))%v = (USRF(I,J)%v+NMSB(I,J)%v*DT)
      ENDIF
    END DO
  END DO
    end if
    if (our_rev_mode%tape) then
! taping
  FEW = (-((DT*5.0D-01)/(DX*DX)))
  FNS = (-((DT*5.0D-01)/(DY*DY)))
  OpenAD_Symbol_1052 = 0_w2f__i8
  DO I = 2,(N+(-1)),1
    OpenAD_Symbol_1053 = 0_w2f__i8
    DO J = 2,(M+(-1)),1
      IF (MASK(I,J).GT.0) THEN
        OpenAD_aux_100 = (DIFF(I,J)%v+DIFF(I+1,J)%v)
        OpenAD_lin_185 = FEW
        XP%v = (FEW*OpenAD_aux_100)
        OpenAD_Symbol_1065 = (I+1)
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_1065
        oad_it_ptr = oad_it_ptr+1
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_lin_185
        oad_dt_ptr = oad_dt_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = I
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = J
        oad_it_ptr = oad_it_ptr+1
        OpenAD_aux_101 = (DIFF(I,J)%v+DIFF(I+(-1),J)%v)
        OpenAD_lin_186 = FEW
        XM%v = (FEW*OpenAD_aux_101)
        OpenAD_Symbol_1066 = (I+(-1))
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_1066
        oad_it_ptr = oad_it_ptr+1
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_lin_186
        oad_dt_ptr = oad_dt_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = I
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = J
        oad_it_ptr = oad_it_ptr+1
        OpenAD_aux_102 = (DIFF(I,J)%v+DIFF(I,J+1)%v)
        OpenAD_lin_187 = FNS
        YP%v = (FNS*OpenAD_aux_102)
        OpenAD_Symbol_1067 = (J+1)
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_1067
        oad_it_ptr = oad_it_ptr+1
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_lin_187
        oad_dt_ptr = oad_dt_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = I
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = J
        oad_it_ptr = oad_it_ptr+1
        OpenAD_aux_103 = (DIFF(I,J)%v+DIFF(I,J+(-1))%v)
        OpenAD_lin_188 = FNS
        YM%v = (FNS*OpenAD_aux_103)
        OpenAD_Symbol_1068 = (J+(-1))
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_1068
        oad_it_ptr = oad_it_ptr+1
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_lin_188
        oad_dt_ptr = oad_dt_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = I
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = J
        oad_it_ptr = oad_it_ptr+1
        CALL GENERATE_ROW(N,M,TOTPTS,I,J,MASK,XP,XM,YP,YM,MATVAL,MATCOL,MATROW,M&
     &ATCOUNT)

        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = N
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = M
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = TOTPTS
        oad_it_ptr = oad_it_ptr+1
        OpenAD_lin_189 = DT
        B(MASK(INT(I),INT(J)))%v = (USRF(I,J)%v+NMSB(I,J)%v*DT)
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_lin_189
        oad_dt_ptr = oad_dt_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = I
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = J
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = MASK(I,J)
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = MASK(I,J)
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_1054 = 1_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_1054
        oad_it_ptr = oad_it_ptr+1
      ELSE
        OpenAD_Symbol_1055 = 0_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_1055
        oad_it_ptr = oad_it_ptr+1
      ENDIF
      OpenAD_Symbol_1053 = (INT(OpenAD_Symbol_1053)+INT(1_w2f__i8))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_1053
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_1052 = (INT(OpenAD_Symbol_1052)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1052
  oad_it_ptr = oad_it_ptr+1
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1047 = oad_it(oad_it_ptr)
  OpenAD_Symbol_1048 = 1
  do while (INT(OpenAD_Symbol_1048).LE.INT(OpenAD_Symbol_1047))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1049 = oad_it(oad_it_ptr)
    OpenAD_Symbol_1050 = 1
    do while (INT(OpenAD_Symbol_1050).LE.INT(OpenAD_Symbol_1049))
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1051 = oad_it(oad_it_ptr)
      IF (OpenAD_Symbol_1051.ne.0) THEN
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1786 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1787 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1788 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1789 = oad_it(oad_it_ptr)
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1790 = oad_dt(oad_dt_ptr)
        NMSB(OpenAD_Symbol_1789,OpenAD_Symbol_1788)%d = NMSB(OpenAD_Symbol_1789,&
     &OpenAD_Symbol_1788)%d+B(OpenAD_Symbol_1786)%d*(OpenAD_Symbol_1790)

        USRF(OpenAD_Symbol_1789,OpenAD_Symbol_1788)%d = USRF(OpenAD_Symbol_1789,&
     &OpenAD_Symbol_1788)%d+B(OpenAD_Symbol_1787)%d

        B(OpenAD_Symbol_1787)%d = 0.0d0
        oad_it_ptr = oad_it_ptr-1
        TOTPTS = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        M = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        N = oad_it(oad_it_ptr)
        CALL GENERATE_ROW(N,M,TOTPTS,I,J,MASK,XP,XM,YP,YM,MATVAL,MATCOL,MATROW,M&
     &ATCOUNT)

        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1791 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1792 = oad_it(oad_it_ptr)
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1793 = oad_dt(oad_dt_ptr)
        OpenAD_prp_74%d = OpenAD_prp_74%d+YM%d*(OpenAD_Symbol_1793)
        YM%d = 0.0d0
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1794 = oad_it(oad_it_ptr)
        DIFF(OpenAD_Symbol_1792,OpenAD_Symbol_1794)%d = DIFF(OpenAD_Symbol_1792,&
     &OpenAD_Symbol_1794)%d+OpenAD_prp_74%d

        DIFF(OpenAD_Symbol_1792,OpenAD_Symbol_1791)%d = DIFF(OpenAD_Symbol_1792,&
     &OpenAD_Symbol_1791)%d+OpenAD_prp_74%d

        OpenAD_prp_74%d = 0.0d0
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1795 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1796 = oad_it(oad_it_ptr)
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1797 = oad_dt(oad_dt_ptr)
        OpenAD_prp_73%d = OpenAD_prp_73%d+YP%d*(OpenAD_Symbol_1797)
        YP%d = 0.0d0
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1798 = oad_it(oad_it_ptr)
        DIFF(OpenAD_Symbol_1796,OpenAD_Symbol_1798)%d = DIFF(OpenAD_Symbol_1796,&
     &OpenAD_Symbol_1798)%d+OpenAD_prp_73%d

        DIFF(OpenAD_Symbol_1796,OpenAD_Symbol_1795)%d = DIFF(OpenAD_Symbol_1796,&
     &OpenAD_Symbol_1795)%d+OpenAD_prp_73%d

        OpenAD_prp_73%d = 0.0d0
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1799 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1800 = oad_it(oad_it_ptr)
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1801 = oad_dt(oad_dt_ptr)
        OpenAD_prp_72%d = OpenAD_prp_72%d+XM%d*(OpenAD_Symbol_1801)
        XM%d = 0.0d0
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1802 = oad_it(oad_it_ptr)
        DIFF(OpenAD_Symbol_1802,OpenAD_Symbol_1799)%d = DIFF(OpenAD_Symbol_1802,&
     &OpenAD_Symbol_1799)%d+OpenAD_prp_72%d

        DIFF(OpenAD_Symbol_1800,OpenAD_Symbol_1799)%d = DIFF(OpenAD_Symbol_1800,&
     &OpenAD_Symbol_1799)%d+OpenAD_prp_72%d

        OpenAD_prp_72%d = 0.0d0
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1803 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1804 = oad_it(oad_it_ptr)
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1805 = oad_dt(oad_dt_ptr)
        OpenAD_prp_71%d = OpenAD_prp_71%d+XP%d*(OpenAD_Symbol_1805)
        XP%d = 0.0d0
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1806 = oad_it(oad_it_ptr)
        DIFF(OpenAD_Symbol_1806,OpenAD_Symbol_1803)%d = DIFF(OpenAD_Symbol_1806,&
     &OpenAD_Symbol_1803)%d+OpenAD_prp_71%d

        DIFF(OpenAD_Symbol_1804,OpenAD_Symbol_1803)%d = DIFF(OpenAD_Symbol_1804,&
     &OpenAD_Symbol_1803)%d+OpenAD_prp_71%d

        OpenAD_prp_71%d = 0.0d0
      ENDIF
      OpenAD_Symbol_1050 = INT(OpenAD_Symbol_1050)+1
    END DO
    OpenAD_Symbol_1048 = INT(OpenAD_Symbol_1048)+1
  END DO
    end if
  end subroutine FINDSUMS
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE GENERATE_ROW(N, M, TOTPTS, I, J, MASK, XP, XM, YP, YM, MATVAL, MATC&
     &OL, MATROW, MATCOUNT)

    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  INTEGER(w2f__i4) N
  INTEGER(w2f__i4) M
  INTEGER(w2f__i4) TOTPTS
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  INTEGER(w2f__i4) MASK(1 : N, 1 : M)
  type(active) :: XP
  type(active) :: XM
  type(active) :: YP
  type(active) :: YM
  type(active) :: MATVAL(1:INT((TOTPTS*5)))
  INTEGER(w2f__i4) MATCOL(1 : INT((TOTPTS * 5)))
  INTEGER(w2f__i4) MATROW(1 : INT((TOTPTS * 5)))
  INTEGER(w2f__i4) MATCOUNT
!
!       **** Local Variables and Functions ****
!
  type(active) :: TMP
  type(active) :: OpenAD_prp_75
  type(active) :: OpenAD_prp_76
  type(active) :: OpenAD_prp_77
  type(active) :: OpenAD_prp_78
  type(active) :: OpenAD_prp_79
  type(active) :: OpenAD_prp_80
  type(active) :: OpenAD_prp_81


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  CALL PUTPCGC(TOTPTS,XP,MASK(I+1,J),MASK(I,J),MATVAL,MATCOL,MATROW,MATCOUNT)
  CALL PUTPCGC(TOTPTS,XM,MASK(I+(-1),J),MASK(I,J),MATVAL,MATCOL,MATROW,MATCOUNT)
  CALL PUTPCGC(TOTPTS,YP,MASK(I,J+1),MASK(I,J),MATVAL,MATCOL,MATROW,MATCOUNT)
  CALL PUTPCGC(TOTPTS,YM,MASK(I,J+(-1)),MASK(I,J),MATVAL,MATCOL,MATROW,MATCOUNT)
  TMP%v = (XP%v+XM%v)
  TMP%v = (YP%v+TMP%v)
  TMP%v = (YM%v+TMP%v)
  TMP%v = (1.0D00-TMP%v)
  CALL PUTPCGC(TOTPTS,TMP,MASK(I,J),MASK(I,J),MATVAL,MATCOL,MATROW,MATCOUNT)
    end if
    if (our_rev_mode%tape) then
! taping
  CALL PUTPCGC(TOTPTS,XP,MASK(I+1,J),MASK(I,J),MATVAL,MATCOL,MATROW,MATCOUNT)
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = TOTPTS
  oad_it_ptr = oad_it_ptr+1
  CALL PUTPCGC(TOTPTS,XM,MASK(I+(-1),J),MASK(I,J),MATVAL,MATCOL,MATROW,MATCOUNT)
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = TOTPTS
  oad_it_ptr = oad_it_ptr+1
  CALL PUTPCGC(TOTPTS,YP,MASK(I,J+1),MASK(I,J),MATVAL,MATCOL,MATROW,MATCOUNT)
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = TOTPTS
  oad_it_ptr = oad_it_ptr+1
  CALL PUTPCGC(TOTPTS,YM,MASK(I,J+(-1)),MASK(I,J),MATVAL,MATCOL,MATROW,MATCOUNT)
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = TOTPTS
  oad_it_ptr = oad_it_ptr+1
  TMP%v = (XP%v+XM%v)
  TMP%v = (YP%v+TMP%v)
  TMP%v = (YM%v+TMP%v)
  TMP%v = (1.0D00-TMP%v)
  CALL PUTPCGC(TOTPTS,TMP,MASK(I,J),MASK(I,J),MATVAL,MATCOL,MATROW,MATCOUNT)
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = TOTPTS
  oad_it_ptr = oad_it_ptr+1
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  TOTPTS = oad_it(oad_it_ptr)
  CALL PUTPCGC(TOTPTS,TMP,MASK(I,J),MASK(I,J),MATVAL,MATCOL,MATROW,MATCOUNT)
  OpenAD_prp_81%d = OpenAD_prp_81%d-TMP%d
  TMP%d = 0.0d0
  TMP%d = TMP%d+OpenAD_prp_81%d
  OpenAD_prp_81%d = 0.0d0
  OpenAD_prp_80%d = OpenAD_prp_80%d+TMP%d
  OpenAD_prp_79%d = OpenAD_prp_79%d+TMP%d
  TMP%d = 0.0d0
  TMP%d = TMP%d+OpenAD_prp_80%d
  OpenAD_prp_80%d = 0.0d0
  YM%d = YM%d+OpenAD_prp_79%d
  OpenAD_prp_79%d = 0.0d0
  OpenAD_prp_78%d = OpenAD_prp_78%d+TMP%d
  OpenAD_prp_77%d = OpenAD_prp_77%d+TMP%d
  TMP%d = 0.0d0
  TMP%d = TMP%d+OpenAD_prp_78%d
  OpenAD_prp_78%d = 0.0d0
  YP%d = YP%d+OpenAD_prp_77%d
  OpenAD_prp_77%d = 0.0d0
  OpenAD_prp_76%d = OpenAD_prp_76%d+TMP%d
  OpenAD_prp_75%d = OpenAD_prp_75%d+TMP%d
  TMP%d = 0.0d0
  XM%d = XM%d+OpenAD_prp_76%d
  OpenAD_prp_76%d = 0.0d0
  XP%d = XP%d+OpenAD_prp_75%d
  OpenAD_prp_75%d = 0.0d0
  oad_it_ptr = oad_it_ptr-1
  TOTPTS = oad_it(oad_it_ptr)
  CALL PUTPCGC(TOTPTS,YM,MASK(I,J+(-1)),MASK(I,J),MATVAL,MATCOL,MATROW,MATCOUNT)
  oad_it_ptr = oad_it_ptr-1
  TOTPTS = oad_it(oad_it_ptr)
  CALL PUTPCGC(TOTPTS,YP,MASK(I,J+1),MASK(I,J),MATVAL,MATCOL,MATROW,MATCOUNT)
  oad_it_ptr = oad_it_ptr-1
  TOTPTS = oad_it(oad_it_ptr)
  CALL PUTPCGC(TOTPTS,XM,MASK(I+(-1),J),MASK(I,J),MATVAL,MATCOL,MATROW,MATCOUNT)
  oad_it_ptr = oad_it_ptr-1
  TOTPTS = oad_it(oad_it_ptr)
  CALL PUTPCGC(TOTPTS,XP,MASK(I+1,J),MASK(I,J),MATVAL,MATCOL,MATROW,MATCOUNT)
    end if
  end subroutine GENERATE_ROW
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE PUTPCGC(TOTPTS, VALUE, COL, ROW, MATVAL, MATCOL, MATROW, MATCOUNT)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_1087
  INTEGER(w2f__i8) OpenAD_Symbol_1088
  INTEGER(w2f__i8) OpenAD_Symbol_1089
  INTEGER(w2f__i8) OpenAD_Symbol_1090
  INTEGER(w2f__i8) OpenAD_Symbol_1091
  INTEGER(w2f__i8) OpenAD_Symbol_1093
  INTEGER(w2f__i8) OpenAD_Symbol_1094
  INTEGER(w2f__i8) OpenAD_Symbol_1095
  INTEGER(w2f__i8) OpenAD_Symbol_1096
  INTEGER(w2f__i8) OpenAD_Symbol_1097
!
!       **** Parameters and Result ****
!
  INTEGER(w2f__i4) TOTPTS
  type(active) :: VALUE
  INTEGER(w2f__i4) COL
  INTEGER(w2f__i4) ROW
  type(active) :: MATVAL(1:INT((TOTPTS*5)))
  INTEGER(w2f__i4) MATCOL(1 : INT((TOTPTS * 5)))
  INTEGER(w2f__i4) MATROW(1 : INT((TOTPTS * 5)))
  INTEGER(w2f__i4) MATCOUNT
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_1092
  INTEGER(w2f__i8) OpenAD_Symbol_1098
  INTEGER(w2f__i4) OpenAD_Symbol_1812


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
 2      CONTINUE
  GO TO 3
 3      CONTINUE
  GO TO 4
 4      CONTINUE
  IF((COL .eq. 0) .OR.(ROW .eq. 0)) THEN
    GO TO 8
  ELSE
    GO TO 5
  ENDIF
 5      CONTINUE
  GO TO 6
 6      CONTINUE
  GO TO 7
 7      CONTINUE
  MATCOUNT = (MATCOUNT + 1)
  MATVAL(INT(MATCOUNT))%v = VALUE%v
  MATCOL(MATCOUNT) = COL
  MATROW(MATCOUNT) = ROW
  GO TO 9
 8      CONTINUE
  GO TO 1
  GO TO 9
 9      CONTINUE
  GO TO 1
 1      CONTINUE
    end if
    if (our_rev_mode%tape) then
! taping
 11     CONTINUE
  GO TO 12
 12     CONTINUE
  OpenAD_Symbol_1087 = 0_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1087
  oad_it_ptr = oad_it_ptr+1
  GO TO 13
 13     CONTINUE
  GO TO 14
 14     CONTINUE
  OpenAD_Symbol_1088 = 1_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1088
  oad_it_ptr = oad_it_ptr+1
  GO TO 15
 15     CONTINUE
  IF ((COL.eq.0).OR.(ROW.eq.0)) THEN
    GO TO 21
  ELSE
    GO TO 16
  ENDIF
 16     CONTINUE
  GO TO 17
 17     CONTINUE
  OpenAD_Symbol_1090 = 3_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1090
  oad_it_ptr = oad_it_ptr+1
  GO TO 18
 18     CONTINUE
  GO TO 19
 19     CONTINUE
  MATCOUNT = (MATCOUNT+1)
  MATVAL(INT(MATCOUNT))%v = VALUE%v
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = MATCOUNT
  oad_it_ptr = oad_it_ptr+1
  MATCOL(MATCOUNT) = COL
  MATROW(MATCOUNT) = ROW
  GO TO 20
 20     CONTINUE
  OpenAD_Symbol_1091 = 4_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1091
  oad_it_ptr = oad_it_ptr+1
  GO TO 23
 21     CONTINUE
  GO TO 10
  GO TO 22
 22     CONTINUE
  OpenAD_Symbol_1089 = 2_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1089
  oad_it_ptr = oad_it_ptr+1
  GO TO 23
 23     CONTINUE
  GO TO 10
 10     CONTINUE
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1092 = oad_it(oad_it_ptr)
  do while (INT(OpenAD_Symbol_1092).ne.INT(0_w2f__i8))
    IF (OpenAD_Symbol_1092.EQ.1) go to 44
    IF (OpenAD_Symbol_1092.EQ.2) go to 45
    IF (OpenAD_Symbol_1092.EQ.3) go to 46
    IF (OpenAD_Symbol_1092.EQ.4) go to 47
 44       CONTINUE
    GO TO 41
 45       CONTINUE
    GO TO 41
 46       CONTINUE
    GO TO 41
 47       CONTINUE
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1812 = oad_it(oad_it_ptr)
    VALUE%d = VALUE%d+MATVAL(OpenAD_Symbol_1812)%d
    MATVAL(OpenAD_Symbol_1812)%d = 0.0d0
    GO TO 41
 41       CONTINUE
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1092 = oad_it(oad_it_ptr)
  END DO
    end if
  end subroutine PUTPCGC
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE FILLARRAY(TOTPTS, USRF, IN, OUT, MASK, N, M)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_1069
  INTEGER(w2f__i8) OpenAD_Symbol_1070
  INTEGER(w2f__i8) OpenAD_Symbol_1071
  INTEGER(w2f__i8) OpenAD_Symbol_1072
  INTEGER(w2f__i8) OpenAD_Symbol_1073
  INTEGER(w2f__i8) OpenAD_Symbol_1074
  INTEGER(w2f__i8) OpenAD_Symbol_1075
  INTEGER(w2f__i8) OpenAD_Symbol_1076
  INTEGER(w2f__i8) OpenAD_Symbol_1077
  INTEGER(w2f__i8) OpenAD_Symbol_1078
  INTEGER(w2f__i8) OpenAD_Symbol_1079
  INTEGER(w2f__i8) OpenAD_Symbol_1080
  INTEGER(w2f__i8) OpenAD_Symbol_1081
  INTEGER(w2f__i8) OpenAD_Symbol_1082
  INTEGER(w2f__i8) OpenAD_Symbol_1083
  INTEGER(w2f__i8) OpenAD_Symbol_1084
  INTEGER(w2f__i8) OpenAD_Symbol_1085
  INTEGER(w2f__i8) OpenAD_Symbol_1086
!
!       **** Parameters and Result ****
!
  INTEGER(w2f__i4) TOTPTS
  INTEGER(w2f__i4) N
  INTEGER(w2f__i4) M
  type(active) :: USRF(1:N,1:M)
  type(active) :: IN(1:TOTPTS)
  type(active) :: OUT(1:N,1:M)
  INTEGER(w2f__i4) MASK(1 : N, 1 : M)
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  INTEGER(w2f__i4) OpenAD_Symbol_1807
  INTEGER(w2f__i4) OpenAD_Symbol_1808
  INTEGER(w2f__i4) OpenAD_Symbol_1809
  INTEGER(w2f__i4) OpenAD_Symbol_1810
  INTEGER(w2f__i4) OpenAD_Symbol_1811


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  DO I = 1, N, 1
    DO J = 1, M, 1
      IF(MASK(I, J) .GT. 0) THEN
        OUT(INT(I),INT(J))%v = IN(MASK(I,J))%v
      ELSE
        OUT(INT(I),INT(J))%v = USRF(I,J)%v
      ENDIF
    END DO
  END DO
    end if
    if (our_rev_mode%tape) then
! taping
  OpenAD_Symbol_1074 = 0_w2f__i8
  DO I = 1,N,1
    OpenAD_Symbol_1075 = 0_w2f__i8
    DO J = 1,M,1
      IF (MASK(I,J).GT.0) THEN
        OUT(INT(I),INT(J))%v = IN(MASK(I,J))%v
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = MASK(I,J)
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = I
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = J
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_1076 = 1_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_1076
        oad_it_ptr = oad_it_ptr+1
      ELSE
        OUT(INT(I),INT(J))%v = USRF(I,J)%v
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = I
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = J
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_1077 = 0_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_1077
        oad_it_ptr = oad_it_ptr+1
      ENDIF
      OpenAD_Symbol_1075 = (INT(OpenAD_Symbol_1075)+INT(1_w2f__i8))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_1075
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_1074 = (INT(OpenAD_Symbol_1074)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1074
  oad_it_ptr = oad_it_ptr+1
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1069 = oad_it(oad_it_ptr)
  OpenAD_Symbol_1070 = 1
  do while (INT(OpenAD_Symbol_1070).LE.INT(OpenAD_Symbol_1069))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1071 = oad_it(oad_it_ptr)
    OpenAD_Symbol_1072 = 1
    do while (INT(OpenAD_Symbol_1072).LE.INT(OpenAD_Symbol_1071))
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1073 = oad_it(oad_it_ptr)
      IF (OpenAD_Symbol_1073.ne.0) THEN
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1807 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1808 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1809 = oad_it(oad_it_ptr)
        IN(OpenAD_Symbol_1809)%d = IN(OpenAD_Symbol_1809)%d+OUT(OpenAD_Symbol_18&
     &08,OpenAD_Symbol_1807)%d

        OUT(OpenAD_Symbol_1808,OpenAD_Symbol_1807)%d = 0.0d0
      ELSE
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1810 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1811 = oad_it(oad_it_ptr)
        USRF(OpenAD_Symbol_1811,OpenAD_Symbol_1810)%d = USRF(OpenAD_Symbol_1811,&
     &OpenAD_Symbol_1810)%d+OUT(OpenAD_Symbol_1811,OpenAD_Symbol_1810)%d

        OUT(OpenAD_Symbol_1811,OpenAD_Symbol_1810)%d = 0.0d0
      ENDIF
      OpenAD_Symbol_1072 = INT(OpenAD_Symbol_1072)+1
    END DO
    OpenAD_Symbol_1070 = INT(OpenAD_Symbol_1070)+1
  END DO
    end if
  end subroutine FILLARRAY
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE MASKER(N, M, THCK, NMSB, MASK, COUNT)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_1099
  INTEGER(w2f__i8) OpenAD_Symbol_1100
  INTEGER(w2f__i8) OpenAD_Symbol_1101
  INTEGER(w2f__i8) OpenAD_Symbol_1102
  INTEGER(w2f__i8) OpenAD_Symbol_1103
  INTEGER(w2f__i8) OpenAD_Symbol_1104
  INTEGER(w2f__i8) OpenAD_Symbol_1105
  INTEGER(w2f__i8) OpenAD_Symbol_1106
  INTEGER(w2f__i8) OpenAD_Symbol_1107
  INTEGER(w2f__i8) OpenAD_Symbol_1108
  INTEGER(w2f__i8) OpenAD_Symbol_1109
  INTEGER(w2f__i8) OpenAD_Symbol_1110
  INTEGER(w2f__i8) OpenAD_Symbol_1111
  INTEGER(w2f__i8) OpenAD_Symbol_1112
  INTEGER(w2f__i8) OpenAD_Symbol_1113
  INTEGER(w2f__i8) OpenAD_Symbol_1114
  INTEGER(w2f__i8) OpenAD_Symbol_1115
  INTEGER(w2f__i8) OpenAD_Symbol_1116
!
!       **** Parameters and Result ****
!
  INTEGER(w2f__i4) N
  INTEGER(w2f__i4) M
  REAL(w2f__8) THCK(1 : N, 1 : M)
  REAL(w2f__8) NMSB(1 : N, 1 : M)
  INTEGER(w2f__i4) MASK(1 : N, 1 : M)
  INTEGER(w2f__i4) COUNT
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  COUNT = 1
  MASK(1 : N, 1 : M) = 0
  DO I = 2, (N +(-1)), 1
    DO J = 2, (M +(-1)), 1
      IF((NMSB(I, J) .GT. 0.0D00) .OR. ANY(THCK(I +(-1) : I + 1, J +(-1) : J + 1&
     &) .ne. 0.0D00)) THEN

        MASK(I, J) = COUNT
        COUNT = (COUNT + 1)
      ENDIF
    END DO
  END DO
  COUNT = (COUNT +(-1))
    end if
    if (our_rev_mode%tape) then
! taping
  COUNT = 1
  MASK(1 : N, 1 : M) = 0
  OpenAD_Symbol_1104 = 0_w2f__i8
  DO I = 2, (N +(-1)), 1
    OpenAD_Symbol_1105 = 0_w2f__i8
    DO J = 2, (M +(-1)), 1
      IF((NMSB(I, J) .GT. 0.0D00) .OR. ANY(THCK(I +(-1) : I + 1, J +(-1) : J + 1&
     &) .ne. 0.0D00)) THEN

        MASK(I, J) = COUNT
        COUNT = (COUNT + 1)
        OpenAD_Symbol_1106 = 1_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_1106
        oad_it_ptr = oad_it_ptr+1
      ELSE
        OpenAD_Symbol_1107 = 0_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_1107
        oad_it_ptr = oad_it_ptr+1
      ENDIF
      OpenAD_Symbol_1105 = (INT(OpenAD_Symbol_1105) + INT( 1_w2f__i8))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_1105
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_1104 = (INT(OpenAD_Symbol_1104) + INT(1_w2f__i8 ))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1104
  oad_it_ptr = oad_it_ptr+1
  COUNT = (COUNT +(-1))
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1099 = oad_it(oad_it_ptr)
  OpenAD_Symbol_1100 = 1
  DO WHILE(INT(OpenAD_Symbol_1100) .LE. INT(OpenAD_Symbol_1099))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1101 = oad_it(oad_it_ptr)
    OpenAD_Symbol_1102 = 1
    DO WHILE(INT(OpenAD_Symbol_1102) .LE. INT(OpenAD_Symbol_1101) )
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1103 = oad_it(oad_it_ptr)
      IF(OpenAD_Symbol_1103 .ne. 0) THEN
      ENDIF
      OpenAD_Symbol_1102 = INT(OpenAD_Symbol_1102) + 1
    END DO
    OpenAD_Symbol_1100 = INT(OpenAD_Symbol_1100) + 1
  END DO
    end if
  end subroutine MASKER
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE CALC_SIGMA(X, N, CS)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) X
  REAL(w2f__8) N
  REAL(w2f__8) CS


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  CS = ((1.0D00 -((X + 1.0D00) **(- N))) /(1.0D00 -(2.0D00 **(- N ))))
    end if
    if (our_rev_mode%tape) then
! taping
  CS = ((1.0D00 -((X + 1.0D00) **(- N))) /(1.0D00 -(2.0D00 **(- N ))))
    end if
    if (our_rev_mode%adjoint) then
! adjoint
    end if
  end subroutine CALC_SIGMA
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE DIAGWRITE2D(VAR)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_1117
  INTEGER(w2f__i8) OpenAD_Symbol_1118
  INTEGER(w2f__i8) OpenAD_Symbol_1119
  INTEGER(w2f__i8) OpenAD_Symbol_1120
  INTEGER(w2f__i8) OpenAD_Symbol_1121
  INTEGER(w2f__i8) OpenAD_Symbol_1122
  INTEGER(w2f__i8) OpenAD_Symbol_1123
  INTEGER(w2f__i8) OpenAD_Symbol_1124
  INTEGER(w2f__i8) OpenAD_Symbol_1125
  INTEGER(w2f__i8) OpenAD_Symbol_1126
  INTEGER(w2f__i8) OpenAD_Symbol_1127
  INTEGER(w2f__i8) OpenAD_Symbol_1128
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) VAR(1 :, 1 :)
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) II
  INTEGER(w2f__i4) JJ
  INTEGER(w2f__i4) N1
  INTEGER(w2f__i4) N2


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  N1 = SIZE(VAR, 1)
  N2 = SIZE(VAR, 2)
!       open(30,file='diagout.txt')
  OPEN(UNIT = 30, FILE = 'diagout.txt')
  DO II = 1, N1, 1
    DO JJ = 1, N2, 1
      WRITE(30,*) II,JJ,VAR(II,JJ)
    END DO
  END DO
!       close(30)
  CLOSE(UNIT = 30)
  STOP
    end if
    if (our_rev_mode%tape) then
! taping
  N1 = SIZE(VAR, 1)
  N2 = SIZE(VAR, 2)
!       open(30,file='diagout.txt')
  OPEN(UNIT = 30, FILE = 'diagout.txt')
  OpenAD_Symbol_1121 = 0_w2f__i8
  DO II = 1, N1, 1
    OpenAD_Symbol_1122 = 0_w2f__i8
    DO JJ = 1, N2, 1
      WRITE(30,*) II,JJ,VAR(II,JJ)
      OpenAD_Symbol_1122 = (INT(OpenAD_Symbol_1122) + INT( 1_w2f__i8))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_1122
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_1121 = (INT(OpenAD_Symbol_1121) + INT(1_w2f__i8 ))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1121
  oad_it_ptr = oad_it_ptr+1
!       close(30)
  CLOSE(UNIT = 30)
  STOP
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1117 = oad_it(oad_it_ptr)
  OpenAD_Symbol_1118 = 1
  DO WHILE(INT(OpenAD_Symbol_1118) .LE. INT(OpenAD_Symbol_1117))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1119 = oad_it(oad_it_ptr)
    OpenAD_Symbol_1120 = 1
    DO WHILE(INT(OpenAD_Symbol_1120) .LE. INT(OpenAD_Symbol_1119) )
      OpenAD_Symbol_1120 = INT(OpenAD_Symbol_1120) + 1
    END DO
    OpenAD_Symbol_1118 = INT(OpenAD_Symbol_1118) + 1
  END DO
    end if
  end subroutine DIAGWRITE2D
END

!$OPENAD XXX File_start [adism_mbal.F90]
MODULE adism_mbal
use OAD_active
use w2f__types
use oad_intrinsics
use glimmer_global ,only: DP
use physcon ,only: SCYR
IMPLICIT NONE
SAVE
!
!     **** Statements ****
!
CONTAINS
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE MASS_BALANCE_MM(NMSB, DX, DY, N, M)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) DX
  REAL(w2f__8) DY
  INTEGER(w2f__i4) N
  INTEGER(w2f__i4) M
  REAL(w2f__8) NMSB(1 : N, 1 : M)
!
!       **** Local Variables and Functions ****
!
  REAL(w2f__8) D(1 : N, 1 : M)
  REAL(w2f__8) OAD_CTMP0(1 : N, 1 : M)
  REAL(w2f__8) OAD_CTMP1(1 : N, 1 : M)
  REAL(w2f__8) RE
  PARAMETER ( RE = 4.5D+05)
  REAL(w2f__8) S
  PARAMETER ( S = 9.99999974737875163555D-06)
  REAL(w2f__8) XS
  PARAMETER ( XS = 7.5D+05)
  REAL(w2f__8) YS
  PARAMETER ( YS = 7.5D+05)


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  CALL DISTANCE(D,7.5D+05,7.5D+05,DX,DY,N,M)
  OAD_CTMP1(1 : INT(N), 1 : INT(M)) = ((4.5D+05 - D(1 : N, 1 : M) ) * 9.99999974&
     &737875163555D-06)

  CALL OAD_S_MIN_D_02(5.0D-01,OAD_CTMP1,OAD_CTMP0)
  NMSB(1 : INT(N), 1 : INT(M)) = (OAD_CTMP0(1 : N, 1 : M) / 3.1556926D+07)
    end if
    if (our_rev_mode%tape) then
! taping
  CALL DISTANCE(D,7.5D+05,7.5D+05,DX,DY,N,M)
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = N
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = M
  oad_it_ptr = oad_it_ptr+1
  OAD_CTMP1(1 : INT(N), 1 : INT(M)) = ((4.5D+05 - D(1 : N, 1 : M) ) * 9.99999974&
     &737875163555D-06)

  CALL OAD_S_MIN_D_02(5.0D-01,OAD_CTMP1,OAD_CTMP0)
  NMSB(1 : INT(N), 1 : INT(M)) = (OAD_CTMP0(1 : N, 1 : M) / 3.1556926D+07)
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  CALL OAD_S_MIN_D_02(5.0D-01,OAD_CTMP1,OAD_CTMP0)
  oad_it_ptr = oad_it_ptr-1
  M = oad_it(oad_it_ptr)
  oad_it_ptr = oad_it_ptr-1
  N = oad_it(oad_it_ptr)
  CALL DISTANCE(D,7.5D+05,7.5D+05,DX,DY,N,M)
    end if
  end subroutine MASS_BALANCE_MM
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE DISTANCE(D, XS, YS, DX, DY, N, M)
    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_1129
  INTEGER(w2f__i8) OpenAD_Symbol_1130
  INTEGER(w2f__i8) OpenAD_Symbol_1131
  INTEGER(w2f__i8) OpenAD_Symbol_1132
  INTEGER(w2f__i8) OpenAD_Symbol_1133
  INTEGER(w2f__i8) OpenAD_Symbol_1134
  INTEGER(w2f__i8) OpenAD_Symbol_1135
  INTEGER(w2f__i8) OpenAD_Symbol_1136
  INTEGER(w2f__i8) OpenAD_Symbol_1137
  INTEGER(w2f__i8) OpenAD_Symbol_1138
  INTEGER(w2f__i8) OpenAD_Symbol_1139
  INTEGER(w2f__i8) OpenAD_Symbol_1140
!
!       **** Parameters and Result ****
!
  REAL(w2f__8) XS
  REAL(w2f__8) YS
  REAL(w2f__8) DX
  REAL(w2f__8) DY
  INTEGER(w2f__i4) N
  INTEGER(w2f__i4) M
  REAL(w2f__8) D(1 : N, 1 : M)
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J


    integer iaddr
    external iaddr
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
  DO I = 1, N, 1
    DO J = 1, M, 1
      D(INT(I), INT(J)) = SQRT((((I +(-1)) * DX - XS) ** 2) +(((J +(-1)) * DY - &
     &YS) ** 2))

    END DO
  END DO
    end if
    if (our_rev_mode%tape) then
! taping
  OpenAD_Symbol_1133 = 0_w2f__i8
  DO I = 1, N, 1
    OpenAD_Symbol_1134 = 0_w2f__i8
    DO J = 1, M, 1
      D(INT(I), INT(J)) = SQRT((((I +(-1)) * DX - XS) ** 2) +(((J +(-1)) * DY - &
     &YS) ** 2))

      OpenAD_Symbol_1134 = (INT(OpenAD_Symbol_1134) + INT( 1_w2f__i8))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_1134
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_1133 = (INT(OpenAD_Symbol_1133) + INT(1_w2f__i8 ))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1133
  oad_it_ptr = oad_it_ptr+1
    end if
    if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1129 = oad_it(oad_it_ptr)
  OpenAD_Symbol_1130 = 1
  DO WHILE(INT(OpenAD_Symbol_1130) .LE. INT(OpenAD_Symbol_1129))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1131 = oad_it(oad_it_ptr)
    OpenAD_Symbol_1132 = 1
    DO WHILE(INT(OpenAD_Symbol_1132) .LE. INT(OpenAD_Symbol_1131) )
      OpenAD_Symbol_1132 = INT(OpenAD_Symbol_1132) + 1
    END DO
    OpenAD_Symbol_1130 = INT(OpenAD_Symbol_1130) + 1
  END DO
    end if
  end subroutine DISTANCE
END

!$OPENAD XXX File_start [adism_body.F90]
MODULE adism_body
use OAD_active
use w2f__types
use oad_intrinsics
IMPLICIT NONE
SAVE
!
!     **** Statements ****
!
CONTAINS
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE BODY(N, M, NZ, DX, DY, DT, SIGMA, DEPTH, THCK, TOPG, USRF, PRCP, AR&
     &TM_PRES, LAT, LON, THCKOLD, USRFOLD, DTHCKDTM, DUSRFDTM, DIFF, PDDTAB, BWA&
     &T, BMLT, TEMP, NCDF_OUT, G, VOL, NCDATA, NMSB_CTRL, BSLIDE_CTRL, ENFAC, SL&
     &IDE_FLAG, CALVING_FLAG , CLIM_FLAG, OUTFREQ, T)

          use OAD_tape
          use OAD_rev
          use OAD_cp
          use adism_mbal
          use glimmer_pdd

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  use adism_core
  use adism_ncdf
  use glide_velo
  use glide_tempfullsoln
  use glimmer_deriv ,only: DF_FIELD_2D_STAGGERED
  use adism_mbal
  use glimmer_pdd
  use physcon ,only: SCYR
  use adism_core
  use adism_ncdf
  use glide_velo
  use glide_tempfullsoln
  use glimmer_deriv ,only: DF_FIELD_2D_STAGGERED
  use adism_mbal
  use glimmer_pdd
  use physcon ,only: SCYR
  use adism_core
  use adism_ncdf
  use glide_velo
  use glide_tempfullsoln
  use glimmer_deriv ,only: DF_FIELD_2D_STAGGERED
  use adism_mbal
  use glimmer_pdd
  use physcon ,only: SCYR
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_1141
  INTEGER(w2f__i8) OpenAD_Symbol_1142
  INTEGER(w2f__i8) OpenAD_Symbol_1143
  INTEGER(w2f__i8) OpenAD_Symbol_1144
  INTEGER(w2f__i8) OpenAD_Symbol_1145
  INTEGER(w2f__i8) OpenAD_Symbol_1146
  INTEGER(w2f__i8) OpenAD_Symbol_1147
  INTEGER(w2f__i8) OpenAD_Symbol_1148
  INTEGER(w2f__i8) OpenAD_Symbol_1149
  INTEGER(w2f__i8) OpenAD_Symbol_1150
  INTEGER(w2f__i8) OpenAD_Symbol_1151
  INTEGER(w2f__i8) OpenAD_Symbol_1152
  INTEGER(w2f__i8) OpenAD_Symbol_1153
  INTEGER(w2f__i8) OpenAD_Symbol_1154
  INTEGER(w2f__i8) OpenAD_Symbol_1155
  INTEGER(w2f__i8) OpenAD_Symbol_1156
  INTEGER(w2f__i8) OpenAD_Symbol_1157
  INTEGER(w2f__i8) OpenAD_Symbol_1158
  INTEGER(w2f__i8) OpenAD_Symbol_1159
  INTEGER(w2f__i8) OpenAD_Symbol_1160
  INTEGER(w2f__i8) OpenAD_Symbol_1161
  INTEGER(w2f__i8) OpenAD_Symbol_1162
  INTEGER(w2f__i8) OpenAD_Symbol_1163
  INTEGER(w2f__i8) OpenAD_Symbol_1164
  INTEGER(w2f__i8) OpenAD_Symbol_1165
  INTEGER(w2f__i8) OpenAD_Symbol_1166
  INTEGER(w2f__i8) OpenAD_Symbol_1167
  INTEGER(w2f__i8) OpenAD_Symbol_1168
  INTEGER(w2f__i8) OpenAD_Symbol_1169
  INTEGER(w2f__i8) OpenAD_Symbol_1170
  INTEGER(w2f__i8) OpenAD_Symbol_1171
  INTEGER(w2f__i8) OpenAD_Symbol_1172
  INTEGER(w2f__i8) OpenAD_Symbol_1173
  INTEGER(w2f__i8) OpenAD_Symbol_1174
  INTEGER(w2f__i8) OpenAD_Symbol_1175
  INTEGER(w2f__i8) OpenAD_Symbol_1177
  INTEGER(w2f__i8) OpenAD_Symbol_1178
  INTEGER(w2f__i8) OpenAD_Symbol_1179
  INTEGER(w2f__i8) OpenAD_Symbol_1180
  INTEGER(w2f__i8) OpenAD_Symbol_1181
  INTEGER(w2f__i8) OpenAD_Symbol_1182
  INTEGER(w2f__i8) OpenAD_Symbol_1183
  INTEGER(w2f__i8) OpenAD_Symbol_1184
  INTEGER(w2f__i8) OpenAD_Symbol_1185
  INTEGER(w2f__i8) OpenAD_Symbol_1186
  INTEGER(w2f__i8) OpenAD_Symbol_1187
  INTEGER(w2f__i8) OpenAD_Symbol_1188
  INTEGER(w2f__i8) OpenAD_Symbol_1189
  INTEGER(w2f__i8) OpenAD_Symbol_1190
  INTEGER(w2f__i8) OpenAD_Symbol_1191
  INTEGER(w2f__i8) OpenAD_Symbol_1192
  INTEGER(w2f__i8) OpenAD_Symbol_1193
  INTEGER(w2f__i8) OpenAD_Symbol_1194
  INTEGER(w2f__i8) OpenAD_Symbol_1195
  INTEGER(w2f__i8) OpenAD_Symbol_1196
  INTEGER(w2f__i8) OpenAD_Symbol_1197
  INTEGER(w2f__i8) OpenAD_Symbol_1198
  INTEGER(w2f__i8) OpenAD_Symbol_1199
  INTEGER(w2f__i8) OpenAD_Symbol_1200
  INTEGER(w2f__i8) OpenAD_Symbol_1201
  INTEGER(w2f__i8) OpenAD_Symbol_1202
  INTEGER(w2f__i8) OpenAD_Symbol_1203
  INTEGER(w2f__i8) OpenAD_Symbol_1204
  INTEGER(w2f__i8) OpenAD_Symbol_1205
  INTEGER(w2f__i8) OpenAD_Symbol_1206
  INTEGER(w2f__i8) OpenAD_Symbol_1207
  INTEGER(w2f__i8) OpenAD_Symbol_1208
  INTEGER(w2f__i8) OpenAD_Symbol_1209
  INTEGER(w2f__i8) OpenAD_Symbol_1210
  INTEGER(w2f__i8) OpenAD_Symbol_1211
!
!       **** Parameters and Result ****
!
  INTEGER(w2f__i4) N
  INTEGER(w2f__i4) M
  INTEGER(w2f__i4) NZ
  REAL(w2f__8) DX
  REAL(w2f__8) DY
  REAL(w2f__8) DT
  REAL(w2f__8) SIGMA(1 : NZ)
  REAL(w2f__8) DEPTH(1 : NZ)
  type(active) :: THCK(1:N,1:M)
  REAL(w2f__8) TOPG(1 : N, 1 : M)
  type(active) :: USRF(1:N,1:M)
  REAL(w2f__8) PRCP(1 : N, 1 : M)
  REAL(w2f__8) ARTM_PRES(1 : N, 1 : M)
  REAL(w2f__8) LAT(1 : N, 1 : M)
  REAL(w2f__8) LON(1 : N, 1 : M)
  type(active) :: THCKOLD(1:N,1:M)
  type(active) :: USRFOLD(1:N,1:M)
  type(active) :: DTHCKDTM(1:N,1:M)
  type(active) :: DUSRFDTM(1:N,1:M)
  type(active) :: DIFF(1:N,1:M)
  REAL(w2f__8) PDDTAB(1 : 31, 1 : 71)
  REAL(w2f__8) BWAT(1 : N, 1 : M)
  type(active) :: BMLT(1:N,1:M)
  type(active) :: TEMP(1:NZ,0:INT((N+1)),0:INT((M+1)))
  LOGICAL(w2f__i4) NCDF_OUT
  REAL(w2f__8) G(1 : N, 1 : M)
  type(active) :: VOL
  type(ADISM_NCDATA) :: NCDATA
  type(active) :: NMSB_CTRL(1:N,1:M)
  REAL(w2f__8) BSLIDE_CTRL(1 : N, 1 : M)
  REAL(w2f__8) ENFAC
  LOGICAL(w2f__i4) SLIDE_FLAG
  LOGICAL(w2f__i4) CALVING_FLAG
  INTEGER(w2f__i4) CLIM_FLAG
  INTEGER(w2f__i4) OUTFREQ
  INTEGER(w2f__i4) T
!
!       **** Local Variables and Functions ****
!
  type(active) :: ARTM(1:N,1:M)
  REAL(w2f__8) BSLIDE(1 : N, 1 : M)
  INTEGER(w2f__i4) COUNT
  LOGICAL(w2f__i4) FLOATMASK(1 : N, 1 : M)
  type(active) :: GA2D(1:N,1:M)
  type(active) :: GA3D(1:NZ,1:N,1:M)
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  INTEGER(w2f__i4) MASK(1 : N, 1 : M)
  type(active) :: NEW_USRF(1:N,1:M)
  type(active) :: NMSB(1:N,1:M)
  type(active) :: OAD_CTMP0(1:NZ,1:INT((N+(-1))),1:INT((M+(-1))))
  type(active) :: OAD_CTMP1(1:NZ,1:INT((N+(-1))),1:INT((M+(-1))))
  type(active) :: OAD_CTMP2(1:INT((N+(-1))),1:INT((M+(-1))))
  type(active) :: OAD_CTMP3(1:INT((N+(-1))),1:INT((M+(-1))))
  type(active) :: OAD_CTMP4(1:NZ,1:N,1:M)
  type(active) :: OAD_CTMP5(1:NZ,1:N,1:M)
  type(active) :: OAD_CTMP6(1:NZ,1:N,1:M)
  REAL(w2f__8) OAD_CTMP7
  INTEGER(w2f__i4) OUTPOS
  REAL(w2f__8) RHO
  PARAMETER ( RHO = 1.0D+03)
  LOGICAL(w2f__i4) SLIDE_MASK(1 : N, 1 : M)
  type(active) :: STAGDIFF(1:INT((N+(-1))),1:INT((M+(-1))))
  REAL(w2f__8) STAGTHCK(1 : INT((N +(-1))), 1 : INT((M +(-1))))
  type(active) :: STAGUBAS(1:INT((N+(-1))),1:INT((M+(-1))))
  type(active) :: STAGVBAS(1:INT((N+(-1))),1:INT((M+(-1))))
  type(active) :: SVAL
  REAL(w2f__8) THKLIM
  type(active) :: TSTAR(1:NZ,0:INT((N+1)),0:INT((M+1)))
  type(active) :: UBAS(1:N,1:M)
  type(active) :: UVEL(1:NZ,1:INT((N+(-1))),1:INT((M+(-1))))
  type(active) :: VBAS(1:N,1:M)
  type(active) :: VVEL(1:NZ,1:INT((N+(-1))),1:INT((M+(-1))))
  type(active) :: WGRD(1:NZ,1:N,1:M)
  type(active) :: WVEL(1:NZ,1:N,1:M)
  INTEGER(w2f__i4) select_expr_temp_0
  INTEGER(w2f__i8) OpenAD_Symbol_1176
  INTEGER(w2f__i8) OpenAD_Symbol_1212
  INTEGER(w2f__i4) OpenAD_Symbol_1213
  INTEGER(w2f__i4) OpenAD_Symbol_1214
  INTEGER(w2f__i4) OpenAD_Symbol_1215
  INTEGER(w2f__i4) OpenAD_Symbol_1216
  INTEGER(w2f__i4) OpenAD_Symbol_1217
  INTEGER(w2f__i4) OpenAD_Symbol_1218
  INTEGER(w2f__i4) OpenAD_Symbol_1219
  INTEGER(w2f__i4) OpenAD_Symbol_1220
  INTEGER(w2f__i4) OpenAD_Symbol_1221
  INTEGER(w2f__i4) OpenAD_Symbol_1222
  INTEGER(w2f__i4) OpenAD_Symbol_1223
  INTEGER(w2f__i4) OpenAD_Symbol_1224
  INTEGER(w2f__i4) OpenAD_Symbol_1225
  INTEGER(w2f__i4) OpenAD_Symbol_1226
  INTEGER(w2f__i4) OpenAD_Symbol_1227
  INTEGER(w2f__i4) OpenAD_Symbol_1228
  INTEGER(w2f__i4) OpenAD_Symbol_1229
  INTEGER(w2f__i4) OpenAD_Symbol_1230
  INTEGER(w2f__i4) OpenAD_Symbol_1231
  INTEGER(w2f__i4) OpenAD_Symbol_1232
  INTEGER(w2f__i4) OpenAD_Symbol_1233
  INTEGER(w2f__i4) OpenAD_Symbol_1234
  INTEGER(w2f__i4) OpenAD_Symbol_1235
  INTEGER(w2f__i4) OpenAD_Symbol_1236
  INTEGER(w2f__i4) OpenAD_Symbol_1237
  INTEGER(w2f__i4) OpenAD_Symbol_1238
  INTEGER(w2f__i4) OpenAD_Symbol_1813
  INTEGER(w2f__i4) OpenAD_Symbol_1814
  INTEGER(w2f__i4) OpenAD_Symbol_1815
  INTEGER(w2f__i4) OpenAD_Symbol_1816
  INTEGER(w2f__i4) OpenAD_Symbol_1817
  INTEGER(w2f__i4) OpenAD_Symbol_1818
  INTEGER(w2f__i4) OpenAD_Symbol_1819
  INTEGER(w2f__i4) OpenAD_Symbol_1820
  INTEGER(w2f__i4) OpenAD_Symbol_1821
  INTEGER(w2f__i4) OpenAD_Symbol_1822
  INTEGER(w2f__i4) OpenAD_Symbol_1823
  INTEGER(w2f__i4) OpenAD_Symbol_1824
  INTEGER(w2f__i4) OpenAD_Symbol_1825
  INTEGER(w2f__i4) OpenAD_Symbol_1826
  INTEGER(w2f__i4) OpenAD_Symbol_1827
  INTEGER(w2f__i4) OpenAD_Symbol_1828
  INTEGER(w2f__i4) OpenAD_Symbol_1829
  INTEGER(w2f__i4) OpenAD_Symbol_1830
  INTEGER(w2f__i4) OpenAD_Symbol_1831
  INTEGER(w2f__i4) OpenAD_Symbol_1832
  INTEGER(w2f__i4) OpenAD_Symbol_1833
  INTEGER(w2f__i4) OpenAD_Symbol_1834
  INTEGER(w2f__i4) OpenAD_Symbol_1835
  INTEGER(w2f__i4) OpenAD_Symbol_1836
  INTEGER(w2f__i4) OpenAD_Symbol_1837
  INTEGER(w2f__i4) OpenAD_Symbol_1838
  INTEGER(w2f__i4) OpenAD_Symbol_1839
  INTEGER(w2f__i4) OpenAD_Symbol_1840
  INTEGER(w2f__i4) OpenAD_Symbol_1841
  INTEGER(w2f__i4) OpenAD_Symbol_1842
  INTEGER(w2f__i4) OpenAD_Symbol_1843
  INTEGER(w2f__i4) OpenAD_Symbol_1844
  INTEGER(w2f__i4) OpenAD_Symbol_1845
  INTEGER(w2f__i4) OpenAD_Symbol_1846
  INTEGER(w2f__i4) OpenAD_Symbol_1847
  REAL(w2f__8) OpenAD_Symbol_1848
  REAL(w2f__8) OpenAD_Symbol_1849
  REAL(w2f__8) OpenAD_Symbol_1850
  REAL(w2f__8) OpenAD_Symbol_1851
  INTEGER(w2f__i4) OpenAD_Symbol_1852
  INTEGER(w2f__i4) OpenAD_Symbol_1853
  INTEGER(w2f__i4) OpenAD_Symbol_1854
  INTEGER(w2f__i4) OpenAD_Symbol_1855
  INTEGER(w2f__i4) OpenAD_Symbol_1856
  INTEGER(w2f__i4) OpenAD_Symbol_1857
  INTEGER(w2f__i4) OpenAD_Symbol_1858
  INTEGER(w2f__i4) OpenAD_Symbol_1859
  INTEGER(w2f__i4) OpenAD_Symbol_1860
  INTEGER(w2f__i4) OpenAD_Symbol_1861
  INTEGER(w2f__i4) OpenAD_Symbol_1862
  INTEGER(w2f__i4) OpenAD_Symbol_1863
  INTEGER(w2f__i4) OpenAD_Symbol_1864
  INTEGER(w2f__i4) OpenAD_Symbol_1865
  INTEGER(w2f__i4) OpenAD_Symbol_1866
  INTEGER(w2f__i4) OpenAD_Symbol_1867
  INTEGER(w2f__i4) OpenAD_Symbol_1868
  INTEGER(w2f__i4) OpenAD_Symbol_1869
  INTEGER(w2f__i4) OpenAD_Symbol_1870
  REAL(w2f__8) OpenAD_acc_98
  REAL(w2f__8) OpenAD_aux_104
  REAL(w2f__8) OpenAD_aux_105(:, :)
  ALLOCATABLE OpenAD_aux_105
  REAL(w2f__8) OpenAD_aux_106(:, :)
  ALLOCATABLE OpenAD_aux_106
  REAL(w2f__8) OpenAD_lin_190
  REAL(w2f__8) OpenAD_lin_191
  REAL(w2f__8) OpenAD_lin_192
  REAL(w2f__8) OpenAD_lin_193
  REAL(w2f__8) OpenAD_lin_194
  type(active) :: OpenAD_prp_82(:,:)
  ALLOCATABLE OpenAD_prp_82
  type(active) :: OpenAD_prp_83
  type(active) :: OpenAD_prp_84(:,:)
  ALLOCATABLE OpenAD_prp_84
  type(active) :: OpenAD_prp_85(:,:)
  ALLOCATABLE OpenAD_prp_85
  type(active) :: OpenAD_prp_86(:,:)
  ALLOCATABLE OpenAD_prp_86
  type(active) :: OpenAD_prp_87(:,:)
  ALLOCATABLE OpenAD_prp_87
  type(active) :: OpenAD_prp_88(:,:)
  ALLOCATABLE OpenAD_prp_88
  type(active) :: OpenAD_prp_89(:,:)
  ALLOCATABLE OpenAD_prp_89
  REAL(w2f__8) OpenAD_tyc_117(:, :)
  ALLOCATABLE OpenAD_tyc_117
  REAL(w2f__8) OpenAD_tyc_119(:, :)
  ALLOCATABLE OpenAD_tyc_119
  type(active) :: OpenAD_tyc_121(:,:)
  ALLOCATABLE OpenAD_tyc_121
  REAL(w2f__8) OpenAD_tyc_123(:, :, :)
  ALLOCATABLE OpenAD_tyc_123
  REAL(w2f__8) OpenAD_tyc_125(:, :)
  ALLOCATABLE OpenAD_tyc_125
  REAL(w2f__8) OpenAD_tyc_126(:, :)
  ALLOCATABLE OpenAD_tyc_126
  REAL(w2f__8) OpenAD_tyc_129(:, :)
  ALLOCATABLE OpenAD_tyc_129
  REAL(w2f__8) OpenAD_tyc_13(:, :)
  ALLOCATABLE OpenAD_tyc_13
  REAL(w2f__8) OpenAD_tyc_130(:, :)
  ALLOCATABLE OpenAD_tyc_130
  REAL(w2f__8) OpenAD_tyc_131(:, :)
  ALLOCATABLE OpenAD_tyc_131
  REAL(w2f__8) OpenAD_tyc_132(:, :)
  ALLOCATABLE OpenAD_tyc_132
  REAL(w2f__8) OpenAD_tyc_133(:, :)
  ALLOCATABLE OpenAD_tyc_133
  REAL(w2f__8) OpenAD_tyc_134(:, :)
  ALLOCATABLE OpenAD_tyc_134
  REAL(w2f__8) OpenAD_tyc_135(:, :)
  ALLOCATABLE OpenAD_tyc_135
  REAL(w2f__8) OpenAD_tyc_136(:, :)
  ALLOCATABLE OpenAD_tyc_136
  REAL(w2f__8) OpenAD_tyc_137(:, :)
  ALLOCATABLE OpenAD_tyc_137
  REAL(w2f__8) OpenAD_tyc_138(:, :)
  ALLOCATABLE OpenAD_tyc_138
  REAL(w2f__8) OpenAD_tyc_139(:, :)
  ALLOCATABLE OpenAD_tyc_139
  REAL(w2f__8) OpenAD_tyc_14(:, :)
  ALLOCATABLE OpenAD_tyc_14
  REAL(w2f__8) OpenAD_tyc_140(:, :, :)
  ALLOCATABLE OpenAD_tyc_140
  REAL(w2f__8) OpenAD_tyc_141(:, :, :)
  ALLOCATABLE OpenAD_tyc_141
  REAL(w2f__8) OpenAD_tyc_142(:, :, :)
  ALLOCATABLE OpenAD_tyc_142
  REAL(w2f__8) OpenAD_tyc_143(:, :, :)
  ALLOCATABLE OpenAD_tyc_143
  REAL(w2f__8) OpenAD_tyc_144(:, :, :)
  ALLOCATABLE OpenAD_tyc_144
  REAL(w2f__8) OpenAD_tyc_145(:, :, :)
  ALLOCATABLE OpenAD_tyc_145
  REAL(w2f__8) OpenAD_tyc_15(:, :)
  ALLOCATABLE OpenAD_tyc_15
  REAL(w2f__8) OpenAD_tyc_16(:, :)
  ALLOCATABLE OpenAD_tyc_16
  REAL(w2f__8) OpenAD_tyc_17(:, :)
  ALLOCATABLE OpenAD_tyc_17
  REAL(w2f__8) OpenAD_tyc_18(:, :)
  ALLOCATABLE OpenAD_tyc_18
  REAL(w2f__8) OpenAD_tyc_19(:, :)
  ALLOCATABLE OpenAD_tyc_19
  REAL(w2f__8) OpenAD_tyc_20(:, :)
  ALLOCATABLE OpenAD_tyc_20
  REAL(w2f__8) OpenAD_tyc_21(:, :)
  ALLOCATABLE OpenAD_tyc_21
  REAL(w2f__8) OpenAD_tyc_22(:, :)
  ALLOCATABLE OpenAD_tyc_22
  REAL(w2f__8) OpenAD_tyc_23(:, :)
  ALLOCATABLE OpenAD_tyc_23
  REAL(w2f__8) OpenAD_tyc_24(:, :, :)
  ALLOCATABLE OpenAD_tyc_24
  REAL(w2f__8) OpenAD_tyc_25(:, :, :)
  ALLOCATABLE OpenAD_tyc_25
  REAL(w2f__8) OpenAD_tyc_26(:, :, :)
  ALLOCATABLE OpenAD_tyc_26
  REAL(w2f__8) OpenAD_tyc_27(:, :, :)
  ALLOCATABLE OpenAD_tyc_27
  REAL(w2f__8) OpenAD_tyc_28(:, :, :)
  ALLOCATABLE OpenAD_tyc_28
  REAL(w2f__8) OpenAD_tyc_29(:, :, :)
  ALLOCATABLE OpenAD_tyc_29
  REAL(w2f__8) OpenAD_tyc_30(:, :)
  ALLOCATABLE OpenAD_tyc_30
  REAL(w2f__8) OpenAD_tyc_31(:, :)
  ALLOCATABLE OpenAD_tyc_31
  REAL(w2f__8) OpenAD_tyc_32(:, :)
  ALLOCATABLE OpenAD_tyc_32
  REAL(w2f__8) OpenAD_tyc_33(:, :, :)
  ALLOCATABLE OpenAD_tyc_33
  type(active) :: OpenAD_tyc_34(:,:)
  ALLOCATABLE OpenAD_tyc_34
  REAL(w2f__8) OpenAD_tyc_35(:, :)
  ALLOCATABLE OpenAD_tyc_35
  REAL(w2f__8) OpenAD_tyc_36(:, :)
  ALLOCATABLE OpenAD_tyc_36
  REAL(w2f__8) OpenAD_tyc_37(:, :)
  ALLOCATABLE OpenAD_tyc_37
  REAL(w2f__8) OpenAD_tyc_38(:, :)
  ALLOCATABLE OpenAD_tyc_38
  REAL(w2f__8) OpenAD_tyc_39(:, :)
  ALLOCATABLE OpenAD_tyc_39
  REAL(w2f__8) OpenAD_tyc_40(:, :)
  ALLOCATABLE OpenAD_tyc_40
  REAL(w2f__8) OpenAD_tyc_41(:, :)
  ALLOCATABLE OpenAD_tyc_41
  REAL(w2f__8) OpenAD_tyc_42(:, :)
  ALLOCATABLE OpenAD_tyc_42
  REAL(w2f__8) OpenAD_tyc_43(:, :)
  ALLOCATABLE OpenAD_tyc_43
  REAL(w2f__8) OpenAD_tyc_44(:, :)
  ALLOCATABLE OpenAD_tyc_44
  REAL(w2f__8) OpenAD_tyc_45(:, :)
  ALLOCATABLE OpenAD_tyc_45
  REAL(w2f__8) OpenAD_tyc_46(:, :)
  ALLOCATABLE OpenAD_tyc_46
  REAL(w2f__8) OpenAD_tyc_47(:, :, :)
  ALLOCATABLE OpenAD_tyc_47
  REAL(w2f__8) OpenAD_tyc_48(:, :, :)
  ALLOCATABLE OpenAD_tyc_48
  REAL(w2f__8) OpenAD_tyc_49(:, :, :)
  ALLOCATABLE OpenAD_tyc_49
  REAL(w2f__8) OpenAD_tyc_50(:, :, :)
  ALLOCATABLE OpenAD_tyc_50
  REAL(w2f__8) OpenAD_tyc_51(:, :, :)
  ALLOCATABLE OpenAD_tyc_51
  REAL(w2f__8) OpenAD_tyc_52(:, :, :)
  ALLOCATABLE OpenAD_tyc_52
  REAL(w2f__8) OpenAD_tyc_83(:, :)
  ALLOCATABLE OpenAD_tyc_83
  REAL(w2f__8) OpenAD_tyc_84(:, :)
  ALLOCATABLE OpenAD_tyc_84
  REAL(w2f__8) OpenAD_tyc_85(:, :)
  ALLOCATABLE OpenAD_tyc_85
  REAL(w2f__8) OpenAD_tyc_86(:, :)
  ALLOCATABLE OpenAD_tyc_86
  REAL(w2f__8) OpenAD_tyc_87(:, :)
  ALLOCATABLE OpenAD_tyc_87
  REAL(w2f__8) OpenAD_tyc_88(:, :)
  ALLOCATABLE OpenAD_tyc_88
  REAL(w2f__8) OpenAD_tyc_89(:, :)
  ALLOCATABLE OpenAD_tyc_89
  REAL(w2f__8) OpenAD_tyc_90(:, :)
  ALLOCATABLE OpenAD_tyc_90
  REAL(w2f__8) OpenAD_tyc_91(:, :)
  ALLOCATABLE OpenAD_tyc_91
  REAL(w2f__8) OpenAD_tyc_92(:, :)
  ALLOCATABLE OpenAD_tyc_92
  REAL(w2f__8) OpenAD_tyc_93(:, :)
  ALLOCATABLE OpenAD_tyc_93
  REAL(w2f__8) OpenAD_tyc_94(:, :, :)
  ALLOCATABLE OpenAD_tyc_94
  REAL(w2f__8) OpenAD_tyc_95(:, :, :)
  ALLOCATABLE OpenAD_tyc_95
  REAL(w2f__8) OpenAD_tyc_96(:, :, :)
  ALLOCATABLE OpenAD_tyc_96
  REAL(w2f__8) OpenAD_tyc_97(:, :, :)
  ALLOCATABLE OpenAD_tyc_97
  REAL(w2f__8) OpenAD_tyc_98(:, :, :)
  ALLOCATABLE OpenAD_tyc_98
  REAL(w2f__8) OpenAD_tyc_99(:, :, :)
  ALLOCATABLE OpenAD_tyc_99


          integer iaddr
          external iaddr
!
!       **** Statements ****
!

         if (our_rev_mode%plain) then
! original function
 2      CONTINUE
  GO TO 3
 3      CONTINUE
!$OPENAD XXX Template body_template.split.f90
  GA3D(1:INT(NZ),1:INT(N),1:INT(M))%v = 0.0
  GA2D(1:INT(N),1:INT(M))%v = 0.0
  UVEL(1:INT(NZ),1:N+(-1),1:M+(-1))%v = 0.0
  VVEL(1:INT(NZ),1:N+(-1),1:M+(-1))%v = 0.0
  UBAS(1:INT(N),1:INT(M))%v = 0.0
  VBAS(1:INT(N),1:INT(M))%v = 0.0
  BSLIDE(1:INT(N),1:INT(M)) = 0.0
  NMSB(1:INT(N),1:INT(M))%v = 0.0
  STAGUBAS(1:N+(-1),1:M+(-1))%v = 0.0
  STAGVBAS(1:N+(-1),1:M+(-1))%v = 0.0
  STAGDIFF(1:N+(-1),1:M+(-1))%v = 0.0
  WVEL(1:INT(NZ),1:INT(N),1:INT(M))%v = 0.0
  WGRD(1:INT(NZ),1:INT(N),1:INT(M))%v = 0.0
  THKLIM = 2.0D+02
  FLOATMASK(1:N,1:M) = .false.
  GO TO 4
 4      CONTINUE
  IF ((T.eq.1).AND.NCDF_OUT) THEN
    GO TO 6
  ELSE
    GO TO 5
  ENDIF
 5      CONTINUE
  GO TO 7
 6      CONTINUE
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_13,THCK)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_13,THCK)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_14,USRF)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_14,USRF)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_15,NMSB)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_15,NMSB)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_16,DIFF)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_16,DIFF)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_17,STAGDIFF)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_17,STAGDIFF)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_18,BMLT)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_18,BMLT)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_19,ARTM)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_19,ARTM)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_20,DUSRFDTM)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_20,DUSRFDTM)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_21,DTHCKDTM)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_21,DTHCKDTM)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_22,STAGUBAS)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_22,STAGUBAS)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_23,STAGVBAS)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_23,STAGVBAS)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_24,TEMP)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_24,TEMP)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_25,UVEL)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_25,UVEL)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_26,VVEL)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_26,VVEL)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_27,WVEL)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_27,WVEL)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_28,WGRD)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_28,WGRD)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_29,GA3D)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_29,GA3D)
  CALL NETCDF_WRITE(NCDATA,SIGMA,LAT,LON,OpenAD_tyc_13,OpenAD_tyc_14,TOPG,OpenAD&
     &_tyc_15,PRCP,OpenAD_tyc_16,OpenAD_tyc_17,BWAT,OpenAD_tyc_18,OpenAD_tyc_19,&
     &ARTM_PRES,OpenAD_tyc_20,OpenAD_tyc_21,OpenAD_tyc_22,OpenAD_tyc_23,OpenAD_t&
     &yc_24,OpenAD_tyc_25,OpenAD_tyc_26,OpenAD_tyc_27,OpenAD_tyc_28,OpenAD_tyc_2&
     &9,MASK,1)

  GO TO 7
 7      CONTINUE
  GO TO 8
 8      CONTINUE
  select_expr_temp_0 = CLIM_FLAG
  GO TO 9
 9      CONTINUE
  IF (select_expr_temp_0.EQ.1) go to 12
  IF (select_expr_temp_0.EQ.2) go to 11
  GO TO 10
 10     CONTINUE
  WRITE(*,*) 'Unknown climate'
  STOP
  RETURN
  GO TO 81
 11     CONTINUE
  CALL GREENLAND_MASSBALANCE(N,M,USRF,LAT,PRCP,ARTM,ARTM_PRES,NMSB,PDDTAB)
  GO TO 13
 12     CONTINUE
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_30,NMSB)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_30,NMSB)
  CALL MASS_BALANCE_MM(OpenAD_tyc_30,DX,DY,N,M)
!!! requested inline of 'oad_ShapeTest' has no defn
  CALL oad_ShapeTest(OpenAD_tyc_30,NMSB)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(NMSB,OpenAD_tyc_30)
  GO TO 13
 13     CONTINUE
  GO TO 14
 14     CONTINUE
  NMSB(1:INT(N),1:INT(M))%v = (NMSB_CTRL(1:N,1:M)%v+NMSB(1:N,1:M)%v)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_31,THCK)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_31,THCK)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_32,NMSB)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_32,NMSB)
  CALL MASKER(N,M,OpenAD_tyc_31,OpenAD_tyc_32,MASK,COUNT)
  CALL CALCTSTAR(N,M,NZ,TSTAR,TEMP,THCK,SIGMA)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_33,TSTAR)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_33,TSTAR)
  CALL GREENLAND_BASALSLIDING(N,M,NZ,OpenAD_tyc_33,BSLIDE,SLIDE_MASK,SLIDE_FLAG)
!!! requested inline of 'oad_ShapeTest' has no defn
  CALL oad_ShapeTest(OpenAD_tyc_33,TSTAR)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(TSTAR,OpenAD_tyc_33)
  BSLIDE(1:INT(N),1:INT(M)) = (BSLIDE_CTRL(1:N,1:M)+BSLIDE(1:N,1:M))
  CALL CALCGLENA(N,M,NZ,GA3D,TSTAR,ENFAC)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_34,STAGTHCK)
  CALL STAGVARB(THCK,OpenAD_tyc_34,N,M)
!!! requested inline of 'oad_ShapeTest' has no defn
  CALL oad_ShapeTest(OpenAD_tyc_34,STAGTHCK)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(STAGTHCK,OpenAD_tyc_34)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_35,THCK)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_35,THCK)
  CALL CALC_GAINTEGRAL(N,M,NZ,SIGMA,DEPTH,GA2D,OpenAD_tyc_35,GA3D)
  CALL CALCDIFF(N,M,DIFF,USRF,THCK,GA2D,BSLIDE,SLIDE_MASK,DX,DY)
  CALL STAGVARB(DIFF,STAGDIFF,N,M)
  CALL CALCTHCK(N,M,DIFF,USRF,NMSB,MASK,DX,DY,DT,COUNT,NEW_USRF)
  THCK(1:INT(N),1:INT(M))%v = (NEW_USRF(1:N,1:M)%v-TOPG(1:N,1:M))
  USRF(1:INT(N),1:INT(M))%v = NEW_USRF(1:N,1:M)%v
  GO TO 15
 15     CONTINUE
  IF (CLIM_FLAG.eq.1) THEN
    GO TO 17
  ELSE
    GO TO 16
  ENDIF
 16     CONTINUE
  GO TO 26
 17     CONTINUE
  GO TO 18
 18     CONTINUE
  I = 1
  GO TO 82
 19     CONTINUE
  I = I+1
 82     CONTINUE
  IF (I.LE.N) THEN
    GO TO 20
  ELSE
    GO TO 26
  ENDIF
 20     CONTINUE
  GO TO 21
 21     CONTINUE
  J = 1
  GO TO 83
 22     CONTINUE
  J = J+1
 83     CONTINUE
  IF (J.LE.M) THEN
    GO TO 23
  ELSE
    GO TO 25
  ENDIF
 23     CONTINUE
  ARTM(INT(I),INT(J))%v = (SQRT((((I+(-1))*DX+(-7.5D+05))**2)+(((J+(-1))*DY+(-7.&
     &5D+05))**2))*1.66999998327810317278D-05+2.38149993896484375D+02+(-2.731499&
     &93896484375D+02))

  GO TO 24
 24     CONTINUE
  GO TO 22
 25     CONTINUE
  GO TO 19
 26     CONTINUE
  GO TO 27
 27     CONTINUE
  SVAL%v = 0.0D00
  GO TO 28
 28     CONTINUE
  I = 1
  GO TO 84
 29     CONTINUE
  I = I+1
 84     CONTINUE
  IF (I.LE.N) THEN
    GO TO 30
  ELSE
    GO TO 40
  ENDIF
 30     CONTINUE
  GO TO 31
 31     CONTINUE
  J = 1
  GO TO 85
 32     CONTINUE
  J = J+1
 85     CONTINUE
  IF (J.LE.M) THEN
    GO TO 33
  ELSE
    GO TO 39
  ENDIF
 33     CONTINUE
  IF (THCK(I,J)%v.LT.0.0D00) THEN
    GO TO 35
  ELSE
    GO TO 34
  ENDIF
 34     CONTINUE
  GO TO 36
 35     CONTINUE
  THCK(INT(I),INT(J))%v = 0.0
  GO TO 36
 36     CONTINUE
  GO TO 37
 37     CONTINUE
  SVAL%v = (THCK(I,J)%v+SVAL%v)
  GO TO 38
 38     CONTINUE
  GO TO 32
 39     CONTINUE
  GO TO 29
 40     CONTINUE
  IF(CALVING_FLAG) THEN
    GO TO 42
  ELSE
    GO TO 41
  ENDIF
 41     CONTINUE
  GO TO 54
 42     CONTINUE
  GO TO 43
 43     CONTINUE
  I = 1
  GO TO 86
 44     CONTINUE
  I = I+1
 86     CONTINUE
  IF (I.LE.N) THEN
    GO TO 45
  ELSE
    GO TO 54
  ENDIF
 45     CONTINUE
  GO TO 46
 46     CONTINUE
  J = 1
  GO TO 87
 47     CONTINUE
  J = J+1
 87     CONTINUE
  IF (J.LE.M) THEN
    GO TO 48
  ELSE
    GO TO 53
  ENDIF
 48     CONTINUE
  IF (THCK(I,J)%v.LT.(2.0D+02-TOPG(I,J)*1.09890109890109899382D00)) THEN
    GO TO 50
  ELSE
    GO TO 49
  ENDIF
 49     CONTINUE
  GO TO 51
 50     CONTINUE
  THCK(INT(I),INT(J))%v = 0.0
  GO TO 51
 51     CONTINUE
  GO TO 52
 52     CONTINUE
  GO TO 47
 53     CONTINUE
  GO TO 44
 54     CONTINUE
  GO TO 55
 55     CONTINUE
  USRF(1:INT(N),1:INT(M))%v = (THCK(1:N,1:M)%v+TOPG(1:N,1:M))
  CALL GREENLAND_BASALVELOCITY(N,M,DX,DY,UBAS,VBAS,USRF,THCK,BSLIDE,SLIDE_MASK)
  CALL STAGVARB(UBAS,STAGUBAS,N,M)
  CALL STAGVARB(VBAS,STAGVBAS,N,M)
  GO TO 56
 56     CONTINUE
  I = 1
  GO TO 88
 57     CONTINUE
  I = I+1
 88     CONTINUE
  IF (I.LE.N) THEN
    GO TO 58
  ELSE
    GO TO 71
  ENDIF
 58     CONTINUE
  GO TO 59
 59     CONTINUE
  J = 1
  GO TO 89
 60     CONTINUE
  J = J+1
 89     CONTINUE
  IF (J.LE.M) THEN
    GO TO 61
  ELSE
    GO TO 70
  ENDIF
 61     CONTINUE
  IF (MASK(I,J).eq.0) THEN
    GO TO 63
  ELSE
    GO TO 62
  ENDIF
 62     CONTINUE
  GO TO 64
 63     CONTINUE
  USRF(INT(I),INT(J))%v = TOPG(I,J)
  GO TO 64
 64     CONTINUE
  GO TO 65
 65     CONTINUE
  IF (TOPG(I,J).GT.USRF(I,J)%v) THEN
    GO TO 67
  ELSE
    GO TO 66
  ENDIF
 66     CONTINUE
  GO TO 68
 67     CONTINUE
  USRF(INT(I),INT(J))%v = TOPG(I,J)
  GO TO 68
 68     CONTINUE
  GO TO 69
 69     CONTINUE
  GO TO 60
 70     CONTINUE
  GO TO 57
 71     CONTINUE
  CALL CALC_VELO(N,M,NZ,DX,DY,SIGMA,GA3D,STAGDIFF,USRF,THCK,STAGUBAS,STAGVBAS,UV&
     &EL,VVEL)

  CALL CALCVERTICALVELOCITY(N,M,NZ,DX,DY,SIGMA,1,THKLIM,THCK,USRF,DTHCKDTM,DUSRF&
     &DTM,BMLT,NMSB,UVEL,VVEL,WVEL,WGRD)

  OAD_CTMP0(1:INT(NZ),1:N+(-1),1:M+(-1))%v = (UVEL(1:NZ,1:N+(-1),1:M+(-1))%v*3.1&
     &556926D+07)

  OAD_CTMP1(1:INT(NZ),1:N+(-1),1:M+(-1))%v = (VVEL(1:NZ,1:N+(-1),1:M+(-1))%v*3.1&
     &556926D+07)

  OAD_CTMP2(1:N+(-1),1:M+(-1))%v = (STAGUBAS(1:N+(-1),1:M+(-1))%v*3.1556926D+07)
  OAD_CTMP3(1:N+(-1),1:M+(-1))%v = (STAGVBAS(1:N+(-1),1:M+(-1))%v*3.1556926D+07)
  OAD_CTMP4(1:INT(NZ),1:INT(N),1:INT(M))%v = (WVEL(1:NZ,1:N,1:M)%v*3.1556926D+07&
     &)

  OAD_CTMP5(1:INT(NZ),1:INT(N),1:INT(M))%v = (WGRD(1:NZ,1:N,1:M)%v*3.1556926D+07&
     &)

  OAD_CTMP6(1:INT(NZ),1:INT(N),1:INT(M))%v = (GA3D(1:NZ,1:N,1:M)%v*3.1556926D+07&
     &)

  OAD_CTMP7 = (DT/3.1556926D+07)
  CALL TSTEP_TEMPFULLSOLN(N,M,NZ,DX,DY,SIGMA,TEMP,ARTM,THCK,USRF,OAD_CTMP0,OAD_C&
     &TMP1,OAD_CTMP2,OAD_CTMP3,OAD_CTMP4,OAD_CTMP5,OAD_CTMP6,G,BWAT,BMLT,OAD_CTM&
     &P7,THKLIM,FLOATMASK)

  BMLT(1:INT(N),1:INT(M))%v = (BMLT(1:N,1:M)%v/3.1556926D+07)
  VOL%v = (DY*DX*SVAL%v)
  WRITE(*,*) T,'Ice volume',VOL%v
  DUSRFDTM(1:INT(N),1:INT(M))%v = ((USRF(1:N,1:M)%v-USRFOLD(1:N,1:M)%v)/DT)
  DTHCKDTM(1:INT(N),1:INT(M))%v = ((THCK(1:N,1:M)%v-THCKOLD(1:N,1:M)%v)/DT)
  USRFOLD(1:INT(N),1:INT(M))%v = USRF(1:N,1:M)%v
  THCKOLD(1:INT(N),1:INT(M))%v = THCK(1:N,1:M)%v
  GO TO 72
 72     CONTINUE
  IF (MOD(T,OUTFREQ).eq.0) THEN
    GO TO 74
  ELSE
    GO TO 73
  ENDIF
 73     CONTINUE
  GO TO 79
 74     CONTINUE
  OUTPOS = ((T/OUTFREQ)+1)
  GO TO 75
 75     CONTINUE
  IF(NCDF_OUT) THEN
    GO TO 77
  ELSE
    GO TO 76
  ENDIF
 76     CONTINUE
  GO TO 78
 77     CONTINUE
  WRITE(*,*) 'Writing to file at time:',T
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_36,THCK)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_36,THCK)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_37,USRF)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_37,USRF)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_38,NMSB)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_38,NMSB)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_39,DIFF)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_39,DIFF)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_40,STAGDIFF)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_40,STAGDIFF)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_41,BMLT)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_41,BMLT)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_42,ARTM)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_42,ARTM)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_43,DUSRFDTM)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_43,DUSRFDTM)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_44,DTHCKDTM)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_44,DTHCKDTM)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_45,STAGUBAS)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_45,STAGUBAS)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_46,STAGVBAS)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_46,STAGVBAS)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_47,TEMP)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_47,TEMP)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_48,UVEL)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_48,UVEL)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_49,VVEL)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_49,VVEL)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_50,WVEL)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_50,WVEL)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_51,WGRD)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_51,WGRD)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_52,GA3D)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_52,GA3D)
  CALL NETCDF_WRITE(NCDATA,SIGMA,LAT,LON,OpenAD_tyc_36,OpenAD_tyc_37,TOPG,OpenAD&
     &_tyc_38,PRCP,OpenAD_tyc_39,OpenAD_tyc_40,BWAT,OpenAD_tyc_41,OpenAD_tyc_42,&
     &ARTM_PRES,OpenAD_tyc_43,OpenAD_tyc_44,OpenAD_tyc_45,OpenAD_tyc_46,OpenAD_t&
     &yc_47,OpenAD_tyc_48,OpenAD_tyc_49,OpenAD_tyc_50,OpenAD_tyc_51,OpenAD_tyc_5&
     &2,MASK,OUTPOS)

  GO TO 78
 78     CONTINUE
  GO TO 79
 79     CONTINUE
  GO TO 80
 80     CONTINUE
  GO TO 81
 81     CONTINUE
  GO TO 1
 1      CONTINUE
          end if
          if (our_rev_mode%tape) then
! taping
 91     CONTINUE
  GO TO 92
 92     CONTINUE
  OpenAD_Symbol_1141 = 0_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1141
  oad_it_ptr = oad_it_ptr+1
  GO TO 93
 93     CONTINUE
!$OPENAD XXX Template body_template.split.f90
  GA3D(1:INT(NZ),1:INT(N),1:INT(M))%v = 0.0
  GA2D(1:INT(N),1:INT(M))%v = 0.0
  UVEL(1:INT(NZ),1:N+(-1),1:M+(-1))%v = 0.0
  VVEL(1:INT(NZ),1:N+(-1),1:M+(-1))%v = 0.0
  UBAS(1:INT(N),1:INT(M))%v = 0.0
  VBAS(1:INT(N),1:INT(M))%v = 0.0
  BSLIDE(1:INT(N),1:INT(M)) = 0.0
  NMSB(1:INT(N),1:INT(M))%v = 0.0
  STAGUBAS(1:N+(-1),1:M+(-1))%v = 0.0
  STAGVBAS(1:N+(-1),1:M+(-1))%v = 0.0
  STAGDIFF(1:N+(-1),1:M+(-1))%v = 0.0
  WVEL(1:INT(NZ),1:INT(N),1:INT(M))%v = 0.0
  WGRD(1:INT(NZ),1:INT(N),1:INT(M))%v = 0.0
  THKLIM = 2.0D+02
  OpenAD_Symbol_1213 = (N+(-1))
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1213
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_1214 = (M+(-1))
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1214
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_1215 = (N+(-1))
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1215
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_1216 = (M+(-1))
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1216
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_1217 = (N+(-1))
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1217
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_1218 = (M+(-1))
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1218
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_1219 = (N+(-1))
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1219
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_1220 = (M+(-1))
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1220
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_1221 = (N+(-1))
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1221
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_1222 = (M+(-1))
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1222
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = NZ
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = N
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = M
  oad_it_ptr = oad_it_ptr+1
  FLOATMASK(1:N,1:M) = .false.
  GO TO 94
 94     CONTINUE
  OpenAD_Symbol_1142 = 1_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1142
  oad_it_ptr = oad_it_ptr+1
  GO TO 95
 95     CONTINUE
  IF ((T.eq.1).AND.NCDF_OUT) THEN
    GO TO 98
  ELSE
    GO TO 96
  ENDIF
 96     CONTINUE
  GO TO 97
 97     CONTINUE
  OpenAD_Symbol_1175 = 34_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1175
  oad_it_ptr = oad_it_ptr+1
  GO TO 100
 98     CONTINUE
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_13,THCK)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_13,THCK)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_14,USRF)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_14,USRF)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_15,NMSB)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_15,NMSB)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_16,DIFF)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_16,DIFF)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_17,STAGDIFF)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_17,STAGDIFF)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_18,BMLT)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_18,BMLT)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_19,ARTM)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_19,ARTM)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_20,DUSRFDTM)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_20,DUSRFDTM)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_21,DTHCKDTM)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_21,DTHCKDTM)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_22,STAGUBAS)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_22,STAGUBAS)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_23,STAGVBAS)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_23,STAGVBAS)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_24,TEMP)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_24,TEMP)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_25,UVEL)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_25,UVEL)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_26,VVEL)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_26,VVEL)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_27,WVEL)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_27,WVEL)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_28,WGRD)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_28,WGRD)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_29,GA3D)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_29,GA3D)
  CALL NETCDF_WRITE(NCDATA,SIGMA,LAT,LON,OpenAD_tyc_13,OpenAD_tyc_14,TOPG,OpenAD&
     &_tyc_15,PRCP,OpenAD_tyc_16,OpenAD_tyc_17,BWAT,OpenAD_tyc_18,OpenAD_tyc_19,&
     &ARTM_PRES,OpenAD_tyc_20,OpenAD_tyc_21,OpenAD_tyc_22,OpenAD_tyc_23,OpenAD_t&
     &yc_24,OpenAD_tyc_25,OpenAD_tyc_26,OpenAD_tyc_27,OpenAD_tyc_28,OpenAD_tyc_2&
     &9,MASK,1)

  GO TO 99
 99     CONTINUE
  OpenAD_Symbol_1143 = 2_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1143
  oad_it_ptr = oad_it_ptr+1
  GO TO 100
 100    CONTINUE
  GO TO 101
 101    CONTINUE
  select_expr_temp_0 = CLIM_FLAG
  GO TO 102
 102    CONTINUE
  OpenAD_Symbol_1144 = 3_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1144
  oad_it_ptr = oad_it_ptr+1
  GO TO 103
 103    CONTINUE
  IF (select_expr_temp_0.EQ.1) go to 108
  IF (select_expr_temp_0.EQ.2) go to 106
  GO TO 104
 104    CONTINUE
  WRITE(*,*) 'Unknown climate'
  STOP
  RETURN
  GO TO 105
 105    CONTINUE
  OpenAD_Symbol_1174 = 33_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1174
  oad_it_ptr = oad_it_ptr+1
  GO TO 205
 106    CONTINUE
  CALL GREENLAND_MASSBALANCE(N,M,USRF,LAT,PRCP,ARTM,ARTM_PRES,NMSB,PDDTAB)
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = N
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = M
  oad_it_ptr = oad_it_ptr+1
  GO TO 107
 107    CONTINUE
  OpenAD_Symbol_1173 = 32_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1173
  oad_it_ptr = oad_it_ptr+1
  GO TO 110
 108    CONTINUE
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_30,NMSB)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_30,NMSB)
  CALL MASS_BALANCE_MM(OpenAD_tyc_30,DX,DY,N,M)
!!! requested inline of 'oad_ShapeTest' has no defn
  CALL oad_ShapeTest(OpenAD_tyc_30,NMSB)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(NMSB,OpenAD_tyc_30)
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = N
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = M
  oad_it_ptr = oad_it_ptr+1
  GO TO 109
 109    CONTINUE
  OpenAD_Symbol_1145 = 4_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1145
  oad_it_ptr = oad_it_ptr+1
  GO TO 110
 110    CONTINUE
  GO TO 111
 111    CONTINUE
  NMSB(1:INT(N),1:INT(M))%v = (NMSB_CTRL(1:N,1:M)%v+NMSB(1:N,1:M)%v)
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = N
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = M
  oad_it_ptr = oad_it_ptr+1
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_31,THCK)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_31,THCK)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_32,NMSB)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_32,NMSB)
  CALL MASKER(N,M,OpenAD_tyc_31,OpenAD_tyc_32,MASK,COUNT)
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = N
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = M
  oad_it_ptr = oad_it_ptr+1
  CALL CALCTSTAR(N,M,NZ,TSTAR,TEMP,THCK,SIGMA)
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = N
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = M
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = NZ
  oad_it_ptr = oad_it_ptr+1
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_33,TSTAR)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_33,TSTAR)
  CALL GREENLAND_BASALSLIDING(N,M,NZ,OpenAD_tyc_33,BSLIDE,SLIDE_MASK,SLIDE_FLAG)
!!! requested inline of 'oad_ShapeTest' has no defn
  CALL oad_ShapeTest(OpenAD_tyc_33,TSTAR)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(TSTAR,OpenAD_tyc_33)
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = N
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = M
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = NZ
  oad_it_ptr = oad_it_ptr+1
  BSLIDE(1:INT(N),1:INT(M)) = (BSLIDE_CTRL(1:N,1:M)+BSLIDE(1:N,1:M))
  CALL CALCGLENA(N,M,NZ,GA3D,TSTAR,ENFAC)
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = N
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = M
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = NZ
  oad_it_ptr = oad_it_ptr+1
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_34,STAGTHCK)
  CALL STAGVARB(THCK,OpenAD_tyc_34,N,M)
!!! requested inline of 'oad_ShapeTest' has no defn
  CALL oad_ShapeTest(OpenAD_tyc_34,STAGTHCK)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(STAGTHCK,OpenAD_tyc_34)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_35,THCK)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_35,THCK)
  CALL CALC_GAINTEGRAL(N,M,NZ,SIGMA,DEPTH,GA2D,OpenAD_tyc_35,GA3D)
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = N
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = M
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = NZ
  oad_it_ptr = oad_it_ptr+1
  CALL CALCDIFF(N,M,DIFF,USRF,THCK,GA2D,BSLIDE,SLIDE_MASK,DX,DY)
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = N
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = M
  oad_it_ptr = oad_it_ptr+1
  CALL STAGVARB(DIFF,STAGDIFF,N,M)
  CALL CALCTHCK(N,M,DIFF,USRF,NMSB,MASK,DX,DY,DT,COUNT,NEW_USRF)
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = N
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = M
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = COUNT
  oad_it_ptr = oad_it_ptr+1
  THCK(1:INT(N),1:INT(M))%v = (NEW_USRF(1:N,1:M)%v-TOPG(1:N,1:M))
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = N
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = M
  oad_it_ptr = oad_it_ptr+1
  USRF(1:INT(N),1:INT(M))%v = NEW_USRF(1:N,1:M)%v
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = N
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = M
  oad_it_ptr = oad_it_ptr+1
  GO TO 112
 112    CONTINUE
  OpenAD_Symbol_1146 = 5_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1146
  oad_it_ptr = oad_it_ptr+1
  GO TO 113
 113    CONTINUE
  IF (CLIM_FLAG.eq.1) THEN
    GO TO 116
  ELSE
    GO TO 114
  ENDIF
 114    CONTINUE
  GO TO 115
 115    CONTINUE
  OpenAD_Symbol_1172 = 31_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1172
  oad_it_ptr = oad_it_ptr+1
  GO TO 128
 116    CONTINUE
  GO TO 117
 117    CONTINUE
  OpenAD_Symbol_1147 = 6_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1147
  oad_it_ptr = oad_it_ptr+1
  GO TO 118
 118    CONTINUE
  I = 1
  GO TO 206
 119    CONTINUE
  I = I+1
 206    CONTINUE
  IF (I.LE.N) THEN
    GO TO 120
  ELSE
    GO TO 128
  ENDIF
 120    CONTINUE
  GO TO 121
 121    CONTINUE
  OpenAD_Symbol_1148 = 7_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1148
  oad_it_ptr = oad_it_ptr+1
  GO TO 122
 122    CONTINUE
  J = 1
  GO TO 207
 123    CONTINUE
  J = J+1
 207    CONTINUE
  IF (J.LE.M) THEN
    GO TO 124
  ELSE
    GO TO 127
  ENDIF
 124    CONTINUE
  ARTM(INT(I),INT(J))%v = (SQRT((((I+(-1))*DX+(-7.5D+05))**2)+(((J+(-1))*DY+(-7.&
     &5D+05))**2))*1.66999998327810317278D-05+2.38149993896484375D+02+(-2.731499&
     &93896484375D+02))

  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = I
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = J
  oad_it_ptr = oad_it_ptr+1
  GO TO 125
 125    CONTINUE
  OpenAD_Symbol_1149 = 8_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1149
  oad_it_ptr = oad_it_ptr+1
  GO TO 126
 126    CONTINUE
  GO TO 123
 127    CONTINUE
  GO TO 119
 128    CONTINUE
  GO TO 129
 129    CONTINUE
  SVAL%v = 0.0D00
  GO TO 130
 130    CONTINUE
  OpenAD_Symbol_1150 = 9_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1150
  oad_it_ptr = oad_it_ptr+1
  GO TO 131
 131    CONTINUE
  I = 1
  GO TO 208
 132    CONTINUE
  I = I+1
 208    CONTINUE
  IF (I.LE.N) THEN
    GO TO 133
  ELSE
    GO TO 147
  ENDIF
 133    CONTINUE
  GO TO 134
 134    CONTINUE
  OpenAD_Symbol_1151 = 10_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1151
  oad_it_ptr = oad_it_ptr+1
  GO TO 135
 135    CONTINUE
  J = 1
  GO TO 209
 136    CONTINUE
  J = J+1
 209    CONTINUE
  IF (J.LE.M) THEN
    GO TO 137
  ELSE
    GO TO 146
  ENDIF
 137    CONTINUE
  IF (THCK(I,J)%v.LT.0.0D00) THEN
    GO TO 140
  ELSE
    GO TO 138
  ENDIF
 138    CONTINUE
  GO TO 139
 139    CONTINUE
  OpenAD_Symbol_1154 = 13_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1154
  oad_it_ptr = oad_it_ptr+1
  GO TO 142
 140    CONTINUE
  THCK(INT(I),INT(J))%v = 0.0
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = I
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = J
  oad_it_ptr = oad_it_ptr+1
  GO TO 141
 141    CONTINUE
  OpenAD_Symbol_1152 = 11_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1152
  oad_it_ptr = oad_it_ptr+1
  GO TO 142
 142    CONTINUE
  GO TO 143
 143    CONTINUE
  SVAL%v = (THCK(I,J)%v+SVAL%v)
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = I
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = J
  oad_it_ptr = oad_it_ptr+1
  GO TO 144
 144    CONTINUE
  OpenAD_Symbol_1153 = 12_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1153
  oad_it_ptr = oad_it_ptr+1
  GO TO 145
 145    CONTINUE
  GO TO 136
 146    CONTINUE
  GO TO 132
 147    CONTINUE
  IF(CALVING_FLAG) THEN
    GO TO 150
  ELSE
    GO TO 148
  ENDIF
 148    CONTINUE
  GO TO 149
 149    CONTINUE
  OpenAD_Symbol_1171 = 30_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1171
  oad_it_ptr = oad_it_ptr+1
  GO TO 166
 150    CONTINUE
  GO TO 151
 151    CONTINUE
  OpenAD_Symbol_1155 = 14_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1155
  oad_it_ptr = oad_it_ptr+1
  GO TO 152
 152    CONTINUE
  I = 1
  GO TO 210
 153    CONTINUE
  I = I+1
 210    CONTINUE
  IF (I.LE.N) THEN
    GO TO 154
  ELSE
    GO TO 166
  ENDIF
 154    CONTINUE
  GO TO 155
 155    CONTINUE
  OpenAD_Symbol_1156 = 15_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1156
  oad_it_ptr = oad_it_ptr+1
  GO TO 156
 156    CONTINUE
  J = 1
  GO TO 211
 157    CONTINUE
  J = J+1
 211    CONTINUE
  IF (J.LE.M) THEN
    GO TO 158
  ELSE
    GO TO 165
  ENDIF
 158    CONTINUE
  IF (THCK(I,J)%v.LT.(2.0D+02-TOPG(I,J)*1.09890109890109899382D00)) THEN
    GO TO 161
  ELSE
    GO TO 159
  ENDIF
 159    CONTINUE
  GO TO 160
 160    CONTINUE
  OpenAD_Symbol_1158 = 17_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1158
  oad_it_ptr = oad_it_ptr+1
  GO TO 163
 161    CONTINUE
  THCK(INT(I),INT(J))%v = 0.0
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = I
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = J
  oad_it_ptr = oad_it_ptr+1
  GO TO 162
 162    CONTINUE
  OpenAD_Symbol_1157 = 16_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1157
  oad_it_ptr = oad_it_ptr+1
  GO TO 163
 163    CONTINUE
  GO TO 164
 164    CONTINUE
  GO TO 157
 165    CONTINUE
  GO TO 153
 166    CONTINUE
  GO TO 167
 167    CONTINUE
  USRF(1:INT(N),1:INT(M))%v = (THCK(1:N,1:M)%v+TOPG(1:N,1:M))
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = N
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = M
  oad_it_ptr = oad_it_ptr+1
  CALL GREENLAND_BASALVELOCITY(N,M,DX,DY,UBAS,VBAS,USRF,THCK,BSLIDE,SLIDE_MASK)
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = N
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = M
  oad_it_ptr = oad_it_ptr+1
  CALL STAGVARB(UBAS,STAGUBAS,N,M)
  CALL STAGVARB(VBAS,STAGVBAS,N,M)
  GO TO 168
 168    CONTINUE
  OpenAD_Symbol_1159 = 18_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1159
  oad_it_ptr = oad_it_ptr+1
  GO TO 169
 169    CONTINUE
  I = 1
  GO TO 212
 170    CONTINUE
  I = I+1
 212    CONTINUE
  IF (I.LE.N) THEN
    GO TO 171
  ELSE
    GO TO 189
  ENDIF
 171    CONTINUE
  GO TO 172
 172    CONTINUE
  OpenAD_Symbol_1160 = 19_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1160
  oad_it_ptr = oad_it_ptr+1
  GO TO 173
 173    CONTINUE
  J = 1
  GO TO 213
 174    CONTINUE
  J = J+1
 213    CONTINUE
  IF (J.LE.M) THEN
    GO TO 175
  ELSE
    GO TO 188
  ENDIF
 175    CONTINUE
  IF (MASK(I,J).eq.0) THEN
    GO TO 178
  ELSE
    GO TO 176
  ENDIF
 176    CONTINUE
  GO TO 177
 177    CONTINUE
  OpenAD_Symbol_1164 = 23_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1164
  oad_it_ptr = oad_it_ptr+1
  GO TO 180
 178    CONTINUE
  USRF(INT(I),INT(J))%v = TOPG(I,J)
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = I
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = J
  oad_it_ptr = oad_it_ptr+1
  GO TO 179
 179    CONTINUE
  OpenAD_Symbol_1161 = 20_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1161
  oad_it_ptr = oad_it_ptr+1
  GO TO 180
 180    CONTINUE
  GO TO 181
 181    CONTINUE
  IF (TOPG(I,J).GT.USRF(I,J)%v) THEN
    GO TO 184
  ELSE
    GO TO 182
  ENDIF
 182    CONTINUE
  GO TO 183
 183    CONTINUE
  OpenAD_Symbol_1163 = 22_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1163
  oad_it_ptr = oad_it_ptr+1
  GO TO 186
 184    CONTINUE
  USRF(INT(I),INT(J))%v = TOPG(I,J)
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = I
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = J
  oad_it_ptr = oad_it_ptr+1
  GO TO 185
 185    CONTINUE
  OpenAD_Symbol_1162 = 21_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1162
  oad_it_ptr = oad_it_ptr+1
  GO TO 186
 186    CONTINUE
  GO TO 187
 187    CONTINUE
  GO TO 174
 188    CONTINUE
  GO TO 170
 189    CONTINUE
  CALL CALC_VELO(N,M,NZ,DX,DY,SIGMA,GA3D,STAGDIFF,USRF,THCK,STAGUBAS,STAGVBAS,UV&
     &EL,VVEL)

  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = N
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = M
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = NZ
  oad_it_ptr = oad_it_ptr+1
  CALL CALCVERTICALVELOCITY(N,M,NZ,DX,DY,SIGMA,1,THKLIM,THCK,USRF,DTHCKDTM,DUSRF&
     &DTM,BMLT,NMSB,UVEL,VVEL,WVEL,WGRD)

  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = N
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = M
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = NZ
  oad_it_ptr = oad_it_ptr+1
  OAD_CTMP0(1:INT(NZ),1:N+(-1),1:M+(-1))%v = (UVEL(1:NZ,1:N+(-1),1:M+(-1))%v*3.1&
     &556926D+07)

  OAD_CTMP1(1:INT(NZ),1:N+(-1),1:M+(-1))%v = (VVEL(1:NZ,1:N+(-1),1:M+(-1))%v*3.1&
     &556926D+07)

  OAD_CTMP2(1:N+(-1),1:M+(-1))%v = (STAGUBAS(1:N+(-1),1:M+(-1))%v*3.1556926D+07)
  OAD_CTMP3(1:N+(-1),1:M+(-1))%v = (STAGVBAS(1:N+(-1),1:M+(-1))%v*3.1556926D+07)
  OAD_CTMP4(1:INT(NZ),1:INT(N),1:INT(M))%v = (WVEL(1:NZ,1:N,1:M)%v*3.1556926D+07&
     &)

  OAD_CTMP5(1:INT(NZ),1:INT(N),1:INT(M))%v = (WGRD(1:NZ,1:N,1:M)%v*3.1556926D+07&
     &)

  OAD_CTMP6(1:INT(NZ),1:INT(N),1:INT(M))%v = (GA3D(1:NZ,1:N,1:M)%v*3.1556926D+07&
     &)

  OAD_CTMP7 = (DT/3.1556926D+07)
  OpenAD_Symbol_1223 = (N+(-1))
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1223
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_1224 = (M+(-1))
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1224
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_1225 = (N+(-1))
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1225
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_1226 = (M+(-1))
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1226
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_1227 = (N+(-1))
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1227
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_1228 = (M+(-1))
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1228
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_1229 = (N+(-1))
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1229
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_1230 = (M+(-1))
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1230
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_1231 = (N+(-1))
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1231
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_1232 = (M+(-1))
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1232
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_1233 = (N+(-1))
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1233
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_1234 = (M+(-1))
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1234
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_1235 = (N+(-1))
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1235
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_1236 = (M+(-1))
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1236
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_1237 = (N+(-1))
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1237
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_1238 = (M+(-1))
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1238
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = NZ
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = N
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = M
  oad_it_ptr = oad_it_ptr+1
  CALL TSTEP_TEMPFULLSOLN(N,M,NZ,DX,DY,SIGMA,TEMP,ARTM,THCK,USRF,OAD_CTMP0,OAD_C&
     &TMP1,OAD_CTMP2,OAD_CTMP3,OAD_CTMP4,OAD_CTMP5,OAD_CTMP6,G,BWAT,BMLT,OAD_CTM&
     &P7,THKLIM,FLOATMASK)

  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = N
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = M
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = NZ
  oad_it_ptr = oad_it_ptr+1
  OpenAD_lin_190 = (INT(1_w2f__i8)/3.1556926D+07)
  BMLT(1:INT(N),1:INT(M))%v = (BMLT(1:N,1:M)%v/3.1556926D+07)
  OpenAD_aux_104 = (DX*SVAL%v)
  OpenAD_lin_192 = DX
  OpenAD_lin_191 = DY
  VOL%v = (DY*OpenAD_aux_104)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_aux_105,USRFOLD(1:N,1:M))
  OpenAD_aux_105 = (USRF(1:N,1:M)%v-USRFOLD(1:N,1:M)%v)
  OpenAD_lin_193 = (INT(1_w2f__i8)/DT)
  DUSRFDTM(1:INT(N),1:INT(M))%v = (OpenAD_aux_105/DT)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_aux_106,THCKOLD(1:N,1:M))
  OpenAD_aux_106 = (THCK(1:N,1:M)%v-THCKOLD(1:N,1:M)%v)
  OpenAD_lin_194 = (INT(1_w2f__i8)/DT)
  DTHCKDTM(1:INT(N),1:INT(M))%v = (OpenAD_aux_106/DT)
  USRFOLD(1:INT(N),1:INT(M))%v = USRF(1:N,1:M)%v
  THCKOLD(1:INT(N),1:INT(M))%v = THCK(1:N,1:M)%v
  OpenAD_acc_98 = (OpenAD_lin_192*OpenAD_lin_191)
  if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
  oad_dt(oad_dt_ptr) = OpenAD_lin_190
  oad_dt_ptr = oad_dt_ptr+1
  if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
  oad_dt(oad_dt_ptr) = OpenAD_acc_98
  oad_dt_ptr = oad_dt_ptr+1
  if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
  oad_dt(oad_dt_ptr) = OpenAD_lin_193
  oad_dt_ptr = oad_dt_ptr+1
  if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
  oad_dt(oad_dt_ptr) = OpenAD_lin_194
  oad_dt_ptr = oad_dt_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = N
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = M
  oad_it_ptr = oad_it_ptr+1
  WRITE(*,*) T,'Ice volume',VOL%v
  GO TO 190
 190    CONTINUE
  OpenAD_Symbol_1165 = 24_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1165
  oad_it_ptr = oad_it_ptr+1
  GO TO 191
 191    CONTINUE
  IF (MOD(T,OUTFREQ).eq.0) THEN
    GO TO 194
  ELSE
    GO TO 192
  ENDIF
 192    CONTINUE
  GO TO 193
 193    CONTINUE
  OpenAD_Symbol_1170 = 29_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1170
  oad_it_ptr = oad_it_ptr+1
  GO TO 202
 194    CONTINUE
  OUTPOS = ((T/OUTFREQ)+1)
  GO TO 195
 195    CONTINUE
  OpenAD_Symbol_1166 = 25_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1166
  oad_it_ptr = oad_it_ptr+1
  GO TO 196
 196    CONTINUE
  IF(NCDF_OUT) THEN
    GO TO 199
  ELSE
    GO TO 197
  ENDIF
 197    CONTINUE
  GO TO 198
 198    CONTINUE
  OpenAD_Symbol_1169 = 28_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1169
  oad_it_ptr = oad_it_ptr+1
  GO TO 201
 199    CONTINUE
  WRITE(*,*) 'Writing to file at time:',T
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_36,THCK)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_36,THCK)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_37,USRF)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_37,USRF)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_38,NMSB)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_38,NMSB)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_39,DIFF)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_39,DIFF)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_40,STAGDIFF)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_40,STAGDIFF)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_41,BMLT)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_41,BMLT)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_42,ARTM)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_42,ARTM)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_43,DUSRFDTM)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_43,DUSRFDTM)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_44,DTHCKDTM)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_44,DTHCKDTM)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_45,STAGUBAS)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_45,STAGUBAS)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_46,STAGVBAS)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_46,STAGVBAS)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_47,TEMP)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_47,TEMP)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_48,UVEL)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_48,UVEL)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_49,VVEL)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_49,VVEL)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_50,WVEL)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_50,WVEL)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_51,WGRD)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_51,WGRD)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_tyc_52,GA3D)
!!! requested inline of 'oad_convert' has no defn
  CALL oad_convert(OpenAD_tyc_52,GA3D)
  CALL NETCDF_WRITE(NCDATA,SIGMA,LAT,LON,OpenAD_tyc_36,OpenAD_tyc_37,TOPG,OpenAD&
     &_tyc_38,PRCP,OpenAD_tyc_39,OpenAD_tyc_40,BWAT,OpenAD_tyc_41,OpenAD_tyc_42,&
     &ARTM_PRES,OpenAD_tyc_43,OpenAD_tyc_44,OpenAD_tyc_45,OpenAD_tyc_46,OpenAD_t&
     &yc_47,OpenAD_tyc_48,OpenAD_tyc_49,OpenAD_tyc_50,OpenAD_tyc_51,OpenAD_tyc_5&
     &2,MASK,OUTPOS)

  GO TO 200
 200    CONTINUE
  OpenAD_Symbol_1167 = 26_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1167
  oad_it_ptr = oad_it_ptr+1
  GO TO 201
 201    CONTINUE
  GO TO 202
 202    CONTINUE
  GO TO 203
 203    CONTINUE
  GO TO 204
 204    CONTINUE
  OpenAD_Symbol_1168 = 27_w2f__i8
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1168
  oad_it_ptr = oad_it_ptr+1
  GO TO 205
 205    CONTINUE
  GO TO 90
 90     CONTINUE
          end if
          if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1176 = oad_it(oad_it_ptr)
  do while (INT(OpenAD_Symbol_1176).ne.INT(0_w2f__i8))
    IF (OpenAD_Symbol_1176.EQ.1) go to 264
    IF (OpenAD_Symbol_1176.EQ.2) go to 265
    IF (OpenAD_Symbol_1176.EQ.3) go to 266
    IF (OpenAD_Symbol_1176.EQ.4) go to 267
    IF (OpenAD_Symbol_1176.EQ.5) go to 268
    IF (OpenAD_Symbol_1176.EQ.6) go to 269
    IF (OpenAD_Symbol_1176.EQ.7) go to 270
    IF (OpenAD_Symbol_1176.EQ.8) go to 271
    IF (OpenAD_Symbol_1176.EQ.9) go to 272
    IF (OpenAD_Symbol_1176.EQ.10) go to 273
    IF (OpenAD_Symbol_1176.EQ.11) go to 274
    IF (OpenAD_Symbol_1176.EQ.12) go to 275
    IF (OpenAD_Symbol_1176.EQ.13) go to 276
    IF (OpenAD_Symbol_1176.EQ.14) go to 277
    IF (OpenAD_Symbol_1176.EQ.15) go to 278
    IF (OpenAD_Symbol_1176.EQ.16) go to 279
    IF (OpenAD_Symbol_1176.EQ.17) go to 280
    IF (OpenAD_Symbol_1176.EQ.18) go to 281
    IF (OpenAD_Symbol_1176.EQ.19) go to 282
    IF (OpenAD_Symbol_1176.EQ.20) go to 283
    IF (OpenAD_Symbol_1176.EQ.21) go to 284
    IF (OpenAD_Symbol_1176.EQ.22) go to 285
    IF (OpenAD_Symbol_1176.EQ.23) go to 286
    IF (OpenAD_Symbol_1176.EQ.24) go to 287
    IF (OpenAD_Symbol_1176.EQ.25) go to 288
    IF (OpenAD_Symbol_1176.EQ.26) go to 289
    IF (OpenAD_Symbol_1176.EQ.27) go to 290
    IF (OpenAD_Symbol_1176.EQ.28) go to 291
    IF (OpenAD_Symbol_1176.EQ.29) go to 292
    IF (OpenAD_Symbol_1176.EQ.30) go to 293
    IF (OpenAD_Symbol_1176.EQ.31) go to 294
    IF (OpenAD_Symbol_1176.EQ.32) go to 295
    IF (OpenAD_Symbol_1176.EQ.33) go to 296
    IF (OpenAD_Symbol_1176.EQ.34) go to 297
 264      CONTINUE
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1813 = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1814 = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1815 = oad_it(oad_it_ptr)
    WGRD(1:OpenAD_Symbol_1815,1:OpenAD_Symbol_1814,1:OpenAD_Symbol_1813)%d = 0.0&
     &d0

    WVEL(1:OpenAD_Symbol_1815,1:OpenAD_Symbol_1814,1:OpenAD_Symbol_1813)%d = 0.0&
     &d0

    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1816 = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1817 = oad_it(oad_it_ptr)
    STAGDIFF(1:OpenAD_Symbol_1817,1:OpenAD_Symbol_1816)%d = 0.0d0
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1818 = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1819 = oad_it(oad_it_ptr)
    STAGVBAS(1:OpenAD_Symbol_1819,1:OpenAD_Symbol_1818)%d = 0.0d0
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1820 = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1821 = oad_it(oad_it_ptr)
    STAGUBAS(1:OpenAD_Symbol_1821,1:OpenAD_Symbol_1820)%d = 0.0d0
    NMSB(1:OpenAD_Symbol_1814,1:OpenAD_Symbol_1813)%d = 0.0d0
    VBAS(1:OpenAD_Symbol_1814,1:OpenAD_Symbol_1813)%d = 0.0d0
    UBAS(1:OpenAD_Symbol_1814,1:OpenAD_Symbol_1813)%d = 0.0d0
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1822 = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1823 = oad_it(oad_it_ptr)
    VVEL(1:OpenAD_Symbol_1815,1:OpenAD_Symbol_1823,1:OpenAD_Symbol_1822)%d = 0.0&
     &d0

    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1824 = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1825 = oad_it(oad_it_ptr)
    UVEL(1:OpenAD_Symbol_1815,1:OpenAD_Symbol_1825,1:OpenAD_Symbol_1824)%d = 0.0&
     &d0

    GA2D(1:OpenAD_Symbol_1814,1:OpenAD_Symbol_1813)%d = 0.0d0
    GA3D(1:OpenAD_Symbol_1815,1:OpenAD_Symbol_1814,1:OpenAD_Symbol_1813)%d = 0.0&
     &d0

    GO TO 261
 265      CONTINUE
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_83,THCK)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_84,USRF)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_85,NMSB)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_86,DIFF)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_87,STAGDIFF)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_88,BMLT)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_89,ARTM)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_90,DUSRFDTM)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_91,DTHCKDTM)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_92,STAGUBAS)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_93,STAGVBAS)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_94,TEMP)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_95,UVEL)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_96,VVEL)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_97,WVEL)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_98,WGRD)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_99,GA3D)
    CALL NETCDF_WRITE(NCDATA,SIGMA,LAT,LON,OpenAD_tyc_83,OpenAD_tyc_84,TOPG,Open&
     &AD_tyc_85,PRCP,OpenAD_tyc_86,OpenAD_tyc_87,BWAT,OpenAD_tyc_88,OpenAD_tyc_8&
     &9,ARTM_PRES,OpenAD_tyc_90,OpenAD_tyc_91,OpenAD_tyc_92,OpenAD_tyc_93,OpenAD&
     &_tyc_94,OpenAD_tyc_95,OpenAD_tyc_96,OpenAD_tyc_97,OpenAD_tyc_98,OpenAD_tyc&
     &_99,MASK,1)

    GO TO 261
 266      CONTINUE
    GO TO 261
 267      CONTINUE
    oad_it_ptr = oad_it_ptr-1
    M = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    N = oad_it(oad_it_ptr)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_117,NMSB)
    CALL MASS_BALANCE_MM(OpenAD_tyc_117,DX,DY,N,M)
    GO TO 261
 268      CONTINUE
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1826 = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1827 = oad_it(oad_it_ptr)
    NEW_USRF(1:OpenAD_Symbol_1827,1:OpenAD_Symbol_1826)%d = NEW_USRF(1:OpenAD_Sy&
     &mbol_1827,1:OpenAD_Symbol_1826)%d+USRF(1:OpenAD_Symbol_1827,1:OpenAD_Symbo&
     &l_1826)%d

    USRF(1:OpenAD_Symbol_1827,1:OpenAD_Symbol_1826)%d = 0.0d0
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1828 = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1829 = oad_it(oad_it_ptr)
    NEW_USRF(1:OpenAD_Symbol_1829,1:OpenAD_Symbol_1828)%d = NEW_USRF(1:OpenAD_Sy&
     &mbol_1829,1:OpenAD_Symbol_1828)%d+THCK(1:OpenAD_Symbol_1829,1:OpenAD_Symbo&
     &l_1828)%d

    THCK(1:OpenAD_Symbol_1829,1:OpenAD_Symbol_1828)%d = 0.0d0
    oad_it_ptr = oad_it_ptr-1
    COUNT = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    M = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    N = oad_it(oad_it_ptr)
    CALL CALCTHCK(N,M,DIFF,USRF,NMSB,MASK,DX,DY,DT,COUNT,NEW_USRF)
    CALL STAGVARB(DIFF,STAGDIFF,N,M)
    oad_it_ptr = oad_it_ptr-1
    M = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    N = oad_it(oad_it_ptr)
    CALL CALCDIFF(N,M,DIFF,USRF,THCK,GA2D,BSLIDE,SLIDE_MASK,DX,DY)
    oad_it_ptr = oad_it_ptr-1
    NZ = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    M = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    N = oad_it(oad_it_ptr)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_119,THCK)
    CALL CALC_GAINTEGRAL(N,M,NZ,SIGMA,DEPTH,GA2D,OpenAD_tyc_119,GA3D)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_121,STAGTHCK)
    CALL STAGVARB(THCK,OpenAD_tyc_121,N,M)
    oad_it_ptr = oad_it_ptr-1
    NZ = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    M = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    N = oad_it(oad_it_ptr)
    CALL CALCGLENA(N,M,NZ,GA3D,TSTAR,ENFAC)
    oad_it_ptr = oad_it_ptr-1
    NZ = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    M = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    N = oad_it(oad_it_ptr)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_123,TSTAR)
    CALL GREENLAND_BASALSLIDING(N,M,NZ,OpenAD_tyc_123,BSLIDE,SLIDE_MASK,SLIDE_FL&
     &AG)

    oad_it_ptr = oad_it_ptr-1
    NZ = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    M = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    N = oad_it(oad_it_ptr)
    CALL CALCTSTAR(N,M,NZ,TSTAR,TEMP,THCK,SIGMA)
    oad_it_ptr = oad_it_ptr-1
    M = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    N = oad_it(oad_it_ptr)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_125,THCK)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_126,NMSB)
    CALL MASKER(N,M,OpenAD_tyc_125,OpenAD_tyc_126,MASK,COUNT)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1830 = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1831 = oad_it(oad_it_ptr)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_prp_82,NMSB(1:N,1:M))
    OpenAD_prp_82%d = OpenAD_prp_82%d+NMSB(1:OpenAD_Symbol_1831,1:OpenAD_Symbol_&
     &1830)%d

    NMSB_CTRL(1:OpenAD_Symbol_1831,1:OpenAD_Symbol_1830)%d = NMSB_CTRL(1:OpenAD_&
     &Symbol_1831,1:OpenAD_Symbol_1830)%d+NMSB(1:OpenAD_Symbol_1831,1:OpenAD_Sym&
     &bol_1830)%d

    NMSB(1:OpenAD_Symbol_1831,1:OpenAD_Symbol_1830)%d = 0.0d0
    NMSB(1:OpenAD_Symbol_1831,1:OpenAD_Symbol_1830)%d = NMSB(1:OpenAD_Symbol_183&
     &1,1:OpenAD_Symbol_1830)%d+OpenAD_prp_82%d

    OpenAD_prp_82%d = 0.0d0
    GO TO 261
 269      CONTINUE
    GO TO 261
 270      CONTINUE
    GO TO 261
 271      CONTINUE
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1832 = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1833 = oad_it(oad_it_ptr)
    ARTM(OpenAD_Symbol_1833,OpenAD_Symbol_1832)%d = 0.0d0
    GO TO 261
 272      CONTINUE
    SVAL%d = 0.0d0
    GO TO 261
 273      CONTINUE
    GO TO 261
 274      CONTINUE
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1834 = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1835 = oad_it(oad_it_ptr)
    THCK(OpenAD_Symbol_1835,OpenAD_Symbol_1834)%d = 0.0d0
    GO TO 261
 275      CONTINUE
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1836 = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1837 = oad_it(oad_it_ptr)
    OpenAD_prp_83%d = OpenAD_prp_83%d+SVAL%d
    THCK(OpenAD_Symbol_1837,OpenAD_Symbol_1836)%d = THCK(OpenAD_Symbol_1837,Open&
     &AD_Symbol_1836)%d+SVAL%d

    SVAL%d = 0.0d0
    SVAL%d = SVAL%d+OpenAD_prp_83%d
    OpenAD_prp_83%d = 0.0d0
    GO TO 261
 276      CONTINUE
    GO TO 261
 277      CONTINUE
    GO TO 261
 278      CONTINUE
    GO TO 261
 279      CONTINUE
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1838 = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1839 = oad_it(oad_it_ptr)
    THCK(OpenAD_Symbol_1839,OpenAD_Symbol_1838)%d = 0.0d0
    GO TO 261
 280      CONTINUE
    GO TO 261
 281      CONTINUE
    CALL STAGVARB(VBAS,STAGVBAS,N,M)
    CALL STAGVARB(UBAS,STAGUBAS,N,M)
    oad_it_ptr = oad_it_ptr-1
    M = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    N = oad_it(oad_it_ptr)
    CALL GREENLAND_BASALVELOCITY(N,M,DX,DY,UBAS,VBAS,USRF,THCK,BSLIDE,SLIDE_MASK&
     &)

    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1840 = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1841 = oad_it(oad_it_ptr)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_prp_84,THCK(1:N,1:M))
    OpenAD_prp_84%d = OpenAD_prp_84%d+USRF(1:OpenAD_Symbol_1841,1:OpenAD_Symbol_&
     &1840)%d

    USRF(1:OpenAD_Symbol_1841,1:OpenAD_Symbol_1840)%d = 0.0d0
    THCK(1:OpenAD_Symbol_1841,1:OpenAD_Symbol_1840)%d = THCK(1:OpenAD_Symbol_184&
     &1,1:OpenAD_Symbol_1840)%d+OpenAD_prp_84%d

    OpenAD_prp_84%d = 0.0d0
    GO TO 261
 282      CONTINUE
    GO TO 261
 283      CONTINUE
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1842 = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1843 = oad_it(oad_it_ptr)
    USRF(OpenAD_Symbol_1843,OpenAD_Symbol_1842)%d = 0.0d0
    GO TO 261
 284      CONTINUE
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1844 = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1845 = oad_it(oad_it_ptr)
    USRF(OpenAD_Symbol_1845,OpenAD_Symbol_1844)%d = 0.0d0
    GO TO 261
 285      CONTINUE
    GO TO 261
 286      CONTINUE
    GO TO 261
 287      CONTINUE
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1846 = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1847 = oad_it(oad_it_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1848 = oad_dt(oad_dt_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1849 = oad_dt(oad_dt_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1850 = oad_dt(oad_dt_ptr)
    oad_dt_ptr = oad_dt_ptr-1
    OpenAD_Symbol_1851 = oad_dt(oad_dt_ptr)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_prp_85,BMLT(1:N,1:M))
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_prp_86,USRFOLD(1:N,1:M))
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_prp_87,THCKOLD(1:N,1:M))
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_prp_88,USRF(1:N,1:M))
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_prp_89,THCK(1:N,1:M))
    OpenAD_prp_89%d = OpenAD_prp_89%d+DTHCKDTM(1:OpenAD_Symbol_1847,1:OpenAD_Sym&
     &bol_1846)%d*(OpenAD_Symbol_1848)

    DTHCKDTM(1:OpenAD_Symbol_1847,1:OpenAD_Symbol_1846)%d = 0.0d0
    OpenAD_prp_88%d = OpenAD_prp_88%d+DUSRFDTM(1:OpenAD_Symbol_1847,1:OpenAD_Sym&
     &bol_1846)%d*(OpenAD_Symbol_1849)

    DUSRFDTM(1:OpenAD_Symbol_1847,1:OpenAD_Symbol_1846)%d = 0.0d0
    SVAL%d = SVAL%d+VOL%d*(OpenAD_Symbol_1850)
    VOL%d = 0.0d0
    THCK(1:OpenAD_Symbol_1847,1:OpenAD_Symbol_1846)%d = THCK(1:OpenAD_Symbol_184&
     &7,1:OpenAD_Symbol_1846)%d+THCKOLD(1:OpenAD_Symbol_1847,1:OpenAD_Symbol_184&
     &6)%d

    THCKOLD(1:OpenAD_Symbol_1847,1:OpenAD_Symbol_1846)%d = 0.0d0
    USRF(1:OpenAD_Symbol_1847,1:OpenAD_Symbol_1846)%d = USRF(1:OpenAD_Symbol_184&
     &7,1:OpenAD_Symbol_1846)%d+USRFOLD(1:OpenAD_Symbol_1847,1:OpenAD_Symbol_184&
     &6)%d

    USRFOLD(1:OpenAD_Symbol_1847,1:OpenAD_Symbol_1846)%d = 0.0d0
    OpenAD_prp_87%d = OpenAD_prp_87%d-OpenAD_prp_89%d
    THCK(1:OpenAD_Symbol_1847,1:OpenAD_Symbol_1846)%d = THCK(1:OpenAD_Symbol_184&
     &7,1:OpenAD_Symbol_1846)%d+OpenAD_prp_89%d

    OpenAD_prp_89%d = 0.0d0
    OpenAD_prp_86%d = OpenAD_prp_86%d-OpenAD_prp_88%d
    USRF(1:OpenAD_Symbol_1847,1:OpenAD_Symbol_1846)%d = USRF(1:OpenAD_Symbol_184&
     &7,1:OpenAD_Symbol_1846)%d+OpenAD_prp_88%d

    OpenAD_prp_88%d = 0.0d0
    OpenAD_prp_85%d = OpenAD_prp_85%d+BMLT(1:OpenAD_Symbol_1847,1:OpenAD_Symbol_&
     &1846)%d*(OpenAD_Symbol_1851)

    BMLT(1:OpenAD_Symbol_1847,1:OpenAD_Symbol_1846)%d = 0.0d0
    THCKOLD(1:OpenAD_Symbol_1847,1:OpenAD_Symbol_1846)%d = THCKOLD(1:OpenAD_Symb&
     &ol_1847,1:OpenAD_Symbol_1846)%d+OpenAD_prp_87%d

    OpenAD_prp_87%d = 0.0d0
    USRFOLD(1:OpenAD_Symbol_1847,1:OpenAD_Symbol_1846)%d = USRFOLD(1:OpenAD_Symb&
     &ol_1847,1:OpenAD_Symbol_1846)%d+OpenAD_prp_86%d

    OpenAD_prp_86%d = 0.0d0
    BMLT(1:OpenAD_Symbol_1847,1:OpenAD_Symbol_1846)%d = BMLT(1:OpenAD_Symbol_184&
     &7,1:OpenAD_Symbol_1846)%d+OpenAD_prp_85%d

    OpenAD_prp_85%d = 0.0d0
    oad_it_ptr = oad_it_ptr-1
    NZ = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    M = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    N = oad_it(oad_it_ptr)
    CALL TSTEP_TEMPFULLSOLN(N,M,NZ,DX,DY,SIGMA,TEMP,ARTM,THCK,USRF,OAD_CTMP0,OAD&
     &_CTMP1,OAD_CTMP2,OAD_CTMP3,OAD_CTMP4,OAD_CTMP5,OAD_CTMP6,G,BWAT,BMLT,OAD_C&
     &TMP7,THKLIM,FLOATMASK)

    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1852 = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1853 = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1854 = oad_it(oad_it_ptr)
    GA3D(1:OpenAD_Symbol_1854,1:OpenAD_Symbol_1853,1:OpenAD_Symbol_1852)%d = GA3&
     &D(1:OpenAD_Symbol_1854,1:OpenAD_Symbol_1853,1:OpenAD_Symbol_1852)%d+OAD_CT&
     &MP6(1:OpenAD_Symbol_1854,1:OpenAD_Symbol_1853,1:OpenAD_Symbol_1852)%d*(3.1&
     &556926D+07)

    OAD_CTMP6(1:OpenAD_Symbol_1854,1:OpenAD_Symbol_1853,1:OpenAD_Symbol_1852)%d &
     &= 0.0d0

    WGRD(1:OpenAD_Symbol_1854,1:OpenAD_Symbol_1853,1:OpenAD_Symbol_1852)%d = WGR&
     &D(1:OpenAD_Symbol_1854,1:OpenAD_Symbol_1853,1:OpenAD_Symbol_1852)%d+OAD_CT&
     &MP5(1:OpenAD_Symbol_1854,1:OpenAD_Symbol_1853,1:OpenAD_Symbol_1852)%d*(3.1&
     &556926D+07)

    OAD_CTMP5(1:OpenAD_Symbol_1854,1:OpenAD_Symbol_1853,1:OpenAD_Symbol_1852)%d &
     &= 0.0d0

    WVEL(1:OpenAD_Symbol_1854,1:OpenAD_Symbol_1853,1:OpenAD_Symbol_1852)%d = WVE&
     &L(1:OpenAD_Symbol_1854,1:OpenAD_Symbol_1853,1:OpenAD_Symbol_1852)%d+OAD_CT&
     &MP4(1:OpenAD_Symbol_1854,1:OpenAD_Symbol_1853,1:OpenAD_Symbol_1852)%d*(3.1&
     &556926D+07)

    OAD_CTMP4(1:OpenAD_Symbol_1854,1:OpenAD_Symbol_1853,1:OpenAD_Symbol_1852)%d &
     &= 0.0d0

    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1855 = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1856 = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1857 = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1858 = oad_it(oad_it_ptr)
    STAGVBAS(1:OpenAD_Symbol_1858,1:OpenAD_Symbol_1857)%d = STAGVBAS(1:OpenAD_Sy&
     &mbol_1858,1:OpenAD_Symbol_1857)%d+OAD_CTMP3(1:OpenAD_Symbol_1856,1:OpenAD_&
     &Symbol_1855)%d*(3.1556926D+07)

    OAD_CTMP3(1:OpenAD_Symbol_1856,1:OpenAD_Symbol_1855)%d = 0.0d0
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1859 = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1860 = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1861 = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1862 = oad_it(oad_it_ptr)
    STAGUBAS(1:OpenAD_Symbol_1862,1:OpenAD_Symbol_1861)%d = STAGUBAS(1:OpenAD_Sy&
     &mbol_1862,1:OpenAD_Symbol_1861)%d+OAD_CTMP2(1:OpenAD_Symbol_1860,1:OpenAD_&
     &Symbol_1859)%d*(3.1556926D+07)

    OAD_CTMP2(1:OpenAD_Symbol_1860,1:OpenAD_Symbol_1859)%d = 0.0d0
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1863 = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1864 = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1865 = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1866 = oad_it(oad_it_ptr)
    VVEL(1:OpenAD_Symbol_1854,1:OpenAD_Symbol_1866,1:OpenAD_Symbol_1865)%d = VVE&
     &L(1:OpenAD_Symbol_1854,1:OpenAD_Symbol_1866,1:OpenAD_Symbol_1865)%d+OAD_CT&
     &MP1(1:OpenAD_Symbol_1854,1:OpenAD_Symbol_1864,1:OpenAD_Symbol_1863)%d*(3.1&
     &556926D+07)

    OAD_CTMP1(1:OpenAD_Symbol_1854,1:OpenAD_Symbol_1864,1:OpenAD_Symbol_1863)%d &
     &= 0.0d0

    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1867 = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1868 = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1869 = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1870 = oad_it(oad_it_ptr)
    UVEL(1:OpenAD_Symbol_1854,1:OpenAD_Symbol_1870,1:OpenAD_Symbol_1869)%d = UVE&
     &L(1:OpenAD_Symbol_1854,1:OpenAD_Symbol_1870,1:OpenAD_Symbol_1869)%d+OAD_CT&
     &MP0(1:OpenAD_Symbol_1854,1:OpenAD_Symbol_1868,1:OpenAD_Symbol_1867)%d*(3.1&
     &556926D+07)

    OAD_CTMP0(1:OpenAD_Symbol_1854,1:OpenAD_Symbol_1868,1:OpenAD_Symbol_1867)%d &
     &= 0.0d0

    oad_it_ptr = oad_it_ptr-1
    NZ = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    M = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    N = oad_it(oad_it_ptr)
    CALL CALCVERTICALVELOCITY(N,M,NZ,DX,DY,SIGMA,1,THKLIM,THCK,USRF,DTHCKDTM,DUS&
     &RFDTM,BMLT,NMSB,UVEL,VVEL,WVEL,WGRD)

    oad_it_ptr = oad_it_ptr-1
    NZ = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    M = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    N = oad_it(oad_it_ptr)
    CALL CALC_VELO(N,M,NZ,DX,DY,SIGMA,GA3D,STAGDIFF,USRF,THCK,STAGUBAS,STAGVBAS,&
     &UVEL,VVEL)

    GO TO 261
 288      CONTINUE
    GO TO 261
 289      CONTINUE
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_129,THCK)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_130,USRF)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_131,NMSB)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_132,DIFF)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_133,STAGDIFF)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_134,BMLT)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_135,ARTM)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_136,DUSRFDTM)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_137,DTHCKDTM)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_138,STAGUBAS)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_139,STAGVBAS)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_140,TEMP)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_141,UVEL)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_142,VVEL)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_143,WVEL)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_144,WGRD)
!!! requested inline of 'oad_AllocateMatching' has no defn
    CALL oad_AllocateMatching(OpenAD_tyc_145,GA3D)
    CALL NETCDF_WRITE(NCDATA,SIGMA,LAT,LON,OpenAD_tyc_129,OpenAD_tyc_130,TOPG,Op&
     &enAD_tyc_131,PRCP,OpenAD_tyc_132,OpenAD_tyc_133,BWAT,OpenAD_tyc_134,OpenAD&
     &_tyc_135,ARTM_PRES,OpenAD_tyc_136,OpenAD_tyc_137,OpenAD_tyc_138,OpenAD_tyc&
     &_139,OpenAD_tyc_140,OpenAD_tyc_141,OpenAD_tyc_142,OpenAD_tyc_143,OpenAD_ty&
     &c_144,OpenAD_tyc_145,MASK,OUTPOS)

    GO TO 261
 290      CONTINUE
    GO TO 261
 291      CONTINUE
    GO TO 261
 292      CONTINUE
    GO TO 261
 293      CONTINUE
    GO TO 261
 294      CONTINUE
    GO TO 261
 295      CONTINUE
    oad_it_ptr = oad_it_ptr-1
    M = oad_it(oad_it_ptr)
    oad_it_ptr = oad_it_ptr-1
    N = oad_it(oad_it_ptr)
    CALL GREENLAND_MASSBALANCE(N,M,USRF,LAT,PRCP,ARTM,ARTM_PRES,NMSB,PDDTAB)
    GO TO 261
 296      CONTINUE
    GO TO 261
 297      CONTINUE
    GO TO 261
 261      CONTINUE
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1176 = oad_it(oad_it_ptr)
  END DO
          end if
          if (our_rev_mode%arg_store) then
  write(unit=cp_io_unit) N
  write(unit=cp_io_unit) M
  write(unit=cp_io_unit) NZ
  write(unit=cp_io_unit) DX
  write(unit=cp_io_unit) DY
  write(unit=cp_io_unit) DT
  write(unit=cp_io_unit) SIGMA
  write(unit=cp_io_unit) DEPTH
  write(unit=cp_io_unit) THCK%v
  write(unit=cp_io_unit) TOPG
  write(unit=cp_io_unit) USRF%v
  write(unit=cp_io_unit) PRCP
  write(unit=cp_io_unit) ARTM_PRES
  write(unit=cp_io_unit) LAT
  write(unit=cp_io_unit) LON
  write(unit=cp_io_unit) DTHCKDTM%v
  write(unit=cp_io_unit) DUSRFDTM%v
  write(unit=cp_io_unit) DIFF%v
  write(unit=cp_io_unit) PDDTAB
  write(unit=cp_io_unit) BWAT
  write(unit=cp_io_unit) BMLT%v
  write(unit=cp_io_unit) TEMP%v
  write(unit=cp_io_unit) G
!!! requested inline of 'cp_arg_store_opaque_scalar' has no defn
  CALL cp_arg_store_opaque_scalar(NCDATA)
  write(unit=cp_io_unit) ENFAC
  write(unit=cp_io_unit) SLIDE_FLAG
  write(unit=cp_io_unit) PDD_DX
  write(unit=cp_io_unit) PDD_DY
  write(unit=cp_io_unit) PDD_IX
  write(unit=cp_io_unit) PDD_IY
  write(unit=cp_io_unit) PDD_PDDFAC_ICE
  write(unit=cp_io_unit) PDD_PDDFAC_SNOW
  write(unit=cp_io_unit) PDD_WMAX
  write(unit=cp_io_unit) NCDF_OUT
  write(unit=cp_io_unit) CALVING_FLAG
  write(unit=cp_io_unit) CLIM_FLAG
  write(unit=cp_io_unit) OUTFREQ
  write(unit=cp_io_unit) T
  write(unit=cp_io_unit) THCKOLD%v
  write(unit=cp_io_unit) USRFOLD%v
  write(unit=cp_io_unit) NMSB_CTRL%v
  write(unit=cp_io_unit) BSLIDE_CTRL
          end if
          if (our_rev_mode%arg_restore) then
  read(unit=cp_io_unit) N
  read(unit=cp_io_unit) M
  read(unit=cp_io_unit) NZ
  read(unit=cp_io_unit) DX
  read(unit=cp_io_unit) DY
  read(unit=cp_io_unit) DT
  read(unit=cp_io_unit) SIGMA
  read(unit=cp_io_unit) DEPTH
  read(unit=cp_io_unit) THCK%v
  read(unit=cp_io_unit) TOPG
  read(unit=cp_io_unit) USRF%v
  read(unit=cp_io_unit) PRCP
  read(unit=cp_io_unit) ARTM_PRES
  read(unit=cp_io_unit) LAT
  read(unit=cp_io_unit) LON
  read(unit=cp_io_unit) DTHCKDTM%v
  read(unit=cp_io_unit) DUSRFDTM%v
  read(unit=cp_io_unit) DIFF%v
  read(unit=cp_io_unit) PDDTAB
  read(unit=cp_io_unit) BWAT
  read(unit=cp_io_unit) BMLT%v
  read(unit=cp_io_unit) TEMP%v
  read(unit=cp_io_unit) G
!!! requested inline of 'cp_arg_restore_opaque_scalar' has no defn
  CALL cp_arg_restore_opaque_scalar(NCDATA)
  read(unit=cp_io_unit) ENFAC
  read(unit=cp_io_unit) SLIDE_FLAG
  read(unit=cp_io_unit) PDD_DX
  read(unit=cp_io_unit) PDD_DY
  read(unit=cp_io_unit) PDD_IX
  read(unit=cp_io_unit) PDD_IY
  read(unit=cp_io_unit) PDD_PDDFAC_ICE
  read(unit=cp_io_unit) PDD_PDDFAC_SNOW
  read(unit=cp_io_unit) PDD_WMAX
  read(unit=cp_io_unit) NCDF_OUT
  read(unit=cp_io_unit) CALVING_FLAG
  read(unit=cp_io_unit) CLIM_FLAG
  read(unit=cp_io_unit) OUTFREQ
  read(unit=cp_io_unit) T
  read(unit=cp_io_unit) THCKOLD%v
  read(unit=cp_io_unit) USRFOLD%v
  read(unit=cp_io_unit) NMSB_CTRL%v
  read(unit=cp_io_unit) BSLIDE_CTRL
          end if
        end subroutine BODY
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE GREENLAND_MASSBALANCE(N, M, USRF, LAT, PRCP, ARTM, ARTM_PRES, NMSB,&
     & PDDTAB)

          use OAD_tape
          use OAD_rev
          use OAD_cp
          use adism_mbal
          use glimmer_pdd

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  use glimmer_global ,only: DP
  use glimmer_pdd ,only: GLIMMER_PDD_MBAL , PDD_NX , PDD_NY
  use adism_core ,only: DIAGWRITE2D
  use physcon ,only: SCYR
  use glimmer_global ,only: DP
  use glimmer_pdd ,only: GLIMMER_PDD_MBAL , PDD_NX , PDD_NY
  use adism_core ,only: DIAGWRITE2D
  use physcon ,only: SCYR
  use glimmer_global ,only: DP
  use glimmer_pdd ,only: GLIMMER_PDD_MBAL , PDD_NX , PDD_NY
  use adism_core ,only: DIAGWRITE2D
  use physcon ,only: SCYR
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_1239
  INTEGER(w2f__i8) OpenAD_Symbol_1240
  INTEGER(w2f__i8) OpenAD_Symbol_1241
  INTEGER(w2f__i8) OpenAD_Symbol_1242
  INTEGER(w2f__i8) OpenAD_Symbol_1243
  INTEGER(w2f__i8) OpenAD_Symbol_1244
  INTEGER(w2f__i8) OpenAD_Symbol_1245
  INTEGER(w2f__i8) OpenAD_Symbol_1246
  INTEGER(w2f__i8) OpenAD_Symbol_1247
  INTEGER(w2f__i8) OpenAD_Symbol_1248
  INTEGER(w2f__i8) OpenAD_Symbol_1249
  INTEGER(w2f__i8) OpenAD_Symbol_1250
  INTEGER(w2f__i8) OpenAD_Symbol_1251
  INTEGER(w2f__i8) OpenAD_Symbol_1252
  INTEGER(w2f__i8) OpenAD_Symbol_1253
  INTEGER(w2f__i8) OpenAD_Symbol_1254
  INTEGER(w2f__i8) OpenAD_Symbol_1255
  INTEGER(w2f__i8) OpenAD_Symbol_1256
  INTEGER(w2f__i8) OpenAD_Symbol_1257
  INTEGER(w2f__i8) OpenAD_Symbol_1258
  INTEGER(w2f__i8) OpenAD_Symbol_1259
  INTEGER(w2f__i8) OpenAD_Symbol_1260
  INTEGER(w2f__i8) OpenAD_Symbol_1261
  INTEGER(w2f__i8) OpenAD_Symbol_1262
  INTEGER(w2f__i8) OpenAD_Symbol_1263
  INTEGER(w2f__i8) OpenAD_Symbol_1264
  INTEGER(w2f__i8) OpenAD_Symbol_1265
  INTEGER(w2f__i8) OpenAD_Symbol_1266
  INTEGER(w2f__i8) OpenAD_Symbol_1267
  INTEGER(w2f__i8) OpenAD_Symbol_1268
  INTEGER(w2f__i8) OpenAD_Symbol_1269
  INTEGER(w2f__i8) OpenAD_Symbol_1270
  INTEGER(w2f__i8) OpenAD_Symbol_1271
  INTEGER(w2f__i8) OpenAD_Symbol_1272
  INTEGER(w2f__i8) OpenAD_Symbol_1273
  INTEGER(w2f__i8) OpenAD_Symbol_1274
  INTEGER(w2f__i8) OpenAD_Symbol_1275
  INTEGER(w2f__i8) OpenAD_Symbol_1276
  INTEGER(w2f__i8) OpenAD_Symbol_1277
  INTEGER(w2f__i8) OpenAD_Symbol_1278
  INTEGER(w2f__i8) OpenAD_Symbol_1279
  INTEGER(w2f__i8) OpenAD_Symbol_1280
  INTEGER(w2f__i8) OpenAD_Symbol_1281
  INTEGER(w2f__i8) OpenAD_Symbol_1282
  INTEGER(w2f__i8) OpenAD_Symbol_1283
  INTEGER(w2f__i8) OpenAD_Symbol_1284
  INTEGER(w2f__i8) OpenAD_Symbol_1285
  INTEGER(w2f__i8) OpenAD_Symbol_1286
!
!       **** Parameters and Result ****
!
  INTEGER(w2f__i4) N
  INTEGER(w2f__i4) M
  type(active) :: USRF(1:N,1:M)
  REAL(w2f__8) LAT(1 : N, 1 : M)
  REAL(w2f__8) PRCP(1 : N, 1 : M)
  type(active) :: ARTM(1:N,1:M)
  REAL(w2f__8) ARTM_PRES(1 : N, 1 : M)
  type(active) :: NMSB(1:N,1:M)
  REAL(w2f__8) PDDTAB(1 : 31, 1 : 71)
!
!       **** Local Variables and Functions ****
!
  type(active) :: ABLT(1:N,1:M)
  type(active) :: ARNG(1:N,1:M)
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  LOGICAL(w2f__i4) LANDSEA(1 : N, 1 : M)
  type(active) :: OAD_CTMP0
  REAL(w2f__8) OAD_CTMP1
  REAL(w2f__8) OAD_CTMP2
  REAL(w2f__8) OAD_CTMP3
  type(active) :: PRCP_SCALE(1:N,1:M)
  INTEGER(w2f__i4) OpenAD_Symbol_1871
  INTEGER(w2f__i4) OpenAD_Symbol_1872
  INTEGER(w2f__i4) OpenAD_Symbol_1873
  INTEGER(w2f__i4) OpenAD_Symbol_1874
  INTEGER(w2f__i4) OpenAD_Symbol_1875
  INTEGER(w2f__i4) OpenAD_Symbol_1876
  INTEGER(w2f__i4) OpenAD_Symbol_1877
  INTEGER(w2f__i4) OpenAD_Symbol_1878
  INTEGER(w2f__i4) OpenAD_Symbol_1879
  INTEGER(w2f__i4) OpenAD_Symbol_1880
  REAL(w2f__8) OpenAD_Symbol_1881
  INTEGER(w2f__i4) OpenAD_Symbol_1882
  INTEGER(w2f__i4) OpenAD_Symbol_1883
  REAL(w2f__8) OpenAD_Symbol_1884
  REAL(w2f__8) OpenAD_acc_99
  REAL(w2f__8) OpenAD_aux_107
  REAL(w2f__8) OpenAD_aux_108
  REAL(w2f__8) OpenAD_lin_195
  REAL(w2f__8) OpenAD_lin_196
  REAL(w2f__8) OpenAD_lin_197
  type(active) :: OpenAD_prp_90(:,:)
  ALLOCATABLE OpenAD_prp_90
  type(active) :: OpenAD_tyc_163
  type(active) :: OpenAD_tyc_165
  type(active) :: OpenAD_tyc_166
  type(active) :: OpenAD_tyc_167
  type(active) :: OpenAD_tyc_53
  type(active) :: OpenAD_tyc_54
  type(active) :: OpenAD_tyc_55
  type(active) :: OpenAD_tyc_56
!
!       **** Statements ****
!


          integer iaddr
          external iaddr
!$OPENAD XXX Template body_template.split.f90

         if (our_rev_mode%plain) then
! original function
  ABLT(1:INT(N),1:INT(M))%v = 0.0
  ARNG(1:INT(N),1:INT(M))%v = 0.0
  PRCP_SCALE(1:INT(N),1:INT(M))%v = 0.0
  DO I = 1,N,1
    DO J = 1,M,1
      IF (USRF(I,J)%v.GT.0.0D00) THEN
        OAD_CTMP1 = ((LAT(I,J)+(-6.5D+01))*2.0D+01)
!!! requested inline of 'oad_convert' has no defn
        CALL oad_convert(OpenAD_tyc_53,OAD_CTMP1)
        CALL OAD_S_MAX_D(USRF(I,J),OpenAD_tyc_53,OAD_CTMP0)
        ARTM(INT(I),INT(J))%v = (4.9130001068115234375D+01-OAD_CTMP0%v*7.9920003&
     &1906366348267D-03-LAT(I,J)*7.57600009441375732422D-01)

        ARNG(INT(I),INT(J))%v = (3.03799991607666015625D+01-USRF(I,J)%v*6.277000&
     &06589293479919D-03-LAT(I,J)*3.26200008392333984375D-01-ARTM(I,J)%v)

      ELSE
        OAD_CTMP3 = ((LAT(I,J)+(-6.5D+01))*2.0D+01)
!!! requested inline of 'oad_convert' has no defn
        CALL oad_convert(OpenAD_tyc_54,0.0D00)
!!! requested inline of 'oad_convert' has no defn
        CALL oad_convert(OpenAD_tyc_55,OAD_CTMP3)
        CALL OAD_S_MAX_D(OpenAD_tyc_54,OpenAD_tyc_55,OpenAD_tyc_56)
!!! requested inline of 'oad_convert' has no defn
        CALL oad_convert(OAD_CTMP2,OpenAD_tyc_56)
        ARTM(INT(I),INT(J))%v = (4.9130001068115234375D+01-OAD_CTMP2*7.992000319&
     &06366348267D-03-LAT(I,J)*7.57600009441375732422D-01)

        ARNG(INT(I),INT(J))%v = (3.03799991607666015625D+01-LAT(I,J)*3.262000083&
     &92333984375D-01-ARTM(I,J)%v)

      ENDIF
    END DO
  END DO
  DO I = 1,N,1
    DO J = 1,M,1
      IF (USRF(I,J)%v.LT.0.0D00) THEN
        LANDSEA(I,J) = .false.
      ELSE
        LANDSEA(I,J) = .true.
      ENDIF
    END DO
  END DO
  DO I = 1,N,1
    DO J = 1,M,1
      PRCP_SCALE(INT(I),INT(J))%v = (PRCP(I,J)*(1.05430000000000001492D00**(ARTM&
     &(I,J)%v-ARTM_PRES(I,J))))

    END DO
  END DO
  CALL GLIMMER_PDD_MBAL(PDDTAB,ARTM,ARNG,PRCP_SCALE,ABLT,NMSB,LANDSEA)
  NMSB(1:INT(N),1:INT(M))%v = (NMSB(1:N,1:M)%v/3.1556926D+07)
          end if
          if (our_rev_mode%tape) then
! taping
!$OPENAD XXX Template body_template.split.f90
  ABLT(1:INT(N),1:INT(M))%v = 0.0
  ARNG(1:INT(N),1:INT(M))%v = 0.0
  PRCP_SCALE(1:INT(N),1:INT(M))%v = 0.0
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = N
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = M
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_1253 = 0_w2f__i8
  DO I = 1,N,1
    OpenAD_Symbol_1254 = 0_w2f__i8
    DO J = 1,M,1
      IF (USRF(I,J)%v.GT.0.0D00) THEN
        OAD_CTMP1 = ((LAT(I,J)+(-6.5D+01))*2.0D+01)
!!! requested inline of 'oad_convert' has no defn
        CALL oad_convert(OpenAD_tyc_53,OAD_CTMP1)
        CALL OAD_S_MAX_D(USRF(I,J),OpenAD_tyc_53,OAD_CTMP0)
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = I
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = J
        oad_it_ptr = oad_it_ptr+1
        ARTM(INT(I),INT(J))%v = (4.9130001068115234375D+01-OAD_CTMP0%v*7.9920003&
     &1906366348267D-03-LAT(I,J)*7.57600009441375732422D-01)

        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = I
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = J
        oad_it_ptr = oad_it_ptr+1
        ARNG(INT(I),INT(J))%v = (3.03799991607666015625D+01-USRF(I,J)%v*6.277000&
     &06589293479919D-03-LAT(I,J)*3.26200008392333984375D-01-ARTM(I,J)%v)

        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = I
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = J
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_1255 = 1_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_1255
        oad_it_ptr = oad_it_ptr+1
      ELSE
        OAD_CTMP3 = ((LAT(I,J)+(-6.5D+01))*2.0D+01)
!!! requested inline of 'oad_convert' has no defn
        CALL oad_convert(OpenAD_tyc_54,0.0D00)
!!! requested inline of 'oad_convert' has no defn
        CALL oad_convert(OpenAD_tyc_55,OAD_CTMP3)
        CALL OAD_S_MAX_D(OpenAD_tyc_54,OpenAD_tyc_55,OpenAD_tyc_56)
!!! requested inline of 'oad_convert' has no defn
        CALL oad_convert(OAD_CTMP2,OpenAD_tyc_56)
        ARTM(INT(I),INT(J))%v = (4.9130001068115234375D+01-OAD_CTMP2*7.992000319&
     &06366348267D-03-LAT(I,J)*7.57600009441375732422D-01)

        ARNG(INT(I),INT(J))%v = (3.03799991607666015625D+01-LAT(I,J)*3.262000083&
     &92333984375D-01-ARTM(I,J)%v)

        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = I
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = J
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_1256 = 0_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_1256
        oad_it_ptr = oad_it_ptr+1
      ENDIF
      OpenAD_Symbol_1254 = (INT(OpenAD_Symbol_1254)+INT(1_w2f__i8))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_1254
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_1253 = (INT(OpenAD_Symbol_1253)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1253
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_1257 = 0_w2f__i8
  DO I = 1,N,1
    OpenAD_Symbol_1258 = 0_w2f__i8
    DO J = 1,M,1
      IF (USRF(I,J)%v.LT.0.0D00) THEN
        LANDSEA(I,J) = .false.
        OpenAD_Symbol_1259 = 1_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_1259
        oad_it_ptr = oad_it_ptr+1
      ELSE
        LANDSEA(I,J) = .true.
        OpenAD_Symbol_1260 = 0_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_1260
        oad_it_ptr = oad_it_ptr+1
      ENDIF
      OpenAD_Symbol_1258 = (INT(OpenAD_Symbol_1258)+INT(1_w2f__i8))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_1258
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_1257 = (INT(OpenAD_Symbol_1257)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1257
  oad_it_ptr = oad_it_ptr+1
  OpenAD_Symbol_1261 = 0_w2f__i8
  DO I = 1,N,1
    OpenAD_Symbol_1262 = 0_w2f__i8
    DO J = 1,M,1
      OpenAD_aux_108 = (ARTM(I,J)%v-ARTM_PRES(I,J))
      OpenAD_aux_107 = (1.05430000000000001492D00**OpenAD_aux_108)
      OpenAD_lin_196 = (LOG(1.05430000000000001492D00)*(1.05430000000000001492D0&
     &0**OpenAD_aux_108))

      OpenAD_lin_195 = PRCP(I,J)
      PRCP_SCALE(INT(I),INT(J))%v = (PRCP(I,J)*OpenAD_aux_107)
      OpenAD_acc_99 = (OpenAD_lin_196*OpenAD_lin_195)
      if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
      oad_dt(oad_dt_ptr) = OpenAD_acc_99
      oad_dt_ptr = oad_dt_ptr+1
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = I
      oad_it_ptr = oad_it_ptr+1
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = J
      oad_it_ptr = oad_it_ptr+1
      OpenAD_Symbol_1262 = (INT(OpenAD_Symbol_1262)+INT(1_w2f__i8))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_1262
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_1261 = (INT(OpenAD_Symbol_1261)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1261
  oad_it_ptr = oad_it_ptr+1
  CALL GLIMMER_PDD_MBAL(PDDTAB,ARTM,ARNG,PRCP_SCALE,ABLT,NMSB,LANDSEA)
  OpenAD_lin_197 = (INT(1_w2f__i8)/3.1556926D+07)
  NMSB(1:INT(N),1:INT(M))%v = (NMSB(1:N,1:M)%v/3.1556926D+07)
  if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
  oad_dt(oad_dt_ptr) = OpenAD_lin_197
  oad_dt_ptr = oad_dt_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = N
  oad_it_ptr = oad_it_ptr+1
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = M
  oad_it_ptr = oad_it_ptr+1
          end if
          if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1882 = oad_it(oad_it_ptr)
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1883 = oad_it(oad_it_ptr)
  oad_dt_ptr = oad_dt_ptr-1
  OpenAD_Symbol_1884 = oad_dt(oad_dt_ptr)
!!! requested inline of 'oad_AllocateMatching' has no defn
  CALL oad_AllocateMatching(OpenAD_prp_90,NMSB(1:N,1:M))
  OpenAD_prp_90%d = OpenAD_prp_90%d+NMSB(1:OpenAD_Symbol_1883,1:OpenAD_Symbol_18&
     &82)%d*(OpenAD_Symbol_1884)

  NMSB(1:OpenAD_Symbol_1883,1:OpenAD_Symbol_1882)%d = 0.0d0
  NMSB(1:OpenAD_Symbol_1883,1:OpenAD_Symbol_1882)%d = NMSB(1:OpenAD_Symbol_1883,&
     &1:OpenAD_Symbol_1882)%d+OpenAD_prp_90%d

  OpenAD_prp_90%d = 0.0d0
  CALL GLIMMER_PDD_MBAL(PDDTAB,ARTM,ARNG,PRCP_SCALE,ABLT,NMSB,LANDSEA)
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1239 = oad_it(oad_it_ptr)
  OpenAD_Symbol_1240 = 1
  do while (INT(OpenAD_Symbol_1240).LE.INT(OpenAD_Symbol_1239))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1241 = oad_it(oad_it_ptr)
    OpenAD_Symbol_1242 = 1
    do while (INT(OpenAD_Symbol_1242).LE.INT(OpenAD_Symbol_1241))
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1879 = oad_it(oad_it_ptr)
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1880 = oad_it(oad_it_ptr)
      oad_dt_ptr = oad_dt_ptr-1
      OpenAD_Symbol_1881 = oad_dt(oad_dt_ptr)
      ARTM(OpenAD_Symbol_1880,OpenAD_Symbol_1879)%d = ARTM(OpenAD_Symbol_1880,Op&
     &enAD_Symbol_1879)%d+PRCP_SCALE(OpenAD_Symbol_1880,OpenAD_Symbol_1879)%d*(O&
     &penAD_Symbol_1881)

      PRCP_SCALE(OpenAD_Symbol_1880,OpenAD_Symbol_1879)%d = 0.0d0
      OpenAD_Symbol_1242 = INT(OpenAD_Symbol_1242)+1
    END DO
    OpenAD_Symbol_1240 = INT(OpenAD_Symbol_1240)+1
  END DO
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1243 = oad_it(oad_it_ptr)
  OpenAD_Symbol_1244 = 1
  do while (INT(OpenAD_Symbol_1244).LE.INT(OpenAD_Symbol_1243))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1245 = oad_it(oad_it_ptr)
    OpenAD_Symbol_1246 = 1
    do while (INT(OpenAD_Symbol_1246).LE.INT(OpenAD_Symbol_1245))
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1247 = oad_it(oad_it_ptr)
      IF (OpenAD_Symbol_1247.ne.0) THEN
      ENDIF
      OpenAD_Symbol_1246 = INT(OpenAD_Symbol_1246)+1
    END DO
    OpenAD_Symbol_1244 = INT(OpenAD_Symbol_1244)+1
  END DO
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1248 = oad_it(oad_it_ptr)
  OpenAD_Symbol_1249 = 1
  do while (INT(OpenAD_Symbol_1249).LE.INT(OpenAD_Symbol_1248))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1250 = oad_it(oad_it_ptr)
    OpenAD_Symbol_1251 = 1
    do while (INT(OpenAD_Symbol_1251).LE.INT(OpenAD_Symbol_1250))
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1252 = oad_it(oad_it_ptr)
      IF (OpenAD_Symbol_1252.ne.0) THEN
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1873 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1874 = oad_it(oad_it_ptr)
        USRF(OpenAD_Symbol_1874,OpenAD_Symbol_1873)%d = USRF(OpenAD_Symbol_1874,&
     &OpenAD_Symbol_1873)%d+ARNG(OpenAD_Symbol_1874,OpenAD_Symbol_1873)%d*(-6.27&
     &700006589293479919D-03)

        ARTM(OpenAD_Symbol_1874,OpenAD_Symbol_1873)%d = ARTM(OpenAD_Symbol_1874,&
     &OpenAD_Symbol_1873)%d-ARNG(OpenAD_Symbol_1874,OpenAD_Symbol_1873)%d

        ARNG(OpenAD_Symbol_1874,OpenAD_Symbol_1873)%d = 0.0d0
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1875 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1876 = oad_it(oad_it_ptr)
        OAD_CTMP0%d = OAD_CTMP0%d+ARTM(OpenAD_Symbol_1876,OpenAD_Symbol_1875)%d*&
     &(-7.99200031906366348267D-03)

        ARTM(OpenAD_Symbol_1876,OpenAD_Symbol_1875)%d = 0.0d0
        oad_it_ptr = oad_it_ptr-1
        J = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        I = oad_it(oad_it_ptr)
        CALL OAD_S_MAX_D(USRF(I,J),OpenAD_tyc_163,OAD_CTMP0)
      ELSE
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1877 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1878 = oad_it(oad_it_ptr)
        ARTM(OpenAD_Symbol_1878,OpenAD_Symbol_1877)%d = ARTM(OpenAD_Symbol_1878,&
     &OpenAD_Symbol_1877)%d-ARNG(OpenAD_Symbol_1878,OpenAD_Symbol_1877)%d

        ARNG(OpenAD_Symbol_1878,OpenAD_Symbol_1877)%d = 0.0d0
        ARTM(OpenAD_Symbol_1878,OpenAD_Symbol_1877)%d = 0.0d0
        CALL OAD_S_MAX_D(OpenAD_tyc_165,OpenAD_tyc_166,OpenAD_tyc_167)
      ENDIF
      OpenAD_Symbol_1251 = INT(OpenAD_Symbol_1251)+1
    END DO
    OpenAD_Symbol_1249 = INT(OpenAD_Symbol_1249)+1
  END DO
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1871 = oad_it(oad_it_ptr)
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1872 = oad_it(oad_it_ptr)
  PRCP_SCALE(1:OpenAD_Symbol_1872,1:OpenAD_Symbol_1871)%d = 0.0d0
  ARNG(1:OpenAD_Symbol_1872,1:OpenAD_Symbol_1871)%d = 0.0d0
  ABLT(1:OpenAD_Symbol_1872,1:OpenAD_Symbol_1871)%d = 0.0d0
          end if
          if (our_rev_mode%arg_store) then
  write(unit=cp_io_unit) ARTM%v
  write(unit=cp_io_unit) NMSB%v
  write(unit=cp_io_unit) PDDTAB
  write(unit=cp_io_unit) PDD_DX
  write(unit=cp_io_unit) PDD_DY
  write(unit=cp_io_unit) PDD_IX
  write(unit=cp_io_unit) PDD_IY
  write(unit=cp_io_unit) PDD_PDDFAC_ICE
  write(unit=cp_io_unit) PDD_PDDFAC_SNOW
  write(unit=cp_io_unit) PDD_WMAX
  write(unit=cp_io_unit) N
  write(unit=cp_io_unit) M
  write(unit=cp_io_unit) USRF%v
  write(unit=cp_io_unit) LAT
  write(unit=cp_io_unit) PRCP
  write(unit=cp_io_unit) ARTM_PRES
          end if
          if (our_rev_mode%arg_restore) then
  read(unit=cp_io_unit) ARTM%v
  read(unit=cp_io_unit) NMSB%v
  read(unit=cp_io_unit) PDDTAB
  read(unit=cp_io_unit) PDD_DX
  read(unit=cp_io_unit) PDD_DY
  read(unit=cp_io_unit) PDD_IX
  read(unit=cp_io_unit) PDD_IY
  read(unit=cp_io_unit) PDD_PDDFAC_ICE
  read(unit=cp_io_unit) PDD_PDDFAC_SNOW
  read(unit=cp_io_unit) PDD_WMAX
  read(unit=cp_io_unit) N
  read(unit=cp_io_unit) M
  read(unit=cp_io_unit) USRF%v
  read(unit=cp_io_unit) LAT
  read(unit=cp_io_unit) PRCP
  read(unit=cp_io_unit) ARTM_PRES
          end if
        end subroutine GREENLAND_MASSBALANCE
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE GREENLAND_BASALSLIDING(N, M, NZ, TSTAR, BSLIDE, SLIDE_MASK, SLIDE_F&
     &LAG)

          use OAD_tape
          use OAD_rev
          use OAD_cp
          use adism_mbal
          use glimmer_pdd

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  use glimmer_global ,only: DP
  use physcon ,only: SCYR
  use glimmer_global ,only: DP
  use physcon ,only: SCYR
  use glimmer_global ,only: DP
  use physcon ,only: SCYR
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_1287
  INTEGER(w2f__i8) OpenAD_Symbol_1288
  INTEGER(w2f__i8) OpenAD_Symbol_1289
  INTEGER(w2f__i8) OpenAD_Symbol_1290
  INTEGER(w2f__i8) OpenAD_Symbol_1291
  INTEGER(w2f__i8) OpenAD_Symbol_1292
  INTEGER(w2f__i8) OpenAD_Symbol_1293
  INTEGER(w2f__i8) OpenAD_Symbol_1294
  INTEGER(w2f__i8) OpenAD_Symbol_1295
  INTEGER(w2f__i8) OpenAD_Symbol_1296
  INTEGER(w2f__i8) OpenAD_Symbol_1297
  INTEGER(w2f__i8) OpenAD_Symbol_1298
  INTEGER(w2f__i8) OpenAD_Symbol_1299
  INTEGER(w2f__i8) OpenAD_Symbol_1300
  INTEGER(w2f__i8) OpenAD_Symbol_1301
  INTEGER(w2f__i8) OpenAD_Symbol_1302
  INTEGER(w2f__i8) OpenAD_Symbol_1303
  INTEGER(w2f__i8) OpenAD_Symbol_1304
  INTEGER(w2f__i8) OpenAD_Symbol_1305
  INTEGER(w2f__i8) OpenAD_Symbol_1306
  INTEGER(w2f__i8) OpenAD_Symbol_1307
  INTEGER(w2f__i8) OpenAD_Symbol_1308
  INTEGER(w2f__i8) OpenAD_Symbol_1309
  INTEGER(w2f__i8) OpenAD_Symbol_1310
!
!       **** Parameters and Result ****
!
  INTEGER(w2f__i4) N
  INTEGER(w2f__i4) M
  INTEGER(w2f__i4) NZ
  REAL(w2f__8) TSTAR(1 : NZ, 0 : INT((N + 1)), 0 : INT((M + 1)))
  REAL(w2f__8) BSLIDE(1 : N, 1 : M)
  LOGICAL(w2f__i4) SLIDE_MASK(1 : N, 1 : M)
  LOGICAL(w2f__i4) SLIDE_FLAG
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J


          integer iaddr
          external iaddr
!
!       **** Statements ****
!

         if (our_rev_mode%plain) then
! original function
!$OPENAD XXX Template body_template.split.f90
  BSLIDE(1 : INT(N), 1 : INT(M)) = 3.168876612054834686D-11
  SLIDE_MASK(1 : N, 1 : M) = .FALSE.
  IF(SLIDE_FLAG) THEN
    DO I = 1, N, 1
      DO J = 1, M, 1
        IF(TSTAR(NZ, I, J) .eq. 0.0D00) THEN
          SLIDE_MASK(I, J) = .TRUE.
        ELSE
          SLIDE_MASK(I, J) = .FALSE.
        ENDIF
      END DO
    END DO
  ENDIF
          end if
          if (our_rev_mode%tape) then
! taping
!$OPENAD XXX Template body_template.split.f90
  BSLIDE(1 : INT(N), 1 : INT(M)) = 3.168876612054834686D-11
  SLIDE_MASK(1 : N, 1 : M) = .FALSE.
  IF(SLIDE_FLAG) THEN
    OpenAD_Symbol_1293 = 0_w2f__i8
    DO I = 1, N, 1
      OpenAD_Symbol_1294 = 0_w2f__i8
      DO J = 1, M, 1
        IF(TSTAR(NZ, I, J) .eq. 0.0D00) THEN
          SLIDE_MASK(I, J) = .TRUE.
          OpenAD_Symbol_1295 = 1_w2f__i8
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_1295
          oad_it_ptr = oad_it_ptr+1
        ELSE
          SLIDE_MASK(I, J) = .FALSE.
          OpenAD_Symbol_1296 = 0_w2f__i8
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_1296
          oad_it_ptr = oad_it_ptr+1
        ENDIF
        OpenAD_Symbol_1294 = (INT(OpenAD_Symbol_1294) + INT( 1_w2f__i8))
      END DO
      if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
      oad_it(oad_it_ptr) = OpenAD_Symbol_1294
      oad_it_ptr = oad_it_ptr+1
      OpenAD_Symbol_1293 = (INT(OpenAD_Symbol_1293) + INT( 1_w2f__i8))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_1293
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_1298 = 1_w2f__i8
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_1298
    oad_it_ptr = oad_it_ptr+1
  ELSE
    OpenAD_Symbol_1297 = 0_w2f__i8
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_1297
    oad_it_ptr = oad_it_ptr+1
  ENDIF
          end if
          if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1287 = oad_it(oad_it_ptr)
  IF(OpenAD_Symbol_1287 .ne. 0) THEN
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1288 = oad_it(oad_it_ptr)
    OpenAD_Symbol_1289 = 1
    DO WHILE(INT(OpenAD_Symbol_1289) .LE. INT(OpenAD_Symbol_1288) )
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1290 = oad_it(oad_it_ptr)
      OpenAD_Symbol_1291 = 1
      DO WHILE(INT(OpenAD_Symbol_1291) .LE. INT( OpenAD_Symbol_1290))
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1292 = oad_it(oad_it_ptr)
        IF(OpenAD_Symbol_1292 .ne. 0) THEN
        ENDIF
        OpenAD_Symbol_1291 = INT(OpenAD_Symbol_1291) + 1
      END DO
      OpenAD_Symbol_1289 = INT(OpenAD_Symbol_1289) + 1
    END DO
  ENDIF
          end if
          if (our_rev_mode%arg_store) then
  write(unit=cp_io_unit) N
  write(unit=cp_io_unit) M
  write(unit=cp_io_unit) NZ
  write(unit=cp_io_unit) SLIDE_FLAG
  write(unit=cp_io_unit) TSTAR
          end if
          if (our_rev_mode%arg_restore) then
  read(unit=cp_io_unit) N
  read(unit=cp_io_unit) M
  read(unit=cp_io_unit) NZ
  read(unit=cp_io_unit) SLIDE_FLAG
  read(unit=cp_io_unit) TSTAR
          end if
        end subroutine GREENLAND_BASALSLIDING
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

  SUBROUTINE GREENLAND_BASALVELOCITY(N, M, DX, DY, UBAS, VBAS, USRF, THCK, BSLID&
     &E, SLIDE_MASK)

          use OAD_tape
          use OAD_rev
          use OAD_cp
          use adism_mbal
          use glimmer_pdd

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

  use w2f__types
  use glimmer_global ,only: DP
  use physcon ,only: GRAV , RHOI
  use adism_core
  use glimmer_global ,only: DP
  use physcon ,only: GRAV , RHOI
  use adism_core
  use glimmer_global ,only: DP
  use physcon ,only: GRAV , RHOI
  use adism_core
  IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
  INTEGER(w2f__i8) OpenAD_Symbol_1311
  INTEGER(w2f__i8) OpenAD_Symbol_1312
  INTEGER(w2f__i8) OpenAD_Symbol_1313
  INTEGER(w2f__i8) OpenAD_Symbol_1314
  INTEGER(w2f__i8) OpenAD_Symbol_1315
  INTEGER(w2f__i8) OpenAD_Symbol_1316
  INTEGER(w2f__i8) OpenAD_Symbol_1317
  INTEGER(w2f__i8) OpenAD_Symbol_1318
  INTEGER(w2f__i8) OpenAD_Symbol_1319
  INTEGER(w2f__i8) OpenAD_Symbol_1320
  INTEGER(w2f__i8) OpenAD_Symbol_1321
  INTEGER(w2f__i8) OpenAD_Symbol_1322
  INTEGER(w2f__i8) OpenAD_Symbol_1323
  INTEGER(w2f__i8) OpenAD_Symbol_1324
  INTEGER(w2f__i8) OpenAD_Symbol_1325
  INTEGER(w2f__i8) OpenAD_Symbol_1326
  INTEGER(w2f__i8) OpenAD_Symbol_1327
  INTEGER(w2f__i8) OpenAD_Symbol_1328
!
!       **** Parameters and Result ****
!
  INTEGER(w2f__i4) N
  INTEGER(w2f__i4) M
  REAL(w2f__8) DX
  REAL(w2f__8) DY
  type(active) :: UBAS(1:N,1:M)
  type(active) :: VBAS(1:N,1:M)
  type(active) :: USRF(1:N,1:M)
  type(active) :: THCK(1:N,1:M)
  REAL(w2f__8) BSLIDE(1 : N, 1 : M)
  LOGICAL(w2f__i4) SLIDE_MASK(1 : N, 1 : M)
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) I
  INTEGER(w2f__i4) J
  INTEGER(w2f__i4) OpenAD_Symbol_1329
  INTEGER(w2f__i4) OpenAD_Symbol_1330
  INTEGER(w2f__i4) OpenAD_Symbol_1331
  INTEGER(w2f__i4) OpenAD_Symbol_1332
  INTEGER(w2f__i4) OpenAD_Symbol_1885
  INTEGER(w2f__i4) OpenAD_Symbol_1886
  REAL(w2f__8) OpenAD_Symbol_1887
  REAL(w2f__8) OpenAD_Symbol_1888
  INTEGER(w2f__i4) OpenAD_Symbol_1889
  INTEGER(w2f__i4) OpenAD_Symbol_1890
  INTEGER(w2f__i4) OpenAD_Symbol_1891
  INTEGER(w2f__i4) OpenAD_Symbol_1892
  REAL(w2f__8) OpenAD_Symbol_1893
  REAL(w2f__8) OpenAD_Symbol_1894
  INTEGER(w2f__i4) OpenAD_Symbol_1895
  INTEGER(w2f__i4) OpenAD_Symbol_1896
  INTEGER(w2f__i4) OpenAD_Symbol_1897
  INTEGER(w2f__i4) OpenAD_Symbol_1898
  REAL(w2f__8) OpenAD_acc_100
  REAL(w2f__8) OpenAD_acc_101
  REAL(w2f__8) OpenAD_acc_102
  REAL(w2f__8) OpenAD_acc_103
  REAL(w2f__8) OpenAD_aux_109
  REAL(w2f__8) OpenAD_aux_110
  REAL(w2f__8) OpenAD_aux_111
  REAL(w2f__8) OpenAD_aux_112
  REAL(w2f__8) OpenAD_aux_113
  REAL(w2f__8) OpenAD_aux_114
  REAL(w2f__8) OpenAD_aux_115
  REAL(w2f__8) OpenAD_aux_116
  REAL(w2f__8) OpenAD_aux_117
  REAL(w2f__8) OpenAD_aux_118
  REAL(w2f__8) OpenAD_lin_198
  REAL(w2f__8) OpenAD_lin_199
  REAL(w2f__8) OpenAD_lin_200
  REAL(w2f__8) OpenAD_lin_201
  REAL(w2f__8) OpenAD_lin_202
  REAL(w2f__8) OpenAD_lin_203
  REAL(w2f__8) OpenAD_lin_204
  REAL(w2f__8) OpenAD_lin_205
  type(active) :: OpenAD_prp_91
  type(active) :: OpenAD_prp_92
  type(active) :: OpenAD_prp_93
  type(active) :: OpenAD_prp_94
  type(active) :: OpenAD_prp_95
  type(active) :: OpenAD_prp_96
  type(active) :: OpenAD_prp_97
  type(active) :: OpenAD_prp_98


          integer iaddr
          external iaddr
!
!       **** Statements ****
!

         if (our_rev_mode%plain) then
! original function
!$OPENAD XXX Template body_template.split.f90
  DO I = 2, (N +(-1)), 1
    DO J = 2, (M +(-1)), 1
      IF(SLIDE_MASK(I, J)) THEN
        UBAS(INT(I),INT(J))%v = (-((THCK(I,J)%v*BSLIDE(I,J)*9.1D+02*9.8100000000&
     &0000049738D00*(USRF(I+1,J)%v-USRF(I+(-1),J)%v))/(DX*2.0D00)))

        VBAS(INT(I),INT(J))%v = (-((THCK(I,J)%v*BSLIDE(I,J)*9.1D+02*9.8100000000&
     &0000049738D00*(USRF(I,J+1)%v-USRF(I,J+(-1))%v))/(DY*2.0D00)))

      ELSE
        UBAS(INT(I),INT(J))%v = 0.0
        VBAS(INT(I),INT(J))%v = 0.0
      ENDIF
    END DO
  END DO
          end if
          if (our_rev_mode%tape) then
! taping
!$OPENAD XXX Template body_template.split.f90
  OpenAD_Symbol_1316 = 0_w2f__i8
  DO I = 2,(N+(-1)),1
    OpenAD_Symbol_1317 = 0_w2f__i8
    DO J = 2,(M+(-1)),1
      IF (SLIDE_MASK(I,J)) THEN
        OpenAD_aux_113 = (BSLIDE(I,J)*9.1D+02*9.81000000000000049738D00)
        OpenAD_aux_111 = (THCK(I,J)%v*OpenAD_aux_113)
        OpenAD_aux_112 = (USRF(I+1,J)%v-USRF(I+(-1),J)%v)
        OpenAD_aux_109 = (OpenAD_aux_111*OpenAD_aux_112)
        OpenAD_aux_110 = (DX*2.0D00)
        OpenAD_lin_201 = OpenAD_aux_113
        OpenAD_lin_199 = OpenAD_aux_112
        OpenAD_lin_200 = OpenAD_aux_111
        OpenAD_lin_198 = (INT(1_w2f__i8)/OpenAD_aux_110)
        UBAS(INT(I),INT(J))%v = (-(OpenAD_aux_109/OpenAD_aux_110))
        OpenAD_acc_100 = (OpenAD_lin_200*OpenAD_lin_198*INT((-1_w2f__i8)))
        OpenAD_acc_101 = (OpenAD_lin_201*OpenAD_lin_199*OpenAD_lin_198*INT((-1_w&
     &2f__i8)))

        OpenAD_Symbol_1329 = (I+1)
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_1329
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_1330 = (I+(-1))
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_1330
        oad_it_ptr = oad_it_ptr+1
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_acc_100
        oad_dt_ptr = oad_dt_ptr+1
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_acc_101
        oad_dt_ptr = oad_dt_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = I
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = J
        oad_it_ptr = oad_it_ptr+1
        OpenAD_aux_118 = (BSLIDE(I,J)*9.1D+02*9.81000000000000049738D00)
        OpenAD_aux_116 = (THCK(I,J)%v*OpenAD_aux_118)
        OpenAD_aux_117 = (USRF(I,J+1)%v-USRF(I,J+(-1))%v)
        OpenAD_aux_114 = (OpenAD_aux_116*OpenAD_aux_117)
        OpenAD_aux_115 = (DY*2.0D00)
        OpenAD_lin_205 = OpenAD_aux_118
        OpenAD_lin_203 = OpenAD_aux_117
        OpenAD_lin_204 = OpenAD_aux_116
        OpenAD_lin_202 = (INT(1_w2f__i8)/OpenAD_aux_115)
        VBAS(INT(I),INT(J))%v = (-(OpenAD_aux_114/OpenAD_aux_115))
        OpenAD_acc_102 = (OpenAD_lin_204*OpenAD_lin_202*INT((-1_w2f__i8)))
        OpenAD_acc_103 = (OpenAD_lin_205*OpenAD_lin_203*OpenAD_lin_202*INT((-1_w&
     &2f__i8)))

        OpenAD_Symbol_1331 = (J+1)
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_1331
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_1332 = (J+(-1))
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_1332
        oad_it_ptr = oad_it_ptr+1
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_acc_102
        oad_dt_ptr = oad_dt_ptr+1
        if (oad_dt_sz.lt.oad_dt_ptr) call oad_dt_grow()
        oad_dt(oad_dt_ptr) = OpenAD_acc_103
        oad_dt_ptr = oad_dt_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = I
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = J
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_1318 = 1_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_1318
        oad_it_ptr = oad_it_ptr+1
      ELSE
        UBAS(INT(I),INT(J))%v = 0.0
        VBAS(INT(I),INT(J))%v = 0.0
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = I
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = J
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_1319 = 0_w2f__i8
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_1319
        oad_it_ptr = oad_it_ptr+1
      ENDIF
      OpenAD_Symbol_1317 = (INT(OpenAD_Symbol_1317)+INT(1_w2f__i8))
    END DO
    if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
    oad_it(oad_it_ptr) = OpenAD_Symbol_1317
    oad_it_ptr = oad_it_ptr+1
    OpenAD_Symbol_1316 = (INT(OpenAD_Symbol_1316)+INT(1_w2f__i8))
  END DO
  if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
  oad_it(oad_it_ptr) = OpenAD_Symbol_1316
  oad_it_ptr = oad_it_ptr+1
          end if
          if (our_rev_mode%adjoint) then
! adjoint
  oad_it_ptr = oad_it_ptr-1
  OpenAD_Symbol_1311 = oad_it(oad_it_ptr)
  OpenAD_Symbol_1312 = 1
  do while (INT(OpenAD_Symbol_1312).LE.INT(OpenAD_Symbol_1311))
    oad_it_ptr = oad_it_ptr-1
    OpenAD_Symbol_1313 = oad_it(oad_it_ptr)
    OpenAD_Symbol_1314 = 1
    do while (INT(OpenAD_Symbol_1314).LE.INT(OpenAD_Symbol_1313))
      oad_it_ptr = oad_it_ptr-1
      OpenAD_Symbol_1315 = oad_it(oad_it_ptr)
      IF (OpenAD_Symbol_1315.ne.0) THEN
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1885 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1886 = oad_it(oad_it_ptr)
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1887 = oad_dt(oad_dt_ptr)
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1888 = oad_dt(oad_dt_ptr)
        OpenAD_prp_95%d = OpenAD_prp_95%d+VBAS(OpenAD_Symbol_1886,OpenAD_Symbol_&
     &1885)%d*(OpenAD_Symbol_1887)

        OpenAD_prp_98%d = OpenAD_prp_98%d+VBAS(OpenAD_Symbol_1886,OpenAD_Symbol_&
     &1885)%d*(OpenAD_Symbol_1888)

        VBAS(OpenAD_Symbol_1886,OpenAD_Symbol_1885)%d = 0.0d0
        OpenAD_prp_97%d = OpenAD_prp_97%d-OpenAD_prp_98%d
        OpenAD_prp_96%d = OpenAD_prp_96%d+OpenAD_prp_98%d
        OpenAD_prp_98%d = 0.0d0
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1889 = oad_it(oad_it_ptr)
        USRF(OpenAD_Symbol_1886,OpenAD_Symbol_1889)%d = USRF(OpenAD_Symbol_1886,&
     &OpenAD_Symbol_1889)%d+OpenAD_prp_97%d

        OpenAD_prp_97%d = 0.0d0
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1890 = oad_it(oad_it_ptr)
        USRF(OpenAD_Symbol_1886,OpenAD_Symbol_1890)%d = USRF(OpenAD_Symbol_1886,&
     &OpenAD_Symbol_1890)%d+OpenAD_prp_96%d

        OpenAD_prp_96%d = 0.0d0
        THCK(OpenAD_Symbol_1886,OpenAD_Symbol_1885)%d = THCK(OpenAD_Symbol_1886,&
     &OpenAD_Symbol_1885)%d+OpenAD_prp_95%d

        OpenAD_prp_95%d = 0.0d0
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1891 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1892 = oad_it(oad_it_ptr)
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1893 = oad_dt(oad_dt_ptr)
        oad_dt_ptr = oad_dt_ptr-1
        OpenAD_Symbol_1894 = oad_dt(oad_dt_ptr)
        OpenAD_prp_91%d = OpenAD_prp_91%d+UBAS(OpenAD_Symbol_1892,OpenAD_Symbol_&
     &1891)%d*(OpenAD_Symbol_1893)

        OpenAD_prp_94%d = OpenAD_prp_94%d+UBAS(OpenAD_Symbol_1892,OpenAD_Symbol_&
     &1891)%d*(OpenAD_Symbol_1894)

        UBAS(OpenAD_Symbol_1892,OpenAD_Symbol_1891)%d = 0.0d0
        OpenAD_prp_93%d = OpenAD_prp_93%d-OpenAD_prp_94%d
        OpenAD_prp_92%d = OpenAD_prp_92%d+OpenAD_prp_94%d
        OpenAD_prp_94%d = 0.0d0
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1895 = oad_it(oad_it_ptr)
        USRF(OpenAD_Symbol_1895,OpenAD_Symbol_1891)%d = USRF(OpenAD_Symbol_1895,&
     &OpenAD_Symbol_1891)%d+OpenAD_prp_93%d

        OpenAD_prp_93%d = 0.0d0
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1896 = oad_it(oad_it_ptr)
        USRF(OpenAD_Symbol_1896,OpenAD_Symbol_1891)%d = USRF(OpenAD_Symbol_1896,&
     &OpenAD_Symbol_1891)%d+OpenAD_prp_92%d

        OpenAD_prp_92%d = 0.0d0
        THCK(OpenAD_Symbol_1892,OpenAD_Symbol_1891)%d = THCK(OpenAD_Symbol_1892,&
     &OpenAD_Symbol_1891)%d+OpenAD_prp_91%d

        OpenAD_prp_91%d = 0.0d0
      ELSE
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1897 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1898 = oad_it(oad_it_ptr)
        VBAS(OpenAD_Symbol_1898,OpenAD_Symbol_1897)%d = 0.0d0
        UBAS(OpenAD_Symbol_1898,OpenAD_Symbol_1897)%d = 0.0d0
      ENDIF
      OpenAD_Symbol_1314 = INT(OpenAD_Symbol_1314)+1
    END DO
    OpenAD_Symbol_1312 = INT(OpenAD_Symbol_1312)+1
  END DO
          end if
          if (our_rev_mode%arg_store) then
  write(unit=cp_io_unit) N
  write(unit=cp_io_unit) M
  write(unit=cp_io_unit) DX
  write(unit=cp_io_unit) DY
  write(unit=cp_io_unit) UBAS%v
  write(unit=cp_io_unit) VBAS%v
  write(unit=cp_io_unit) USRF%v
  write(unit=cp_io_unit) THCK%v
  write(unit=cp_io_unit) BSLIDE
  write(unit=cp_io_unit) SLIDE_MASK
          end if
          if (our_rev_mode%arg_restore) then
  read(unit=cp_io_unit) N
  read(unit=cp_io_unit) M
  read(unit=cp_io_unit) DX
  read(unit=cp_io_unit) DY
  read(unit=cp_io_unit) UBAS%v
  read(unit=cp_io_unit) VBAS%v
  read(unit=cp_io_unit) USRF%v
  read(unit=cp_io_unit) THCK%v
  read(unit=cp_io_unit) BSLIDE
  read(unit=cp_io_unit) SLIDE_MASK
          end if
        end subroutine GREENLAND_BASALVELOCITY
END

!$OPENAD XXX File_start [adism_loop.F90]
      MODULE adism_loop
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
      SAVE
!
!     **** Statements ****
!
      CONTAINS

        SUBROUTINE LOOP(N, M, NZ, DX, DY, DT, NT, SIGMA, DEPTH, THCK, TOPG, PRCP&
     &, ARTM_PRES, LAT, LON, USRF, PDDTAB, TEMP, NCDF_OUT, G, VOL, NCDATA, NMSB_&
     &CTRL, BSLIDE_CTRL, ENFAC, SLIDE_FLAG, CALVING_FLAG, CLIM_FLAG, OUTFREQ)

  use OAD_tape
  use OAD_rev
  use OAD_cp
  use revolve
         use w2f__types
        use adism_body
        use adism_core
        use adism_ncdf
        use glimmer_pdd
        use adism_body
        use adism_core
        use adism_ncdf
        use glimmer_pdd
        use adism_body
        use adism_core
        use adism_ncdf
        use glimmer_pdd
        IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
        INTEGER(w2f__i8) OpenAD_Symbol_1333
        INTEGER(w2f__i8) OpenAD_Symbol_1334
        INTEGER(w2f__i8) OpenAD_Symbol_1335
        INTEGER(w2f__i8) OpenAD_Symbol_1336
        INTEGER(w2f__i8) OpenAD_Symbol_1337
        INTEGER(w2f__i8) OpenAD_Symbol_1338
!
!       **** Parameters and Result ****
!
        INTEGER(w2f__i4) N
        INTEGER(w2f__i4) M
        INTEGER(w2f__i4) NZ
        REAL(w2f__8) DX
        REAL(w2f__8) DY
        REAL(w2f__8) DT
        INTEGER(w2f__i4) NT
        REAL(w2f__8) SIGMA(1 : NZ)
        REAL(w2f__8) DEPTH(1 : NZ)
        type(active) :: THCK(1:N,1:M)
        REAL(w2f__8) TOPG(1 : N, 1 : M)
        REAL(w2f__8) PRCP(1 : N, 1 : M)
        REAL(w2f__8) ARTM_PRES(1 : N, 1 : M)
        REAL(w2f__8) LAT(1 : N, 1 : M)
        REAL(w2f__8) LON(1 : N, 1 : M)
        type(active) :: USRF(1:N,1:M)
        REAL(w2f__8) PDDTAB(1 : 31, 1 : 71)
        type(active) :: TEMP(1:NZ,0:INT((N+1)),0:INT((M+1)))
        LOGICAL(w2f__i4) NCDF_OUT
        REAL(w2f__8) G(1 : N, 1 : M)
        type(active) :: VOL
        type(ADISM_NCDATA) :: NCDATA
        type(active) :: NMSB_CTRL(1:N,1:M)
        REAL(w2f__8) BSLIDE_CTRL(1 : N, 1 : M)
        REAL(w2f__8) ENFAC
        LOGICAL(w2f__i4) SLIDE_FLAG
        LOGICAL(w2f__i4) CALVING_FLAG
        INTEGER(w2f__i4) CLIM_FLAG
        INTEGER(w2f__i4) OUTFREQ
!
!       **** Local Variables and Functions ****
!
        type(active) :: BMLT(1:N,1:M)
        REAL(w2f__8) BWAT(1 : N, 1 : M)
        type(active) :: DIFF(1:N,1:M)
        type(active) :: DTHCKDTM(1:N,1:M)
        type(active) :: DUSRFDTM(1:N,1:M)
        INTEGER(w2f__i4) T
        type(active) :: THCKOLD(1:N,1:M)
        type(active) :: USRFOLD(1:N,1:M)
        INTEGER(w2f__i4) OpenAD_Symbol_1899
        INTEGER(w2f__i4) OpenAD_Symbol_1900
        INTEGER(w2f__i4) OpenAD_Symbol_1901
        INTEGER(w2f__i4) OpenAD_Symbol_1902
        type(active) :: OpenAD_prp_99(:,:)
        ALLOCATABLE OpenAD_prp_99
!
!       **** Statements ****
!

  LOGICAL :: initialized=.FALSE.
  TYPE(rvAction),save :: theAction
  CHARACTER(80) :: errorMsg
  integer, save :: jointCPCount
  integer, save :: currIter
!$OPENAD XXX Template loop_template.revolve.f90

  if (our_rev_mode%plain) then
! repeat loop contents from here, but this is just for illustration as we won't u
! se it
     usrfold = usrf
     thckold = thck
     dthckdtm%v = 0.0
     dusrfdtm%v = 0.0
     bwat = 0.0
     bmlt%v = 0.0
     do t=1,nt
        call body(n,m,nz,dx,dy,dt,sigma,depth,thck,topg,usrf,prcp,artm_pres,lat,&
     &lon,thckold,usrfold,dthckdtm, dusrfdtm,diff,pddtab,bwat,bmlt,temp,ncdf_out&
     &,G,vol,ncdata,nmsb_ctrl,bslide_ctrl,enFac,slide_flag, calving_flag,clim_fl&
     &ag,outfreq,t)

     enddo
! repeat loop contents to here
  end if
  if (our_rev_mode%tape) then
! repeat steps before the loop contents  here since we are in outer split mode
     usrfold = usrf
     thckold = thck
     dthckdtm%v = 0.0
     dusrfdtm%v = 0.0
     bwat = 0.0
     bmlt%v = 0.0
! repeated until here
     currIter=0
     jointCPcount=cp_fNumber()
     initialized=rvInit(nt,4,errorMsg,theAction)
     call rvVerbose(0)
     IF (.NOT.initialized) WRITE(*,'(A,A)') 'Error: ', errorMsg
     do while (theAction%actionFlag/=rvDone)
        theAction=rvNextAction()
        select case (theAction%actionFlag)
        case (rvStore)
           call cp_write_open(theAction%cpNum+jointCPCount)
           our_rev_mode%arg_store=.TRUE.
           our_rev_mode%tape=.FALSE.
           call body(n,m,nz,dx,dy,dt,sigma,depth,thck,topg,usrf,prcp,artm_pres,l&
     &at,lon,thckold,usrfold,dthckdtm, dusrfdtm,diff,pddtab,bwat,bmlt,temp,ncdf_&
     &out,G,vol,ncdata,nmsb_ctrl,bslide_ctrl,enFac,slide_flag, calving_flag,clim&
     &_flag,outfreq,currIter+1)

           our_rev_mode%arg_store=.FALSE.
           our_rev_mode%tape=.TRUE.
           call cp_close
        case (rvForward)
           call OAD_revPlain
           do currIter=currIter,theAction%iteration-1
              call body(n,m,nz,dx,dy,dt,sigma,depth,thck,topg,usrf,prcp,artm_pre&
     &s,lat,lon,thckold,usrfold,dthckdtm, dusrfdtm,diff,pddtab,bwat,bmlt,temp,nc&
     &df_out,G,vol,ncdata,nmsb_ctrl,bslide_ctrl,enFac,slide_flag, calving_flag,c&
     &lim_flag,outfreq,currIter+1)

           end do
           call OAD_revTape
        case (rvFirstUTurn)
           call body(n,m,nz,dx,dy,dt,sigma,depth,thck,topg,usrf,prcp,artm_pres,l&
     &at,lon,thckold,usrfold,dthckdtm, dusrfdtm,diff,pddtab,bwat,bmlt,temp,ncdf_&
     &out,G,vol,ncdata,nmsb_ctrl,bslide_ctrl,enFac,slide_flag, calving_flag,clim&
     &_flag,outfreq,currIter+1)

! get out now ...
           exit
        end select
     end do
  end if
  if (our_rev_mode%adjoint) then
     IF (.NOT.initialized) WRITE(*,'(A)') 'Error: not initialized'
     do while (theAction%actionFlag/=rvDone)
        select case (theAction%actionFlag)
        case (rvFirstUTurn)
!we taped already ... see above ... do the adjoint
           call body(n,m,nz,dx,dy,dt,sigma,depth,thck,topg,usrf,prcp,artm_pres,l&
     &at,lon,thckold,usrfold,dthckdtm, dusrfdtm,diff,pddtab,bwat,bmlt,temp,ncdf_&
     &out,G,vol,ncdata,nmsb_ctrl,bslide_ctrl,enFac,slide_flag,calving_flag,clim_&
     &flag,outfreq,currIter+1)

        case (rvStore)
           call cp_write_open(theAction%cpNum+jointCPCount)
           our_rev_mode%arg_store=.TRUE.
           our_rev_mode%adjoint=.FALSE.
           call body(n,m,nz,dx,dy,dt,sigma,depth,thck,topg,usrf,prcp,artm_pres,l&
     &at,lon,thckold,usrfold,dthckdtm, dusrfdtm,diff,pddtab,bwat,bmlt,temp,ncdf_&
     &out,G,vol,ncdata,nmsb_ctrl,bslide_ctrl,enFac,slide_flag, calving_flag,clim&
     &_flag,outfreq,currIter+1)

           our_rev_mode%arg_store=.FALSE.
           our_rev_mode%adjoint=.TRUE.
           call cp_close
        case (rvRestore)
           call cp_read_open(theAction%cpNum+jointCPCount)
           our_rev_mode%arg_restore=.TRUE.
           our_rev_mode%adjoint=.FALSE.
           call body(n,m,nz,dx,dy,dt,sigma,depth,thck,topg,usrf,prcp,artm_pres,l&
     &at,lon,thckold,usrfold,dthckdtm, dusrfdtm,diff,pddtab,bwat,bmlt,temp,ncdf_&
     &out,G,vol,ncdata,nmsb_ctrl,bslide_ctrl,enFac,slide_flag, calving_flag,clim&
     &_flag,outfreq,currIter+1)

           our_rev_mode%arg_restore=.FALSE.
           our_rev_mode%adjoint=.TRUE.
           currIter=theAction%iteration
           call cp_close
        case (rvForward)
           call OAD_revPlain
           do currIter=currIter,theAction%iteration-1
              call body(n,m,nz,dx,dy,dt,sigma,depth,thck,topg,usrf,prcp,artm_pre&
     &s,lat,lon,thckold,usrfold,dthckdtm, dusrfdtm,diff,pddtab,bwat,bmlt,temp,nc&
     &df_out,G,vol,ncdata,nmsb_ctrl,bslide_ctrl,enFac,slide_flag, calving_flag,c&
     &lim_flag,outfreq,currIter+1)

           end do
           call OAD_revAdjoint
        case (rvUTurn)
           call OAD_revTape
           call body(n,m,nz,dx,dy,dt,sigma,depth,thck,topg,usrf,prcp,artm_pres,l&
     &at,lon,thckold,usrfold,dthckdtm, dusrfdtm,diff,pddtab,bwat,bmlt,temp,ncdf_&
     &out,G,vol,ncdata,nmsb_ctrl,bslide_ctrl,enFac,slide_flag, calving_flag,clim&
     &_flag,outfreq,currIter+1)

           call OAD_revAdjoint
           call body(n,m,nz,dx,dy,dt,sigma,depth,thck,topg,usrf,prcp,artm_pres,l&
     &at,lon,thckold,usrfold,dthckdtm, dusrfdtm,diff,pddtab,bwat,bmlt,temp,ncdf_&
     &out,G,vol,ncdata,nmsb_ctrl,bslide_ctrl,enFac,slide_flag, calving_flag,clim&
     &_flag,outfreq,currIter+1)

        end select
        theAction=rvNextAction()
     end do
  end if
end subroutine LOOP
      END

!$OPENAD XXX File_start [adism_wrap.F90]
      MODULE adism_wrap
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
      SAVE
!
!     **** Statements ****
!
      CONTAINS
!#########################################################
! This file is part of OpenAD released under the LGPL.   #
! The full COPYRIGHT notice can be found in the top      #
! level directory of the OpenAD distribution             #
!#########################################################

        SUBROUTINE MAIN(N, M, NZ, DX, DY, DT, NT, THCK, TOPG, PRCP, ARTM_PRES, L&
     &AT, LON, USRF, NMSB_CTRL, BSLIDE_CTRL, G_CTRL, TEMP , VOL, GTHF, ENFAC, SL&
     &IDE_FLAG, NCDF_OUT, CALVING_FLAG, CLIM_FLAG, OUTFILE, OUTFREQ)

    use OAD_tape
    use OAD_rev

! original arguments get inserted before version
!         ! and declared here together with all local variables
!         ! generated by xaifBooster

        use w2f__types
        use adism_ncdf
        use adism_loop
        use adism_core
        use glimmer_pdd
        use adism_ncdf
        use adism_loop
        use adism_core
        use glimmer_pdd
        use adism_ncdf
        use adism_loop
        use adism_core
        use glimmer_pdd
        IMPLICIT NONE
!
!       **** Global Variables & Derived Type Definitions ****
!
        INTEGER(w2f__i8) OpenAD_Symbol_1339
        INTEGER(w2f__i8) OpenAD_Symbol_1340
        INTEGER(w2f__i8) OpenAD_Symbol_1341
        INTEGER(w2f__i8) OpenAD_Symbol_1342
        INTEGER(w2f__i8) OpenAD_Symbol_1343
        INTEGER(w2f__i8) OpenAD_Symbol_1344
        INTEGER(w2f__i8) OpenAD_Symbol_1345
        INTEGER(w2f__i8) OpenAD_Symbol_1346
        INTEGER(w2f__i8) OpenAD_Symbol_1347
        INTEGER(w2f__i8) OpenAD_Symbol_1348
        INTEGER(w2f__i8) OpenAD_Symbol_1349
        INTEGER(w2f__i8) OpenAD_Symbol_1350
        INTEGER(w2f__i8) OpenAD_Symbol_1351
        INTEGER(w2f__i8) OpenAD_Symbol_1352
        INTEGER(w2f__i8) OpenAD_Symbol_1353
        INTEGER(w2f__i8) OpenAD_Symbol_1354
        INTEGER(w2f__i8) OpenAD_Symbol_1355
        INTEGER(w2f__i8) OpenAD_Symbol_1356
        INTEGER(w2f__i8) OpenAD_Symbol_1357
        INTEGER(w2f__i8) OpenAD_Symbol_1358
        INTEGER(w2f__i8) OpenAD_Symbol_1359
        INTEGER(w2f__i8) OpenAD_Symbol_1360
        INTEGER(w2f__i8) OpenAD_Symbol_1361
        INTEGER(w2f__i8) OpenAD_Symbol_1362
!
!       **** Parameters and Result ****
!
        INTEGER(w2f__i4) N
        INTEGER(w2f__i4) M
        INTEGER(w2f__i4) NZ
        REAL(w2f__8) DX
        REAL(w2f__8) DY
        REAL(w2f__8) DT
        INTEGER(w2f__i4) NT
        type(active) :: THCK(1:N,1:M)
        REAL(w2f__8) TOPG(1 : N, 1 : M)
        REAL(w2f__8) PRCP(1 : N, 1 : M)
        REAL(w2f__8) ARTM_PRES(1 : N, 1 : M)
        REAL(w2f__8) LAT(1 : N, 1 : M)
        REAL(w2f__8) LON(1 : N, 1 : M)
        type(active) :: USRF(1:N,1:M)
        type(active) :: NMSB_CTRL(1:N,1:M)
        REAL(w2f__8) BSLIDE_CTRL(1 : N, 1 : M)
        REAL(w2f__8) G_CTRL(1 : N, 1 : M)
        type(active) :: TEMP(1:NZ,0:INT((N+1)),0:INT((M+1)))
        type(active) :: VOL
        REAL(w2f__8) GTHF
        REAL(w2f__8) ENFAC
        LOGICAL(w2f__i4) SLIDE_FLAG
        LOGICAL(w2f__i4) NCDF_OUT
        LOGICAL(w2f__i4) CALVING_FLAG
        INTEGER(w2f__i4) CLIM_FLAG
        CHARACTER(200) OUTFILE
        INTEGER(w2f__i4) OUTFREQ
!
!       **** Local Variables and Functions ****
!
        REAL(w2f__8) DEPTH(1 : NZ)
        REAL(w2f__8) G(1 : N, 1 : M)
        type(ADISM_NCDATA) :: NCDATA
        REAL(w2f__8) PDDTAB(1 : 31, 1 : 71)
        REAL(w2f__8) SIGMA(1 : NZ)
        REAL(w2f__8) TMP1
        INTEGER(w2f__i4) UP
        INTEGER(w2f__i4) OpenAD_Symbol_1903
        INTEGER(w2f__i4) OpenAD_Symbol_1904
        type(active) :: OpenAD_prp_100(:,:)
        ALLOCATABLE OpenAD_prp_100


    integer iaddr
    external iaddr
!
!       **** Top Level Pragmas ****
!
!$OPENAD INDEPENDENT(NMSB_CTRL)
!$OPENAD DEPENDENT(VOL)
!
!       **** Statements ****
!

   if (our_rev_mode%plain) then
! original function
        G(1 : INT(N), 1 : INT(M)) = GTHF
        G(1 : INT(N), 1 : INT(M)) = (G_CTRL(1 : N, 1 : M) + G(1 : N, 1 : M))
        DO UP = 1, NZ, 1
          TMP1 = (REAL(UP +(-1), 8) / REAL(NZ +(-1), 8))
          CALL CALC_SIGMA(TMP1,2.0D00,SIGMA(UP))
        END DO
        DO UP = 1, (NZ +(-1)), 1
          DEPTH(INT(UP)) = ((((SIGMA(UP) + SIGMA(UP + 1)) * 5.0D-01) ** 3) *(SIG&
     &MA(UP + 1) - SIGMA(UP)))

        END DO
        DEPTH(INT(NZ)) = 0.0D00
        USRF(1:INT(N),1:INT(M))%v = (THCK(1:N,1:M)%v+TOPG(1:N,1:M))
        CALL GLIMMER_PDD_INIT(PDDTAB)
        IF(NCDF_OUT) THEN
          CALL NETCDF_CREATE(NCDATA,N,M,NZ,OUTFILE)
        ENDIF
        CALL LOOP(N,M,NZ,DX,DY,DT,NT,SIGMA,DEPTH,THCK,TOPG,PRCP,ARTM_PRES,LAT,LO&
     &N,USRF,PDDTAB,TEMP,NCDF_OUT,G,VOL,NCDATA,NMSB_CTRL,BSLIDE_CTRL,ENFAC,SLIDE&
     &_FLAG,CALVING_FLAG,CLIM_FLAG,OUTFREQ)

        IF(NCDF_OUT) THEN
          CALL NETCDF_CLOSE(NCDATA)
        ENDIF
    end if
    if (our_rev_mode%tape) then
! taping
        G(1:INT(N),1:INT(M)) = GTHF
        G(1:INT(N),1:INT(M)) = (G_CTRL(1:N,1:M)+G(1:N,1:M))
        OpenAD_Symbol_1345 = 0_w2f__i8
        DO UP = 1,NZ,1
          TMP1 = (REAL(UP+(-1),8)/REAL(NZ+(-1),8))
          CALL CALC_SIGMA(TMP1,2.0D00,SIGMA(UP))
          OpenAD_Symbol_1345 = (INT(OpenAD_Symbol_1345)+INT(1_w2f__i8))
        END DO
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_1345
        oad_it_ptr = oad_it_ptr+1
        OpenAD_Symbol_1346 = 0_w2f__i8
        DO UP = 1,(NZ+(-1)),1
          DEPTH(INT(UP)) = ((((SIGMA(UP)+SIGMA(UP+1))*5.0D-01)**3)*(SIGMA(UP+1)-&
     &SIGMA(UP)))

          OpenAD_Symbol_1346 = (INT(OpenAD_Symbol_1346)+INT(1_w2f__i8))
        END DO
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = OpenAD_Symbol_1346
        oad_it_ptr = oad_it_ptr+1
        DEPTH(INT(NZ)) = 0.0D00
        USRF(1:INT(N),1:INT(M))%v = (THCK(1:N,1:M)%v+TOPG(1:N,1:M))
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = N
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = M
        oad_it_ptr = oad_it_ptr+1
        CALL GLIMMER_PDD_INIT(PDDTAB)
        IF(NCDF_OUT) THEN
          CALL NETCDF_CREATE(NCDATA,N,M,NZ,OUTFILE)
          OpenAD_Symbol_1347 = 1_w2f__i8
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_1347
          oad_it_ptr = oad_it_ptr+1
        ELSE
          OpenAD_Symbol_1348 = 0_w2f__i8
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_1348
          oad_it_ptr = oad_it_ptr+1
        ENDIF
        CALL LOOP(N,M,NZ,DX,DY,DT,NT,SIGMA,DEPTH,THCK,TOPG,PRCP,ARTM_PRES,LAT,LO&
     &N,USRF,PDDTAB,TEMP,NCDF_OUT,G,VOL,NCDATA,NMSB_CTRL,BSLIDE_CTRL,ENFAC,SLIDE&
     &_FLAG,CALVING_FLAG,CLIM_FLAG,OUTFREQ)

        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = N
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = M
        oad_it_ptr = oad_it_ptr+1
        if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
        oad_it(oad_it_ptr) = NZ
        oad_it_ptr = oad_it_ptr+1
        IF(NCDF_OUT) THEN
          CALL NETCDF_CLOSE(NCDATA)
          OpenAD_Symbol_1349 = 1_w2f__i8
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_1349
          oad_it_ptr = oad_it_ptr+1
        ELSE
          OpenAD_Symbol_1350 = 0_w2f__i8
          if (oad_it_sz.lt.oad_it_ptr) call oad_it_grow()
          oad_it(oad_it_ptr) = OpenAD_Symbol_1350
          oad_it_ptr = oad_it_ptr+1
        ENDIF
    end if
    if (our_rev_mode%adjoint) then
! adjoint
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1339 = oad_it(oad_it_ptr)
        IF (OpenAD_Symbol_1339.ne.0) THEN
          CALL NETCDF_CLOSE(NCDATA)
        ENDIF
        oad_it_ptr = oad_it_ptr-1
        NZ = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        M = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        N = oad_it(oad_it_ptr)
        CALL LOOP(N,M,NZ,DX,DY,DT,NT,SIGMA,DEPTH,THCK,TOPG,PRCP,ARTM_PRES,LAT,LO&
     &N,USRF,PDDTAB,TEMP,NCDF_OUT,G,VOL,NCDATA,NMSB_CTRL,BSLIDE_CTRL,ENFAC,SLIDE&
     &_FLAG,CALVING_FLAG,CLIM_FLAG,OUTFREQ)

        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1340 = oad_it(oad_it_ptr)
        IF (OpenAD_Symbol_1340.ne.0) THEN
          CALL NETCDF_CREATE(NCDATA,N,M,NZ,OUTFILE)
        ENDIF
        CALL GLIMMER_PDD_INIT(PDDTAB)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1903 = oad_it(oad_it_ptr)
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1904 = oad_it(oad_it_ptr)
!!! requested inline of 'oad_AllocateMatching' has no defn
        CALL oad_AllocateMatching(OpenAD_prp_100,THCK(1:N,1:M))
        OpenAD_prp_100%d = OpenAD_prp_100%d+USRF(1:OpenAD_Symbol_1904,1:OpenAD_S&
     &ymbol_1903)%d

        USRF(1:OpenAD_Symbol_1904,1:OpenAD_Symbol_1903)%d = 0.0d0
        THCK(1:OpenAD_Symbol_1904,1:OpenAD_Symbol_1903)%d = THCK(1:OpenAD_Symbol&
     &_1904,1:OpenAD_Symbol_1903)%d+OpenAD_prp_100%d

        OpenAD_prp_100%d = 0.0d0
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1341 = oad_it(oad_it_ptr)
        OpenAD_Symbol_1342 = 1
        do while (INT(OpenAD_Symbol_1342).LE.INT(OpenAD_Symbol_1341))
          OpenAD_Symbol_1342 = INT(OpenAD_Symbol_1342)+1
        END DO
        oad_it_ptr = oad_it_ptr-1
        OpenAD_Symbol_1343 = oad_it(oad_it_ptr)
        OpenAD_Symbol_1344 = 1
        do while (INT(OpenAD_Symbol_1344).LE.INT(OpenAD_Symbol_1343))
          CALL CALC_SIGMA(TMP1,2.0D00,SIGMA(UP))
          OpenAD_Symbol_1344 = INT(OpenAD_Symbol_1344)+1
        END DO
    end if
  end subroutine MAIN
      END
