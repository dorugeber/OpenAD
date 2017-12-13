C ***********************************************************
C Fortran file translated from WHIRL Mon Jan 15 14:41:53 2007
C ***********************************************************
C ***********************************************************

      MODULE all_globals_mod
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Statements ****
C
      END MODULE

      SUBROUTINE head(J, X, Y)
      use w2f__types
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      TYPE (OpenADTy_active) OpenAD_Symbol_0
      INTEGER(w2f__i8) OpenAD_Symbol_1
      INTEGER(w2f__i8) OpenAD_Symbol_2
      INTEGER(w2f__i8) OpenAD_Symbol_3
C
C     **** Parameters and Result ****
C
      INTEGER(w2f__i4) J
      TYPE (OpenADTy_active) X(1 : 1)
      TYPE (OpenADTy_active) Y(1 : 1)
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      INTEGER(w2f__i8) OpenAD_Symbol_4
      INTEGER(w2f__i8) OpenAD_Symbol_5
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C     $OpenAD$ BEGIN REPLACEMENT 1
C$OPENAD XXX Template ad_template.f
C$OPENAD XXX Simple loop
      DO I = 1, 2, 1
        __value__(Y(INT(J))) = __value__(X(J))
      END DO
      RETURN
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
C$OPENAD XXX Simple loop
      DO I = 1, 2, 1
        __value__(Y(INT(J))) = __value__(X(J))
      END DO
C     $OpenAD$ INLINE push_i(subst)
      CALL push_i(J)
      RETURN
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE pop_i(subst)
      CALL pop_i(J)
      I = 1 + 1 *((2 - 1) / 1)
      DO WHILE(I .GE. 1)
C       $OpenAD$ INLINE CondIncZeroDeriv(subst,subst)
        CALL CondIncZeroDeriv(__deriv__(Y(J)), __deriv__(
     > OpenAD_Symbol_0))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_Symbol_0), __deriv__(X(J)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_Symbol_0))
        I = I - 1
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 4
C     $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
      CALL cp_arg_store_integer_scalar(J)
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(X))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 5
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 6
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(X))
C     $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
      CALL cp_arg_restore_integer_scalar(J)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 7
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 8
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(Y))
C     $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
      CALL cp_arg_store_integer_scalar(J)
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(X))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 9
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(X))
C     $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
      CALL cp_arg_restore_integer_scalar(J)
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(Y))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 10
C$OPENAD XXX Template ad_template.f
C$OPENAD XXX Simple loop
      OpenAD_Symbol_3 = 0_w2f__i8
      DO I = 1, 2, 1
        __value__(Y(INT(J))) = __value__(X(J))
C       $OpenAD$ INLINE push_i(subst)
        CALL push_i(J)
C       $OpenAD$ INLINE push_i(subst)
        CALL push_i(J)
        OpenAD_Symbol_3 = (INT(OpenAD_Symbol_3) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i(subst)
      CALL push_i(OpenAD_Symbol_3)
      RETURN
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_i(subst)
      CALL pop_i(OpenAD_Symbol_1)
      OpenAD_Symbol_2 = 1
      DO WHILE(INT(OpenAD_Symbol_2) .LE. INT(OpenAD_Symbol_1))
C       $OpenAD$ INLINE pop_i(subst)
        CALL pop_i(OpenAD_Symbol_4)
C       $OpenAD$ INLINE CondIncZeroDeriv(subst,subst)
        CALL CondIncZeroDeriv(__deriv__(Y(INT(OpenAD_Symbol_4))),
     >  __deriv__(OpenAD_Symbol_0))
C       $OpenAD$ INLINE pop_i(subst)
        CALL pop_i(OpenAD_Symbol_5)
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_Symbol_0), __deriv__(X(INT(
     > OpenAD_Symbol_5))))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_Symbol_0))
        OpenAD_Symbol_2 = INT(OpenAD_Symbol_2) + 1
      END DO
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE
