
C$OPENAD XXX File_start [OAD_intrinsics.f90]
      MODULE oad_intrinsics
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Statements ****
C
      END MODULE

C$OPENAD XXX File_start [all_globals_mod.f]
      MODULE all_globals_mod
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Statements ****
C
      END MODULE

C$OPENAD XXX File_start [head.f]
      MODULE m
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      TYPE  T
        TYPE (OpenADTy_active) V(1 : 2)
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
        TYPE (T) TA(1 :)
C
C       **** Local Variables and Functions ****
C
        INTEGER(w2f__i4) I
        INTEGER(w2f__i4) J
        TYPE (OpenADTy_active) OpenAD_prp_0
C
C       **** Statements ****
C
        J = 1
        DO I = 1, UBOUND(TA, 1), 1
          __value__(TA(I)%V(INT(J))) = (__value__(TA(I)%V(J)) * 2.0D00)
          CALL setderiv(__deriv__(OpenAD_prp_0), __deriv__(TA(I)%V(J)))
          CALL sax(2.0D00, __deriv__(OpenAD_prp_0), __deriv__(TA(I)%V(J
     > )))
          J = (J + 1)
        END DO
        END SUBROUTINE
      END

      SUBROUTINE head(X, Y)
      use w2f__types
      use oad_intrinsics
      use m
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X(1 : 2)
      TYPE (OpenADTy_active) Y(1 : 2)
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      TYPE (T) TA(1 : 2)
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
      DO I = 1, 2, 1
        __value__(TA(I)%V(INT(I))) = __value__(X(I))
        CALL setderiv(__deriv__(TA(I)%V(I)), __deriv__(X(I)))
      END DO
      CALL FOO(__deriv__(TA))
      DO I = 1, 2, 1
        __value__(Y(INT(I))) = __value__(TA(I)%V(I))
        CALL setderiv(__deriv__(Y(I)), __deriv__(TA(I)%V(I)))
      END DO
      END SUBROUTINE
