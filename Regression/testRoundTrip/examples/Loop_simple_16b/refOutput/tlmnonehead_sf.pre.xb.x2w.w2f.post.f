
!$OPENAD XXX File_start [OAD_intrinsics.f90]
MODULE oad_intrinsics
use OAD_active
use w2f__types
IMPLICIT NONE
SAVE
!
!     **** Statements ****
!
END MODULE

C$OPENAD XXX File_start [all_globals_mod.f]
      MODULE all_globals_mod
      use OAD_active
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Statements ****
C
      END MODULE

C$OPENAD XXX File_start [head.f]
      SUBROUTINE lagran(I, X, A, SP, LAG)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      INTEGER(w2f__i4) I
      REAL(w2f__8) X
      type(active) :: A(1:4)
      INTEGER(w2f__i4) SP
      type(active) :: LAG
      INTENT(OUT) LAG
C
C     **** Local Variables and Functions ****
C
      type(active) :: DENOM
      INTEGER(w2f__i4) K
      type(active) :: NUMER
      REAL(w2f__8) OpenAD_acc_0
      REAL(w2f__8) OpenAD_aux_0
      REAL(w2f__8) OpenAD_aux_1
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_2
      REAL(w2f__8) OpenAD_lin_3
      REAL(w2f__8) OpenAD_lin_4
      REAL(w2f__8) OpenAD_lin_5
      type(active) :: OpenAD_prp_0
      type(active) :: OpenAD_prp_1
      type(active) :: OpenAD_prp_2
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      NUMER%v = 1.0D00
      DENOM%v = 1.0D00
      CALL zero_deriv(NUMER)
      CALL zero_deriv(DENOM)
C$OPENAD XXX Simple loop
      DO K = 1,SP,1
        IF (I.ne.K) THEN
          OpenAD_aux_0 = (A(I)%v-A(K)%v)
          OpenAD_lin_0 = OpenAD_aux_0
          OpenAD_lin_1 = DENOM%v
          DENOM%v = (DENOM%v*OpenAD_aux_0)
          OpenAD_aux_1 = (X-A(K)%v)
          OpenAD_lin_2 = OpenAD_aux_1
          OpenAD_lin_3 = NUMER%v
          NUMER%v = (NUMER%v*OpenAD_aux_1)
          OpenAD_acc_0 = (INT((-1_w2f__i8))*OpenAD_lin_3)
          CALL setderiv(OpenAD_prp_0,DENOM)
          CALL setderiv(OpenAD_prp_1,NUMER)
          CALL setderiv(OpenAD_prp_2,A(I))
          CALL dec_deriv(OpenAD_prp_2,A(K))
          CALL sax(OpenAD_lin_0,OpenAD_prp_0,DENOM)
          CALL saxpy(OpenAD_lin_1,OpenAD_prp_2,DENOM)
          CALL sax(OpenAD_lin_2,OpenAD_prp_1,NUMER)
          CALL saxpy(OpenAD_acc_0,A(K),NUMER)
        ENDIF
      END DO
      OpenAD_lin_4 = (INT(1_w2f__i8)/DENOM%v)
      OpenAD_lin_5 = (-(NUMER%v/(DENOM%v*DENOM%v)))
      LAG%v = (NUMER%v/DENOM%v)
      CALL sax(OpenAD_lin_4,NUMER,LAG)
      CALL saxpy(OpenAD_lin_5,DENOM,LAG)
      END SUBROUTINE

      SUBROUTINE head(X, Y)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X(1:4)
      INTENT(IN) X
      type(active) :: Y(1:1)
      INTENT(OUT) Y
C
C     **** Local Variables and Functions ****
C
      EXTERNAL lagran
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      CALL lagran(2,2.0D00,X,4,Y(1))
      END SUBROUTINE
