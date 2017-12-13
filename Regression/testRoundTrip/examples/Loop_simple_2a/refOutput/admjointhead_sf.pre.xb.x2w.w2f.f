
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
      TYPE (OpenADTy_active) X(1 : 3)
      TYPE (OpenADTy_active) Y(1 : 3)
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) J
      INTEGER(w2f__i4) K
      INTEGER(w2f__i4) OpenAD_Symbol_15
      INTEGER(w2f__i4) OpenAD_Symbol_16
      INTEGER(w2f__i4) OpenAD_Symbol_17
      REAL(w2f__8) OpenAD_Symbol_18
      REAL(w2f__8) OpenAD_Symbol_19
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
C$OPENAD XXX Simple loop
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
      OpenAD_Symbol_2 = 0_w2f__i8
      DO I = 1, 3, 1
        __value__(Y(INT(I))) = __value__(X(I))
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(I)
        OpenAD_Symbol_2 = (INT(OpenAD_Symbol_2) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_2)
C$OPENAD XXX Simple loop
      DO I = 1, 3, 1
        DO J = 1, 3, 1
          DO K = 1, 3, 1
            OpenAD_lin_0 = __value__(Y(I))
            OpenAD_lin_1 = __value__(X(J))
            __value__(Y(INT(I))) = (__value__(X(J)) * __value__(Y(I)))
C           $OpenAD$ INLINE push_s0(subst)
            CALL push_s0(OpenAD_lin_0)
C           $OpenAD$ INLINE push_s0(subst)
            CALL push_s0(OpenAD_lin_1)
          END DO
        END DO
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
      I = 1 + 1 *((3 - 1) / 1)
      DO WHILE(I .GE. 1)
        J = 1 + 1 *((3 - 1) / 1)
        DO WHILE(J .GE. 1)
          K = 1 + 1 *((3 - 1) / 1)
          DO WHILE(K .GE. 1)
C           $OpenAD$ INLINE pop_s0(subst)
            CALL pop_s0(OpenAD_Symbol_18)
C           $OpenAD$ INLINE pop_s0(subst)
            CALL pop_s0(OpenAD_Symbol_19)
C           $OpenAD$ INLINE Saxpy(subst,subst,subst)
            CALL Saxpy(OpenAD_Symbol_18, __deriv__(Y(I)), __deriv__(
     > OpenAD_prp_0))
C           $OpenAD$ INLINE Saxpy(subst,subst,subst)
            CALL Saxpy(OpenAD_Symbol_19, __deriv__(Y(I)), __deriv__(X(J
     > )))
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(Y(I)))
C           $OpenAD$ INLINE IncDeriv(subst,subst)
            CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(Y(I)))
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
            K = K - 1
          END DO
          J = J - 1
        END DO
        I = I - 1
      END DO
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_0)
      OpenAD_Symbol_1 = 1
      DO WHILE(INT(OpenAD_Symbol_1) .LE. INT(OpenAD_Symbol_0))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_15)
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(Y(OpenAD_Symbol_15)), __deriv__(X(
     > OpenAD_Symbol_15)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(Y(OpenAD_Symbol_15)))
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
      OpenAD_Symbol_11 = 0_w2f__i8
      DO I = 1, 3, 1
        __value__(Y(INT(I))) = __value__(X(I))
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(I)
        OpenAD_Symbol_11 = (INT(OpenAD_Symbol_11) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_11)
C$OPENAD XXX Simple loop
      OpenAD_Symbol_12 = 0_w2f__i8
      DO I = 1, 3, 1
        OpenAD_Symbol_13 = 0_w2f__i8
        DO J = 1, 3, 1
          OpenAD_Symbol_14 = 0_w2f__i8
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
            OpenAD_Symbol_14 = (INT(OpenAD_Symbol_14) + INT(1_w2f__i8))
          END DO
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_14)
          OpenAD_Symbol_13 = (INT(OpenAD_Symbol_13) + INT(1_w2f__i8))
        END DO
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_13)
        OpenAD_Symbol_12 = (INT(OpenAD_Symbol_12) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_12)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_3)
      OpenAD_Symbol_4 = 1
      DO WHILE(INT(OpenAD_Symbol_4) .LE. INT(OpenAD_Symbol_3))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_5)
        OpenAD_Symbol_6 = 1
        DO WHILE(INT(OpenAD_Symbol_6) .LE. INT(OpenAD_Symbol_5))
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_7)
          OpenAD_Symbol_8 = 1
          DO WHILE(INT(OpenAD_Symbol_8) .LE. INT(OpenAD_Symbol_7))
C           $OpenAD$ INLINE pop_i_s0(subst)
            CALL pop_i_s0(OpenAD_Symbol_16)
C           $OpenAD$ INLINE pop_i_s0(subst)
            CALL pop_i_s0(OpenAD_Symbol_17)
C           $OpenAD$ INLINE pop_s0(subst)
            CALL pop_s0(OpenAD_Symbol_18)
C           $OpenAD$ INLINE pop_s0(subst)
            CALL pop_s0(OpenAD_Symbol_19)
C           $OpenAD$ INLINE Saxpy(subst,subst,subst)
            CALL Saxpy(OpenAD_Symbol_18, __deriv__(Y(OpenAD_Symbol_17))
     > , __deriv__(OpenAD_prp_0))
C           $OpenAD$ INLINE Saxpy(subst,subst,subst)
            CALL Saxpy(OpenAD_Symbol_19, __deriv__(Y(OpenAD_Symbol_17))
     > , __deriv__(X(OpenAD_Symbol_16)))
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(Y(OpenAD_Symbol_17)))
C           $OpenAD$ INLINE IncDeriv(subst,subst)
            CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(Y(
     > OpenAD_Symbol_17)))
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
            OpenAD_Symbol_8 = INT(OpenAD_Symbol_8) + 1
          END DO
          OpenAD_Symbol_6 = INT(OpenAD_Symbol_6) + 1
        END DO
        OpenAD_Symbol_4 = INT(OpenAD_Symbol_4) + 1
      END DO
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_9)
      OpenAD_Symbol_10 = 1
      DO WHILE(INT(OpenAD_Symbol_10) .LE. INT(OpenAD_Symbol_9))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_15)
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(Y(OpenAD_Symbol_15)), __deriv__(X(
     > OpenAD_Symbol_15)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(Y(OpenAD_Symbol_15)))
        OpenAD_Symbol_10 = INT(OpenAD_Symbol_10) + 1
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE
