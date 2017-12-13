C ***********************************************************
C Fortran file translated from WHIRL Tue Jul 17 09:47:32 2007
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

      SUBROUTINE head(X, Y)
      use w2f__types
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      REAL(w2f__8) OpenAD_Symbol_0
      REAL(w2f__8) OpenAD_Symbol_1
      TYPE (OpenADTy_active) OpenAD_Symbol_10
      REAL(w2f__8) OpenAD_Symbol_11
      TYPE (OpenADTy_active) OpenAD_Symbol_12
      INTEGER(w2f__i8) OpenAD_Symbol_13
      INTEGER(w2f__i8) OpenAD_Symbol_14
      INTEGER(w2f__i8) OpenAD_Symbol_15
      INTEGER(w2f__i8) OpenAD_Symbol_16
      INTEGER(w2f__i8) OpenAD_Symbol_17
      INTEGER(w2f__i8) OpenAD_Symbol_18
      INTEGER(w2f__i8) OpenAD_Symbol_19
      REAL(w2f__8) OpenAD_Symbol_2
      INTEGER(w2f__i8) OpenAD_Symbol_20
      INTEGER(w2f__i8) OpenAD_Symbol_21
      INTEGER(w2f__i8) OpenAD_Symbol_22
      INTEGER(w2f__i8) OpenAD_Symbol_23
      INTEGER(w2f__i8) OpenAD_Symbol_24
      INTEGER(w2f__i8) OpenAD_Symbol_25
      INTEGER(w2f__i8) OpenAD_Symbol_26
      REAL(w2f__8) OpenAD_Symbol_3
      REAL(w2f__8) OpenAD_Symbol_4
      REAL(w2f__8) OpenAD_Symbol_6
      REAL(w2f__8) OpenAD_Symbol_7
      TYPE (OpenADTy_active) OpenAD_Symbol_8
      REAL(w2f__8) OpenAD_Symbol_9
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X(1 : 3)
      TYPE (OpenADTy_active) Y(1 : 3)
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) select_expr_temp_0
      INTEGER(w2f__i8) OpenAD_Symbol_27
      REAL(w2f__8) OpenAD_Symbol_28
      INTEGER(w2f__i8) OpenAD_Symbol_29
      INTEGER(w2f__i8) OpenAD_Symbol_30
      REAL(w2f__8) OpenAD_Symbol_31
      INTEGER(w2f__i8) OpenAD_Symbol_32
      INTEGER(w2f__i8) OpenAD_Symbol_33
      REAL(w2f__8) OpenAD_Symbol_34
      INTEGER(w2f__i8) OpenAD_Symbol_35
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
      DO I = 1, 3, 1
        select_expr_temp_0 = I
        IF ( I  .EQ.  1)  GO TO  19
        IF ( I  .EQ.  2)  GO TO  23
        GO TO 24
24      CONTINUE
        __value__(Y(INT(I))) = (__value__(X(I)) * 2.0D00)
        GO TO 21
19      CONTINUE
        __value__(Y(INT(I))) = SIN(__value__(X(I)))
        GO TO 21
23      CONTINUE
        __value__(Y(INT(I))) = COS(__value__(X(I)))
        GO TO 21
21      CONTINUE
      END DO
      RETURN
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      OpenAD_Symbol_16 = 0_w2f__i8
      DO I = 1, 3, 1
        select_expr_temp_0 = I
        IF ( I  .EQ.  1)  GO TO  91
        IF ( I  .EQ.  2)  GO TO  95
        GO TO 96
96      CONTINUE
        OpenAD_Symbol_6 = (__value__(X(I)) * 2.0D00)
        OpenAD_Symbol_4 = 2.0D00
        __value__(Y(INT(I))) = OpenAD_Symbol_6
        OpenAD_Symbol_11 = OpenAD_Symbol_4
C       $OpenAD$ INLINE push_i(subst)
        CALL push_i(I)
C       $OpenAD$ INLINE push(subst)
        CALL push(OpenAD_Symbol_11)
C       $OpenAD$ INLINE push_i(subst)
        CALL push_i(I)
        OpenAD_Symbol_19 = 3_w2f__i8
C       $OpenAD$ INLINE push_i(subst)
        CALL push_i(OpenAD_Symbol_19)
        GO TO 104
91      CONTINUE
        OpenAD_Symbol_1 = SIN(__value__(X(I)))
        OpenAD_Symbol_0 = COS(__value__(X(I)))
        __value__(Y(INT(I))) = OpenAD_Symbol_1
        OpenAD_Symbol_7 = OpenAD_Symbol_0
C       $OpenAD$ INLINE push_i(subst)
        CALL push_i(I)
C       $OpenAD$ INLINE push(subst)
        CALL push(OpenAD_Symbol_7)
C       $OpenAD$ INLINE push_i(subst)
        CALL push_i(I)
        OpenAD_Symbol_17 = 1_w2f__i8
C       $OpenAD$ INLINE push_i(subst)
        CALL push_i(OpenAD_Symbol_17)
        GO TO 104
95      CONTINUE
        OpenAD_Symbol_3 = COS(__value__(X(I)))
        OpenAD_Symbol_2 = (- SIN(__value__(X(I))))
        __value__(Y(INT(I))) = OpenAD_Symbol_3
        OpenAD_Symbol_9 = OpenAD_Symbol_2
C       $OpenAD$ INLINE push_i(subst)
        CALL push_i(I)
C       $OpenAD$ INLINE push(subst)
        CALL push(OpenAD_Symbol_9)
C       $OpenAD$ INLINE push_i(subst)
        CALL push_i(I)
        OpenAD_Symbol_18 = 2_w2f__i8
C       $OpenAD$ INLINE push_i(subst)
        CALL push_i(OpenAD_Symbol_18)
        GO TO 104
104     CONTINUE
        OpenAD_Symbol_16 = (INT(OpenAD_Symbol_16) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i(subst)
      CALL push_i(OpenAD_Symbol_16)
      RETURN
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE pop_i(subst)
      CALL pop_i(OpenAD_Symbol_13)
      OpenAD_Symbol_14 = 1
      DO WHILE(INT(OpenAD_Symbol_14) .LE. INT(OpenAD_Symbol_13))
C       $OpenAD$ INLINE pop_i(subst)
        CALL pop_i(OpenAD_Symbol_15)
        IF ( OpenAD_Symbol_15  .EQ.  1)  GO TO  188
        IF ( OpenAD_Symbol_15  .EQ.  2)  GO TO  187
        IF ( OpenAD_Symbol_15  .EQ.  3)  GO TO  186
188     CONTINUE
C       $OpenAD$ INLINE pop_i(subst)
        CALL pop_i(OpenAD_Symbol_27)
C       $OpenAD$ INLINE Pop(subst)
        CALL Pop(OpenAD_Symbol_28)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_28, __deriv__(Y(INT(OpenAD_Symbol_27))
     > ), __deriv__(OpenAD_Symbol_8))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(Y(INT(OpenAD_Symbol_27))))
C       $OpenAD$ INLINE pop_i(subst)
        CALL pop_i(OpenAD_Symbol_29)
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_Symbol_8), __deriv__(X(INT(
     > OpenAD_Symbol_29))))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_Symbol_8))
        GO TO 190
187     CONTINUE
C       $OpenAD$ INLINE pop_i(subst)
        CALL pop_i(OpenAD_Symbol_30)
C       $OpenAD$ INLINE Pop(subst)
        CALL Pop(OpenAD_Symbol_31)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_31, __deriv__(Y(INT(OpenAD_Symbol_30))
     > ), __deriv__(OpenAD_Symbol_10))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(Y(INT(OpenAD_Symbol_30))))
C       $OpenAD$ INLINE pop_i(subst)
        CALL pop_i(OpenAD_Symbol_32)
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_Symbol_10), __deriv__(X(INT(
     > OpenAD_Symbol_32))))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_Symbol_10))
        GO TO 190
186     CONTINUE
C       $OpenAD$ INLINE pop_i(subst)
        CALL pop_i(OpenAD_Symbol_33)
C       $OpenAD$ INLINE Pop(subst)
        CALL Pop(OpenAD_Symbol_34)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_34, __deriv__(Y(INT(OpenAD_Symbol_33))
     > ), __deriv__(OpenAD_Symbol_12))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(Y(INT(OpenAD_Symbol_33))))
C       $OpenAD$ INLINE pop_i(subst)
        CALL pop_i(OpenAD_Symbol_35)
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_Symbol_12), __deriv__(X(INT(
     > OpenAD_Symbol_35))))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_Symbol_12))
        GO TO 190
        OpenAD_Symbol_14 = INT(OpenAD_Symbol_14) + 1
190     CONTINUE
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 4
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(X))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 5
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 6
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(X))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 7
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 8
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(Y))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(X))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 9
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(X))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(Y))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 10
C$OPENAD XXX Template ad_template.f
      OpenAD_Symbol_23 = 0_w2f__i8
      DO I = 1, 3, 1
        select_expr_temp_0 = I
        IF ( I  .EQ.  1)  GO TO  284
        IF ( I  .EQ.  2)  GO TO  288
        GO TO 289
289     CONTINUE
        OpenAD_Symbol_6 = (__value__(X(I)) * 2.0D00)
        OpenAD_Symbol_4 = 2.0D00
        __value__(Y(INT(I))) = OpenAD_Symbol_6
        OpenAD_Symbol_11 = OpenAD_Symbol_4
C       $OpenAD$ INLINE push_i(subst)
        CALL push_i(I)
C       $OpenAD$ INLINE push(subst)
        CALL push(OpenAD_Symbol_11)
C       $OpenAD$ INLINE push_i(subst)
        CALL push_i(I)
        OpenAD_Symbol_26 = 3_w2f__i8
C       $OpenAD$ INLINE push_i(subst)
        CALL push_i(OpenAD_Symbol_26)
        GO TO 297
284     CONTINUE
        OpenAD_Symbol_1 = SIN(__value__(X(I)))
        OpenAD_Symbol_0 = COS(__value__(X(I)))
        __value__(Y(INT(I))) = OpenAD_Symbol_1
        OpenAD_Symbol_7 = OpenAD_Symbol_0
C       $OpenAD$ INLINE push_i(subst)
        CALL push_i(I)
C       $OpenAD$ INLINE push(subst)
        CALL push(OpenAD_Symbol_7)
C       $OpenAD$ INLINE push_i(subst)
        CALL push_i(I)
        OpenAD_Symbol_24 = 1_w2f__i8
C       $OpenAD$ INLINE push_i(subst)
        CALL push_i(OpenAD_Symbol_24)
        GO TO 297
288     CONTINUE
        OpenAD_Symbol_3 = COS(__value__(X(I)))
        OpenAD_Symbol_2 = (- SIN(__value__(X(I))))
        __value__(Y(INT(I))) = OpenAD_Symbol_3
        OpenAD_Symbol_9 = OpenAD_Symbol_2
C       $OpenAD$ INLINE push_i(subst)
        CALL push_i(I)
C       $OpenAD$ INLINE push(subst)
        CALL push(OpenAD_Symbol_9)
C       $OpenAD$ INLINE push_i(subst)
        CALL push_i(I)
        OpenAD_Symbol_25 = 2_w2f__i8
C       $OpenAD$ INLINE push_i(subst)
        CALL push_i(OpenAD_Symbol_25)
        GO TO 297
297     CONTINUE
        OpenAD_Symbol_23 = (INT(OpenAD_Symbol_23) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i(subst)
      CALL push_i(OpenAD_Symbol_23)
      RETURN
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_i(subst)
      CALL pop_i(OpenAD_Symbol_20)
      OpenAD_Symbol_21 = 1
      DO WHILE(INT(OpenAD_Symbol_21) .LE. INT(OpenAD_Symbol_20))
C       $OpenAD$ INLINE pop_i(subst)
        CALL pop_i(OpenAD_Symbol_22)
        IF ( OpenAD_Symbol_22  .EQ.  1)  GO TO  381
        IF ( OpenAD_Symbol_22  .EQ.  2)  GO TO  380
        IF ( OpenAD_Symbol_22  .EQ.  3)  GO TO  379
381     CONTINUE
C       $OpenAD$ INLINE pop_i(subst)
        CALL pop_i(OpenAD_Symbol_27)
C       $OpenAD$ INLINE Pop(subst)
        CALL Pop(OpenAD_Symbol_28)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_28, __deriv__(Y(INT(OpenAD_Symbol_27))
     > ), __deriv__(OpenAD_Symbol_8))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(Y(INT(OpenAD_Symbol_27))))
C       $OpenAD$ INLINE pop_i(subst)
        CALL pop_i(OpenAD_Symbol_29)
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_Symbol_8), __deriv__(X(INT(
     > OpenAD_Symbol_29))))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_Symbol_8))
        GO TO 383
380     CONTINUE
C       $OpenAD$ INLINE pop_i(subst)
        CALL pop_i(OpenAD_Symbol_30)
C       $OpenAD$ INLINE Pop(subst)
        CALL Pop(OpenAD_Symbol_31)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_31, __deriv__(Y(INT(OpenAD_Symbol_30))
     > ), __deriv__(OpenAD_Symbol_10))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(Y(INT(OpenAD_Symbol_30))))
C       $OpenAD$ INLINE pop_i(subst)
        CALL pop_i(OpenAD_Symbol_32)
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_Symbol_10), __deriv__(X(INT(
     > OpenAD_Symbol_32))))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_Symbol_10))
        GO TO 383
379     CONTINUE
C       $OpenAD$ INLINE pop_i(subst)
        CALL pop_i(OpenAD_Symbol_33)
C       $OpenAD$ INLINE Pop(subst)
        CALL Pop(OpenAD_Symbol_34)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_34, __deriv__(Y(INT(OpenAD_Symbol_33))
     > ), __deriv__(OpenAD_Symbol_12))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(Y(INT(OpenAD_Symbol_33))))
C       $OpenAD$ INLINE pop_i(subst)
        CALL pop_i(OpenAD_Symbol_35)
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_Symbol_12), __deriv__(X(INT(
     > OpenAD_Symbol_35))))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_Symbol_12))
        GO TO 383
        OpenAD_Symbol_21 = INT(OpenAD_Symbol_21) + 1
383     CONTINUE
      END DO
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE
