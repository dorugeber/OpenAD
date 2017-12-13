
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
      INTEGER(w2f__i8) OpenAD_Symbol_30
      INTEGER(w2f__i8) OpenAD_Symbol_31
      INTEGER(w2f__i8) OpenAD_Symbol_32
      INTEGER(w2f__i8) OpenAD_Symbol_33
      INTEGER(w2f__i8) OpenAD_Symbol_34
      INTEGER(w2f__i8) OpenAD_Symbol_35
      INTEGER(w2f__i8) OpenAD_Symbol_36
      INTEGER(w2f__i8) OpenAD_Symbol_37
      INTEGER(w2f__i8) OpenAD_Symbol_38
      INTEGER(w2f__i8) OpenAD_Symbol_39
      INTEGER(w2f__i8) OpenAD_Symbol_4
      INTEGER(w2f__i8) OpenAD_Symbol_40
      INTEGER(w2f__i8) OpenAD_Symbol_41
      INTEGER(w2f__i8) OpenAD_Symbol_42
      INTEGER(w2f__i8) OpenAD_Symbol_43
      INTEGER(w2f__i8) OpenAD_Symbol_44
      INTEGER(w2f__i8) OpenAD_Symbol_45
      INTEGER(w2f__i8) OpenAD_Symbol_46
      INTEGER(w2f__i8) OpenAD_Symbol_47
      INTEGER(w2f__i8) OpenAD_Symbol_48
      INTEGER(w2f__i8) OpenAD_Symbol_49
      INTEGER(w2f__i8) OpenAD_Symbol_5
      INTEGER(w2f__i8) OpenAD_Symbol_50
      INTEGER(w2f__i8) OpenAD_Symbol_51
      INTEGER(w2f__i8) OpenAD_Symbol_52
      INTEGER(w2f__i8) OpenAD_Symbol_53
      INTEGER(w2f__i8) OpenAD_Symbol_54
      INTEGER(w2f__i8) OpenAD_Symbol_55
      INTEGER(w2f__i8) OpenAD_Symbol_56
      INTEGER(w2f__i8) OpenAD_Symbol_57
      INTEGER(w2f__i8) OpenAD_Symbol_58
      INTEGER(w2f__i8) OpenAD_Symbol_59
      INTEGER(w2f__i8) OpenAD_Symbol_6
      INTEGER(w2f__i8) OpenAD_Symbol_60
      INTEGER(w2f__i8) OpenAD_Symbol_61
      INTEGER(w2f__i8) OpenAD_Symbol_62
      INTEGER(w2f__i8) OpenAD_Symbol_63
      INTEGER(w2f__i8) OpenAD_Symbol_64
      INTEGER(w2f__i8) OpenAD_Symbol_65
      INTEGER(w2f__i8) OpenAD_Symbol_66
      INTEGER(w2f__i8) OpenAD_Symbol_67
      INTEGER(w2f__i8) OpenAD_Symbol_68
      INTEGER(w2f__i8) OpenAD_Symbol_69
      INTEGER(w2f__i8) OpenAD_Symbol_7
      INTEGER(w2f__i8) OpenAD_Symbol_70
      INTEGER(w2f__i8) OpenAD_Symbol_71
      INTEGER(w2f__i8) OpenAD_Symbol_72
      INTEGER(w2f__i8) OpenAD_Symbol_73
      INTEGER(w2f__i8) OpenAD_Symbol_74
      INTEGER(w2f__i8) OpenAD_Symbol_75
      INTEGER(w2f__i8) OpenAD_Symbol_76
      INTEGER(w2f__i8) OpenAD_Symbol_77
      INTEGER(w2f__i8) OpenAD_Symbol_78
      INTEGER(w2f__i8) OpenAD_Symbol_79
      INTEGER(w2f__i8) OpenAD_Symbol_8
      INTEGER(w2f__i8) OpenAD_Symbol_80
      INTEGER(w2f__i8) OpenAD_Symbol_81
      INTEGER(w2f__i8) OpenAD_Symbol_82
      INTEGER(w2f__i8) OpenAD_Symbol_83
      INTEGER(w2f__i8) OpenAD_Symbol_9
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
      TYPE (OpenADTy_active) DPDX
      TYPE (OpenADTy_active) DPDY
      REAL(w2f__8) FOUR
      PARAMETER ( FOUR = 4.0D00)
      REAL(w2f__8) HX
      REAL(w2f__8) HX2
      REAL(w2f__8) HY
      REAL(w2f__8) HY2
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) J
      INTEGER(w2f__i4) K
      INTEGER(w2f__i4) N
      REAL(w2f__8) NXP1
      REAL(w2f__8) NYP1
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
      INTEGER(w2f__i4) OpenAD_Symbol_100
      INTEGER(w2f__i4) OpenAD_Symbol_101
      INTEGER(w2f__i4) OpenAD_Symbol_102
      INTEGER(w2f__i4) OpenAD_Symbol_103
      INTEGER(w2f__i4) OpenAD_Symbol_104
      INTEGER(w2f__i4) OpenAD_Symbol_105
      REAL(w2f__8) OpenAD_Symbol_106
      REAL(w2f__8) OpenAD_Symbol_107
      REAL(w2f__8) OpenAD_Symbol_108
      REAL(w2f__8) OpenAD_Symbol_109
      REAL(w2f__8) OpenAD_Symbol_110
      REAL(w2f__8) OpenAD_Symbol_111
      REAL(w2f__8) OpenAD_Symbol_112
      REAL(w2f__8) OpenAD_Symbol_113
      REAL(w2f__8) OpenAD_Symbol_114
      REAL(w2f__8) OpenAD_Symbol_115
      REAL(w2f__8) OpenAD_Symbol_116
      REAL(w2f__8) OpenAD_Symbol_117
      INTEGER(w2f__i4) OpenAD_Symbol_118
      INTEGER(w2f__i4) OpenAD_Symbol_119
      INTEGER(w2f__i4) OpenAD_Symbol_120
      INTEGER(w2f__i4) OpenAD_Symbol_121
      INTEGER(w2f__i4) OpenAD_Symbol_122
      INTEGER(w2f__i4) OpenAD_Symbol_123
      INTEGER(w2f__i4) OpenAD_Symbol_124
      INTEGER(w2f__i4) OpenAD_Symbol_125
      INTEGER(w2f__i4) OpenAD_Symbol_126
      INTEGER(w2f__i4) OpenAD_Symbol_127
      INTEGER(w2f__i4) OpenAD_Symbol_128
      INTEGER(w2f__i4) OpenAD_Symbol_129
      INTEGER(w2f__i4) OpenAD_Symbol_130
      INTEGER(w2f__i4) OpenAD_Symbol_131
      INTEGER(w2f__i4) OpenAD_Symbol_132
      INTEGER(w2f__i4) OpenAD_Symbol_133
      INTEGER(w2f__i4) OpenAD_Symbol_84
      INTEGER(w2f__i4) OpenAD_Symbol_85
      INTEGER(w2f__i4) OpenAD_Symbol_86
      INTEGER(w2f__i4) OpenAD_Symbol_87
      INTEGER(w2f__i4) OpenAD_Symbol_88
      INTEGER(w2f__i4) OpenAD_Symbol_89
      INTEGER(w2f__i4) OpenAD_Symbol_90
      INTEGER(w2f__i4) OpenAD_Symbol_91
      INTEGER(w2f__i4) OpenAD_Symbol_92
      INTEGER(w2f__i4) OpenAD_Symbol_93
      INTEGER(w2f__i4) OpenAD_Symbol_94
      INTEGER(w2f__i4) OpenAD_Symbol_95
      INTEGER(w2f__i4) OpenAD_Symbol_96
      INTEGER(w2f__i4) OpenAD_Symbol_97
      INTEGER(w2f__i4) OpenAD_Symbol_98
      INTEGER(w2f__i4) OpenAD_Symbol_99
      REAL(w2f__8) OpenAD_acc_0
      REAL(w2f__8) OpenAD_acc_1
      REAL(w2f__8) OpenAD_acc_10
      REAL(w2f__8) OpenAD_acc_11
      REAL(w2f__8) OpenAD_acc_12
      REAL(w2f__8) OpenAD_acc_13
      REAL(w2f__8) OpenAD_acc_14
      REAL(w2f__8) OpenAD_acc_15
      REAL(w2f__8) OpenAD_acc_16
      REAL(w2f__8) OpenAD_acc_17
      REAL(w2f__8) OpenAD_acc_18
      REAL(w2f__8) OpenAD_acc_19
      REAL(w2f__8) OpenAD_acc_2
      REAL(w2f__8) OpenAD_acc_20
      REAL(w2f__8) OpenAD_acc_3
      REAL(w2f__8) OpenAD_acc_4
      REAL(w2f__8) OpenAD_acc_5
      REAL(w2f__8) OpenAD_acc_6
      REAL(w2f__8) OpenAD_acc_7
      REAL(w2f__8) OpenAD_acc_8
      REAL(w2f__8) OpenAD_acc_9
      REAL(w2f__8) OpenAD_aux_0
      REAL(w2f__8) OpenAD_aux_1
      REAL(w2f__8) OpenAD_aux_10
      REAL(w2f__8) OpenAD_aux_11
      REAL(w2f__8) OpenAD_aux_12
      REAL(w2f__8) OpenAD_aux_13
      REAL(w2f__8) OpenAD_aux_14
      REAL(w2f__8) OpenAD_aux_15
      REAL(w2f__8) OpenAD_aux_16
      REAL(w2f__8) OpenAD_aux_17
      REAL(w2f__8) OpenAD_aux_18
      REAL(w2f__8) OpenAD_aux_19
      REAL(w2f__8) OpenAD_aux_2
      REAL(w2f__8) OpenAD_aux_20
      REAL(w2f__8) OpenAD_aux_21
      REAL(w2f__8) OpenAD_aux_22
      REAL(w2f__8) OpenAD_aux_3
      REAL(w2f__8) OpenAD_aux_4
      REAL(w2f__8) OpenAD_aux_5
      REAL(w2f__8) OpenAD_aux_6
      REAL(w2f__8) OpenAD_aux_7
      REAL(w2f__8) OpenAD_aux_8
      REAL(w2f__8) OpenAD_aux_9
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_10
      REAL(w2f__8) OpenAD_lin_11
      REAL(w2f__8) OpenAD_lin_12
      REAL(w2f__8) OpenAD_lin_13
      REAL(w2f__8) OpenAD_lin_14
      REAL(w2f__8) OpenAD_lin_15
      REAL(w2f__8) OpenAD_lin_16
      REAL(w2f__8) OpenAD_lin_17
      REAL(w2f__8) OpenAD_lin_18
      REAL(w2f__8) OpenAD_lin_19
      REAL(w2f__8) OpenAD_lin_2
      REAL(w2f__8) OpenAD_lin_20
      REAL(w2f__8) OpenAD_lin_3
      REAL(w2f__8) OpenAD_lin_4
      REAL(w2f__8) OpenAD_lin_5
      REAL(w2f__8) OpenAD_lin_6
      REAL(w2f__8) OpenAD_lin_7
      REAL(w2f__8) OpenAD_lin_8
      REAL(w2f__8) OpenAD_lin_9
      TYPE (OpenADTy_active) OpenAD_prp_0
      TYPE (OpenADTy_active) OpenAD_prp_1
      TYPE (OpenADTy_active) OpenAD_prp_10
      TYPE (OpenADTy_active) OpenAD_prp_11
      TYPE (OpenADTy_active) OpenAD_prp_2
      TYPE (OpenADTy_active) OpenAD_prp_3
      TYPE (OpenADTy_active) OpenAD_prp_4
      TYPE (OpenADTy_active) OpenAD_prp_5
      TYPE (OpenADTy_active) OpenAD_prp_6
      TYPE (OpenADTy_active) OpenAD_prp_7
      TYPE (OpenADTy_active) OpenAD_prp_8
      TYPE (OpenADTy_active) OpenAD_prp_9
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
      N = (NX * NY)
      NXP1 = (NX + 1)
      NYP1 = (NY + 1)
      HX = (1.0D00 / NXP1)
      HY = (1.0D00 / NYP1)
      HY2 = (HY * HY)
      HX2 = (HX * HX)
      DO I = 1, NY, 1
        DO J = 1, NX, 1
          K = (J + NX *(I +(-1)))
          IF((I .eq. 1) .OR.(J .eq. 1)) THEN
            __value__(PBL) = 0.0D00
          ELSE
            __value__(PBL) = __value__(X(K - NX + (-1)))
          ENDIF
          IF(I .eq. 1) THEN
            __value__(PB) = 0.0D00
            __value__(PBB) = __value__(X(K))
          ELSE
            IF(I .eq. 2) THEN
              __value__(PB) = __value__(X(K - NX))
              __value__(PBB) = 0.0D00
            ELSE
              __value__(PB) = __value__(X(K - NX))
              __value__(PBB) = __value__(X(K - NX * 2))
            ENDIF
          ENDIF
          IF((NX .eq. J) .OR.(I .eq. 1)) THEN
            __value__(PBR) = 0.0D00
          ELSE
            __value__(PBR) = __value__(X(K - NX + 1))
          ENDIF
          IF(J .eq. 1) THEN
            __value__(PL) = 0.0D00
            __value__(PLL) = __value__(X(K))
          ELSE
            IF(J .eq. 2) THEN
              __value__(PL) = __value__(X(K + (-1)))
              __value__(PLL) = 0.0D00
            ELSE
              __value__(PL) = __value__(X(K + (-1)))
              __value__(PLL) = __value__(X(K + (-2)))
            ENDIF
          ENDIF
          __value__(P) = __value__(X(K))
          IF(J .eq.(NX +(-1))) THEN
            __value__(PR) = __value__(X(K + 1))
            __value__(PRR) = 0.0D00
          ELSE
            IF(NX .eq. J) THEN
              __value__(PR) = 0.0D00
              __value__(PRR) = __value__(X(K))
            ELSE
              __value__(PR) = __value__(X(K + 1))
              __value__(PRR) = __value__(X(K + 2))
            ENDIF
          ENDIF
          IF((NY .eq. I) .OR.(J .eq. 1)) THEN
            __value__(PTL) = 0.0D00
          ELSE
            __value__(PTL) = __value__(X(NX + K + (-1)))
          ENDIF
          IF(I .eq.(NY +(-1))) THEN
            __value__(PT) = __value__(X(NX + K))
            __value__(PTT) = 0.0D00
          ELSE
            IF(NY .eq. I) THEN
              __value__(PT) = 0.0D00
              __value__(PTT) = __value__(X(K))
            ELSE
              __value__(PT) = __value__(X(NX + K))
              __value__(PTT) = __value__(X(K + NX * 2))
            ENDIF
          ENDIF
          IF((NX .eq. J) .OR.(NY .eq. I)) THEN
            __value__(PTR) = 0.0D00
          ELSE
            __value__(PTR) = __value__(X(NX + K + 1))
          ENDIF
          __value__(DPDY) = ((__value__(PT) - __value__(PB)) /(HY *
     >  2.0D00))
          __value__(DPDX) = ((__value__(PR) - __value__(PL)) /(HX *
     >  2.0D00))
          __value__(PBLAP) = (((__value__(PBB) + __value__(P) -
     >  __value__(PB) * 2.0D00) / HY2) +((__value__(PBL) + __value__(
     > PBR) - __value__(PB) * 2.0D00) / HX2))
          __value__(PLLAP) = (((__value__(PBL) + __value__(PTL) -
     >  __value__(PL) * 2.0D00) / HY2) +((__value__(PLL) + __value__(P)
     >  - __value__(PL) * 2.0D00) / HX2))
          __value__(PLAP) = (((__value__(PB) + __value__(PT) -
     >  __value__(P) * 2.0D00) / HY2) +((__value__(PL) + __value__(PR)
     >  - __value__(P) * 2.0D00) / HX2))
          __value__(PRLAP) = (((__value__(P) + __value__(PRR) -
     >  __value__(PR) * 2.0D00) / HX2) +((__value__(PBR) + __value__(
     > PTR) - __value__(PR) * 2.0D00) / HY2))
          __value__(PTLAP) = (((__value__(P) + __value__(PTT) -
     >  __value__(PT) * 2.0D00) / HY2) +((__value__(PTL) + __value__(
     > PTR) - __value__(PT) * 2.0D00) / HX2))
          __value__(FVEC(INT(K))) = (((__value__(PBLAP) + __value__(
     > PTLAP) - __value__(PLAP) * 2.0D00) / HY2) +((__value__(PLLAP) +
     >  __value__(PRLAP) - __value__(PLAP) * 2.0D00) / HX2) - R *(((
     > __value__(DPDY) *(__value__(PRLAP) - __value__(PLLAP))) /(HX *
     >  2.0D00)) -((__value__(DPDX) *(__value__(PTLAP) - __value__(
     > PBLAP))) /(HY * 2.0D00))))
        END DO
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      N = (NX * NY)
      NXP1 = (NX + 1)
      NYP1 = (NY + 1)
      HX = (1.0D00 / NXP1)
      HY = (1.0D00 / NYP1)
      HY2 = (HY * HY)
      HX2 = (HX * HX)
      OpenAD_Symbol_16 = 0_w2f__i8
      DO I = 1, NY, 1
        OpenAD_Symbol_17 = 0_w2f__i8
        DO J = 1, NX, 1
          K = (J + NX *(I +(-1)))
          IF((I .eq. 1) .OR.(J .eq. 1)) THEN
            __value__(PBL) = 0.0D00
            OpenAD_Symbol_18 = 1_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_18)
          ELSE
            __value__(PBL) = __value__(X(K - NX + (-1)))
            OpenAD_Symbol_99 = (K - NX +(-1))
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_99)
            OpenAD_Symbol_19 = 0_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_19)
          ENDIF
          IF(I .eq. 1) THEN
            __value__(PB) = 0.0D00
            __value__(PBB) = __value__(X(K))
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(K)
            OpenAD_Symbol_22 = 1_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_22)
          ELSE
            IF(I .eq. 2) THEN
              __value__(PB) = __value__(X(K - NX))
              __value__(PBB) = 0.0D00
              OpenAD_Symbol_96 = (K - NX)
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_96)
              OpenAD_Symbol_20 = 1_w2f__i8
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_20)
            ELSE
              __value__(PB) = __value__(X(K - NX))
              __value__(PBB) = __value__(X(K - NX * 2))
              OpenAD_Symbol_97 = (K - NX)
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_97)
              OpenAD_Symbol_98 = (K - NX * 2)
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_98)
              OpenAD_Symbol_21 = 0_w2f__i8
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_21)
            ENDIF
            OpenAD_Symbol_23 = 0_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_23)
          ENDIF
          IF((NX .eq. J) .OR.(I .eq. 1)) THEN
            __value__(PBR) = 0.0D00
            OpenAD_Symbol_24 = 1_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_24)
          ELSE
            __value__(PBR) = __value__(X(K - NX + 1))
            OpenAD_Symbol_95 = (K - NX + 1)
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_95)
            OpenAD_Symbol_25 = 0_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_25)
          ENDIF
          IF(J .eq. 1) THEN
            __value__(PL) = 0.0D00
            __value__(PLL) = __value__(X(K))
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(K)
            OpenAD_Symbol_28 = 1_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_28)
          ELSE
            IF(J .eq. 2) THEN
              __value__(PL) = __value__(X(K + (-1)))
              __value__(PLL) = 0.0D00
              OpenAD_Symbol_92 = (K +(-1))
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_92)
              OpenAD_Symbol_26 = 1_w2f__i8
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_26)
            ELSE
              __value__(PL) = __value__(X(K + (-1)))
              __value__(PLL) = __value__(X(K + (-2)))
              OpenAD_Symbol_93 = (K +(-1))
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_93)
              OpenAD_Symbol_94 = (K +(-2))
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_94)
              OpenAD_Symbol_27 = 0_w2f__i8
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_27)
            ENDIF
            OpenAD_Symbol_29 = 0_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_29)
          ENDIF
          __value__(P) = __value__(X(K))
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(K)
          IF(J .eq.(NX +(-1))) THEN
            __value__(PR) = __value__(X(K + 1))
            __value__(PRR) = 0.0D00
            OpenAD_Symbol_84 = (K + 1)
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_84)
            OpenAD_Symbol_32 = 1_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_32)
          ELSE
            IF(NX .eq. J) THEN
              __value__(PR) = 0.0D00
              __value__(PRR) = __value__(X(K))
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(K)
              OpenAD_Symbol_30 = 1_w2f__i8
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_30)
            ELSE
              __value__(PR) = __value__(X(K + 1))
              __value__(PRR) = __value__(X(K + 2))
              OpenAD_Symbol_90 = (K + 1)
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_90)
              OpenAD_Symbol_91 = (K + 2)
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_91)
              OpenAD_Symbol_31 = 0_w2f__i8
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_31)
            ENDIF
            OpenAD_Symbol_33 = 0_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_33)
          ENDIF
          IF((NY .eq. I) .OR.(J .eq. 1)) THEN
            __value__(PTL) = 0.0D00
            OpenAD_Symbol_34 = 1_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_34)
          ELSE
            __value__(PTL) = __value__(X(NX + K + (-1)))
            OpenAD_Symbol_89 = (NX + K +(-1))
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_89)
            OpenAD_Symbol_35 = 0_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_35)
          ENDIF
          IF(I .eq.(NY +(-1))) THEN
            __value__(PT) = __value__(X(NX + K))
            __value__(PTT) = 0.0D00
            OpenAD_Symbol_85 = (NX + K)
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_85)
            OpenAD_Symbol_38 = 1_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_38)
          ELSE
            IF(NY .eq. I) THEN
              __value__(PT) = 0.0D00
              __value__(PTT) = __value__(X(K))
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(K)
              OpenAD_Symbol_36 = 1_w2f__i8
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_36)
            ELSE
              __value__(PT) = __value__(X(NX + K))
              __value__(PTT) = __value__(X(K + NX * 2))
              OpenAD_Symbol_87 = (NX + K)
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_87)
              OpenAD_Symbol_88 = (K + NX * 2)
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_88)
              OpenAD_Symbol_37 = 0_w2f__i8
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_37)
            ENDIF
            OpenAD_Symbol_39 = 0_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_39)
          ENDIF
          IF((NX .eq. J) .OR.(NY .eq. I)) THEN
            __value__(PTR) = 0.0D00
            OpenAD_Symbol_40 = 1_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_40)
          ELSE
            __value__(PTR) = __value__(X(NX + K + 1))
            OpenAD_Symbol_86 = (NX + K + 1)
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_86)
            OpenAD_Symbol_41 = 0_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_41)
          ENDIF
          OpenAD_aux_0 = (__value__(PT) - __value__(PB))
          OpenAD_aux_1 = (HY * 2.0D00)
          OpenAD_lin_0 = (INT(1_w2f__i8) / OpenAD_aux_1)
          __value__(DPDY) = (OpenAD_aux_0 / OpenAD_aux_1)
          OpenAD_aux_2 = (__value__(PR) - __value__(PL))
          OpenAD_aux_3 = (HX * 2.0D00)
          OpenAD_lin_1 = (INT(1_w2f__i8) / OpenAD_aux_3)
          __value__(DPDX) = (OpenAD_aux_2 / OpenAD_aux_3)
          OpenAD_aux_4 = (__value__(PBB) + __value__(P) - __value__(PB)
     >  * 2.0D00)
          OpenAD_aux_5 = (__value__(PBL) + __value__(PBR) - __value__(
     > PB) * 2.0D00)
          OpenAD_lin_2 = (INT(1_w2f__i8) / HY2)
          OpenAD_lin_3 = (INT(1_w2f__i8) / HX2)
          __value__(PBLAP) = ((OpenAD_aux_4 / HY2) +(OpenAD_aux_5 / HX2
     > ))
          OpenAD_aux_6 = (__value__(PBL) + __value__(PTL) - __value__(
     > PL) * 2.0D00)
          OpenAD_aux_7 = (__value__(PLL) + __value__(P) - __value__(PL)
     >  * 2.0D00)
          OpenAD_lin_4 = (INT(1_w2f__i8) / HY2)
          OpenAD_lin_5 = (INT(1_w2f__i8) / HX2)
          __value__(PLLAP) = ((OpenAD_aux_6 / HY2) +(OpenAD_aux_7 / HX2
     > ))
          OpenAD_aux_8 = (__value__(PB) + __value__(PT) - __value__(P)
     >  * 2.0D00)
          OpenAD_aux_9 = (__value__(PL) + __value__(PR) - __value__(P)
     >  * 2.0D00)
          OpenAD_lin_6 = (INT(1_w2f__i8) / HY2)
          OpenAD_lin_7 = (INT(1_w2f__i8) / HX2)
          __value__(PLAP) = ((OpenAD_aux_8 / HY2) +(OpenAD_aux_9 / HX2)
     > )
          OpenAD_aux_10 = (__value__(P) + __value__(PRR) - __value__(PR
     > ) * 2.0D00)
          OpenAD_aux_11 = (__value__(PBR) + __value__(PTR) - __value__(
     > PR) * 2.0D00)
          OpenAD_lin_8 = (INT(1_w2f__i8) / HX2)
          OpenAD_lin_9 = (INT(1_w2f__i8) / HY2)
          __value__(PRLAP) = ((OpenAD_aux_10 / HX2) +(OpenAD_aux_11 /
     >  HY2))
          OpenAD_aux_12 = (__value__(P) + __value__(PTT) - __value__(PT
     > ) * 2.0D00)
          OpenAD_aux_13 = (__value__(PTL) + __value__(PTR) - __value__(
     > PT) * 2.0D00)
          OpenAD_lin_10 = (INT(1_w2f__i8) / HY2)
          OpenAD_lin_11 = (INT(1_w2f__i8) / HX2)
          __value__(PTLAP) = ((OpenAD_aux_12 / HY2) +(OpenAD_aux_13 /
     >  HX2))
          OpenAD_aux_14 = (__value__(PBLAP) + __value__(PTLAP) -
     >  __value__(PLAP) * 2.0D00)
          OpenAD_aux_15 = (__value__(PLLAP) + __value__(PRLAP) -
     >  __value__(PLAP) * 2.0D00)
          OpenAD_aux_19 = (__value__(PRLAP) - __value__(PLLAP))
          OpenAD_aux_17 = (__value__(DPDY) * OpenAD_aux_19)
          OpenAD_aux_18 = (HX * 2.0D00)
          OpenAD_aux_22 = (__value__(PTLAP) - __value__(PBLAP))
          OpenAD_aux_20 = (__value__(DPDX) * OpenAD_aux_22)
          OpenAD_aux_21 = (HY * 2.0D00)
          OpenAD_aux_16 = ((OpenAD_aux_17 / OpenAD_aux_18) -(
     > OpenAD_aux_20 / OpenAD_aux_21))
          OpenAD_lin_12 = (INT(1_w2f__i8) / HY2)
          OpenAD_lin_13 = (INT(1_w2f__i8) / HX2)
          OpenAD_lin_16 = OpenAD_aux_19
          OpenAD_lin_17 = __value__(DPDY)
          OpenAD_lin_15 = (INT(1_w2f__i8) / OpenAD_aux_18)
          OpenAD_lin_19 = OpenAD_aux_22
          OpenAD_lin_20 = __value__(DPDX)
          OpenAD_lin_18 = (INT(1_w2f__i8) / OpenAD_aux_21)
          OpenAD_lin_14 = R
          __value__(FVEC(INT(K))) = ((OpenAD_aux_14 / HY2) +(
     > OpenAD_aux_15 / HX2) - R * OpenAD_aux_16)
          OpenAD_acc_0 = (OpenAD_lin_14 * INT((-1_w2f__i8)))
          OpenAD_acc_1 = (INT((-1_w2f__i8)) * OpenAD_acc_0)
          OpenAD_acc_2 = (OpenAD_lin_20 * OpenAD_lin_18 * OpenAD_acc_1)
          OpenAD_acc_3 = (OpenAD_lin_1 * OpenAD_lin_19 * OpenAD_lin_18
     >  * OpenAD_acc_1)
          OpenAD_acc_4 = (OpenAD_lin_17 * OpenAD_lin_15 * OpenAD_acc_0)
          OpenAD_acc_5 = (OpenAD_lin_0 * OpenAD_lin_16 * OpenAD_lin_15
     >  * OpenAD_acc_0)
          OpenAD_acc_6 = (OpenAD_lin_13 + OpenAD_acc_4)
          OpenAD_acc_7 = (OpenAD_lin_13 + INT((-1_w2f__i8)) *
     >  OpenAD_acc_4)
          OpenAD_acc_8 = (2.0D00 * INT((-1_w2f__i8)) * OpenAD_lin_13 +
     >  2.0D00 * INT((-1_w2f__i8)) * OpenAD_lin_12)
          OpenAD_acc_9 = (OpenAD_lin_12 + OpenAD_acc_2)
          OpenAD_acc_10 = (OpenAD_lin_12 + INT((-1_w2f__i8)) *
     >  OpenAD_acc_2)
          OpenAD_acc_11 = (OpenAD_lin_11 * OpenAD_acc_9)
          OpenAD_acc_12 = (OpenAD_lin_10 * OpenAD_acc_9)
          OpenAD_acc_13 = (OpenAD_lin_9 * OpenAD_acc_6)
          OpenAD_acc_14 = (OpenAD_lin_8 * OpenAD_acc_6)
          OpenAD_acc_15 = (OpenAD_lin_7 * OpenAD_acc_8)
          OpenAD_acc_16 = (OpenAD_lin_6 * OpenAD_acc_8)
          OpenAD_acc_17 = (OpenAD_lin_5 * OpenAD_acc_7)
          OpenAD_acc_18 = (OpenAD_lin_4 * OpenAD_acc_7)
          OpenAD_acc_19 = (OpenAD_lin_3 * OpenAD_acc_10)
          OpenAD_acc_20 = (OpenAD_lin_2 * OpenAD_acc_10)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_3)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_5)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_11)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_12)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_13)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_14)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_15)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_16)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_17)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_18)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_19)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_20)
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(K)
          OpenAD_Symbol_17 = (INT(OpenAD_Symbol_17) + INT(1_w2f__i8))
        END DO
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_17)
        OpenAD_Symbol_16 = (INT(OpenAD_Symbol_16) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_16)
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
          CALL pop_i_s0(OpenAD_Symbol_105)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_106)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_107)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_108)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_109)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_110)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_111)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_112)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_113)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_114)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_115)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_116)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_117)
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_106, __deriv__(FVEC(
     > OpenAD_Symbol_105)), __deriv__(OpenAD_prp_2))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_107, __deriv__(FVEC(
     > OpenAD_Symbol_105)), __deriv__(OpenAD_prp_3))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_108, __deriv__(FVEC(
     > OpenAD_Symbol_105)), __deriv__(OpenAD_prp_4))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_109, __deriv__(FVEC(
     > OpenAD_Symbol_105)), __deriv__(OpenAD_prp_5))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_110, __deriv__(FVEC(
     > OpenAD_Symbol_105)), __deriv__(OpenAD_prp_6))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_111, __deriv__(FVEC(
     > OpenAD_Symbol_105)), __deriv__(OpenAD_prp_7))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_112, __deriv__(FVEC(
     > OpenAD_Symbol_105)), __deriv__(OpenAD_prp_8))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_113, __deriv__(FVEC(
     > OpenAD_Symbol_105)), __deriv__(OpenAD_prp_9))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_114, __deriv__(FVEC(
     > OpenAD_Symbol_105)), __deriv__(OpenAD_prp_10))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_115, __deriv__(FVEC(
     > OpenAD_Symbol_105)), __deriv__(OpenAD_prp_11))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_116, __deriv__(FVEC(
     > OpenAD_Symbol_105)), __deriv__(OpenAD_prp_0))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_117, __deriv__(FVEC(
     > OpenAD_Symbol_105)), __deriv__(OpenAD_prp_1))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(FVEC(OpenAD_Symbol_105)))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(-2.0D00, __deriv__(OpenAD_prp_11), __deriv__(PT))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_11), __deriv__(PTR))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_11), __deriv__(PTL))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_11))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(-2.0D00, __deriv__(OpenAD_prp_10), __deriv__(PT))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_10), __deriv__(PTT))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_10), __deriv__(P))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_10))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(-2.0D00, __deriv__(OpenAD_prp_9), __deriv__(PR))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_9), __deriv__(PTR))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_9), __deriv__(PBR))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_9))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(-2.0D00, __deriv__(OpenAD_prp_8), __deriv__(PR))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_8), __deriv__(PRR))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_8), __deriv__(P))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_8))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(-2.0D00, __deriv__(OpenAD_prp_7), __deriv__(P))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_7), __deriv__(PR))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_7), __deriv__(PL))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_7))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(-2.0D00, __deriv__(OpenAD_prp_6), __deriv__(P))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_6), __deriv__(PT))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_6), __deriv__(PB))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_6))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(-2.0D00, __deriv__(OpenAD_prp_5), __deriv__(PL))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_5), __deriv__(P))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_5), __deriv__(PLL))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_5))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(-2.0D00, __deriv__(OpenAD_prp_4), __deriv__(PL))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_4), __deriv__(PTL))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_4), __deriv__(PBL))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_4))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(-2.0D00, __deriv__(OpenAD_prp_3), __deriv__(PB))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_3), __deriv__(PBR))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_3), __deriv__(PBL))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_3))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(-2.0D00, __deriv__(OpenAD_prp_2), __deriv__(PB))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_2), __deriv__(P))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_2), __deriv__(PBB))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_2))
C         $OpenAD$ INLINE DecDeriv(subst,subst)
          CALL DecDeriv(__deriv__(OpenAD_prp_1), __deriv__(PL))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_1), __deriv__(PR))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_1))
C         $OpenAD$ INLINE DecDeriv(subst,subst)
          CALL DecDeriv(__deriv__(OpenAD_prp_0), __deriv__(PB))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(PT))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_4)
          IF(OpenAD_Symbol_4 .ne. 0) THEN
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(PTR))
          ELSE
C           $OpenAD$ INLINE pop_i_s0(subst)
            CALL pop_i_s0(OpenAD_Symbol_118)
C           $OpenAD$ INLINE IncDeriv(subst,subst)
            CALL IncDeriv(__deriv__(PTR), __deriv__(X(OpenAD_Symbol_118
     > )))
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(PTR))
          ENDIF
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_5)
          IF(OpenAD_Symbol_5 .ne. 0) THEN
C           $OpenAD$ INLINE pop_i_s0(subst)
            CALL pop_i_s0(OpenAD_Symbol_104)
C           $OpenAD$ INLINE IncDeriv(subst,subst)
            CALL IncDeriv(__deriv__(PT), __deriv__(X(OpenAD_Symbol_104)
     > ))
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(PT))
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(PTT))
          ELSE
C           $OpenAD$ INLINE pop_i_s0(subst)
            CALL pop_i_s0(OpenAD_Symbol_6)
            IF(OpenAD_Symbol_6 .ne. 0) THEN
C             $OpenAD$ INLINE pop_i_s0(subst)
              CALL pop_i_s0(OpenAD_Symbol_119)
C             $OpenAD$ INLINE IncDeriv(subst,subst)
              CALL IncDeriv(__deriv__(PTT), __deriv__(X(
     > OpenAD_Symbol_119)))
C             $OpenAD$ INLINE ZeroDeriv(subst)
              CALL ZeroDeriv(__deriv__(PTT))
C             $OpenAD$ INLINE ZeroDeriv(subst)
              CALL ZeroDeriv(__deriv__(PT))
            ELSE
C             $OpenAD$ INLINE pop_i_s0(subst)
              CALL pop_i_s0(OpenAD_Symbol_120)
C             $OpenAD$ INLINE IncDeriv(subst,subst)
              CALL IncDeriv(__deriv__(PTT), __deriv__(X(
     > OpenAD_Symbol_120)))
C             $OpenAD$ INLINE ZeroDeriv(subst)
              CALL ZeroDeriv(__deriv__(PTT))
C             $OpenAD$ INLINE pop_i_s0(subst)
              CALL pop_i_s0(OpenAD_Symbol_121)
C             $OpenAD$ INLINE IncDeriv(subst,subst)
              CALL IncDeriv(__deriv__(PT), __deriv__(X(
     > OpenAD_Symbol_121)))
C             $OpenAD$ INLINE ZeroDeriv(subst)
              CALL ZeroDeriv(__deriv__(PT))
            ENDIF
          ENDIF
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_7)
          IF(OpenAD_Symbol_7 .ne. 0) THEN
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(PTL))
          ELSE
C           $OpenAD$ INLINE pop_i_s0(subst)
            CALL pop_i_s0(OpenAD_Symbol_122)
C           $OpenAD$ INLINE IncDeriv(subst,subst)
            CALL IncDeriv(__deriv__(PTL), __deriv__(X(OpenAD_Symbol_122
     > )))
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(PTL))
          ENDIF
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_8)
          IF(OpenAD_Symbol_8 .ne. 0) THEN
C           $OpenAD$ INLINE pop_i_s0(subst)
            CALL pop_i_s0(OpenAD_Symbol_103)
C           $OpenAD$ INLINE IncDeriv(subst,subst)
            CALL IncDeriv(__deriv__(PR), __deriv__(X(OpenAD_Symbol_103)
     > ))
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(PR))
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(PRR))
          ELSE
C           $OpenAD$ INLINE pop_i_s0(subst)
            CALL pop_i_s0(OpenAD_Symbol_9)
            IF(OpenAD_Symbol_9 .ne. 0) THEN
C             $OpenAD$ INLINE pop_i_s0(subst)
              CALL pop_i_s0(OpenAD_Symbol_123)
C             $OpenAD$ INLINE IncDeriv(subst,subst)
              CALL IncDeriv(__deriv__(PRR), __deriv__(X(
     > OpenAD_Symbol_123)))
C             $OpenAD$ INLINE ZeroDeriv(subst)
              CALL ZeroDeriv(__deriv__(PRR))
C             $OpenAD$ INLINE ZeroDeriv(subst)
              CALL ZeroDeriv(__deriv__(PR))
            ELSE
C             $OpenAD$ INLINE pop_i_s0(subst)
              CALL pop_i_s0(OpenAD_Symbol_124)
C             $OpenAD$ INLINE IncDeriv(subst,subst)
              CALL IncDeriv(__deriv__(PRR), __deriv__(X(
     > OpenAD_Symbol_124)))
C             $OpenAD$ INLINE ZeroDeriv(subst)
              CALL ZeroDeriv(__deriv__(PRR))
C             $OpenAD$ INLINE pop_i_s0(subst)
              CALL pop_i_s0(OpenAD_Symbol_125)
C             $OpenAD$ INLINE IncDeriv(subst,subst)
              CALL IncDeriv(__deriv__(PR), __deriv__(X(
     > OpenAD_Symbol_125)))
C             $OpenAD$ INLINE ZeroDeriv(subst)
              CALL ZeroDeriv(__deriv__(PR))
            ENDIF
          ENDIF
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_102)
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(P), __deriv__(X(OpenAD_Symbol_102)))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(P))
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_10)
          IF(OpenAD_Symbol_10 .ne. 0) THEN
C           $OpenAD$ INLINE pop_i_s0(subst)
            CALL pop_i_s0(OpenAD_Symbol_101)
C           $OpenAD$ INLINE IncDeriv(subst,subst)
            CALL IncDeriv(__deriv__(PLL), __deriv__(X(OpenAD_Symbol_101
     > )))
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(PLL))
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(PL))
          ELSE
C           $OpenAD$ INLINE pop_i_s0(subst)
            CALL pop_i_s0(OpenAD_Symbol_11)
            IF(OpenAD_Symbol_11 .ne. 0) THEN
C             $OpenAD$ INLINE pop_i_s0(subst)
              CALL pop_i_s0(OpenAD_Symbol_126)
C             $OpenAD$ INLINE IncDeriv(subst,subst)
              CALL IncDeriv(__deriv__(PL), __deriv__(X(
     > OpenAD_Symbol_126)))
C             $OpenAD$ INLINE ZeroDeriv(subst)
              CALL ZeroDeriv(__deriv__(PL))
C             $OpenAD$ INLINE ZeroDeriv(subst)
              CALL ZeroDeriv(__deriv__(PLL))
            ELSE
C             $OpenAD$ INLINE pop_i_s0(subst)
              CALL pop_i_s0(OpenAD_Symbol_127)
C             $OpenAD$ INLINE IncDeriv(subst,subst)
              CALL IncDeriv(__deriv__(PLL), __deriv__(X(
     > OpenAD_Symbol_127)))
C             $OpenAD$ INLINE ZeroDeriv(subst)
              CALL ZeroDeriv(__deriv__(PLL))
C             $OpenAD$ INLINE pop_i_s0(subst)
              CALL pop_i_s0(OpenAD_Symbol_128)
C             $OpenAD$ INLINE IncDeriv(subst,subst)
              CALL IncDeriv(__deriv__(PL), __deriv__(X(
     > OpenAD_Symbol_128)))
C             $OpenAD$ INLINE ZeroDeriv(subst)
              CALL ZeroDeriv(__deriv__(PL))
            ENDIF
          ENDIF
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_12)
          IF(OpenAD_Symbol_12 .ne. 0) THEN
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(PBR))
          ELSE
C           $OpenAD$ INLINE pop_i_s0(subst)
            CALL pop_i_s0(OpenAD_Symbol_129)
C           $OpenAD$ INLINE IncDeriv(subst,subst)
            CALL IncDeriv(__deriv__(PBR), __deriv__(X(OpenAD_Symbol_129
     > )))
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(PBR))
          ENDIF
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_13)
          IF(OpenAD_Symbol_13 .ne. 0) THEN
C           $OpenAD$ INLINE pop_i_s0(subst)
            CALL pop_i_s0(OpenAD_Symbol_100)
C           $OpenAD$ INLINE IncDeriv(subst,subst)
            CALL IncDeriv(__deriv__(PBB), __deriv__(X(OpenAD_Symbol_100
     > )))
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(PBB))
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(PB))
          ELSE
C           $OpenAD$ INLINE pop_i_s0(subst)
            CALL pop_i_s0(OpenAD_Symbol_14)
            IF(OpenAD_Symbol_14 .ne. 0) THEN
C             $OpenAD$ INLINE pop_i_s0(subst)
              CALL pop_i_s0(OpenAD_Symbol_130)
C             $OpenAD$ INLINE IncDeriv(subst,subst)
              CALL IncDeriv(__deriv__(PB), __deriv__(X(
     > OpenAD_Symbol_130)))
C             $OpenAD$ INLINE ZeroDeriv(subst)
              CALL ZeroDeriv(__deriv__(PB))
C             $OpenAD$ INLINE ZeroDeriv(subst)
              CALL ZeroDeriv(__deriv__(PBB))
            ELSE
C             $OpenAD$ INLINE pop_i_s0(subst)
              CALL pop_i_s0(OpenAD_Symbol_131)
C             $OpenAD$ INLINE IncDeriv(subst,subst)
              CALL IncDeriv(__deriv__(PBB), __deriv__(X(
     > OpenAD_Symbol_131)))
C             $OpenAD$ INLINE ZeroDeriv(subst)
              CALL ZeroDeriv(__deriv__(PBB))
C             $OpenAD$ INLINE pop_i_s0(subst)
              CALL pop_i_s0(OpenAD_Symbol_132)
C             $OpenAD$ INLINE IncDeriv(subst,subst)
              CALL IncDeriv(__deriv__(PB), __deriv__(X(
     > OpenAD_Symbol_132)))
C             $OpenAD$ INLINE ZeroDeriv(subst)
              CALL ZeroDeriv(__deriv__(PB))
            ENDIF
          ENDIF
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_15)
          IF(OpenAD_Symbol_15 .ne. 0) THEN
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(PBL))
          ELSE
C           $OpenAD$ INLINE pop_i_s0(subst)
            CALL pop_i_s0(OpenAD_Symbol_133)
C           $OpenAD$ INLINE IncDeriv(subst,subst)
            CALL IncDeriv(__deriv__(PBL), __deriv__(X(OpenAD_Symbol_133
     > )))
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(PBL))
          ENDIF
          OpenAD_Symbol_3 = INT(OpenAD_Symbol_3) + 1
        END DO
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
      N = (NX * NY)
      NXP1 = (NX + 1)
      NYP1 = (NY + 1)
      HX = (1.0D00 / NXP1)
      HY = (1.0D00 / NYP1)
      HY2 = (HY * HY)
      HX2 = (HX * HX)
      OpenAD_Symbol_58 = 0_w2f__i8
      DO I = 1, NY, 1
        OpenAD_Symbol_59 = 0_w2f__i8
        DO J = 1, NX, 1
          K = (J + NX *(I +(-1)))
          IF((I .eq. 1) .OR.(J .eq. 1)) THEN
            __value__(PBL) = 0.0D00
            OpenAD_Symbol_60 = 1_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_60)
          ELSE
            __value__(PBL) = __value__(X(K - NX + (-1)))
            OpenAD_Symbol_99 = (K - NX +(-1))
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_99)
            OpenAD_Symbol_61 = 0_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_61)
          ENDIF
          IF(I .eq. 1) THEN
            __value__(PB) = 0.0D00
            __value__(PBB) = __value__(X(K))
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(K)
            OpenAD_Symbol_64 = 1_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_64)
          ELSE
            IF(I .eq. 2) THEN
              __value__(PB) = __value__(X(K - NX))
              __value__(PBB) = 0.0D00
              OpenAD_Symbol_96 = (K - NX)
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_96)
              OpenAD_Symbol_62 = 1_w2f__i8
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_62)
            ELSE
              __value__(PB) = __value__(X(K - NX))
              __value__(PBB) = __value__(X(K - NX * 2))
              OpenAD_Symbol_97 = (K - NX)
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_97)
              OpenAD_Symbol_98 = (K - NX * 2)
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_98)
              OpenAD_Symbol_63 = 0_w2f__i8
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_63)
            ENDIF
            OpenAD_Symbol_65 = 0_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_65)
          ENDIF
          IF((NX .eq. J) .OR.(I .eq. 1)) THEN
            __value__(PBR) = 0.0D00
            OpenAD_Symbol_66 = 1_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_66)
          ELSE
            __value__(PBR) = __value__(X(K - NX + 1))
            OpenAD_Symbol_95 = (K - NX + 1)
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_95)
            OpenAD_Symbol_67 = 0_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_67)
          ENDIF
          IF(J .eq. 1) THEN
            __value__(PL) = 0.0D00
            __value__(PLL) = __value__(X(K))
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(K)
            OpenAD_Symbol_70 = 1_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_70)
          ELSE
            IF(J .eq. 2) THEN
              __value__(PL) = __value__(X(K + (-1)))
              __value__(PLL) = 0.0D00
              OpenAD_Symbol_92 = (K +(-1))
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_92)
              OpenAD_Symbol_68 = 1_w2f__i8
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_68)
            ELSE
              __value__(PL) = __value__(X(K + (-1)))
              __value__(PLL) = __value__(X(K + (-2)))
              OpenAD_Symbol_93 = (K +(-1))
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_93)
              OpenAD_Symbol_94 = (K +(-2))
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_94)
              OpenAD_Symbol_69 = 0_w2f__i8
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_69)
            ENDIF
            OpenAD_Symbol_71 = 0_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_71)
          ENDIF
          __value__(P) = __value__(X(K))
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(K)
          IF(J .eq.(NX +(-1))) THEN
            __value__(PR) = __value__(X(K + 1))
            __value__(PRR) = 0.0D00
            OpenAD_Symbol_84 = (K + 1)
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_84)
            OpenAD_Symbol_74 = 1_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_74)
          ELSE
            IF(NX .eq. J) THEN
              __value__(PR) = 0.0D00
              __value__(PRR) = __value__(X(K))
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(K)
              OpenAD_Symbol_72 = 1_w2f__i8
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_72)
            ELSE
              __value__(PR) = __value__(X(K + 1))
              __value__(PRR) = __value__(X(K + 2))
              OpenAD_Symbol_90 = (K + 1)
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_90)
              OpenAD_Symbol_91 = (K + 2)
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_91)
              OpenAD_Symbol_73 = 0_w2f__i8
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_73)
            ENDIF
            OpenAD_Symbol_75 = 0_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_75)
          ENDIF
          IF((NY .eq. I) .OR.(J .eq. 1)) THEN
            __value__(PTL) = 0.0D00
            OpenAD_Symbol_76 = 1_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_76)
          ELSE
            __value__(PTL) = __value__(X(NX + K + (-1)))
            OpenAD_Symbol_89 = (NX + K +(-1))
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_89)
            OpenAD_Symbol_77 = 0_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_77)
          ENDIF
          IF(I .eq.(NY +(-1))) THEN
            __value__(PT) = __value__(X(NX + K))
            __value__(PTT) = 0.0D00
            OpenAD_Symbol_85 = (NX + K)
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_85)
            OpenAD_Symbol_80 = 1_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_80)
          ELSE
            IF(NY .eq. I) THEN
              __value__(PT) = 0.0D00
              __value__(PTT) = __value__(X(K))
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(K)
              OpenAD_Symbol_78 = 1_w2f__i8
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_78)
            ELSE
              __value__(PT) = __value__(X(NX + K))
              __value__(PTT) = __value__(X(K + NX * 2))
              OpenAD_Symbol_87 = (NX + K)
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_87)
              OpenAD_Symbol_88 = (K + NX * 2)
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_88)
              OpenAD_Symbol_79 = 0_w2f__i8
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_79)
            ENDIF
            OpenAD_Symbol_81 = 0_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_81)
          ENDIF
          IF((NX .eq. J) .OR.(NY .eq. I)) THEN
            __value__(PTR) = 0.0D00
            OpenAD_Symbol_82 = 1_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_82)
          ELSE
            __value__(PTR) = __value__(X(NX + K + 1))
            OpenAD_Symbol_86 = (NX + K + 1)
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_86)
            OpenAD_Symbol_83 = 0_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_83)
          ENDIF
          OpenAD_aux_0 = (__value__(PT) - __value__(PB))
          OpenAD_aux_1 = (HY * 2.0D00)
          OpenAD_lin_0 = (INT(1_w2f__i8) / OpenAD_aux_1)
          __value__(DPDY) = (OpenAD_aux_0 / OpenAD_aux_1)
          OpenAD_aux_2 = (__value__(PR) - __value__(PL))
          OpenAD_aux_3 = (HX * 2.0D00)
          OpenAD_lin_1 = (INT(1_w2f__i8) / OpenAD_aux_3)
          __value__(DPDX) = (OpenAD_aux_2 / OpenAD_aux_3)
          OpenAD_aux_4 = (__value__(PBB) + __value__(P) - __value__(PB)
     >  * 2.0D00)
          OpenAD_aux_5 = (__value__(PBL) + __value__(PBR) - __value__(
     > PB) * 2.0D00)
          OpenAD_lin_2 = (INT(1_w2f__i8) / HY2)
          OpenAD_lin_3 = (INT(1_w2f__i8) / HX2)
          __value__(PBLAP) = ((OpenAD_aux_4 / HY2) +(OpenAD_aux_5 / HX2
     > ))
          OpenAD_aux_6 = (__value__(PBL) + __value__(PTL) - __value__(
     > PL) * 2.0D00)
          OpenAD_aux_7 = (__value__(PLL) + __value__(P) - __value__(PL)
     >  * 2.0D00)
          OpenAD_lin_4 = (INT(1_w2f__i8) / HY2)
          OpenAD_lin_5 = (INT(1_w2f__i8) / HX2)
          __value__(PLLAP) = ((OpenAD_aux_6 / HY2) +(OpenAD_aux_7 / HX2
     > ))
          OpenAD_aux_8 = (__value__(PB) + __value__(PT) - __value__(P)
     >  * 2.0D00)
          OpenAD_aux_9 = (__value__(PL) + __value__(PR) - __value__(P)
     >  * 2.0D00)
          OpenAD_lin_6 = (INT(1_w2f__i8) / HY2)
          OpenAD_lin_7 = (INT(1_w2f__i8) / HX2)
          __value__(PLAP) = ((OpenAD_aux_8 / HY2) +(OpenAD_aux_9 / HX2)
     > )
          OpenAD_aux_10 = (__value__(P) + __value__(PRR) - __value__(PR
     > ) * 2.0D00)
          OpenAD_aux_11 = (__value__(PBR) + __value__(PTR) - __value__(
     > PR) * 2.0D00)
          OpenAD_lin_8 = (INT(1_w2f__i8) / HX2)
          OpenAD_lin_9 = (INT(1_w2f__i8) / HY2)
          __value__(PRLAP) = ((OpenAD_aux_10 / HX2) +(OpenAD_aux_11 /
     >  HY2))
          OpenAD_aux_12 = (__value__(P) + __value__(PTT) - __value__(PT
     > ) * 2.0D00)
          OpenAD_aux_13 = (__value__(PTL) + __value__(PTR) - __value__(
     > PT) * 2.0D00)
          OpenAD_lin_10 = (INT(1_w2f__i8) / HY2)
          OpenAD_lin_11 = (INT(1_w2f__i8) / HX2)
          __value__(PTLAP) = ((OpenAD_aux_12 / HY2) +(OpenAD_aux_13 /
     >  HX2))
          OpenAD_aux_14 = (__value__(PBLAP) + __value__(PTLAP) -
     >  __value__(PLAP) * 2.0D00)
          OpenAD_aux_15 = (__value__(PLLAP) + __value__(PRLAP) -
     >  __value__(PLAP) * 2.0D00)
          OpenAD_aux_19 = (__value__(PRLAP) - __value__(PLLAP))
          OpenAD_aux_17 = (__value__(DPDY) * OpenAD_aux_19)
          OpenAD_aux_18 = (HX * 2.0D00)
          OpenAD_aux_22 = (__value__(PTLAP) - __value__(PBLAP))
          OpenAD_aux_20 = (__value__(DPDX) * OpenAD_aux_22)
          OpenAD_aux_21 = (HY * 2.0D00)
          OpenAD_aux_16 = ((OpenAD_aux_17 / OpenAD_aux_18) -(
     > OpenAD_aux_20 / OpenAD_aux_21))
          OpenAD_lin_12 = (INT(1_w2f__i8) / HY2)
          OpenAD_lin_13 = (INT(1_w2f__i8) / HX2)
          OpenAD_lin_16 = OpenAD_aux_19
          OpenAD_lin_17 = __value__(DPDY)
          OpenAD_lin_15 = (INT(1_w2f__i8) / OpenAD_aux_18)
          OpenAD_lin_19 = OpenAD_aux_22
          OpenAD_lin_20 = __value__(DPDX)
          OpenAD_lin_18 = (INT(1_w2f__i8) / OpenAD_aux_21)
          OpenAD_lin_14 = R
          __value__(FVEC(INT(K))) = ((OpenAD_aux_14 / HY2) +(
     > OpenAD_aux_15 / HX2) - R * OpenAD_aux_16)
          OpenAD_acc_0 = (OpenAD_lin_14 * INT((-1_w2f__i8)))
          OpenAD_acc_1 = (INT((-1_w2f__i8)) * OpenAD_acc_0)
          OpenAD_acc_2 = (OpenAD_lin_20 * OpenAD_lin_18 * OpenAD_acc_1)
          OpenAD_acc_3 = (OpenAD_lin_1 * OpenAD_lin_19 * OpenAD_lin_18
     >  * OpenAD_acc_1)
          OpenAD_acc_4 = (OpenAD_lin_17 * OpenAD_lin_15 * OpenAD_acc_0)
          OpenAD_acc_5 = (OpenAD_lin_0 * OpenAD_lin_16 * OpenAD_lin_15
     >  * OpenAD_acc_0)
          OpenAD_acc_6 = (OpenAD_lin_13 + OpenAD_acc_4)
          OpenAD_acc_7 = (OpenAD_lin_13 + INT((-1_w2f__i8)) *
     >  OpenAD_acc_4)
          OpenAD_acc_8 = (2.0D00 * INT((-1_w2f__i8)) * OpenAD_lin_13 +
     >  2.0D00 * INT((-1_w2f__i8)) * OpenAD_lin_12)
          OpenAD_acc_9 = (OpenAD_lin_12 + OpenAD_acc_2)
          OpenAD_acc_10 = (OpenAD_lin_12 + INT((-1_w2f__i8)) *
     >  OpenAD_acc_2)
          OpenAD_acc_11 = (OpenAD_lin_11 * OpenAD_acc_9)
          OpenAD_acc_12 = (OpenAD_lin_10 * OpenAD_acc_9)
          OpenAD_acc_13 = (OpenAD_lin_9 * OpenAD_acc_6)
          OpenAD_acc_14 = (OpenAD_lin_8 * OpenAD_acc_6)
          OpenAD_acc_15 = (OpenAD_lin_7 * OpenAD_acc_8)
          OpenAD_acc_16 = (OpenAD_lin_6 * OpenAD_acc_8)
          OpenAD_acc_17 = (OpenAD_lin_5 * OpenAD_acc_7)
          OpenAD_acc_18 = (OpenAD_lin_4 * OpenAD_acc_7)
          OpenAD_acc_19 = (OpenAD_lin_3 * OpenAD_acc_10)
          OpenAD_acc_20 = (OpenAD_lin_2 * OpenAD_acc_10)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_3)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_5)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_11)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_12)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_13)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_14)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_15)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_16)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_17)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_18)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_19)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_20)
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(K)
          OpenAD_Symbol_59 = (INT(OpenAD_Symbol_59) + INT(1_w2f__i8))
        END DO
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_59)
        OpenAD_Symbol_58 = (INT(OpenAD_Symbol_58) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_58)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_42)
      OpenAD_Symbol_43 = 1
      DO WHILE(INT(OpenAD_Symbol_43) .LE. INT(OpenAD_Symbol_42))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_44)
        OpenAD_Symbol_45 = 1
        DO WHILE(INT(OpenAD_Symbol_45) .LE. INT(OpenAD_Symbol_44))
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_105)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_106)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_107)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_108)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_109)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_110)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_111)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_112)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_113)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_114)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_115)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_116)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_117)
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_106, __deriv__(FVEC(
     > OpenAD_Symbol_105)), __deriv__(OpenAD_prp_2))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_107, __deriv__(FVEC(
     > OpenAD_Symbol_105)), __deriv__(OpenAD_prp_3))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_108, __deriv__(FVEC(
     > OpenAD_Symbol_105)), __deriv__(OpenAD_prp_4))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_109, __deriv__(FVEC(
     > OpenAD_Symbol_105)), __deriv__(OpenAD_prp_5))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_110, __deriv__(FVEC(
     > OpenAD_Symbol_105)), __deriv__(OpenAD_prp_6))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_111, __deriv__(FVEC(
     > OpenAD_Symbol_105)), __deriv__(OpenAD_prp_7))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_112, __deriv__(FVEC(
     > OpenAD_Symbol_105)), __deriv__(OpenAD_prp_8))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_113, __deriv__(FVEC(
     > OpenAD_Symbol_105)), __deriv__(OpenAD_prp_9))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_114, __deriv__(FVEC(
     > OpenAD_Symbol_105)), __deriv__(OpenAD_prp_10))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_115, __deriv__(FVEC(
     > OpenAD_Symbol_105)), __deriv__(OpenAD_prp_11))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_116, __deriv__(FVEC(
     > OpenAD_Symbol_105)), __deriv__(OpenAD_prp_0))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_117, __deriv__(FVEC(
     > OpenAD_Symbol_105)), __deriv__(OpenAD_prp_1))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(FVEC(OpenAD_Symbol_105)))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(-2.0D00, __deriv__(OpenAD_prp_11), __deriv__(PT))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_11), __deriv__(PTR))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_11), __deriv__(PTL))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_11))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(-2.0D00, __deriv__(OpenAD_prp_10), __deriv__(PT))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_10), __deriv__(PTT))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_10), __deriv__(P))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_10))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(-2.0D00, __deriv__(OpenAD_prp_9), __deriv__(PR))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_9), __deriv__(PTR))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_9), __deriv__(PBR))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_9))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(-2.0D00, __deriv__(OpenAD_prp_8), __deriv__(PR))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_8), __deriv__(PRR))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_8), __deriv__(P))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_8))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(-2.0D00, __deriv__(OpenAD_prp_7), __deriv__(P))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_7), __deriv__(PR))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_7), __deriv__(PL))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_7))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(-2.0D00, __deriv__(OpenAD_prp_6), __deriv__(P))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_6), __deriv__(PT))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_6), __deriv__(PB))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_6))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(-2.0D00, __deriv__(OpenAD_prp_5), __deriv__(PL))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_5), __deriv__(P))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_5), __deriv__(PLL))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_5))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(-2.0D00, __deriv__(OpenAD_prp_4), __deriv__(PL))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_4), __deriv__(PTL))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_4), __deriv__(PBL))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_4))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(-2.0D00, __deriv__(OpenAD_prp_3), __deriv__(PB))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_3), __deriv__(PBR))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_3), __deriv__(PBL))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_3))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(-2.0D00, __deriv__(OpenAD_prp_2), __deriv__(PB))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_2), __deriv__(P))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_2), __deriv__(PBB))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_2))
C         $OpenAD$ INLINE DecDeriv(subst,subst)
          CALL DecDeriv(__deriv__(OpenAD_prp_1), __deriv__(PL))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_1), __deriv__(PR))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_1))
C         $OpenAD$ INLINE DecDeriv(subst,subst)
          CALL DecDeriv(__deriv__(OpenAD_prp_0), __deriv__(PB))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(PT))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_46)
          IF(OpenAD_Symbol_46 .ne. 0) THEN
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(PTR))
          ELSE
C           $OpenAD$ INLINE pop_i_s0(subst)
            CALL pop_i_s0(OpenAD_Symbol_118)
C           $OpenAD$ INLINE IncDeriv(subst,subst)
            CALL IncDeriv(__deriv__(PTR), __deriv__(X(OpenAD_Symbol_118
     > )))
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(PTR))
          ENDIF
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_47)
          IF(OpenAD_Symbol_47 .ne. 0) THEN
C           $OpenAD$ INLINE pop_i_s0(subst)
            CALL pop_i_s0(OpenAD_Symbol_104)
C           $OpenAD$ INLINE IncDeriv(subst,subst)
            CALL IncDeriv(__deriv__(PT), __deriv__(X(OpenAD_Symbol_104)
     > ))
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(PT))
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(PTT))
          ELSE
C           $OpenAD$ INLINE pop_i_s0(subst)
            CALL pop_i_s0(OpenAD_Symbol_48)
            IF(OpenAD_Symbol_48 .ne. 0) THEN
C             $OpenAD$ INLINE pop_i_s0(subst)
              CALL pop_i_s0(OpenAD_Symbol_119)
C             $OpenAD$ INLINE IncDeriv(subst,subst)
              CALL IncDeriv(__deriv__(PTT), __deriv__(X(
     > OpenAD_Symbol_119)))
C             $OpenAD$ INLINE ZeroDeriv(subst)
              CALL ZeroDeriv(__deriv__(PTT))
C             $OpenAD$ INLINE ZeroDeriv(subst)
              CALL ZeroDeriv(__deriv__(PT))
            ELSE
C             $OpenAD$ INLINE pop_i_s0(subst)
              CALL pop_i_s0(OpenAD_Symbol_120)
C             $OpenAD$ INLINE IncDeriv(subst,subst)
              CALL IncDeriv(__deriv__(PTT), __deriv__(X(
     > OpenAD_Symbol_120)))
C             $OpenAD$ INLINE ZeroDeriv(subst)
              CALL ZeroDeriv(__deriv__(PTT))
C             $OpenAD$ INLINE pop_i_s0(subst)
              CALL pop_i_s0(OpenAD_Symbol_121)
C             $OpenAD$ INLINE IncDeriv(subst,subst)
              CALL IncDeriv(__deriv__(PT), __deriv__(X(
     > OpenAD_Symbol_121)))
C             $OpenAD$ INLINE ZeroDeriv(subst)
              CALL ZeroDeriv(__deriv__(PT))
            ENDIF
          ENDIF
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_49)
          IF(OpenAD_Symbol_49 .ne. 0) THEN
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(PTL))
          ELSE
C           $OpenAD$ INLINE pop_i_s0(subst)
            CALL pop_i_s0(OpenAD_Symbol_122)
C           $OpenAD$ INLINE IncDeriv(subst,subst)
            CALL IncDeriv(__deriv__(PTL), __deriv__(X(OpenAD_Symbol_122
     > )))
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(PTL))
          ENDIF
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_50)
          IF(OpenAD_Symbol_50 .ne. 0) THEN
C           $OpenAD$ INLINE pop_i_s0(subst)
            CALL pop_i_s0(OpenAD_Symbol_103)
C           $OpenAD$ INLINE IncDeriv(subst,subst)
            CALL IncDeriv(__deriv__(PR), __deriv__(X(OpenAD_Symbol_103)
     > ))
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(PR))
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(PRR))
          ELSE
C           $OpenAD$ INLINE pop_i_s0(subst)
            CALL pop_i_s0(OpenAD_Symbol_51)
            IF(OpenAD_Symbol_51 .ne. 0) THEN
C             $OpenAD$ INLINE pop_i_s0(subst)
              CALL pop_i_s0(OpenAD_Symbol_123)
C             $OpenAD$ INLINE IncDeriv(subst,subst)
              CALL IncDeriv(__deriv__(PRR), __deriv__(X(
     > OpenAD_Symbol_123)))
C             $OpenAD$ INLINE ZeroDeriv(subst)
              CALL ZeroDeriv(__deriv__(PRR))
C             $OpenAD$ INLINE ZeroDeriv(subst)
              CALL ZeroDeriv(__deriv__(PR))
            ELSE
C             $OpenAD$ INLINE pop_i_s0(subst)
              CALL pop_i_s0(OpenAD_Symbol_124)
C             $OpenAD$ INLINE IncDeriv(subst,subst)
              CALL IncDeriv(__deriv__(PRR), __deriv__(X(
     > OpenAD_Symbol_124)))
C             $OpenAD$ INLINE ZeroDeriv(subst)
              CALL ZeroDeriv(__deriv__(PRR))
C             $OpenAD$ INLINE pop_i_s0(subst)
              CALL pop_i_s0(OpenAD_Symbol_125)
C             $OpenAD$ INLINE IncDeriv(subst,subst)
              CALL IncDeriv(__deriv__(PR), __deriv__(X(
     > OpenAD_Symbol_125)))
C             $OpenAD$ INLINE ZeroDeriv(subst)
              CALL ZeroDeriv(__deriv__(PR))
            ENDIF
          ENDIF
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_102)
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(P), __deriv__(X(OpenAD_Symbol_102)))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(P))
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_52)
          IF(OpenAD_Symbol_52 .ne. 0) THEN
C           $OpenAD$ INLINE pop_i_s0(subst)
            CALL pop_i_s0(OpenAD_Symbol_101)
C           $OpenAD$ INLINE IncDeriv(subst,subst)
            CALL IncDeriv(__deriv__(PLL), __deriv__(X(OpenAD_Symbol_101
     > )))
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(PLL))
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(PL))
          ELSE
C           $OpenAD$ INLINE pop_i_s0(subst)
            CALL pop_i_s0(OpenAD_Symbol_53)
            IF(OpenAD_Symbol_53 .ne. 0) THEN
C             $OpenAD$ INLINE pop_i_s0(subst)
              CALL pop_i_s0(OpenAD_Symbol_126)
C             $OpenAD$ INLINE IncDeriv(subst,subst)
              CALL IncDeriv(__deriv__(PL), __deriv__(X(
     > OpenAD_Symbol_126)))
C             $OpenAD$ INLINE ZeroDeriv(subst)
              CALL ZeroDeriv(__deriv__(PL))
C             $OpenAD$ INLINE ZeroDeriv(subst)
              CALL ZeroDeriv(__deriv__(PLL))
            ELSE
C             $OpenAD$ INLINE pop_i_s0(subst)
              CALL pop_i_s0(OpenAD_Symbol_127)
C             $OpenAD$ INLINE IncDeriv(subst,subst)
              CALL IncDeriv(__deriv__(PLL), __deriv__(X(
     > OpenAD_Symbol_127)))
C             $OpenAD$ INLINE ZeroDeriv(subst)
              CALL ZeroDeriv(__deriv__(PLL))
C             $OpenAD$ INLINE pop_i_s0(subst)
              CALL pop_i_s0(OpenAD_Symbol_128)
C             $OpenAD$ INLINE IncDeriv(subst,subst)
              CALL IncDeriv(__deriv__(PL), __deriv__(X(
     > OpenAD_Symbol_128)))
C             $OpenAD$ INLINE ZeroDeriv(subst)
              CALL ZeroDeriv(__deriv__(PL))
            ENDIF
          ENDIF
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_54)
          IF(OpenAD_Symbol_54 .ne. 0) THEN
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(PBR))
          ELSE
C           $OpenAD$ INLINE pop_i_s0(subst)
            CALL pop_i_s0(OpenAD_Symbol_129)
C           $OpenAD$ INLINE IncDeriv(subst,subst)
            CALL IncDeriv(__deriv__(PBR), __deriv__(X(OpenAD_Symbol_129
     > )))
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(PBR))
          ENDIF
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_55)
          IF(OpenAD_Symbol_55 .ne. 0) THEN
C           $OpenAD$ INLINE pop_i_s0(subst)
            CALL pop_i_s0(OpenAD_Symbol_100)
C           $OpenAD$ INLINE IncDeriv(subst,subst)
            CALL IncDeriv(__deriv__(PBB), __deriv__(X(OpenAD_Symbol_100
     > )))
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(PBB))
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(PB))
          ELSE
C           $OpenAD$ INLINE pop_i_s0(subst)
            CALL pop_i_s0(OpenAD_Symbol_56)
            IF(OpenAD_Symbol_56 .ne. 0) THEN
C             $OpenAD$ INLINE pop_i_s0(subst)
              CALL pop_i_s0(OpenAD_Symbol_130)
C             $OpenAD$ INLINE IncDeriv(subst,subst)
              CALL IncDeriv(__deriv__(PB), __deriv__(X(
     > OpenAD_Symbol_130)))
C             $OpenAD$ INLINE ZeroDeriv(subst)
              CALL ZeroDeriv(__deriv__(PB))
C             $OpenAD$ INLINE ZeroDeriv(subst)
              CALL ZeroDeriv(__deriv__(PBB))
            ELSE
C             $OpenAD$ INLINE pop_i_s0(subst)
              CALL pop_i_s0(OpenAD_Symbol_131)
C             $OpenAD$ INLINE IncDeriv(subst,subst)
              CALL IncDeriv(__deriv__(PBB), __deriv__(X(
     > OpenAD_Symbol_131)))
C             $OpenAD$ INLINE ZeroDeriv(subst)
              CALL ZeroDeriv(__deriv__(PBB))
C             $OpenAD$ INLINE pop_i_s0(subst)
              CALL pop_i_s0(OpenAD_Symbol_132)
C             $OpenAD$ INLINE IncDeriv(subst,subst)
              CALL IncDeriv(__deriv__(PB), __deriv__(X(
     > OpenAD_Symbol_132)))
C             $OpenAD$ INLINE ZeroDeriv(subst)
              CALL ZeroDeriv(__deriv__(PB))
            ENDIF
          ENDIF
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_57)
          IF(OpenAD_Symbol_57 .ne. 0) THEN
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(PBL))
          ELSE
C           $OpenAD$ INLINE pop_i_s0(subst)
            CALL pop_i_s0(OpenAD_Symbol_133)
C           $OpenAD$ INLINE IncDeriv(subst,subst)
            CALL IncDeriv(__deriv__(PBL), __deriv__(X(OpenAD_Symbol_133
     > )))
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(PBL))
          ENDIF
          OpenAD_Symbol_45 = INT(OpenAD_Symbol_45) + 1
        END DO
        OpenAD_Symbol_43 = INT(OpenAD_Symbol_43) + 1
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE
