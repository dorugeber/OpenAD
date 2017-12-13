
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
      SUBROUTINE lagran(I, X, A, SP, LAG)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      INTEGER(w2f__i4) I
      REAL(w2f__8) X
      TYPE (OpenADTy_active) A(1 : 4)
      INTEGER(w2f__i4) SP
      TYPE (OpenADTy_active) LAG
      INTENT(OUT)  LAG
C
C     **** Local Variables and Functions ****
C
      TYPE (OpenADTy_active) DENOM
      INTEGER(w2f__i4) K
      TYPE (OpenADTy_active) NUMER
      REAL(w2f__8) OpenAD_acc_0
      REAL(w2f__8) OpenAD_aux_0
      REAL(w2f__8) OpenAD_aux_1
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_2
      REAL(w2f__8) OpenAD_lin_3
      REAL(w2f__8) OpenAD_lin_4
      REAL(w2f__8) OpenAD_lin_5
      TYPE (OpenADTy_active) OpenAD_prp_0
      TYPE (OpenADTy_active) OpenAD_prp_1
      TYPE (OpenADTy_active) OpenAD_prp_2
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      __value__(NUMER) = 1.0D00
      __value__(DENOM) = 1.0D00
      CALL zero_deriv(__deriv__(NUMER))
      CALL zero_deriv(__deriv__(DENOM))
C$OPENAD XXX Simple loop
      DO K = 1, SP, 1
        IF(I .ne. K) THEN
          OpenAD_aux_0 = (__value__(A(I)) - __value__(A(K)))
          OpenAD_lin_0 = OpenAD_aux_0
          OpenAD_lin_1 = __value__(DENOM)
          __value__(DENOM) = (__value__(DENOM) * OpenAD_aux_0)
          OpenAD_aux_1 = (X - __value__(A(K)))
          OpenAD_lin_2 = OpenAD_aux_1
          OpenAD_lin_3 = __value__(NUMER)
          __value__(NUMER) = (__value__(NUMER) * OpenAD_aux_1)
          OpenAD_acc_0 = (INT((-1_w2f__i8)) * OpenAD_lin_3)
          CALL setderiv(__deriv__(OpenAD_prp_0), __deriv__(DENOM))
          CALL setderiv(__deriv__(OpenAD_prp_1), __deriv__(NUMER))
          CALL setderiv(__deriv__(OpenAD_prp_2), __deriv__(A(I)))
          CALL dec_deriv(__deriv__(OpenAD_prp_2), __deriv__(A(K)))
          CALL sax(OpenAD_lin_0, __deriv__(OpenAD_prp_0), __deriv__(
     > DENOM))
          CALL saxpy(OpenAD_lin_1, __deriv__(OpenAD_prp_2), __deriv__(
     > DENOM))
          CALL sax(OpenAD_lin_2, __deriv__(OpenAD_prp_1), __deriv__(
     > NUMER))
          CALL saxpy(OpenAD_acc_0, __deriv__(A(K)), __deriv__(NUMER))
        ENDIF
      END DO
      OpenAD_lin_4 = (INT(1_w2f__i8) / __value__(DENOM))
      OpenAD_lin_5 = (-(__value__(NUMER) /(__value__(DENOM) * __value__
     > (DENOM))))
      __value__(LAG) = (__value__(NUMER) / __value__(DENOM))
      CALL sax(OpenAD_lin_4, __deriv__(NUMER), __deriv__(LAG))
      CALL saxpy(OpenAD_lin_5, __deriv__(DENOM), __deriv__(LAG))
      END SUBROUTINE

      SUBROUTINE head(X, Y)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X(1 : 4)
      INTENT(IN)  X
      TYPE (OpenADTy_active) Y(1 : 1)
      INTENT(OUT)  Y
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
      CALL lagran(2, 2.0D00, __deriv__(X), 4, __deriv__(Y(1)))
      END SUBROUTINE
