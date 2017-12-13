
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
      MODULE globals
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      INTEGER(w2f__i4) I
C
C     **** Statements ****
C
      END MODULE

      SUBROUTINE foo(P, Q)
      use w2f__types
      use oad_intrinsics
      use globals
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) P
      INTENT(IN)  P
      TYPE (OpenADTy_active) Q
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      TYPE (OpenADTy_active) OpenAD_prp_0
C
C     **** Statements ****
C
      OpenAD_lin_0 = __value__(Q)
      OpenAD_lin_1 = __value__(P)
      __value__(Q) = (__value__(P) * __value__(Q))
      I = (I + 1)
      CALL setderiv(__deriv__(OpenAD_prp_0), __deriv__(Q))
      CALL sax(OpenAD_lin_0, __deriv__(P), __deriv__(Q))
      CALL saxpy(OpenAD_lin_1, __deriv__(OpenAD_prp_0), __deriv__(Q))
      END SUBROUTINE

      SUBROUTINE head(X, Y)
      use w2f__types
      use oad_intrinsics
      use globals
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X(1 : 2)
      INTENT(IN)  X
      TYPE (OpenADTy_active) Y(1 : 1)
      INTENT(OUT)  Y
C
C     **** Local Variables and Functions ****
C
      EXTERNAL foo
      TYPE (OpenADTy_active) V(1 : 2)
      REAL(w2f__8) OpenAD_lin_2
      REAL(w2f__8) OpenAD_lin_3
      TYPE (OpenADTy_active) OpenAD_prp_1
      TYPE (OpenADTy_active) OpenAD_prp_2
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      I = 1
      __value__(Y(1)) = 1.0D00
      __value__(V(1)) = 9.085830301E-03
      __value__(V(2)) = (-2.9846420512E-04)
      CALL zero_deriv(__deriv__(Y(1)))
      CALL zero_deriv(__deriv__(V(1)))
      CALL zero_deriv(__deriv__(V(2)))
      CALL foo(__deriv__(X(1)), __deriv__(Y(1)))
      CALL foo(__deriv__(X(I)), __deriv__(V(I)))
      OpenAD_lin_2 = __value__(V(2))
      OpenAD_lin_3 = __value__(Y(1))
      __value__(Y(1)) = (__value__(Y(1)) * __value__(V(2)))
      CALL setderiv(__deriv__(OpenAD_prp_1), __deriv__(Y(1)))
      CALL setderiv(__deriv__(OpenAD_prp_2), __deriv__(V(2)))
      CALL sax(OpenAD_lin_2, __deriv__(OpenAD_prp_1), __deriv__(Y(1)))
      CALL saxpy(OpenAD_lin_3, __deriv__(OpenAD_prp_2), __deriv__(Y(1))
     > )
      END SUBROUTINE
