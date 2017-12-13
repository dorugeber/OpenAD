
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
      TYPE (OpenADTy_active) X(1 : 2)
      INTENT(IN)  X
      TYPE (OpenADTy_active) Y(1 : 2)
      INTENT(OUT)  Y
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_2
      REAL(w2f__8) OpenAD_lin_3
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      IF(__value__(X(1)) .LT. __value__(X(2))) THEN
        DO I = 1, 2, 1
          OpenAD_lin_0 = __value__(X(I))
          OpenAD_lin_1 = __value__(X(I))
          __value__(Y(INT(I))) = (__value__(X(I)) * __value__(X(I)))
          CALL sax(OpenAD_lin_0, __deriv__(X(I)), __deriv__(Y(I)))
          CALL saxpy(OpenAD_lin_1, __deriv__(X(I)), __deriv__(Y(I)))
        END DO
      ELSE
        DO I = 1, 2, 1
          OpenAD_lin_2 = __value__(X(I))
          OpenAD_lin_3 = __value__(X(I))
          __value__(Y(INT(I))) = (__value__(X(I)) * __value__(X(I)))
          CALL sax(OpenAD_lin_2, __deriv__(X(I)), __deriv__(Y(I)))
          CALL saxpy(OpenAD_lin_3, __deriv__(X(I)), __deriv__(Y(I)))
        END DO
      ENDIF
      END SUBROUTINE
