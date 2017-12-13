
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
      SUBROUTINE head(X, Y)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X(1 : 3)
      INTENT(IN)  X
      TYPE (OpenADTy_active) Y(1 : 3)
      INTENT(OUT)  Y
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) J
      INTEGER(w2f__i4) K
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      TYPE (OpenADTy_active) OpenAD_prp_0
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      DO I = 1, 3, 1
        __value__(Y(INT(I))) = __value__(X(I))
        CALL setderiv(__deriv__(Y(I)), __deriv__(X(I)))
      END DO
      DO I = 1, 3, 1
        DO J = 1, 3, 1
          DO K = 1, 3, 1
            OpenAD_lin_0 = __value__(Y(I))
            OpenAD_lin_1 = __value__(X(J))
            __value__(Y(INT(I))) = (__value__(X(J)) * __value__(Y(I)))
            CALL setderiv(__deriv__(OpenAD_prp_0), __deriv__(Y(I)))
            CALL sax(OpenAD_lin_0, __deriv__(X(J)), __deriv__(Y(I)))
            CALL saxpy(OpenAD_lin_1, __deriv__(OpenAD_prp_0), __deriv__
     > (Y(I)))
          END DO
        END DO
      END DO
      END SUBROUTINE
