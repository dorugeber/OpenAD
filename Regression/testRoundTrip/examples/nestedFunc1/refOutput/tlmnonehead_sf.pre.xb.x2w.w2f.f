
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
      SUBROUTINE head(J, X, Y)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      INTEGER(w2f__i4) J
      TYPE (OpenADTy_active) X(1 : J)
      INTENT(IN)  X
      TYPE (OpenADTy_active) Y(1 : 1)
      INTENT(OUT)  Y
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i8) t__1
      INTEGER(w2f__i8) t__2
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      __value__(Y(1)) = 1.0
      CALL zero_deriv(__deriv__(Y(1)))
      CALL FOO()
      CONTAINS

        SUBROUTINE FOO()
        use w2f__types
        IMPLICIT NONE
C
C       **** Local Variables and Functions ****
C
        INTEGER(w2f__i4) I
        REAL(w2f__8) OpenAD_lin_0
        REAL(w2f__8) OpenAD_lin_1
        TYPE (OpenADTy_active) OpenAD_prp_0
C
C       **** Statements ****
C
        DO I = 1, J, 1
          OpenAD_lin_0 = __value__(Y(1))
          OpenAD_lin_1 = __value__(X(J))
          __value__(Y(1)) = (__value__(X(J)) * __value__(Y(1)))
          CALL setderiv(__deriv__(OpenAD_prp_0), __deriv__(Y(1)))
          CALL sax(OpenAD_lin_0, __deriv__(X(J)), __deriv__(Y(1)))
          CALL saxpy(OpenAD_lin_1, __deriv__(OpenAD_prp_0), __deriv__(Y
     > (1)))
        END DO
        END SUBROUTINE
      END
