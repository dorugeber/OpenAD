
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
      use oad_intrinsics
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      INTEGER(w2f__i8) OpenAD_Symbol_0
      INTEGER(w2f__i8) OpenAD_Symbol_1
      INTEGER(w2f__i8) OpenAD_Symbol_10
      INTEGER(w2f__i8) OpenAD_Symbol_11
      INTEGER(w2f__i8) OpenAD_Symbol_12
      INTEGER(w2f__i8) OpenAD_Symbol_13
      INTEGER(w2f__i8) OpenAD_Symbol_14
      INTEGER(w2f__i8) OpenAD_Symbol_15
      INTEGER(w2f__i8) OpenAD_Symbol_16
      INTEGER(w2f__i8) OpenAD_Symbol_17
      INTEGER(w2f__i8) OpenAD_Symbol_2
      INTEGER(w2f__i8) OpenAD_Symbol_3
      INTEGER(w2f__i8) OpenAD_Symbol_4
      INTEGER(w2f__i8) OpenAD_Symbol_5
      INTEGER(w2f__i8) OpenAD_Symbol_6
      INTEGER(w2f__i8) OpenAD_Symbol_7
      INTEGER(w2f__i8) OpenAD_Symbol_8
      INTEGER(w2f__i8) OpenAD_Symbol_9
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X(1 : 2)
      TYPE (OpenADTy_active) Y(1 : 2)
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) OpenAD_Symbol_18
      REAL(w2f__8) OpenAD_Symbol_19
      REAL(w2f__8) OpenAD_Symbol_20
      INTEGER(w2f__i4) OpenAD_Symbol_21
      REAL(w2f__8) OpenAD_Symbol_22
      REAL(w2f__8) OpenAD_Symbol_23
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
C     $OpenAD$ BEGIN REPLACEMENT 1
C$OPENAD XXX Template ad_template.f
      IF(__value__(X(1)) .LT. __value__(X(2))) THEN
        DO I = 1, 2, 1
          __value__(Y(INT(I))) = (__value__(X(I)) * __value__(X(I)))
        END DO
      ELSE
        DO I = 1, 2, 1
          __value__(Y(INT(I))) = (__value__(X(I)) * __value__(X(I)))
        END DO
      ENDIF
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      IF(__value__(X(1)) .LT. __value__(X(2))) THEN
        OpenAD_Symbol_5 = 0_w2f__i8
        DO I = 1, 2, 1
          OpenAD_lin_0 = __value__(X(I))
          OpenAD_lin_1 = __value__(X(I))
          __value__(Y(INT(I))) = (__value__(X(I)) * __value__(X(I)))
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_lin_0)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_lin_1)
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(I)
          OpenAD_Symbol_5 = (INT(OpenAD_Symbol_5) + INT(1_w2f__i8))
        END DO
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_5)
        OpenAD_Symbol_7 = 1_w2f__i8
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_7)
      ELSE
        OpenAD_Symbol_6 = 0_w2f__i8
        DO I = 1, 2, 1
          OpenAD_lin_2 = __value__(X(I))
          OpenAD_lin_3 = __value__(X(I))
          __value__(Y(INT(I))) = (__value__(X(I)) * __value__(X(I)))
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_lin_2)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_lin_3)
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(I)
          OpenAD_Symbol_6 = (INT(OpenAD_Symbol_6) + INT(1_w2f__i8))
        END DO
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_6)
        OpenAD_Symbol_8 = 0_w2f__i8
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_8)
      ENDIF
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_0)
      IF(OpenAD_Symbol_0 .ne. 0) THEN
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_3)
        OpenAD_Symbol_4 = 1
        DO WHILE(INT(OpenAD_Symbol_4) .LE. INT(OpenAD_Symbol_3))
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_18)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_19)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_20)
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_19, __deriv__(Y(OpenAD_Symbol_18)),
     >  __deriv__(X(OpenAD_Symbol_18)))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_20, __deriv__(Y(OpenAD_Symbol_18)),
     >  __deriv__(X(OpenAD_Symbol_18)))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(Y(OpenAD_Symbol_18)))
          OpenAD_Symbol_4 = INT(OpenAD_Symbol_4) + 1
        END DO
      ELSE
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_1)
        OpenAD_Symbol_2 = 1
        DO WHILE(INT(OpenAD_Symbol_2) .LE. INT(OpenAD_Symbol_1))
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_21)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_22)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_23)
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_22, __deriv__(Y(OpenAD_Symbol_21)),
     >  __deriv__(X(OpenAD_Symbol_21)))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_23, __deriv__(Y(OpenAD_Symbol_21)),
     >  __deriv__(X(OpenAD_Symbol_21)))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(Y(OpenAD_Symbol_21)))
          OpenAD_Symbol_2 = INT(OpenAD_Symbol_2) + 1
        END DO
      ENDIF
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
      IF(__value__(X(1)) .LT. __value__(X(2))) THEN
        OpenAD_Symbol_14 = 0_w2f__i8
        DO I = 1, 2, 1
          OpenAD_lin_0 = __value__(X(I))
          OpenAD_lin_1 = __value__(X(I))
          __value__(Y(INT(I))) = (__value__(X(I)) * __value__(X(I)))
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_lin_0)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_lin_1)
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(I)
          OpenAD_Symbol_14 = (INT(OpenAD_Symbol_14) + INT(1_w2f__i8))
        END DO
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_14)
        OpenAD_Symbol_16 = 1_w2f__i8
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_16)
      ELSE
        OpenAD_Symbol_15 = 0_w2f__i8
        DO I = 1, 2, 1
          OpenAD_lin_2 = __value__(X(I))
          OpenAD_lin_3 = __value__(X(I))
          __value__(Y(INT(I))) = (__value__(X(I)) * __value__(X(I)))
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_lin_2)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_lin_3)
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(I)
          OpenAD_Symbol_15 = (INT(OpenAD_Symbol_15) + INT(1_w2f__i8))
        END DO
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_15)
        OpenAD_Symbol_17 = 0_w2f__i8
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_17)
      ENDIF
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_9)
      IF(OpenAD_Symbol_9 .ne. 0) THEN
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_12)
        OpenAD_Symbol_13 = 1
        DO WHILE(INT(OpenAD_Symbol_13) .LE. INT(OpenAD_Symbol_12))
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_18)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_19)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_20)
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_19, __deriv__(Y(OpenAD_Symbol_18)),
     >  __deriv__(X(OpenAD_Symbol_18)))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_20, __deriv__(Y(OpenAD_Symbol_18)),
     >  __deriv__(X(OpenAD_Symbol_18)))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(Y(OpenAD_Symbol_18)))
          OpenAD_Symbol_13 = INT(OpenAD_Symbol_13) + 1
        END DO
      ELSE
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_10)
        OpenAD_Symbol_11 = 1
        DO WHILE(INT(OpenAD_Symbol_11) .LE. INT(OpenAD_Symbol_10))
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_21)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_22)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_23)
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_22, __deriv__(Y(OpenAD_Symbol_21)),
     >  __deriv__(X(OpenAD_Symbol_21)))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_23, __deriv__(Y(OpenAD_Symbol_21)),
     >  __deriv__(X(OpenAD_Symbol_21)))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(Y(OpenAD_Symbol_21)))
          OpenAD_Symbol_11 = INT(OpenAD_Symbol_11) + 1
        END DO
      ENDIF
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE
