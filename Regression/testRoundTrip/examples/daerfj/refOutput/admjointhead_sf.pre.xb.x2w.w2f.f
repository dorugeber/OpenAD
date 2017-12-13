
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
      SUBROUTINE head(X, FVEC)
      use w2f__types
      use oad_intrinsics
      use oad_intrinsics
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      INTEGER(w2f__i8) OpenAD_Symbol_0
      INTEGER(w2f__i8) OpenAD_Symbol_1
      INTEGER(w2f__i8) OpenAD_Symbol_2
      INTEGER(w2f__i8) OpenAD_Symbol_3
      INTEGER(w2f__i8) OpenAD_Symbol_4
      INTEGER(w2f__i8) OpenAD_Symbol_5
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X(1 : 4)
      TYPE (OpenADTy_active) FVEC(1 : 11)
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      TYPE (OpenADTy_active) TEMP1
      TYPE (OpenADTy_active) TEMP2
      REAL(w2f__8) V(1 : 11)
      REAL(w2f__8) Y(1 : 11)
      REAL(w2f__8) OpenAD_Symbol_10
      INTEGER(w2f__i4) OpenAD_Symbol_6
      REAL(w2f__8) OpenAD_Symbol_7
      REAL(w2f__8) OpenAD_Symbol_8
      REAL(w2f__8) OpenAD_Symbol_9
      REAL(w2f__8) OpenAD_acc_0
      REAL(w2f__8) OpenAD_acc_1
      REAL(w2f__8) OpenAD_acc_2
      REAL(w2f__8) OpenAD_aux_0
      REAL(w2f__8) OpenAD_aux_1
      REAL(w2f__8) OpenAD_aux_2
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_2
      REAL(w2f__8) OpenAD_lin_3
      REAL(w2f__8) OpenAD_lin_4
      REAL(w2f__8) OpenAD_lin_5
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(FVEC)
C
C     **** Statements ****
C
C     $OpenAD$ BEGIN REPLACEMENT 1
C$OPENAD XXX Template ad_template.f
      V(1) = 4.0D00
      V(2) = 2.0D00
      V(3) = 1.0D00
      V(4) = 5.0D-01
      V(5) = 2.5D-01
      V(6) = 1.67000000000000009548D-01
      V(7) = 1.25D-01
      V(8) = 1.00000000000000005551D-01
      V(9) = 8.32999999999999990452D-02
      V(10) = 7.14000000000000051292D-02
      V(11) = 6.25D-02
      Y(1) = 1.95700000000000012834D-01
      Y(2) = 1.94700000000000011946D-01
      Y(3) = 1.73499999999999987566D-01
      Y(4) = 1.60000000000000003331D-01
      Y(5) = 8.44000000000000027978D-02
      Y(6) = 6.27000000000000057288D-02
      Y(7) = 4.56000000000000016431D-02
      Y(8) = 3.42000000000000012323D-02
      Y(9) = 3.23000000000000023204D-02
      Y(10) = 2.35000000000000000555D-02
      Y(11) = 2.46000000000000003386D-02
      DO I = 1, 11, 1
        __value__(TEMP1) = (V(I) *(__value__(X(2)) + V(I)))
        __value__(TEMP2) = (__value__(X(4)) + V(I) *(__value__(X(3)) +
     >  V(I)))
        __value__(FVEC(INT(I))) = (Y(I) -((__value__(X(1)) * __value__(
     > TEMP1)) / __value__(TEMP2)))
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      V(1) = 4.0D00
      V(2) = 2.0D00
      V(3) = 1.0D00
      V(4) = 5.0D-01
      V(5) = 2.5D-01
      V(6) = 1.67000000000000009548D-01
      V(7) = 1.25D-01
      V(8) = 1.00000000000000005551D-01
      V(9) = 8.32999999999999990452D-02
      V(10) = 7.14000000000000051292D-02
      V(11) = 6.25D-02
      Y(1) = 1.95700000000000012834D-01
      Y(2) = 1.94700000000000011946D-01
      Y(3) = 1.73499999999999987566D-01
      Y(4) = 1.60000000000000003331D-01
      Y(5) = 8.44000000000000027978D-02
      Y(6) = 6.27000000000000057288D-02
      Y(7) = 4.56000000000000016431D-02
      Y(8) = 3.42000000000000012323D-02
      Y(9) = 3.23000000000000023204D-02
      Y(10) = 2.35000000000000000555D-02
      Y(11) = 2.46000000000000003386D-02
      OpenAD_Symbol_2 = 0_w2f__i8
      DO I = 1, 11, 1
        OpenAD_aux_0 = (__value__(X(2)) + V(I))
        OpenAD_lin_0 = V(I)
        __value__(TEMP1) = (V(I) * OpenAD_aux_0)
        OpenAD_aux_1 = (__value__(X(3)) + V(I))
        OpenAD_lin_1 = V(I)
        __value__(TEMP2) = (__value__(X(4)) + V(I) * OpenAD_aux_1)
        OpenAD_aux_2 = (__value__(X(1)) * __value__(TEMP1))
        OpenAD_lin_4 = __value__(TEMP1)
        OpenAD_lin_5 = __value__(X(1))
        OpenAD_lin_2 = (INT(1_w2f__i8) / __value__(TEMP2))
        OpenAD_lin_3 = (-(OpenAD_aux_2 /(__value__(TEMP2) * __value__(
     > TEMP2))))
        __value__(FVEC(INT(I))) = (Y(I) -(OpenAD_aux_2 / __value__(
     > TEMP2)))
        OpenAD_acc_0 = (OpenAD_lin_3 * INT((-1_w2f__i8)))
        OpenAD_acc_1 = (OpenAD_lin_4 * OpenAD_lin_2 * INT((-1_w2f__i8))
     > )
        OpenAD_acc_2 = (OpenAD_lin_0 * OpenAD_lin_5 * OpenAD_lin_2 *
     >  INT((-1_w2f__i8)))
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_lin_1)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_0)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_1)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_2)
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(I)
        OpenAD_Symbol_2 = (INT(OpenAD_Symbol_2) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_2)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_0)
      OpenAD_Symbol_1 = 1
      DO WHILE(INT(OpenAD_Symbol_1) .LE. INT(OpenAD_Symbol_0))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_6)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_7)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_8)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_9)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_10)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_7, __deriv__(FVEC(OpenAD_Symbol_6)),
     >  __deriv__(X(2)))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_8, __deriv__(FVEC(OpenAD_Symbol_6)),
     >  __deriv__(X(1)))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_9, __deriv__(FVEC(OpenAD_Symbol_6)),
     >  __deriv__(TEMP2))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(FVEC(OpenAD_Symbol_6)))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_10, __deriv__(TEMP2), __deriv__(X(3)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(TEMP2), __deriv__(X(4)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(TEMP2))
        OpenAD_Symbol_1 = INT(OpenAD_Symbol_1) + 1
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 4
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 5
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 6
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 7
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 8
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 9
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 10
C$OPENAD XXX Template ad_template.f
      V(1) = 4.0D00
      V(2) = 2.0D00
      V(3) = 1.0D00
      V(4) = 5.0D-01
      V(5) = 2.5D-01
      V(6) = 1.67000000000000009548D-01
      V(7) = 1.25D-01
      V(8) = 1.00000000000000005551D-01
      V(9) = 8.32999999999999990452D-02
      V(10) = 7.14000000000000051292D-02
      V(11) = 6.25D-02
      Y(1) = 1.95700000000000012834D-01
      Y(2) = 1.94700000000000011946D-01
      Y(3) = 1.73499999999999987566D-01
      Y(4) = 1.60000000000000003331D-01
      Y(5) = 8.44000000000000027978D-02
      Y(6) = 6.27000000000000057288D-02
      Y(7) = 4.56000000000000016431D-02
      Y(8) = 3.42000000000000012323D-02
      Y(9) = 3.23000000000000023204D-02
      Y(10) = 2.35000000000000000555D-02
      Y(11) = 2.46000000000000003386D-02
      OpenAD_Symbol_5 = 0_w2f__i8
      DO I = 1, 11, 1
        OpenAD_aux_0 = (__value__(X(2)) + V(I))
        OpenAD_lin_0 = V(I)
        __value__(TEMP1) = (V(I) * OpenAD_aux_0)
        OpenAD_aux_1 = (__value__(X(3)) + V(I))
        OpenAD_lin_1 = V(I)
        __value__(TEMP2) = (__value__(X(4)) + V(I) * OpenAD_aux_1)
        OpenAD_aux_2 = (__value__(X(1)) * __value__(TEMP1))
        OpenAD_lin_4 = __value__(TEMP1)
        OpenAD_lin_5 = __value__(X(1))
        OpenAD_lin_2 = (INT(1_w2f__i8) / __value__(TEMP2))
        OpenAD_lin_3 = (-(OpenAD_aux_2 /(__value__(TEMP2) * __value__(
     > TEMP2))))
        __value__(FVEC(INT(I))) = (Y(I) -(OpenAD_aux_2 / __value__(
     > TEMP2)))
        OpenAD_acc_0 = (OpenAD_lin_3 * INT((-1_w2f__i8)))
        OpenAD_acc_1 = (OpenAD_lin_4 * OpenAD_lin_2 * INT((-1_w2f__i8))
     > )
        OpenAD_acc_2 = (OpenAD_lin_0 * OpenAD_lin_5 * OpenAD_lin_2 *
     >  INT((-1_w2f__i8)))
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_lin_1)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_0)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_1)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_2)
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(I)
        OpenAD_Symbol_5 = (INT(OpenAD_Symbol_5) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_5)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_3)
      OpenAD_Symbol_4 = 1
      DO WHILE(INT(OpenAD_Symbol_4) .LE. INT(OpenAD_Symbol_3))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_6)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_7)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_8)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_9)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_10)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_7, __deriv__(FVEC(OpenAD_Symbol_6)),
     >  __deriv__(X(2)))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_8, __deriv__(FVEC(OpenAD_Symbol_6)),
     >  __deriv__(X(1)))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_9, __deriv__(FVEC(OpenAD_Symbol_6)),
     >  __deriv__(TEMP2))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(FVEC(OpenAD_Symbol_6)))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_10, __deriv__(TEMP2), __deriv__(X(3)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(TEMP2), __deriv__(X(4)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(TEMP2))
        OpenAD_Symbol_4 = INT(OpenAD_Symbol_4) + 1
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE
