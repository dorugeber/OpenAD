
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
      TYPE (OpenADTy_active) X(1 : 10)
      INTENT(IN)  X
      TYPE (OpenADTy_active) Y(1 : 10)
      INTENT(OUT)  Y
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      REAL(w2f__8) PI
      REAL(w2f__8) OpenAD_acc_0
      REAL(w2f__8) OpenAD_aux_0
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_2
      REAL(w2f__8) OpenAD_lin_3
      REAL(w2f__8) OpenAD_lin_4
      TYPE (OpenADTy_active) OpenAD_prp_0
      TYPE (OpenADTy_active) OpenAD_prp_1
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      PI = 3.14149999618530273438D00
      DO I = 1, 9, 1
        IF(I .GT. 5) THEN
          OpenAD_aux_0 = SIN(__value__(X(I)))
          OpenAD_lin_1 = COS(__value__(X(I)))
          OpenAD_lin_0 = PI
          __value__(Y(INT(I))) = (PI * OpenAD_aux_0)
          OpenAD_acc_0 = (OpenAD_lin_1 * OpenAD_lin_0)
          CALL sax(OpenAD_acc_0, __deriv__(X(I)), __deriv__(Y(I)))
        ELSE
          OpenAD_lin_2 = (- SIN(__value__(X(I))))
          __value__(Y(INT(I))) = (PI + COS(__value__(X(I))))
          CALL sax(OpenAD_lin_2, __deriv__(X(I)), __deriv__(Y(I)))
        ENDIF
      END DO
      OpenAD_lin_3 = __value__(Y(9))
      OpenAD_lin_4 = __value__(Y(1))
      __value__(Y(10)) = (__value__(Y(1)) * __value__(Y(9)))
      CALL setderiv(__deriv__(OpenAD_prp_0), __deriv__(Y(1)))
      CALL setderiv(__deriv__(OpenAD_prp_1), __deriv__(Y(9)))
      CALL sax(OpenAD_lin_3, __deriv__(OpenAD_prp_0), __deriv__(Y(10)))
      CALL saxpy(OpenAD_lin_4, __deriv__(OpenAD_prp_1), __deriv__(Y(10)
     > ))
      END SUBROUTINE
