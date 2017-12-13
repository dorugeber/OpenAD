
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
      INTEGER(w2f__i8) OpenAD_Symbol_18
      INTEGER(w2f__i8) OpenAD_Symbol_19
      INTEGER(w2f__i8) OpenAD_Symbol_2
      INTEGER(w2f__i8) OpenAD_Symbol_20
      INTEGER(w2f__i8) OpenAD_Symbol_21
      INTEGER(w2f__i8) OpenAD_Symbol_22
      INTEGER(w2f__i8) OpenAD_Symbol_23
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
      TYPE (OpenADTy_active) X(1 : 3)
      TYPE (OpenADTy_active) Y(1 : 3)
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) J
      INTEGER(w2f__i4) K
      INTEGER(w2f__i4) OpenAD_Symbol_24
      INTEGER(w2f__i4) OpenAD_Symbol_25
      INTEGER(w2f__i4) OpenAD_Symbol_26
      REAL(w2f__8) OpenAD_Symbol_27
      REAL(w2f__8) OpenAD_Symbol_28
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      TYPE (OpenADTy_active) OpenAD_prp_0
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
        __value__(Y(INT(I))) = __value__(X(I))
      END DO
      DO I = 1, 3, 1
        DO J = 1, 3, 1
          DO K = 1, 3, 1
            __value__(Y(INT(I))) = (__value__(X(J)) * __value__(Y(I)))
          END DO
        END DO
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      OpenAD_Symbol_8 = 0_w2f__i8
      DO I = 1, 3, 1
        __value__(Y(INT(I))) = __value__(X(I))
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(I)
        OpenAD_Symbol_8 = (INT(OpenAD_Symbol_8) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_8)
      OpenAD_Symbol_9 = 0_w2f__i8
      DO I = 1, 3, 1
        OpenAD_Symbol_10 = 0_w2f__i8
        DO J = 1, 3, 1
          OpenAD_Symbol_11 = 0_w2f__i8
          DO K = 1, 3, 1
            OpenAD_lin_0 = __value__(Y(I))
            OpenAD_lin_1 = __value__(X(J))
            __value__(Y(INT(I))) = (__value__(X(J)) * __value__(Y(I)))
C           $OpenAD$ INLINE push_s0(subst)
            CALL push_s0(OpenAD_lin_0)
C           $OpenAD$ INLINE push_s0(subst)
            CALL push_s0(OpenAD_lin_1)
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(I)
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(J)
            OpenAD_Symbol_11 = (INT(OpenAD_Symbol_11) + INT(1_w2f__i8))
          END DO
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_11)
          OpenAD_Symbol_10 = (INT(OpenAD_Symbol_10) + INT(1_w2f__i8))
        END DO
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_10)
        OpenAD_Symbol_9 = (INT(OpenAD_Symbol_9) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_9)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_0)
      OpenAD_Symbol_1 = 1
      DO WHILE(INT(OpenAD_Symbol_1) .LE. INT(OpenAD_Symbol_0))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_2)
        OpenAD_Symbol_3 = 1
        DO WHILE(INT(OpenAD_Symbol_3) .LE. INT(OpenAD_Symbol_2))
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_4)
          OpenAD_Symbol_5 = 1
          DO WHILE(INT(OpenAD_Symbol_5) .LE. INT(OpenAD_Symbol_4))
C           $OpenAD$ INLINE pop_i_s0(subst)
            CALL pop_i_s0(OpenAD_Symbol_25)
C           $OpenAD$ INLINE pop_i_s0(subst)
            CALL pop_i_s0(OpenAD_Symbol_26)
C           $OpenAD$ INLINE pop_s0(subst)
            CALL pop_s0(OpenAD_Symbol_27)
C           $OpenAD$ INLINE pop_s0(subst)
            CALL pop_s0(OpenAD_Symbol_28)
C           $OpenAD$ INLINE Saxpy(subst,subst,subst)
            CALL Saxpy(OpenAD_Symbol_27, __deriv__(Y(OpenAD_Symbol_26))
     > , __deriv__(OpenAD_prp_0))
C           $OpenAD$ INLINE Saxpy(subst,subst,subst)
            CALL Saxpy(OpenAD_Symbol_28, __deriv__(Y(OpenAD_Symbol_26))
     > , __deriv__(X(OpenAD_Symbol_25)))
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(Y(OpenAD_Symbol_26)))
C           $OpenAD$ INLINE IncDeriv(subst,subst)
            CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(Y(
     > OpenAD_Symbol_26)))
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
            OpenAD_Symbol_5 = INT(OpenAD_Symbol_5) + 1
          END DO
          OpenAD_Symbol_3 = INT(OpenAD_Symbol_3) + 1
        END DO
        OpenAD_Symbol_1 = INT(OpenAD_Symbol_1) + 1
      END DO
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_6)
      OpenAD_Symbol_7 = 1
      DO WHILE(INT(OpenAD_Symbol_7) .LE. INT(OpenAD_Symbol_6))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_24)
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(Y(OpenAD_Symbol_24)), __deriv__(X(
     > OpenAD_Symbol_24)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(Y(OpenAD_Symbol_24)))
        OpenAD_Symbol_7 = INT(OpenAD_Symbol_7) + 1
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
      OpenAD_Symbol_20 = 0_w2f__i8
      DO I = 1, 3, 1
        __value__(Y(INT(I))) = __value__(X(I))
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(I)
        OpenAD_Symbol_20 = (INT(OpenAD_Symbol_20) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_20)
      OpenAD_Symbol_21 = 0_w2f__i8
      DO I = 1, 3, 1
        OpenAD_Symbol_22 = 0_w2f__i8
        DO J = 1, 3, 1
          OpenAD_Symbol_23 = 0_w2f__i8
          DO K = 1, 3, 1
            OpenAD_lin_0 = __value__(Y(I))
            OpenAD_lin_1 = __value__(X(J))
            __value__(Y(INT(I))) = (__value__(X(J)) * __value__(Y(I)))
C           $OpenAD$ INLINE push_s0(subst)
            CALL push_s0(OpenAD_lin_0)
C           $OpenAD$ INLINE push_s0(subst)
            CALL push_s0(OpenAD_lin_1)
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(I)
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(J)
            OpenAD_Symbol_23 = (INT(OpenAD_Symbol_23) + INT(1_w2f__i8))
          END DO
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_23)
          OpenAD_Symbol_22 = (INT(OpenAD_Symbol_22) + INT(1_w2f__i8))
        END DO
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_22)
        OpenAD_Symbol_21 = (INT(OpenAD_Symbol_21) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_21)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_12)
      OpenAD_Symbol_13 = 1
      DO WHILE(INT(OpenAD_Symbol_13) .LE. INT(OpenAD_Symbol_12))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_14)
        OpenAD_Symbol_15 = 1
        DO WHILE(INT(OpenAD_Symbol_15) .LE. INT(OpenAD_Symbol_14))
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_16)
          OpenAD_Symbol_17 = 1
          DO WHILE(INT(OpenAD_Symbol_17) .LE. INT(OpenAD_Symbol_16))
C           $OpenAD$ INLINE pop_i_s0(subst)
            CALL pop_i_s0(OpenAD_Symbol_25)
C           $OpenAD$ INLINE pop_i_s0(subst)
            CALL pop_i_s0(OpenAD_Symbol_26)
C           $OpenAD$ INLINE pop_s0(subst)
            CALL pop_s0(OpenAD_Symbol_27)
C           $OpenAD$ INLINE pop_s0(subst)
            CALL pop_s0(OpenAD_Symbol_28)
C           $OpenAD$ INLINE Saxpy(subst,subst,subst)
            CALL Saxpy(OpenAD_Symbol_27, __deriv__(Y(OpenAD_Symbol_26))
     > , __deriv__(OpenAD_prp_0))
C           $OpenAD$ INLINE Saxpy(subst,subst,subst)
            CALL Saxpy(OpenAD_Symbol_28, __deriv__(Y(OpenAD_Symbol_26))
     > , __deriv__(X(OpenAD_Symbol_25)))
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(Y(OpenAD_Symbol_26)))
C           $OpenAD$ INLINE IncDeriv(subst,subst)
            CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(Y(
     > OpenAD_Symbol_26)))
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
            OpenAD_Symbol_17 = INT(OpenAD_Symbol_17) + 1
          END DO
          OpenAD_Symbol_15 = INT(OpenAD_Symbol_15) + 1
        END DO
        OpenAD_Symbol_13 = INT(OpenAD_Symbol_13) + 1
      END DO
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_18)
      OpenAD_Symbol_19 = 1
      DO WHILE(INT(OpenAD_Symbol_19) .LE. INT(OpenAD_Symbol_18))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_24)
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(Y(OpenAD_Symbol_24)), __deriv__(X(
     > OpenAD_Symbol_24)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(Y(OpenAD_Symbol_24)))
        OpenAD_Symbol_19 = INT(OpenAD_Symbol_19) + 1
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE
