
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
      TYPE (OpenADTy_active) X(1 : 4)
      TYPE (OpenADTy_active) Y(1 : 4)
C
C     **** Local Variables and Functions ****
C
      TYPE (OpenADTy_active) T1
      TYPE (OpenADTy_active) T2
      TYPE (OpenADTy_active) T3
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_10
      REAL(w2f__8) OpenAD_lin_11
      REAL(w2f__8) OpenAD_lin_2
      REAL(w2f__8) OpenAD_lin_3
      REAL(w2f__8) OpenAD_lin_4
      REAL(w2f__8) OpenAD_lin_5
      REAL(w2f__8) OpenAD_lin_6
      REAL(w2f__8) OpenAD_lin_7
      REAL(w2f__8) OpenAD_lin_8
      REAL(w2f__8) OpenAD_lin_9
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      OpenAD_lin_0 = (INT(1_w2f__i8) / __value__(X(2)))
      OpenAD_lin_1 = (-(__value__(X(1)) /(__value__(X(2)) * __value__(X
     > (2)))))
      __value__(T1) = (__value__(X(1)) / __value__(X(2)))
      OpenAD_lin_2 = __value__(X(4))
      OpenAD_lin_3 = __value__(X(3))
      __value__(T2) = (__value__(X(3)) * __value__(X(4)))
      OpenAD_lin_4 = __value__(T2)
      OpenAD_lin_5 = __value__(T1)
      __value__(T3) = (__value__(T1) * __value__(T2))
      OpenAD_lin_6 = __value__(T3)
      OpenAD_lin_7 = __value__(T1)
      __value__(Y(1)) = (__value__(T1) * __value__(T3))
      CALL sax(OpenAD_lin_0, __deriv__(X(1)), __deriv__(T1))
      CALL saxpy(OpenAD_lin_1, __deriv__(X(2)), __deriv__(T1))
      CALL sax(OpenAD_lin_2, __deriv__(X(3)), __deriv__(T2))
      CALL saxpy(OpenAD_lin_3, __deriv__(X(4)), __deriv__(T2))
      CALL sax(OpenAD_lin_4, __deriv__(T1), __deriv__(T3))
      CALL saxpy(OpenAD_lin_5, __deriv__(T2), __deriv__(T3))
      CALL sax(OpenAD_lin_6, __deriv__(T1), __deriv__(Y(1)))
      CALL saxpy(OpenAD_lin_7, __deriv__(T3), __deriv__(Y(1)))
      OpenAD_lin_8 = (-(1.0D00 /(__value__(T3) * __value__(T3))))
      __value__(Y(2)) = (1.0D00 / __value__(T3))
      CALL sax(OpenAD_lin_8, __deriv__(T3), __deriv__(Y(2)))
      OpenAD_lin_9 = COS(__value__(T3))
      __value__(Y(3)) = SIN(__value__(T3))
      CALL sax(OpenAD_lin_9, __deriv__(T3), __deriv__(Y(3)))
      OpenAD_lin_10 = __value__(T3)
      OpenAD_lin_11 = __value__(T2)
      __value__(Y(4)) = (__value__(T2) * __value__(T3))
      CALL sax(OpenAD_lin_10, __deriv__(T2), __deriv__(Y(4)))
      CALL saxpy(OpenAD_lin_11, __deriv__(T3), __deriv__(Y(4)))
      END SUBROUTINE
