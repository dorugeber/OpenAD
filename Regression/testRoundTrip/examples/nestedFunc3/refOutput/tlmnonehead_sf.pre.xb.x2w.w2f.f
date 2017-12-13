
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
      TYPE (OpenADTy_active) Y(1 : 1)
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      CALL FOO()
      CALL BAR()
      CONTAINS

        SUBROUTINE FOO()
        use w2f__types
        IMPLICIT NONE
C
C       **** Local Variables and Functions ****
C
        REAL(w2f__8) OpenAD_lin_0
        REAL(w2f__8) OpenAD_lin_1
C
C       **** Statements ****
C
        OpenAD_lin_0 = __value__(X(2))
        OpenAD_lin_1 = __value__(X(1))
        __value__(Y(1)) = (__value__(X(1)) * __value__(X(2)))
        CALL sax(OpenAD_lin_0, __deriv__(X(1)), __deriv__(Y(1)))
        CALL saxpy(OpenAD_lin_1, __deriv__(X(2)), __deriv__(Y(1)))
        END SUBROUTINE

        SUBROUTINE BAR()
        use w2f__types
        IMPLICIT NONE
C
C       **** Local Variables and Functions ****
C
        REAL(w2f__8) OpenAD_acc_0
        REAL(w2f__8) OpenAD_acc_1
        REAL(w2f__8) OpenAD_acc_2
        REAL(w2f__8) OpenAD_aux_0
        REAL(w2f__8) OpenAD_aux_1
        REAL(w2f__8) OpenAD_aux_2
        REAL(w2f__8) OpenAD_lin_2
        REAL(w2f__8) OpenAD_lin_3
        REAL(w2f__8) OpenAD_lin_4
        REAL(w2f__8) OpenAD_lin_5
        REAL(w2f__8) OpenAD_lin_6
        REAL(w2f__8) OpenAD_lin_7
        TYPE (OpenADTy_active) OpenAD_prp_0
C
C       **** Statements ****
C
        OpenAD_aux_0 = COS(__value__(X(2)))
        OpenAD_aux_2 = SIN(__value__(X(1)))
        OpenAD_aux_1 = (__value__(Y(1)) * OpenAD_aux_2)
        OpenAD_lin_4 = (- SIN(__value__(X(2))))
        OpenAD_lin_2 = OpenAD_aux_1
        OpenAD_lin_5 = OpenAD_aux_2
        OpenAD_lin_7 = COS(__value__(X(1)))
        OpenAD_lin_6 = __value__(Y(1))
        OpenAD_lin_3 = OpenAD_aux_0
        __value__(Y(1)) = (OpenAD_aux_0 * OpenAD_aux_1)
        OpenAD_acc_0 = (OpenAD_lin_4 * OpenAD_lin_2)
        OpenAD_acc_1 = (OpenAD_lin_5 * OpenAD_lin_3)
        OpenAD_acc_2 = (OpenAD_lin_7 * OpenAD_lin_6 * OpenAD_lin_3)
        CALL setderiv(__deriv__(OpenAD_prp_0), __deriv__(Y(1)))
        CALL sax(OpenAD_acc_0, __deriv__(X(2)), __deriv__(Y(1)))
        CALL saxpy(OpenAD_acc_1, __deriv__(OpenAD_prp_0), __deriv__(Y(1
     > )))
        CALL saxpy(OpenAD_acc_2, __deriv__(X(1)), __deriv__(Y(1)))
        END SUBROUTINE
      END
