
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
      INTEGER(w2f__i8) OpenAD_Symbol_24
      INTEGER(w2f__i8) OpenAD_Symbol_25
      INTEGER(w2f__i8) OpenAD_Symbol_26
      INTEGER(w2f__i8) OpenAD_Symbol_27
      INTEGER(w2f__i8) OpenAD_Symbol_28
      INTEGER(w2f__i8) OpenAD_Symbol_29
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
      TYPE (OpenADTy_active) X(1 : 1)
      TYPE (OpenADTy_active) Y(1 : 1)
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) J
      INTEGER(w2f__i4) K
      REAL(w2f__8) OpenAD_Symbol_30
      REAL(w2f__8) OpenAD_Symbol_31
      REAL(w2f__8) OpenAD_Symbol_32
      REAL(w2f__8) OpenAD_Symbol_33
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_2
      REAL(w2f__8) OpenAD_lin_3
      TYPE (OpenADTy_active) OpenAD_prp_0
      TYPE (OpenADTy_active) OpenAD_prp_1
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
      DO I = 1, 1, 1
        __value__(Y(1)) = __value__(X(1))
        IF(__value__(Y(1)) .GT. 0.0D00) THEN
          DO J = 1, 3, 1
            DO K = 1, 3, 1
              IF(__value__(X(1)) .GT. 0.0D00) THEN
                __value__(Y(1)) = (__value__(X(1)) * __value__(Y(1)))
              ENDIF
            END DO
          END DO
        ELSE
          __value__(Y(1)) = (__value__(Y(1)) / __value__(X(1)))
        ENDIF
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      OpenAD_Symbol_8 = 0_w2f__i8
      DO I = 1, 1, 1
        __value__(Y(1)) = __value__(X(1))
        IF(__value__(Y(1)) .GT. 0.0D00) THEN
          OpenAD_Symbol_9 = 0_w2f__i8
          DO J = 1, 3, 1
            OpenAD_Symbol_10 = 0_w2f__i8
            DO K = 1, 3, 1
              IF(__value__(X(1)) .GT. 0.0D00) THEN
                OpenAD_lin_0 = __value__(Y(1))
                OpenAD_lin_1 = __value__(X(1))
                __value__(Y(1)) = (__value__(X(1)) * __value__(Y(1)))
C               $OpenAD$ INLINE push_s0(subst)
                CALL push_s0(OpenAD_lin_0)
C               $OpenAD$ INLINE push_s0(subst)
                CALL push_s0(OpenAD_lin_1)
                OpenAD_Symbol_11 = 1_w2f__i8
C               $OpenAD$ INLINE push_i_s0(subst)
                CALL push_i_s0(OpenAD_Symbol_11)
              ELSE
                OpenAD_Symbol_12 = 0_w2f__i8
C               $OpenAD$ INLINE push_i_s0(subst)
                CALL push_i_s0(OpenAD_Symbol_12)
              ENDIF
              OpenAD_Symbol_10 = (INT(OpenAD_Symbol_10) + INT(1_w2f__i8
     > ))
            END DO
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_10)
            OpenAD_Symbol_9 = (INT(OpenAD_Symbol_9) + INT(1_w2f__i8))
          END DO
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_9)
          OpenAD_Symbol_14 = 1_w2f__i8
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_14)
        ELSE
          OpenAD_lin_2 = (INT(1_w2f__i8) / __value__(X(1)))
          OpenAD_lin_3 = (-(__value__(Y(1)) /(__value__(X(1)) *
     >  __value__(X(1)))))
          __value__(Y(1)) = (__value__(Y(1)) / __value__(X(1)))
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_lin_2)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_lin_3)
          OpenAD_Symbol_13 = 0_w2f__i8
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_13)
        ENDIF
        OpenAD_Symbol_8 = (INT(OpenAD_Symbol_8) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_8)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_0)
      OpenAD_Symbol_1 = 1
      DO WHILE(INT(OpenAD_Symbol_1) .LE. INT(OpenAD_Symbol_0))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_2)
        IF(OpenAD_Symbol_2 .ne. 0) THEN
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_3)
          OpenAD_Symbol_4 = 1
          DO WHILE(INT(OpenAD_Symbol_4) .LE. INT(OpenAD_Symbol_3))
C           $OpenAD$ INLINE pop_i_s0(subst)
            CALL pop_i_s0(OpenAD_Symbol_5)
            OpenAD_Symbol_6 = 1
            DO WHILE(INT(OpenAD_Symbol_6) .LE. INT(OpenAD_Symbol_5))
C             $OpenAD$ INLINE pop_i_s0(subst)
              CALL pop_i_s0(OpenAD_Symbol_7)
              IF(OpenAD_Symbol_7 .ne. 0) THEN
C               $OpenAD$ INLINE pop_s0(subst)
                CALL pop_s0(OpenAD_Symbol_30)
C               $OpenAD$ INLINE pop_s0(subst)
                CALL pop_s0(OpenAD_Symbol_31)
C               $OpenAD$ INLINE Saxpy(subst,subst,subst)
                CALL Saxpy(OpenAD_Symbol_30, __deriv__(Y(1)), __deriv__
     > (OpenAD_prp_0))
C               $OpenAD$ INLINE Saxpy(subst,subst,subst)
                CALL Saxpy(OpenAD_Symbol_31, __deriv__(Y(1)), __deriv__
     > (X(1)))
C               $OpenAD$ INLINE ZeroDeriv(subst)
                CALL ZeroDeriv(__deriv__(Y(1)))
C               $OpenAD$ INLINE IncDeriv(subst,subst)
                CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(Y(1)))
C               $OpenAD$ INLINE ZeroDeriv(subst)
                CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
              ENDIF
              OpenAD_Symbol_6 = INT(OpenAD_Symbol_6) + 1
            END DO
            OpenAD_Symbol_4 = INT(OpenAD_Symbol_4) + 1
          END DO
        ELSE
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_32)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_33)
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_32, __deriv__(Y(1)), __deriv__(X(1))
     > )
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_33, __deriv__(Y(1)), __deriv__(
     > OpenAD_prp_1))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(Y(1)))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_1), __deriv__(Y(1)))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_1))
        ENDIF
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(Y(1)), __deriv__(X(1)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(Y(1)))
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
      OpenAD_Symbol_23 = 0_w2f__i8
      DO I = 1, 1, 1
        __value__(Y(1)) = __value__(X(1))
        IF(__value__(Y(1)) .GT. 0.0D00) THEN
          OpenAD_Symbol_24 = 0_w2f__i8
          DO J = 1, 3, 1
            OpenAD_Symbol_25 = 0_w2f__i8
            DO K = 1, 3, 1
              IF(__value__(X(1)) .GT. 0.0D00) THEN
                OpenAD_lin_0 = __value__(Y(1))
                OpenAD_lin_1 = __value__(X(1))
                __value__(Y(1)) = (__value__(X(1)) * __value__(Y(1)))
C               $OpenAD$ INLINE push_s0(subst)
                CALL push_s0(OpenAD_lin_0)
C               $OpenAD$ INLINE push_s0(subst)
                CALL push_s0(OpenAD_lin_1)
                OpenAD_Symbol_26 = 1_w2f__i8
C               $OpenAD$ INLINE push_i_s0(subst)
                CALL push_i_s0(OpenAD_Symbol_26)
              ELSE
                OpenAD_Symbol_27 = 0_w2f__i8
C               $OpenAD$ INLINE push_i_s0(subst)
                CALL push_i_s0(OpenAD_Symbol_27)
              ENDIF
              OpenAD_Symbol_25 = (INT(OpenAD_Symbol_25) + INT(1_w2f__i8
     > ))
            END DO
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_25)
            OpenAD_Symbol_24 = (INT(OpenAD_Symbol_24) + INT(1_w2f__i8))
          END DO
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_24)
          OpenAD_Symbol_29 = 1_w2f__i8
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_29)
        ELSE
          OpenAD_lin_2 = (INT(1_w2f__i8) / __value__(X(1)))
          OpenAD_lin_3 = (-(__value__(Y(1)) /(__value__(X(1)) *
     >  __value__(X(1)))))
          __value__(Y(1)) = (__value__(Y(1)) / __value__(X(1)))
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_lin_2)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_lin_3)
          OpenAD_Symbol_28 = 0_w2f__i8
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_28)
        ENDIF
        OpenAD_Symbol_23 = (INT(OpenAD_Symbol_23) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_23)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_15)
      OpenAD_Symbol_16 = 1
      DO WHILE(INT(OpenAD_Symbol_16) .LE. INT(OpenAD_Symbol_15))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_17)
        IF(OpenAD_Symbol_17 .ne. 0) THEN
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_18)
          OpenAD_Symbol_19 = 1
          DO WHILE(INT(OpenAD_Symbol_19) .LE. INT(OpenAD_Symbol_18))
C           $OpenAD$ INLINE pop_i_s0(subst)
            CALL pop_i_s0(OpenAD_Symbol_20)
            OpenAD_Symbol_21 = 1
            DO WHILE(INT(OpenAD_Symbol_21) .LE. INT(OpenAD_Symbol_20))
C             $OpenAD$ INLINE pop_i_s0(subst)
              CALL pop_i_s0(OpenAD_Symbol_22)
              IF(OpenAD_Symbol_22 .ne. 0) THEN
C               $OpenAD$ INLINE pop_s0(subst)
                CALL pop_s0(OpenAD_Symbol_30)
C               $OpenAD$ INLINE pop_s0(subst)
                CALL pop_s0(OpenAD_Symbol_31)
C               $OpenAD$ INLINE Saxpy(subst,subst,subst)
                CALL Saxpy(OpenAD_Symbol_30, __deriv__(Y(1)), __deriv__
     > (OpenAD_prp_0))
C               $OpenAD$ INLINE Saxpy(subst,subst,subst)
                CALL Saxpy(OpenAD_Symbol_31, __deriv__(Y(1)), __deriv__
     > (X(1)))
C               $OpenAD$ INLINE ZeroDeriv(subst)
                CALL ZeroDeriv(__deriv__(Y(1)))
C               $OpenAD$ INLINE IncDeriv(subst,subst)
                CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(Y(1)))
C               $OpenAD$ INLINE ZeroDeriv(subst)
                CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
              ENDIF
              OpenAD_Symbol_21 = INT(OpenAD_Symbol_21) + 1
            END DO
            OpenAD_Symbol_19 = INT(OpenAD_Symbol_19) + 1
          END DO
        ELSE
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_32)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_33)
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_32, __deriv__(Y(1)), __deriv__(X(1))
     > )
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_33, __deriv__(Y(1)), __deriv__(
     > OpenAD_prp_1))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(Y(1)))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_1), __deriv__(Y(1)))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_1))
        ENDIF
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(Y(1)), __deriv__(X(1)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(Y(1)))
        OpenAD_Symbol_16 = INT(OpenAD_Symbol_16) + 1
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE
