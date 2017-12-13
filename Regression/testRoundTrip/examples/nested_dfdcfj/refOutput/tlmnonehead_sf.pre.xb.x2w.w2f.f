
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
      SUBROUTINE head(NX, NY, X, FVEC, R)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      INTEGER(w2f__i4) NX
      INTEGER(w2f__i4) NY
      TYPE (OpenADTy_active) X(1 : INT((NX * NY)))
      TYPE (OpenADTy_active) FVEC(1 : INT((NX * NY)))
      REAL(w2f__8) R
C
C     **** Local Variables and Functions ****
C
      EXTERNAL foo
      REAL(w2f__8) FOUR
      PARAMETER ( FOUR = 4.0D00)
      REAL(w2f__8) HX
      REAL(w2f__8) HX2
      REAL(w2f__8) HY
      REAL(w2f__8) HY2
      INTEGER(w2f__i4) K
      INTEGER(w2f__i4) N
      REAL(w2f__8) NXP1
      REAL(w2f__8) NYP1
      REAL(w2f__8) ONE
      PARAMETER ( ONE = 1.0D00)
      REAL(w2f__8) THREE
      PARAMETER ( THREE = 3.0D00)
      REAL(w2f__8) TWO
      PARAMETER ( TWO = 2.0D00)
      REAL(w2f__8) ZERO
      PARAMETER ( ZERO = 0.0D00)
      REAL(w2f__8) OpenAD_aux_0
      REAL(w2f__8) OpenAD_lin_0
      TYPE (OpenADTy_active) OpenAD_prp_0
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(FVEC)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      N = (NX * NY)
      NXP1 = (NX + 1)
      NYP1 = (NY + 1)
      HX = (1.0D00 / NXP1)
      HY = (1.0D00 / NYP1)
      HY2 = (HY * HY)
      HX2 = (HX * HX)
      CALL foo(NX, NY, __deriv__(X), __deriv__(FVEC), R, N, HX, HY, HY2
     > , HX2)
      DO K = 1, N, 1
        OpenAD_aux_0 = (HX2 * HY2)
        OpenAD_lin_0 = OpenAD_aux_0
        __value__(FVEC(INT(K))) = (__value__(FVEC(K)) * OpenAD_aux_0)
        CALL setderiv(__deriv__(OpenAD_prp_0), __deriv__(FVEC(K)))
        CALL sax(OpenAD_lin_0, __deriv__(OpenAD_prp_0), __deriv__(FVEC(
     > K)))
      END DO
      END SUBROUTINE

      SUBROUTINE foo(NX, NY, X, FVEC, R, N, HX, HY, HY2, HX2)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      INTEGER(w2f__i4) NX
      INTEGER(w2f__i4) NY
      TYPE (OpenADTy_active) X(1 : INT((NX * NY)))
      TYPE (OpenADTy_active) FVEC(1 : INT((NX * NY)))
      REAL(w2f__8) R
      INTEGER(w2f__i4) N
      REAL(w2f__8) HX
      REAL(w2f__8) HY
      REAL(w2f__8) HY2
      REAL(w2f__8) HX2
C
C     **** Local Variables and Functions ****
C
      EXTERNAL bar
      TYPE (OpenADTy_active) DPDX
      TYPE (OpenADTy_active) DPDY
      REAL(w2f__8) FOUR
      PARAMETER ( FOUR = 4.0D00)
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) J
      INTEGER(w2f__i4) K
      REAL(w2f__8) ONE
      PARAMETER ( ONE = 1.0D00)
      TYPE (OpenADTy_active) P
      TYPE (OpenADTy_active) PB
      TYPE (OpenADTy_active) PBB
      TYPE (OpenADTy_active) PBL
      TYPE (OpenADTy_active) PBLAP
      TYPE (OpenADTy_active) PBR
      TYPE (OpenADTy_active) PL
      TYPE (OpenADTy_active) PLAP
      TYPE (OpenADTy_active) PLL
      TYPE (OpenADTy_active) PLLAP
      TYPE (OpenADTy_active) PR
      TYPE (OpenADTy_active) PRLAP
      TYPE (OpenADTy_active) PRR
      TYPE (OpenADTy_active) PT
      TYPE (OpenADTy_active) PTL
      TYPE (OpenADTy_active) PTLAP
      TYPE (OpenADTy_active) PTR
      TYPE (OpenADTy_active) PTT
      REAL(w2f__8) THREE
      PARAMETER ( THREE = 3.0D00)
      REAL(w2f__8) TWO
      PARAMETER ( TWO = 2.0D00)
      REAL(w2f__8) ZERO
      PARAMETER ( ZERO = 0.0D00)
      REAL(w2f__8) OpenAD_acc_0
      REAL(w2f__8) OpenAD_acc_1
      REAL(w2f__8) OpenAD_acc_2
      REAL(w2f__8) OpenAD_acc_3
      REAL(w2f__8) OpenAD_acc_4
      REAL(w2f__8) OpenAD_acc_5
      REAL(w2f__8) OpenAD_aux_1
      REAL(w2f__8) OpenAD_aux_10
      REAL(w2f__8) OpenAD_aux_11
      REAL(w2f__8) OpenAD_aux_12
      REAL(w2f__8) OpenAD_aux_13
      REAL(w2f__8) OpenAD_aux_2
      REAL(w2f__8) OpenAD_aux_3
      REAL(w2f__8) OpenAD_aux_4
      REAL(w2f__8) OpenAD_aux_5
      REAL(w2f__8) OpenAD_aux_6
      REAL(w2f__8) OpenAD_aux_7
      REAL(w2f__8) OpenAD_aux_8
      REAL(w2f__8) OpenAD_aux_9
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
      TYPE (OpenADTy_active) OpenAD_prp_1
      TYPE (OpenADTy_active) OpenAD_prp_2
      TYPE (OpenADTy_active) OpenAD_prp_3
      TYPE (OpenADTy_active) OpenAD_prp_4
      TYPE (OpenADTy_active) OpenAD_prp_5
      TYPE (OpenADTy_active) OpenAD_prp_6
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      DO I = 1, NY, 1
        DO J = 1, NX, 1
          K = (J + NX *(I +(-1)))
          IF((I .eq. 1) .OR.(J .eq. 1)) THEN
            __value__(PBL) = 0.0D00
            CALL zero_deriv(__deriv__(PBL))
          ELSE
            __value__(PBL) = __value__(X(K - NX + (-1)))
            CALL setderiv(__deriv__(PBL), __deriv__(X(K - NX + (-1))))
          ENDIF
          IF(I .eq. 1) THEN
            __value__(PB) = 0.0D00
            __value__(PBB) = __value__(X(K))
            CALL zero_deriv(__deriv__(PB))
            CALL setderiv(__deriv__(PBB), __deriv__(X(K)))
          ELSE
            IF(I .eq. 2) THEN
              __value__(PB) = __value__(X(K - NX))
              __value__(PBB) = 0.0D00
              CALL zero_deriv(__deriv__(PBB))
              CALL setderiv(__deriv__(PB), __deriv__(X(K - NX)))
            ELSE
              __value__(PB) = __value__(X(K - NX))
              __value__(PBB) = __value__(X(K - NX * 2))
              CALL setderiv(__deriv__(PB), __deriv__(X(K - NX)))
              CALL setderiv(__deriv__(PBB), __deriv__(X(K - NX * 2)))
            ENDIF
          ENDIF
          IF((NX .eq. J) .OR.(I .eq. 1)) THEN
            __value__(PBR) = 0.0D00
            CALL zero_deriv(__deriv__(PBR))
          ELSE
            __value__(PBR) = __value__(X(K - NX + 1))
            CALL setderiv(__deriv__(PBR), __deriv__(X(K - NX + 1)))
          ENDIF
          IF(J .eq. 1) THEN
            __value__(PL) = 0.0D00
            __value__(PLL) = __value__(X(K))
            CALL zero_deriv(__deriv__(PL))
            CALL setderiv(__deriv__(PLL), __deriv__(X(K)))
          ELSE
            IF(J .eq. 2) THEN
              __value__(PL) = __value__(X(K + (-1)))
              __value__(PLL) = 0.0D00
              CALL zero_deriv(__deriv__(PLL))
              CALL setderiv(__deriv__(PL), __deriv__(X(K + (-1))))
            ELSE
              __value__(PL) = __value__(X(K + (-1)))
              __value__(PLL) = __value__(X(K + (-2)))
              CALL setderiv(__deriv__(PL), __deriv__(X(K + (-1))))
              CALL setderiv(__deriv__(PLL), __deriv__(X(K + (-2))))
            ENDIF
          ENDIF
          __value__(P) = __value__(X(K))
          CALL setderiv(__deriv__(P), __deriv__(X(K)))
          IF(J .eq.(NX +(-1))) THEN
            __value__(PR) = __value__(X(K + 1))
            __value__(PRR) = 0.0D00
            CALL zero_deriv(__deriv__(PRR))
            CALL setderiv(__deriv__(PR), __deriv__(X(K + 1)))
          ELSE
            IF(NX .eq. J) THEN
              __value__(PR) = 0.0D00
              __value__(PRR) = __value__(X(K))
              CALL zero_deriv(__deriv__(PR))
              CALL setderiv(__deriv__(PRR), __deriv__(X(K)))
            ELSE
              __value__(PR) = __value__(X(K + 1))
              __value__(PRR) = __value__(X(K + 2))
              CALL setderiv(__deriv__(PR), __deriv__(X(K + 1)))
              CALL setderiv(__deriv__(PRR), __deriv__(X(K + 2)))
            ENDIF
          ENDIF
          IF((NY .eq. I) .OR.(J .eq. 1)) THEN
            __value__(PTL) = 0.0D00
            CALL zero_deriv(__deriv__(PTL))
          ELSE
            __value__(PTL) = __value__(X(NX + K + (-1)))
            CALL setderiv(__deriv__(PTL), __deriv__(X(NX + K + (-1))))
          ENDIF
          IF(I .eq.(NY +(-1))) THEN
            __value__(PT) = __value__(X(NX + K))
            __value__(PTT) = 0.0D00
            CALL zero_deriv(__deriv__(PTT))
            CALL setderiv(__deriv__(PT), __deriv__(X(NX + K)))
          ELSE
            IF(NY .eq. I) THEN
              __value__(PT) = 0.0D00
              __value__(PTT) = (__value__(X(K)) + HY * 2.0D00)
              CALL zero_deriv(__deriv__(PT))
              CALL setderiv(__deriv__(PTT), __deriv__(X(K)))
            ELSE
              __value__(PT) = __value__(X(NX + K))
              __value__(PTT) = __value__(X(K + NX * 2))
              CALL setderiv(__deriv__(PT), __deriv__(X(NX + K)))
              CALL setderiv(__deriv__(PTT), __deriv__(X(K + NX * 2)))
            ENDIF
          ENDIF
          IF((NX .eq. J) .OR.(NY .eq. I)) THEN
            __value__(PTR) = 0.0D00
            CALL zero_deriv(__deriv__(PTR))
          ELSE
            __value__(PTR) = __value__(X(NX + K + 1))
            CALL setderiv(__deriv__(PTR), __deriv__(X(NX + K + 1)))
          ENDIF
          OpenAD_aux_1 = (__value__(PT) - __value__(PB))
          OpenAD_aux_2 = (HY * 2.0D00)
          OpenAD_lin_1 = (INT(1_w2f__i8) / OpenAD_aux_2)
          __value__(DPDY) = (OpenAD_aux_1 / OpenAD_aux_2)
          OpenAD_aux_3 = (__value__(PR) - __value__(PL))
          OpenAD_aux_4 = (HX * 2.0D00)
          OpenAD_lin_2 = (INT(1_w2f__i8) / OpenAD_aux_4)
          __value__(DPDX) = (OpenAD_aux_3 / OpenAD_aux_4)
          CALL setderiv(__deriv__(OpenAD_prp_1), __deriv__(PT))
          CALL dec_deriv(__deriv__(OpenAD_prp_1), __deriv__(PB))
          CALL setderiv(__deriv__(OpenAD_prp_2), __deriv__(PR))
          CALL dec_deriv(__deriv__(OpenAD_prp_2), __deriv__(PL))
          CALL sax(OpenAD_lin_1, __deriv__(OpenAD_prp_1), __deriv__(
     > DPDY))
          CALL sax(OpenAD_lin_2, __deriv__(OpenAD_prp_2), __deriv__(
     > DPDX))
          CALL bar(__deriv__(PBB), __deriv__(PBL), __deriv__(PB),
     >  __deriv__(PBR), __deriv__(PLL), __deriv__(PL), __deriv__(P),
     >  __deriv__(PR), __deriv__(PRR), __deriv__(PTL), __deriv__(PT),
     >  __deriv__(PTR), __deriv__(PTT), HY2, HX2, __deriv__(PLAP),
     >  __deriv__(PBLAP), __deriv__(PLLAP), __deriv__(PRLAP), __deriv__
     > (PTLAP))
          OpenAD_aux_5 = (__value__(PBLAP) + __value__(PTLAP) -
     >  __value__(PLAP) * 2.0D00)
          OpenAD_aux_6 = (__value__(PLLAP) + __value__(PRLAP) -
     >  __value__(PLAP) * 2.0D00)
          OpenAD_aux_10 = (__value__(PRLAP) - __value__(PLLAP))
          OpenAD_aux_8 = (__value__(DPDY) * OpenAD_aux_10)
          OpenAD_aux_9 = (HX * 2.0D00)
          OpenAD_aux_13 = (__value__(PTLAP) - __value__(PBLAP))
          OpenAD_aux_11 = (__value__(DPDX) * OpenAD_aux_13)
          OpenAD_aux_12 = (HY * 2.0D00)
          OpenAD_aux_7 = ((OpenAD_aux_8 / OpenAD_aux_9) -(OpenAD_aux_11
     >  / OpenAD_aux_12))
          OpenAD_lin_3 = (INT(1_w2f__i8) / HY2)
          OpenAD_lin_4 = (INT(1_w2f__i8) / HX2)
          OpenAD_lin_7 = OpenAD_aux_10
          OpenAD_lin_8 = __value__(DPDY)
          OpenAD_lin_6 = (INT(1_w2f__i8) / OpenAD_aux_9)
          OpenAD_lin_10 = OpenAD_aux_13
          OpenAD_lin_11 = __value__(DPDX)
          OpenAD_lin_9 = (INT(1_w2f__i8) / OpenAD_aux_12)
          OpenAD_lin_5 = R
          __value__(FVEC(INT(K))) = ((OpenAD_aux_5 / HY2) +(
     > OpenAD_aux_6 / HX2) - R * OpenAD_aux_7)
          OpenAD_acc_0 = (OpenAD_lin_5 * INT((-1_w2f__i8)))
          OpenAD_acc_1 = (INT((-1_w2f__i8)) * OpenAD_acc_0)
          OpenAD_acc_2 = (OpenAD_lin_10 * OpenAD_lin_9 * OpenAD_acc_1)
          OpenAD_acc_3 = (OpenAD_lin_11 * OpenAD_lin_9 * OpenAD_acc_1)
          OpenAD_acc_4 = (OpenAD_lin_7 * OpenAD_lin_6 * OpenAD_acc_0)
          OpenAD_acc_5 = (OpenAD_lin_8 * OpenAD_lin_6 * OpenAD_acc_0)
          CALL setderiv(__deriv__(OpenAD_prp_3), __deriv__(PRLAP))
          CALL dec_deriv(__deriv__(OpenAD_prp_3), __deriv__(PLLAP))
          CALL setderiv(__deriv__(OpenAD_prp_4), __deriv__(PTLAP))
          CALL dec_deriv(__deriv__(OpenAD_prp_4), __deriv__(PBLAP))
          CALL setderiv(__deriv__(OpenAD_prp_5), __deriv__(PBLAP))
          CALL inc_deriv(__deriv__(OpenAD_prp_5), __deriv__(PTLAP))
          CALL saxpy(-2.0D00, __deriv__(PLAP), __deriv__(OpenAD_prp_5))
          CALL setderiv(__deriv__(OpenAD_prp_6), __deriv__(PLLAP))
          CALL inc_deriv(__deriv__(OpenAD_prp_6), __deriv__(PRLAP))
          CALL saxpy(-2.0D00, __deriv__(PLAP), __deriv__(OpenAD_prp_6))
          CALL sax(OpenAD_acc_2, __deriv__(DPDX), __deriv__(FVEC(K)))
          CALL saxpy(OpenAD_acc_3, __deriv__(OpenAD_prp_4), __deriv__(
     > FVEC(K)))
          CALL saxpy(OpenAD_acc_4, __deriv__(DPDY), __deriv__(FVEC(K)))
          CALL saxpy(OpenAD_acc_5, __deriv__(OpenAD_prp_3), __deriv__(
     > FVEC(K)))
          CALL saxpy(OpenAD_lin_3, __deriv__(OpenAD_prp_5), __deriv__(
     > FVEC(K)))
          CALL saxpy(OpenAD_lin_4, __deriv__(OpenAD_prp_6), __deriv__(
     > FVEC(K)))
        END DO
      END DO
      END SUBROUTINE

      SUBROUTINE bar(PBB, PBL, PB, PBR, PLL, PL, P, PR, PRR, PTL, PT,
     >  PTR, PTT, HY2, HX2, PLAP, PBLAP, PLLAP, PRLAP, PTLAP)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) PBB
      TYPE (OpenADTy_active) PBL
      TYPE (OpenADTy_active) PB
      TYPE (OpenADTy_active) PBR
      TYPE (OpenADTy_active) PLL
      TYPE (OpenADTy_active) PL
      TYPE (OpenADTy_active) P
      TYPE (OpenADTy_active) PR
      TYPE (OpenADTy_active) PRR
      TYPE (OpenADTy_active) PTL
      TYPE (OpenADTy_active) PT
      TYPE (OpenADTy_active) PTR
      TYPE (OpenADTy_active) PTT
      REAL(w2f__8) HY2
      REAL(w2f__8) HX2
      TYPE (OpenADTy_active) PLAP
      TYPE (OpenADTy_active) PBLAP
      TYPE (OpenADTy_active) PLLAP
      TYPE (OpenADTy_active) PRLAP
      TYPE (OpenADTy_active) PTLAP
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) FOUR
      PARAMETER ( FOUR = 4.0D00)
      REAL(w2f__8) ONE
      PARAMETER ( ONE = 1.0D00)
      REAL(w2f__8) THREE
      PARAMETER ( THREE = 3.0D00)
      REAL(w2f__8) TWO
      PARAMETER ( TWO = 2.0D00)
      REAL(w2f__8) ZERO
      PARAMETER ( ZERO = 0.0D00)
      REAL(w2f__8) OpenAD_aux_14
      REAL(w2f__8) OpenAD_aux_15
      REAL(w2f__8) OpenAD_aux_16
      REAL(w2f__8) OpenAD_aux_17
      REAL(w2f__8) OpenAD_aux_18
      REAL(w2f__8) OpenAD_aux_19
      REAL(w2f__8) OpenAD_aux_20
      REAL(w2f__8) OpenAD_aux_21
      REAL(w2f__8) OpenAD_aux_22
      REAL(w2f__8) OpenAD_aux_23
      REAL(w2f__8) OpenAD_lin_12
      REAL(w2f__8) OpenAD_lin_13
      REAL(w2f__8) OpenAD_lin_14
      REAL(w2f__8) OpenAD_lin_15
      REAL(w2f__8) OpenAD_lin_16
      REAL(w2f__8) OpenAD_lin_17
      REAL(w2f__8) OpenAD_lin_18
      REAL(w2f__8) OpenAD_lin_19
      REAL(w2f__8) OpenAD_lin_20
      REAL(w2f__8) OpenAD_lin_21
      TYPE (OpenADTy_active) OpenAD_prp_10
      TYPE (OpenADTy_active) OpenAD_prp_11
      TYPE (OpenADTy_active) OpenAD_prp_12
      TYPE (OpenADTy_active) OpenAD_prp_13
      TYPE (OpenADTy_active) OpenAD_prp_14
      TYPE (OpenADTy_active) OpenAD_prp_15
      TYPE (OpenADTy_active) OpenAD_prp_16
      TYPE (OpenADTy_active) OpenAD_prp_7
      TYPE (OpenADTy_active) OpenAD_prp_8
      TYPE (OpenADTy_active) OpenAD_prp_9
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      OpenAD_aux_14 = (__value__(PBB) + __value__(P) - __value__(PB) *
     >  2.0D00)
      OpenAD_aux_15 = (__value__(PBL) + __value__(PBR) - __value__(PB)
     >  * 2.0D00)
      OpenAD_lin_12 = (INT(1_w2f__i8) / HY2)
      OpenAD_lin_13 = (INT(1_w2f__i8) / HX2)
      __value__(PBLAP) = ((OpenAD_aux_14 / HY2) +(OpenAD_aux_15 / HX2))
      OpenAD_aux_16 = (__value__(PBL) + __value__(PTL) - __value__(PL)
     >  * 2.0D00)
      OpenAD_aux_17 = (__value__(PLL) + __value__(P) - __value__(PL) *
     >  2.0D00)
      OpenAD_lin_14 = (INT(1_w2f__i8) / HY2)
      OpenAD_lin_15 = (INT(1_w2f__i8) / HX2)
      __value__(PLLAP) = ((OpenAD_aux_16 / HY2) +(OpenAD_aux_17 / HX2))
      OpenAD_aux_18 = (__value__(PB) + __value__(PT) - __value__(P) *
     >  2.0D00)
      OpenAD_aux_19 = (__value__(PL) + __value__(PR) - __value__(P) *
     >  2.0D00)
      OpenAD_lin_16 = (INT(1_w2f__i8) / HY2)
      OpenAD_lin_17 = (INT(1_w2f__i8) / HX2)
      __value__(PLAP) = ((OpenAD_aux_18 / HY2) +(OpenAD_aux_19 / HX2))
      OpenAD_aux_20 = (__value__(PBR) + __value__(PTR) - __value__(PR)
     >  * 2.0D00)
      OpenAD_aux_21 = (__value__(P) + __value__(PRR) - __value__(PR) *
     >  2.0D00)
      OpenAD_lin_18 = (INT(1_w2f__i8) / HY2)
      OpenAD_lin_19 = (INT(1_w2f__i8) / HX2)
      __value__(PRLAP) = ((OpenAD_aux_20 / HY2) +(OpenAD_aux_21 / HX2))
      OpenAD_aux_22 = (__value__(P) + __value__(PTT) - __value__(PT) *
     >  2.0D00)
      OpenAD_aux_23 = (__value__(PTL) + __value__(PTR) - __value__(PT)
     >  * 2.0D00)
      OpenAD_lin_20 = (INT(1_w2f__i8) / HY2)
      OpenAD_lin_21 = (INT(1_w2f__i8) / HX2)
      __value__(PTLAP) = ((OpenAD_aux_22 / HY2) +(OpenAD_aux_23 / HX2))
      CALL setderiv(__deriv__(OpenAD_prp_7), __deriv__(PBB))
      CALL inc_deriv(__deriv__(OpenAD_prp_7), __deriv__(P))
      CALL saxpy(-2.0D00, __deriv__(PB), __deriv__(OpenAD_prp_7))
      CALL setderiv(__deriv__(OpenAD_prp_8), __deriv__(PBL))
      CALL inc_deriv(__deriv__(OpenAD_prp_8), __deriv__(PBR))
      CALL saxpy(-2.0D00, __deriv__(PB), __deriv__(OpenAD_prp_8))
      CALL setderiv(__deriv__(OpenAD_prp_9), __deriv__(PBL))
      CALL inc_deriv(__deriv__(OpenAD_prp_9), __deriv__(PTL))
      CALL saxpy(-2.0D00, __deriv__(PL), __deriv__(OpenAD_prp_9))
      CALL setderiv(__deriv__(OpenAD_prp_10), __deriv__(PLL))
      CALL inc_deriv(__deriv__(OpenAD_prp_10), __deriv__(P))
      CALL saxpy(-2.0D00, __deriv__(PL), __deriv__(OpenAD_prp_10))
      CALL setderiv(__deriv__(OpenAD_prp_11), __deriv__(PB))
      CALL inc_deriv(__deriv__(OpenAD_prp_11), __deriv__(PT))
      CALL saxpy(-2.0D00, __deriv__(P), __deriv__(OpenAD_prp_11))
      CALL setderiv(__deriv__(OpenAD_prp_12), __deriv__(PL))
      CALL inc_deriv(__deriv__(OpenAD_prp_12), __deriv__(PR))
      CALL saxpy(-2.0D00, __deriv__(P), __deriv__(OpenAD_prp_12))
      CALL setderiv(__deriv__(OpenAD_prp_13), __deriv__(PBR))
      CALL inc_deriv(__deriv__(OpenAD_prp_13), __deriv__(PTR))
      CALL saxpy(-2.0D00, __deriv__(PR), __deriv__(OpenAD_prp_13))
      CALL setderiv(__deriv__(OpenAD_prp_14), __deriv__(P))
      CALL inc_deriv(__deriv__(OpenAD_prp_14), __deriv__(PRR))
      CALL saxpy(-2.0D00, __deriv__(PR), __deriv__(OpenAD_prp_14))
      CALL setderiv(__deriv__(OpenAD_prp_15), __deriv__(P))
      CALL inc_deriv(__deriv__(OpenAD_prp_15), __deriv__(PTT))
      CALL saxpy(-2.0D00, __deriv__(PT), __deriv__(OpenAD_prp_15))
      CALL setderiv(__deriv__(OpenAD_prp_16), __deriv__(PTL))
      CALL inc_deriv(__deriv__(OpenAD_prp_16), __deriv__(PTR))
      CALL saxpy(-2.0D00, __deriv__(PT), __deriv__(OpenAD_prp_16))
      CALL sax(OpenAD_lin_12, __deriv__(OpenAD_prp_7), __deriv__(PBLAP)
     > )
      CALL saxpy(OpenAD_lin_13, __deriv__(OpenAD_prp_8), __deriv__(
     > PBLAP))
      CALL sax(OpenAD_lin_14, __deriv__(OpenAD_prp_9), __deriv__(PLLAP)
     > )
      CALL saxpy(OpenAD_lin_15, __deriv__(OpenAD_prp_10), __deriv__(
     > PLLAP))
      CALL sax(OpenAD_lin_16, __deriv__(OpenAD_prp_11), __deriv__(PLAP)
     > )
      CALL saxpy(OpenAD_lin_17, __deriv__(OpenAD_prp_12), __deriv__(
     > PLAP))
      CALL sax(OpenAD_lin_18, __deriv__(OpenAD_prp_13), __deriv__(PRLAP
     > ))
      CALL saxpy(OpenAD_lin_19, __deriv__(OpenAD_prp_14), __deriv__(
     > PRLAP))
      CALL sax(OpenAD_lin_20, __deriv__(OpenAD_prp_15), __deriv__(PTLAP
     > ))
      CALL saxpy(OpenAD_lin_21, __deriv__(OpenAD_prp_16), __deriv__(
     > PTLAP))
      END SUBROUTINE
