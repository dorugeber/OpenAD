
C$OPENAD XXX File_start [head.f]
      MODULE m
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      TYPE T
        type(active) :: V(1:2)
      END TYPE

C
C     **** Statements ****
C
      CONTAINS

        SUBROUTINE FOO(TA)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        type(T) :: TA(1:)
C
C       **** Local Variables and Functions ****
C
        INTEGER(w2f__i4) I
        INTEGER(w2f__i4) J
        type(active) :: OpenAD_prp_0
C
C       **** Statements ****
C
        J = 1
        DO I = 1, UBOUND(TA, 1), 1
          TA(I)%V(INT(J))%v = (TA(I)%V(J)%v*2.0D00)
          CALL setderiv(OpenAD_prp_0,TA(I)%V(J))
          CALL sax(2.0D00,OpenAD_prp_0,TA(I)%V(J))
          J = (J+1)
        END DO
        END SUBROUTINE
      END

      SUBROUTINE head(X, Y)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      use m
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X(1:2)
      type(active) :: Y(1:2)
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      type(T) :: TA(1:2)
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
      DO I = 1, 2, 1
        TA(I)%V(INT(I))%v = X(I)%v
        CALL setderiv(TA(I)%V(I),X(I))
      END DO
      CALL FOO(TA)
      DO I = 1,2,1
        Y(INT(I))%v = TA(I)%V(I)%v
        CALL setderiv(Y(I),TA(I)%V(I))
      END DO
      END SUBROUTINE
