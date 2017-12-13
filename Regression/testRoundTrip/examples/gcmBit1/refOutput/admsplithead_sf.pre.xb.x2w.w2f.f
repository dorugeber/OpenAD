
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
      SUBROUTINE find_bulkmod(LOCPRES, TFLD, SFLD, BULKMOD)
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
      TYPE (OpenADTy_active) LOCPRES(1 : 2, 1 : 2)
      TYPE (OpenADTy_active) TFLD(1 : 2, 1 : 2, 1 : 2, 1 : 2, 1 : 2)
      TYPE (OpenADTy_active) SFLD(1 : 2, 1 : 2, 1 : 2, 1 : 2, 1 : 2)
      TYPE (OpenADTy_active) BULKMOD(1 : 2, 1 : 2)
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) BI
      INTEGER(w2f__i4) BJ
      TYPE (OpenADTy_active) BMFRESH
      TYPE (OpenADTy_active) BMPRES
      TYPE (OpenADTy_active) BMSALT
      REAL(w2f__8) EOSJMDCKFW(1 : 5)
      REAL(w2f__8) EOSJMDCKP(1 : 14)
      REAL(w2f__8) EOSJMDCKSW(1 : 7)
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) J
      INTEGER(w2f__i4) K
      TYPE (OpenADTy_active) P
      TYPE (OpenADTy_active) P2
      TYPE (OpenADTy_active) S
      TYPE (OpenADTy_active) S3O2
      REAL(w2f__8) SITOBAR
      TYPE (OpenADTy_active) T
      TYPE (OpenADTy_active) T2
      TYPE (OpenADTy_active) T3
      TYPE (OpenADTy_active) T4
      INTEGER(w2f__i4) OpenAD_Symbol_61
      INTEGER(w2f__i4) OpenAD_Symbol_62
      INTEGER(w2f__i4) OpenAD_Symbol_63
      INTEGER(w2f__i4) OpenAD_Symbol_64
      INTEGER(w2f__i4) OpenAD_Symbol_65
      REAL(w2f__8) OpenAD_Symbol_66
      REAL(w2f__8) OpenAD_Symbol_67
      REAL(w2f__8) OpenAD_Symbol_68
      REAL(w2f__8) OpenAD_Symbol_69
      REAL(w2f__8) OpenAD_Symbol_70
      INTEGER(w2f__i4) OpenAD_Symbol_71
      INTEGER(w2f__i4) OpenAD_Symbol_72
      REAL(w2f__8) OpenAD_Symbol_73
      REAL(w2f__8) OpenAD_Symbol_74
      REAL(w2f__8) OpenAD_Symbol_75
      REAL(w2f__8) OpenAD_Symbol_76
      REAL(w2f__8) OpenAD_Symbol_77
      REAL(w2f__8) OpenAD_Symbol_78
      REAL(w2f__8) OpenAD_Symbol_79
      REAL(w2f__8) OpenAD_Symbol_80
      REAL(w2f__8) OpenAD_Symbol_81
      REAL(w2f__8) OpenAD_Symbol_82
      REAL(w2f__8) OpenAD_Symbol_83
      REAL(w2f__8) OpenAD_Symbol_84
      REAL(w2f__8) OpenAD_Symbol_85
      REAL(w2f__8) OpenAD_Symbol_86
      REAL(w2f__8) OpenAD_Symbol_87
      REAL(w2f__8) OpenAD_Symbol_88
      REAL(w2f__8) OpenAD_Symbol_89
      REAL(w2f__8) OpenAD_Symbol_90
      REAL(w2f__8) OpenAD_Symbol_91
      REAL(w2f__8) OpenAD_Symbol_92
      REAL(w2f__8) OpenAD_Symbol_93
      REAL(w2f__8) OpenAD_Symbol_94
      REAL(w2f__8) OpenAD_Symbol_95
      REAL(w2f__8) OpenAD_Symbol_96
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
      REAL(w2f__8) OpenAD_acc_21
      REAL(w2f__8) OpenAD_acc_22
      REAL(w2f__8) OpenAD_acc_3
      REAL(w2f__8) OpenAD_acc_4
      REAL(w2f__8) OpenAD_acc_5
      REAL(w2f__8) OpenAD_acc_6
      REAL(w2f__8) OpenAD_acc_7
      REAL(w2f__8) OpenAD_acc_8
      REAL(w2f__8) OpenAD_acc_9
      REAL(w2f__8) OpenAD_aux_0
      REAL(w2f__8) OpenAD_aux_1
      REAL(w2f__8) OpenAD_aux_2
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
      REAL(w2f__8) OpenAD_lin_21
      REAL(w2f__8) OpenAD_lin_22
      REAL(w2f__8) OpenAD_lin_23
      REAL(w2f__8) OpenAD_lin_24
      REAL(w2f__8) OpenAD_lin_25
      REAL(w2f__8) OpenAD_lin_26
      REAL(w2f__8) OpenAD_lin_27
      REAL(w2f__8) OpenAD_lin_28
      REAL(w2f__8) OpenAD_lin_29
      REAL(w2f__8) OpenAD_lin_3
      REAL(w2f__8) OpenAD_lin_30
      REAL(w2f__8) OpenAD_lin_31
      REAL(w2f__8) OpenAD_lin_32
      REAL(w2f__8) OpenAD_lin_33
      REAL(w2f__8) OpenAD_lin_34
      REAL(w2f__8) OpenAD_lin_35
      REAL(w2f__8) OpenAD_lin_36
      REAL(w2f__8) OpenAD_lin_37
      REAL(w2f__8) OpenAD_lin_38
      REAL(w2f__8) OpenAD_lin_39
      REAL(w2f__8) OpenAD_lin_4
      REAL(w2f__8) OpenAD_lin_40
      REAL(w2f__8) OpenAD_lin_41
      REAL(w2f__8) OpenAD_lin_42
      REAL(w2f__8) OpenAD_lin_43
      REAL(w2f__8) OpenAD_lin_44
      REAL(w2f__8) OpenAD_lin_45
      REAL(w2f__8) OpenAD_lin_46
      REAL(w2f__8) OpenAD_lin_47
      REAL(w2f__8) OpenAD_lin_48
      REAL(w2f__8) OpenAD_lin_5
      REAL(w2f__8) OpenAD_lin_6
      REAL(w2f__8) OpenAD_lin_7
      REAL(w2f__8) OpenAD_lin_8
      REAL(w2f__8) OpenAD_lin_9
C
C     **** Statements ****
C
C     $OpenAD$ BEGIN REPLACEMENT 1
C$OPENAD XXX Template ad_template.f
      EOSJMDCKFW(1) = 1.9659330078E+04
      EOSJMDCKFW(2) = 1.4443040466E+02
      EOSJMDCKFW(3) = (-1.7061029673)
      EOSJMDCKFW(4) = 9.64870397E-03
      EOSJMDCKFW(5) = (-4.1902530938E-05)
      EOSJMDCKSW(1) = 5.2848548889E+01
      EOSJMDCKSW(2) = (-3.1010890007E-01)
      EOSJMDCKSW(3) = 6.2832632102E-03
      EOSJMDCKSW(4) = (-5.0841881603E-05)
      EOSJMDCKSW(5) = 3.8866400719E-01
      EOSJMDCKSW(6) = 9.085830301E-03
      EOSJMDCKSW(7) = (-4.6199240023E-04)
      EOSJMDCKP(1) = 3.1865189075
      EOSJMDCKP(2) = 2.2122759372E-02
      EOSJMDCKP(3) = (-2.9846420512E-04)
      EOSJMDCKP(4) = 1.9564149625E-06
      EOSJMDCKP(5) = 6.7043881863E-03
      EOSJMDCKP(6) = (-1.8473180535E-04)
      EOSJMDCKP(7) = 2.0593310524E-07
      EOSJMDCKP(8) = 1.480265928E-04
      EOSJMDCKP(9) = 2.1028979972E-04
      EOSJMDCKP(10) = (-1.2020160284E-05)
      EOSJMDCKP(11) = 1.3946799982E-07
      EOSJMDCKP(12) = (-2.0402369501E-06)
      EOSJMDCKP(13) = 6.128772867E-08
      EOSJMDCKP(14) = 6.2073229889E-10
      K = 1
      BI = 1
      BJ = 1
      SITOBAR = 1.0D00
      DO J = 1, 2, 1
        DO I = 1, 2, 1
          __value__(T) = __value__(TFLD(I, J, K, BI, BJ))
          __value__(T2) = (__value__(T) * __value__(T))
          __value__(T3) = (__value__(T) * __value__(T2))
          __value__(T4) = (__value__(T) * __value__(T3))
          __value__(S) = __value__(SFLD(I, J, K, BI, BJ))
          IF(__value__(S) .GT. 0.0D00) THEN
            __value__(S3O2) = (__value__(S) * SQRT(__value__(S)))
          ELSE
            __value__(S) = 0.0D00
            __value__(S3O2) = 0.0D00
          ENDIF
          __value__(P) = (__value__(LOCPRES(I, J)) * SITOBAR)
          __value__(P2) = (__value__(P) * __value__(P))
          __value__(BMFRESH) = (EOSJMDCKFW(1) + EOSJMDCKFW(2) *
     >  __value__(T) + EOSJMDCKFW(3) * __value__(T2) + EOSJMDCKFW(4) *
     >  __value__(T3) + EOSJMDCKFW(5) * __value__(T4))
          __value__(BMSALT) = (__value__(S) *(EOSJMDCKSW(1) +
     >  EOSJMDCKSW(2) * __value__(T) + EOSJMDCKSW(3) * __value__(T2) +
     >  EOSJMDCKSW(4) * __value__(T3)) + __value__(S3O2) *(EOSJMDCKSW(5
     > ) + EOSJMDCKSW(6) * __value__(T) + EOSJMDCKSW(7) * __value__(T2)
     > ))
          __value__(BMPRES) = (__value__(P) *(EOSJMDCKP(1) + EOSJMDCKP(
     > 2) * __value__(T) + EOSJMDCKP(3) * __value__(T2) + EOSJMDCKP(4)
     >  * __value__(T3)) + __value__(P) * __value__(S) *(EOSJMDCKP(5) +
     >  EOSJMDCKP(6) * __value__(T) + EOSJMDCKP(7) * __value__(T2)) +
     >  EOSJMDCKP(8) * __value__(P) * __value__(S3O2) + __value__(P2) *
     > (EOSJMDCKP(9) + EOSJMDCKP(10) * __value__(T) + EOSJMDCKP(11) *
     >  __value__(T2)) + __value__(P2) * __value__(S) *(EOSJMDCKP(12) +
     >  EOSJMDCKP(13) * __value__(T) + EOSJMDCKP(14) * __value__(T2)))
          __value__(BULKMOD(INT(I), INT(J))) = (__value__(BMPRES) +
     >  __value__(BMFRESH) + __value__(BMSALT))
        END DO
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      EOSJMDCKFW(1) = 1.9659330078E+04
      EOSJMDCKFW(2) = 1.4443040466E+02
      EOSJMDCKFW(3) = (-1.7061029673)
      EOSJMDCKFW(4) = 9.64870397E-03
      EOSJMDCKFW(5) = (-4.1902530938E-05)
      EOSJMDCKSW(1) = 5.2848548889E+01
      EOSJMDCKSW(2) = (-3.1010890007E-01)
      EOSJMDCKSW(3) = 6.2832632102E-03
      EOSJMDCKSW(4) = (-5.0841881603E-05)
      EOSJMDCKSW(5) = 3.8866400719E-01
      EOSJMDCKSW(6) = 9.085830301E-03
      EOSJMDCKSW(7) = (-4.6199240023E-04)
      EOSJMDCKP(1) = 3.1865189075
      EOSJMDCKP(2) = 2.2122759372E-02
      EOSJMDCKP(3) = (-2.9846420512E-04)
      EOSJMDCKP(4) = 1.9564149625E-06
      EOSJMDCKP(5) = 6.7043881863E-03
      EOSJMDCKP(6) = (-1.8473180535E-04)
      EOSJMDCKP(7) = 2.0593310524E-07
      EOSJMDCKP(8) = 1.480265928E-04
      EOSJMDCKP(9) = 2.1028979972E-04
      EOSJMDCKP(10) = (-1.2020160284E-05)
      EOSJMDCKP(11) = 1.3946799982E-07
      EOSJMDCKP(12) = (-2.0402369501E-06)
      EOSJMDCKP(13) = 6.128772867E-08
      EOSJMDCKP(14) = 6.2073229889E-10
      K = 1
      BI = 1
      BJ = 1
      SITOBAR = 1.0D00
      OpenAD_Symbol_5 = 0_w2f__i8
      DO J = 1, 2, 1
        OpenAD_Symbol_6 = 0_w2f__i8
        DO I = 1, 2, 1
          __value__(T) = __value__(TFLD(I, J, K, BI, BJ))
          OpenAD_lin_0 = __value__(T)
          OpenAD_lin_1 = __value__(T)
          __value__(T2) = (__value__(T) * __value__(T))
          OpenAD_lin_2 = __value__(T2)
          OpenAD_lin_3 = __value__(T)
          __value__(T3) = (__value__(T) * __value__(T2))
          OpenAD_lin_4 = __value__(T3)
          OpenAD_lin_5 = __value__(T)
          __value__(T4) = (__value__(T) * __value__(T3))
          __value__(S) = __value__(SFLD(I, J, K, BI, BJ))
          OpenAD_acc_0 = (OpenAD_lin_0 + OpenAD_lin_1)
          OpenAD_acc_1 = (OpenAD_lin_2 + OpenAD_acc_0 * OpenAD_lin_3)
          OpenAD_acc_2 = (OpenAD_lin_4 + OpenAD_acc_1 * OpenAD_lin_5)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_0)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_1)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_2)
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(I)
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(J)
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(K)
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(BI)
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(BJ)
          IF(__value__(S) .GT. 0.0D00) THEN
            OpenAD_aux_0 = SQRT(__value__(S))
            OpenAD_lin_6 = OpenAD_aux_0
            OpenAD_lin_8 = (5.0D-01 / OpenAD_aux_0)
            OpenAD_lin_7 = __value__(S)
            __value__(S3O2) = (__value__(S) * OpenAD_aux_0)
            OpenAD_acc_3 = (OpenAD_lin_8 * OpenAD_lin_7)
C           $OpenAD$ INLINE push_s0(subst)
            CALL push_s0(OpenAD_lin_6)
C           $OpenAD$ INLINE push_s0(subst)
            CALL push_s0(OpenAD_acc_3)
            OpenAD_Symbol_7 = 1_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_7)
          ELSE
            __value__(S) = 0.0D00
            __value__(S3O2) = 0.0D00
            OpenAD_Symbol_8 = 0_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_8)
          ENDIF
          OpenAD_lin_9 = SITOBAR
          __value__(P) = (__value__(LOCPRES(I, J)) * SITOBAR)
          OpenAD_lin_10 = __value__(P)
          OpenAD_lin_11 = __value__(P)
          __value__(P2) = (__value__(P) * __value__(P))
          OpenAD_lin_12 = EOSJMDCKFW(2)
          OpenAD_lin_13 = EOSJMDCKFW(3)
          OpenAD_lin_14 = EOSJMDCKFW(4)
          OpenAD_lin_15 = EOSJMDCKFW(5)
          __value__(BMFRESH) = (EOSJMDCKFW(1) + EOSJMDCKFW(2) *
     >  __value__(T) + EOSJMDCKFW(3) * __value__(T2) + EOSJMDCKFW(4) *
     >  __value__(T3) + EOSJMDCKFW(5) * __value__(T4))
          OpenAD_aux_1 = (EOSJMDCKSW(1) + EOSJMDCKSW(2) * __value__(T)
     >  + EOSJMDCKSW(3) * __value__(T2) + EOSJMDCKSW(4) * __value__(T3)
     > )
          OpenAD_aux_2 = (EOSJMDCKSW(5) + EOSJMDCKSW(6) * __value__(T)
     >  + EOSJMDCKSW(7) * __value__(T2))
          OpenAD_lin_16 = OpenAD_aux_1
          OpenAD_lin_18 = EOSJMDCKSW(2)
          OpenAD_lin_19 = EOSJMDCKSW(3)
          OpenAD_lin_20 = EOSJMDCKSW(4)
          OpenAD_lin_17 = __value__(S)
          OpenAD_lin_21 = OpenAD_aux_2
          OpenAD_lin_23 = EOSJMDCKSW(6)
          OpenAD_lin_24 = EOSJMDCKSW(7)
          OpenAD_lin_22 = __value__(S3O2)
          __value__(BMSALT) = (__value__(S) * OpenAD_aux_1 + __value__(
     > S3O2) * OpenAD_aux_2)
          OpenAD_aux_3 = (EOSJMDCKP(1) + EOSJMDCKP(2) * __value__(T) +
     >  EOSJMDCKP(3) * __value__(T2) + EOSJMDCKP(4) * __value__(T3))
          OpenAD_aux_4 = (__value__(P) * __value__(S))
          OpenAD_aux_5 = (EOSJMDCKP(5) + EOSJMDCKP(6) * __value__(T) +
     >  EOSJMDCKP(7) * __value__(T2))
          OpenAD_aux_6 = (__value__(P) * __value__(S3O2))
          OpenAD_aux_7 = (EOSJMDCKP(9) + EOSJMDCKP(10) * __value__(T) +
     >  EOSJMDCKP(11) * __value__(T2))
          OpenAD_aux_8 = (__value__(P2) * __value__(S))
          OpenAD_aux_9 = (EOSJMDCKP(12) + EOSJMDCKP(13) * __value__(T)
     >  + EOSJMDCKP(14) * __value__(T2))
          OpenAD_lin_25 = OpenAD_aux_3
          OpenAD_lin_27 = EOSJMDCKP(2)
          OpenAD_lin_28 = EOSJMDCKP(3)
          OpenAD_lin_29 = EOSJMDCKP(4)
          OpenAD_lin_26 = __value__(P)
          OpenAD_lin_32 = __value__(S)
          OpenAD_lin_33 = __value__(P)
          OpenAD_lin_30 = OpenAD_aux_5
          OpenAD_lin_34 = EOSJMDCKP(6)
          OpenAD_lin_35 = EOSJMDCKP(7)
          OpenAD_lin_31 = OpenAD_aux_4
          OpenAD_lin_37 = __value__(S3O2)
          OpenAD_lin_38 = __value__(P)
          OpenAD_lin_36 = EOSJMDCKP(8)
          OpenAD_lin_39 = OpenAD_aux_7
          OpenAD_lin_41 = EOSJMDCKP(10)
          OpenAD_lin_42 = EOSJMDCKP(11)
          OpenAD_lin_40 = __value__(P2)
          OpenAD_lin_45 = __value__(S)
          OpenAD_lin_46 = __value__(P2)
          OpenAD_lin_43 = OpenAD_aux_9
          OpenAD_lin_47 = EOSJMDCKP(13)
          OpenAD_lin_48 = EOSJMDCKP(14)
          OpenAD_lin_44 = OpenAD_aux_8
          __value__(BMPRES) = (__value__(P) * OpenAD_aux_3 +
     >  OpenAD_aux_4 * OpenAD_aux_5 + EOSJMDCKP(8) * OpenAD_aux_6 +
     >  __value__(P2) * OpenAD_aux_7 + OpenAD_aux_8 * OpenAD_aux_9)
          __value__(BULKMOD(INT(I), INT(J))) = (__value__(BMPRES) +
     >  __value__(BMFRESH) + __value__(BMSALT))
          OpenAD_acc_4 = (OpenAD_lin_46 * OpenAD_lin_43)
          OpenAD_acc_5 = (OpenAD_lin_33 * OpenAD_lin_30)
          OpenAD_acc_6 = (OpenAD_lin_29 * OpenAD_lin_26)
          OpenAD_acc_7 = (OpenAD_lin_20 * OpenAD_lin_17)
          OpenAD_acc_8 = (OpenAD_lin_48 * OpenAD_lin_44)
          OpenAD_acc_9 = (OpenAD_lin_42 * OpenAD_lin_40)
          OpenAD_acc_10 = (OpenAD_lin_35 * OpenAD_lin_31)
          OpenAD_acc_11 = (OpenAD_lin_28 * OpenAD_lin_26)
          OpenAD_acc_12 = (OpenAD_lin_24 * OpenAD_lin_22)
          OpenAD_acc_13 = (OpenAD_lin_19 * OpenAD_lin_17)
          OpenAD_acc_14 = (OpenAD_lin_47 * OpenAD_lin_44)
          OpenAD_acc_15 = (OpenAD_lin_41 * OpenAD_lin_40)
          OpenAD_acc_16 = (OpenAD_lin_38 * OpenAD_lin_36)
          OpenAD_acc_17 = (OpenAD_lin_34 * OpenAD_lin_31)
          OpenAD_acc_18 = (OpenAD_lin_27 * OpenAD_lin_26)
          OpenAD_acc_19 = (OpenAD_lin_23 * OpenAD_lin_22)
          OpenAD_acc_20 = (OpenAD_lin_18 * OpenAD_lin_17)
          OpenAD_acc_21 = (OpenAD_lin_10 + OpenAD_lin_11)
          OpenAD_acc_22 = (OpenAD_lin_9 * OpenAD_lin_25 + OpenAD_lin_9
     >  * OpenAD_acc_21 * OpenAD_lin_45 * OpenAD_lin_43 + OpenAD_lin_9
     >  * OpenAD_acc_21 * OpenAD_lin_39 + OpenAD_lin_9 * OpenAD_lin_32
     >  * OpenAD_lin_30 + OpenAD_lin_9 * OpenAD_lin_37 * OpenAD_lin_36)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_22)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_lin_16)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_4)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_5)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_6)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_lin_21)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_7)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_8)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_9)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_10)
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
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_lin_13)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_lin_14)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_lin_15)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_lin_12)
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(I)
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(J)
          OpenAD_Symbol_6 = (INT(OpenAD_Symbol_6) + INT(1_w2f__i8))
        END DO
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_6)
        OpenAD_Symbol_5 = (INT(OpenAD_Symbol_5) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_5)
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
          CALL pop_i_s0(OpenAD_Symbol_71)
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_72)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_73)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_74)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_75)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_76)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_77)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_78)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_79)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_80)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_81)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_82)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_83)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_84)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_85)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_86)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_87)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_88)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_89)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_90)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_91)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_92)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_93)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_94)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_95)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_96)
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_73, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(T))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_74, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(T4))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_75, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(T3))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_76, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(T2))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_77, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(T))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_78, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(T))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_79, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(T))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_80, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(T))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_81, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(S3O2))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_82, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(T))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_83, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(T))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_84, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(T2))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_85, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(T2))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_86, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(T2))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_87, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(T2))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_88, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(T2))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_89, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(T2))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_90, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(T3))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_91, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(S3O2))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_92, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(T3))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_93, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(S))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_94, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(S))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_95, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(S))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_96, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(LOCPRES(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(BULKMOD(OpenAD_Symbol_72,
     >  OpenAD_Symbol_71)))
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_4)
          IF(OpenAD_Symbol_4 .ne. 0) THEN
C           $OpenAD$ INLINE pop_s0(subst)
            CALL pop_s0(OpenAD_Symbol_69)
C           $OpenAD$ INLINE pop_s0(subst)
            CALL pop_s0(OpenAD_Symbol_70)
C           $OpenAD$ INLINE Saxpy(subst,subst,subst)
            CALL Saxpy(OpenAD_Symbol_69, __deriv__(S3O2), __deriv__(S))
C           $OpenAD$ INLINE Saxpy(subst,subst,subst)
            CALL Saxpy(OpenAD_Symbol_70, __deriv__(S3O2), __deriv__(S))
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(S3O2))
          ELSE
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(S3O2))
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(S))
          ENDIF
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_61)
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_62)
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_63)
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_64)
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_65)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_66)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_67)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_68)
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(S), __deriv__(SFLD(OpenAD_Symbol_65,
     >  OpenAD_Symbol_64, OpenAD_Symbol_63, OpenAD_Symbol_62,
     >  OpenAD_Symbol_61)))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(S))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_66, __deriv__(T4), __deriv__(T))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(T4))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_67, __deriv__(T3), __deriv__(T))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(T3))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_68, __deriv__(T2), __deriv__(T))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(T2))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(T), __deriv__(TFLD(OpenAD_Symbol_65,
     >  OpenAD_Symbol_64, OpenAD_Symbol_63, OpenAD_Symbol_62,
     >  OpenAD_Symbol_61)))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(T))
          OpenAD_Symbol_3 = INT(OpenAD_Symbol_3) + 1
        END DO
        OpenAD_Symbol_1 = INT(OpenAD_Symbol_1) + 1
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 4
C     $OpenAD$ INLINE cp_arg_store_real_matrix_a(subst)
      CALL cp_arg_store_real_matrix_a(__deriv__(LOCPRES))
C     $OpenAD$ INLINE cp_arg_store_real_five_tensor_a(subst)
      CALL cp_arg_store_real_five_tensor_a(__deriv__(TFLD))
C     $OpenAD$ INLINE cp_arg_store_real_five_tensor_a(subst)
      CALL cp_arg_store_real_five_tensor_a(__deriv__(SFLD))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 5
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 6
C     $OpenAD$ INLINE cp_arg_restore_real_five_tensor_a(subst)
      CALL cp_arg_restore_real_five_tensor_a(__deriv__(SFLD))
C     $OpenAD$ INLINE cp_arg_restore_real_five_tensor_a(subst)
      CALL cp_arg_restore_real_five_tensor_a(__deriv__(TFLD))
C     $OpenAD$ INLINE cp_arg_restore_real_matrix_a(subst)
      CALL cp_arg_restore_real_matrix_a(__deriv__(LOCPRES))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 7
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 8
C     $OpenAD$ INLINE cp_arg_store_real_matrix_a(subst)
      CALL cp_arg_store_real_matrix_a(__deriv__(BULKMOD))
C     $OpenAD$ INLINE cp_arg_store_real_matrix_a(subst)
      CALL cp_arg_store_real_matrix_a(__deriv__(LOCPRES))
C     $OpenAD$ INLINE cp_arg_store_real_five_tensor_a(subst)
      CALL cp_arg_store_real_five_tensor_a(__deriv__(TFLD))
C     $OpenAD$ INLINE cp_arg_store_real_five_tensor_a(subst)
      CALL cp_arg_store_real_five_tensor_a(__deriv__(SFLD))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 9
C     $OpenAD$ INLINE cp_arg_restore_real_five_tensor_a(subst)
      CALL cp_arg_restore_real_five_tensor_a(__deriv__(SFLD))
C     $OpenAD$ INLINE cp_arg_restore_real_five_tensor_a(subst)
      CALL cp_arg_restore_real_five_tensor_a(__deriv__(TFLD))
C     $OpenAD$ INLINE cp_arg_restore_real_matrix_a(subst)
      CALL cp_arg_restore_real_matrix_a(__deriv__(LOCPRES))
C     $OpenAD$ INLINE cp_arg_restore_real_matrix_a(subst)
      CALL cp_arg_restore_real_matrix_a(__deriv__(BULKMOD))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 10
C$OPENAD XXX Template ad_template.f
      EOSJMDCKFW(1) = 1.9659330078E+04
      EOSJMDCKFW(2) = 1.4443040466E+02
      EOSJMDCKFW(3) = (-1.7061029673)
      EOSJMDCKFW(4) = 9.64870397E-03
      EOSJMDCKFW(5) = (-4.1902530938E-05)
      EOSJMDCKSW(1) = 5.2848548889E+01
      EOSJMDCKSW(2) = (-3.1010890007E-01)
      EOSJMDCKSW(3) = 6.2832632102E-03
      EOSJMDCKSW(4) = (-5.0841881603E-05)
      EOSJMDCKSW(5) = 3.8866400719E-01
      EOSJMDCKSW(6) = 9.085830301E-03
      EOSJMDCKSW(7) = (-4.6199240023E-04)
      EOSJMDCKP(1) = 3.1865189075
      EOSJMDCKP(2) = 2.2122759372E-02
      EOSJMDCKP(3) = (-2.9846420512E-04)
      EOSJMDCKP(4) = 1.9564149625E-06
      EOSJMDCKP(5) = 6.7043881863E-03
      EOSJMDCKP(6) = (-1.8473180535E-04)
      EOSJMDCKP(7) = 2.0593310524E-07
      EOSJMDCKP(8) = 1.480265928E-04
      EOSJMDCKP(9) = 2.1028979972E-04
      EOSJMDCKP(10) = (-1.2020160284E-05)
      EOSJMDCKP(11) = 1.3946799982E-07
      EOSJMDCKP(12) = (-2.0402369501E-06)
      EOSJMDCKP(13) = 6.128772867E-08
      EOSJMDCKP(14) = 6.2073229889E-10
      K = 1
      BI = 1
      BJ = 1
      SITOBAR = 1.0D00
      OpenAD_Symbol_14 = 0_w2f__i8
      DO J = 1, 2, 1
        OpenAD_Symbol_15 = 0_w2f__i8
        DO I = 1, 2, 1
          __value__(T) = __value__(TFLD(I, J, K, BI, BJ))
          OpenAD_lin_0 = __value__(T)
          OpenAD_lin_1 = __value__(T)
          __value__(T2) = (__value__(T) * __value__(T))
          OpenAD_lin_2 = __value__(T2)
          OpenAD_lin_3 = __value__(T)
          __value__(T3) = (__value__(T) * __value__(T2))
          OpenAD_lin_4 = __value__(T3)
          OpenAD_lin_5 = __value__(T)
          __value__(T4) = (__value__(T) * __value__(T3))
          __value__(S) = __value__(SFLD(I, J, K, BI, BJ))
          OpenAD_acc_0 = (OpenAD_lin_0 + OpenAD_lin_1)
          OpenAD_acc_1 = (OpenAD_lin_2 + OpenAD_acc_0 * OpenAD_lin_3)
          OpenAD_acc_2 = (OpenAD_lin_4 + OpenAD_acc_1 * OpenAD_lin_5)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_0)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_1)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_2)
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(I)
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(J)
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(K)
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(BI)
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(BJ)
          IF(__value__(S) .GT. 0.0D00) THEN
            OpenAD_aux_0 = SQRT(__value__(S))
            OpenAD_lin_6 = OpenAD_aux_0
            OpenAD_lin_8 = (5.0D-01 / OpenAD_aux_0)
            OpenAD_lin_7 = __value__(S)
            __value__(S3O2) = (__value__(S) * OpenAD_aux_0)
            OpenAD_acc_3 = (OpenAD_lin_8 * OpenAD_lin_7)
C           $OpenAD$ INLINE push_s0(subst)
            CALL push_s0(OpenAD_lin_6)
C           $OpenAD$ INLINE push_s0(subst)
            CALL push_s0(OpenAD_acc_3)
            OpenAD_Symbol_16 = 1_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_16)
          ELSE
            __value__(S) = 0.0D00
            __value__(S3O2) = 0.0D00
            OpenAD_Symbol_17 = 0_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_17)
          ENDIF
          OpenAD_lin_9 = SITOBAR
          __value__(P) = (__value__(LOCPRES(I, J)) * SITOBAR)
          OpenAD_lin_10 = __value__(P)
          OpenAD_lin_11 = __value__(P)
          __value__(P2) = (__value__(P) * __value__(P))
          OpenAD_lin_12 = EOSJMDCKFW(2)
          OpenAD_lin_13 = EOSJMDCKFW(3)
          OpenAD_lin_14 = EOSJMDCKFW(4)
          OpenAD_lin_15 = EOSJMDCKFW(5)
          __value__(BMFRESH) = (EOSJMDCKFW(1) + EOSJMDCKFW(2) *
     >  __value__(T) + EOSJMDCKFW(3) * __value__(T2) + EOSJMDCKFW(4) *
     >  __value__(T3) + EOSJMDCKFW(5) * __value__(T4))
          OpenAD_aux_1 = (EOSJMDCKSW(1) + EOSJMDCKSW(2) * __value__(T)
     >  + EOSJMDCKSW(3) * __value__(T2) + EOSJMDCKSW(4) * __value__(T3)
     > )
          OpenAD_aux_2 = (EOSJMDCKSW(5) + EOSJMDCKSW(6) * __value__(T)
     >  + EOSJMDCKSW(7) * __value__(T2))
          OpenAD_lin_16 = OpenAD_aux_1
          OpenAD_lin_18 = EOSJMDCKSW(2)
          OpenAD_lin_19 = EOSJMDCKSW(3)
          OpenAD_lin_20 = EOSJMDCKSW(4)
          OpenAD_lin_17 = __value__(S)
          OpenAD_lin_21 = OpenAD_aux_2
          OpenAD_lin_23 = EOSJMDCKSW(6)
          OpenAD_lin_24 = EOSJMDCKSW(7)
          OpenAD_lin_22 = __value__(S3O2)
          __value__(BMSALT) = (__value__(S) * OpenAD_aux_1 + __value__(
     > S3O2) * OpenAD_aux_2)
          OpenAD_aux_3 = (EOSJMDCKP(1) + EOSJMDCKP(2) * __value__(T) +
     >  EOSJMDCKP(3) * __value__(T2) + EOSJMDCKP(4) * __value__(T3))
          OpenAD_aux_4 = (__value__(P) * __value__(S))
          OpenAD_aux_5 = (EOSJMDCKP(5) + EOSJMDCKP(6) * __value__(T) +
     >  EOSJMDCKP(7) * __value__(T2))
          OpenAD_aux_6 = (__value__(P) * __value__(S3O2))
          OpenAD_aux_7 = (EOSJMDCKP(9) + EOSJMDCKP(10) * __value__(T) +
     >  EOSJMDCKP(11) * __value__(T2))
          OpenAD_aux_8 = (__value__(P2) * __value__(S))
          OpenAD_aux_9 = (EOSJMDCKP(12) + EOSJMDCKP(13) * __value__(T)
     >  + EOSJMDCKP(14) * __value__(T2))
          OpenAD_lin_25 = OpenAD_aux_3
          OpenAD_lin_27 = EOSJMDCKP(2)
          OpenAD_lin_28 = EOSJMDCKP(3)
          OpenAD_lin_29 = EOSJMDCKP(4)
          OpenAD_lin_26 = __value__(P)
          OpenAD_lin_32 = __value__(S)
          OpenAD_lin_33 = __value__(P)
          OpenAD_lin_30 = OpenAD_aux_5
          OpenAD_lin_34 = EOSJMDCKP(6)
          OpenAD_lin_35 = EOSJMDCKP(7)
          OpenAD_lin_31 = OpenAD_aux_4
          OpenAD_lin_37 = __value__(S3O2)
          OpenAD_lin_38 = __value__(P)
          OpenAD_lin_36 = EOSJMDCKP(8)
          OpenAD_lin_39 = OpenAD_aux_7
          OpenAD_lin_41 = EOSJMDCKP(10)
          OpenAD_lin_42 = EOSJMDCKP(11)
          OpenAD_lin_40 = __value__(P2)
          OpenAD_lin_45 = __value__(S)
          OpenAD_lin_46 = __value__(P2)
          OpenAD_lin_43 = OpenAD_aux_9
          OpenAD_lin_47 = EOSJMDCKP(13)
          OpenAD_lin_48 = EOSJMDCKP(14)
          OpenAD_lin_44 = OpenAD_aux_8
          __value__(BMPRES) = (__value__(P) * OpenAD_aux_3 +
     >  OpenAD_aux_4 * OpenAD_aux_5 + EOSJMDCKP(8) * OpenAD_aux_6 +
     >  __value__(P2) * OpenAD_aux_7 + OpenAD_aux_8 * OpenAD_aux_9)
          __value__(BULKMOD(INT(I), INT(J))) = (__value__(BMPRES) +
     >  __value__(BMFRESH) + __value__(BMSALT))
          OpenAD_acc_4 = (OpenAD_lin_46 * OpenAD_lin_43)
          OpenAD_acc_5 = (OpenAD_lin_33 * OpenAD_lin_30)
          OpenAD_acc_6 = (OpenAD_lin_29 * OpenAD_lin_26)
          OpenAD_acc_7 = (OpenAD_lin_20 * OpenAD_lin_17)
          OpenAD_acc_8 = (OpenAD_lin_48 * OpenAD_lin_44)
          OpenAD_acc_9 = (OpenAD_lin_42 * OpenAD_lin_40)
          OpenAD_acc_10 = (OpenAD_lin_35 * OpenAD_lin_31)
          OpenAD_acc_11 = (OpenAD_lin_28 * OpenAD_lin_26)
          OpenAD_acc_12 = (OpenAD_lin_24 * OpenAD_lin_22)
          OpenAD_acc_13 = (OpenAD_lin_19 * OpenAD_lin_17)
          OpenAD_acc_14 = (OpenAD_lin_47 * OpenAD_lin_44)
          OpenAD_acc_15 = (OpenAD_lin_41 * OpenAD_lin_40)
          OpenAD_acc_16 = (OpenAD_lin_38 * OpenAD_lin_36)
          OpenAD_acc_17 = (OpenAD_lin_34 * OpenAD_lin_31)
          OpenAD_acc_18 = (OpenAD_lin_27 * OpenAD_lin_26)
          OpenAD_acc_19 = (OpenAD_lin_23 * OpenAD_lin_22)
          OpenAD_acc_20 = (OpenAD_lin_18 * OpenAD_lin_17)
          OpenAD_acc_21 = (OpenAD_lin_10 + OpenAD_lin_11)
          OpenAD_acc_22 = (OpenAD_lin_9 * OpenAD_lin_25 + OpenAD_lin_9
     >  * OpenAD_acc_21 * OpenAD_lin_45 * OpenAD_lin_43 + OpenAD_lin_9
     >  * OpenAD_acc_21 * OpenAD_lin_39 + OpenAD_lin_9 * OpenAD_lin_32
     >  * OpenAD_lin_30 + OpenAD_lin_9 * OpenAD_lin_37 * OpenAD_lin_36)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_22)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_lin_16)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_4)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_5)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_6)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_lin_21)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_7)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_8)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_9)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_10)
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
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_lin_13)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_lin_14)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_lin_15)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_lin_12)
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(I)
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(J)
          OpenAD_Symbol_15 = (INT(OpenAD_Symbol_15) + INT(1_w2f__i8))
        END DO
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_15)
        OpenAD_Symbol_14 = (INT(OpenAD_Symbol_14) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_14)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_9)
      OpenAD_Symbol_10 = 1
      DO WHILE(INT(OpenAD_Symbol_10) .LE. INT(OpenAD_Symbol_9))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_11)
        OpenAD_Symbol_12 = 1
        DO WHILE(INT(OpenAD_Symbol_12) .LE. INT(OpenAD_Symbol_11))
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_71)
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_72)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_73)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_74)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_75)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_76)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_77)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_78)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_79)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_80)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_81)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_82)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_83)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_84)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_85)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_86)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_87)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_88)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_89)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_90)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_91)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_92)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_93)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_94)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_95)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_96)
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_73, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(T))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_74, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(T4))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_75, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(T3))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_76, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(T2))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_77, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(T))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_78, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(T))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_79, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(T))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_80, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(T))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_81, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(S3O2))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_82, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(T))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_83, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(T))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_84, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(T2))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_85, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(T2))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_86, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(T2))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_87, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(T2))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_88, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(T2))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_89, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(T2))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_90, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(T3))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_91, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(S3O2))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_92, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(T3))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_93, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(S))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_94, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(S))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_95, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(S))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_96, __deriv__(BULKMOD(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)), __deriv__(LOCPRES(
     > OpenAD_Symbol_72, OpenAD_Symbol_71)))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(BULKMOD(OpenAD_Symbol_72,
     >  OpenAD_Symbol_71)))
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_13)
          IF(OpenAD_Symbol_13 .ne. 0) THEN
C           $OpenAD$ INLINE pop_s0(subst)
            CALL pop_s0(OpenAD_Symbol_69)
C           $OpenAD$ INLINE pop_s0(subst)
            CALL pop_s0(OpenAD_Symbol_70)
C           $OpenAD$ INLINE Saxpy(subst,subst,subst)
            CALL Saxpy(OpenAD_Symbol_69, __deriv__(S3O2), __deriv__(S))
C           $OpenAD$ INLINE Saxpy(subst,subst,subst)
            CALL Saxpy(OpenAD_Symbol_70, __deriv__(S3O2), __deriv__(S))
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(S3O2))
          ELSE
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(S3O2))
C           $OpenAD$ INLINE ZeroDeriv(subst)
            CALL ZeroDeriv(__deriv__(S))
          ENDIF
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_61)
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_62)
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_63)
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_64)
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_65)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_66)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_67)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_68)
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(S), __deriv__(SFLD(OpenAD_Symbol_65,
     >  OpenAD_Symbol_64, OpenAD_Symbol_63, OpenAD_Symbol_62,
     >  OpenAD_Symbol_61)))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(S))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_66, __deriv__(T4), __deriv__(T))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(T4))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_67, __deriv__(T3), __deriv__(T))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(T3))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_68, __deriv__(T2), __deriv__(T))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(T2))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(T), __deriv__(TFLD(OpenAD_Symbol_65,
     >  OpenAD_Symbol_64, OpenAD_Symbol_63, OpenAD_Symbol_62,
     >  OpenAD_Symbol_61)))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(T))
          OpenAD_Symbol_12 = INT(OpenAD_Symbol_12) + 1
        END DO
        OpenAD_Symbol_10 = INT(OpenAD_Symbol_10) + 1
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ INLINE cp_arg_store_real_matrix_a_d(subst)
      CALL cp_arg_store_real_matrix_a_d(__deriv__(BULKMOD))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ INLINE cp_arg_restore_real_matrix_a_d(subst)
      CALL cp_arg_restore_real_matrix_a_d(__deriv__(BULKMOD))
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE

      SUBROUTINE head(X, Y)
      use w2f__types
      use oad_intrinsics
      use oad_intrinsics
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      INTEGER(w2f__i8) OpenAD_Symbol_18
      INTEGER(w2f__i8) OpenAD_Symbol_19
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
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X(1 : 32)
      TYPE (OpenADTy_active) Y(1 : 4)
C
C     **** Local Variables and Functions ****
C
      TYPE (OpenADTy_active) BULKMOD(1 : 2, 1 : 2)
      EXTERNAL find_bulkmod
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) J
      INTEGER(w2f__i4) K
      INTEGER(w2f__i4) L
      TYPE (OpenADTy_active) LOCPRES(1 : 2, 1 : 2)
      INTEGER(w2f__i4) M
      TYPE (OpenADTy_active) SFLD(1 : 2, 1 : 2, 1 : 2, 1 : 2, 1 : 2)
      TYPE (OpenADTy_active) TFLD(1 : 2, 1 : 2, 1 : 2, 1 : 2, 1 : 2)
      INTEGER(w2f__i4) XCOUNT
      INTEGER(w2f__i4) OpenAD_Symbol_100
      INTEGER(w2f__i4) OpenAD_Symbol_101
      INTEGER(w2f__i4) OpenAD_Symbol_102
      INTEGER(w2f__i4) OpenAD_Symbol_103
      INTEGER(w2f__i4) OpenAD_Symbol_104
      INTEGER(w2f__i4) OpenAD_Symbol_105
      INTEGER(w2f__i4) OpenAD_Symbol_106
      INTEGER(w2f__i4) OpenAD_Symbol_107
      INTEGER(w2f__i4) OpenAD_Symbol_108
      INTEGER(w2f__i4) OpenAD_Symbol_60
      INTEGER(w2f__i4) OpenAD_Symbol_97
      INTEGER(w2f__i4) OpenAD_Symbol_98
      INTEGER(w2f__i4) OpenAD_Symbol_99
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
      XCOUNT = 1
      DO I = 1, 2, 1
        DO J = 1, 2, 1
          __value__(LOCPRES(INT(I), INT(J))) = (__value__(X(XCOUNT)) *
     >  2.0D00)
          DO K = 1, 2, 1
            DO L = 1, 2, 1
              DO M = 1, 2, 1
                __value__(TFLD(INT(I), INT(J), INT(K), INT(L), INT(M)))
     >  = (__value__(X(XCOUNT)) * 3.0D00)
                __value__(SFLD(INT(I), INT(J), INT(K), INT(L), INT(M)))
     >  = (__value__(X(XCOUNT)) * 4.0D00)
                XCOUNT = (XCOUNT + 1)
              END DO
            END DO
          END DO
        END DO
      END DO
      CALL find_bulkmod(__deriv__(LOCPRES), __deriv__(TFLD), __deriv__(
     > SFLD), __deriv__(BULKMOD))
      DO I = 1, 2, 1
        DO J = 1, 2, 1
          __value__(Y(J + I * 2 + (-2))) = __value__(BULKMOD(I, J))
        END DO
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      XCOUNT = 1
      OpenAD_Symbol_32 = 0_w2f__i8
      DO I = 1, 2, 1
        OpenAD_Symbol_33 = 0_w2f__i8
        DO J = 1, 2, 1
          __value__(LOCPRES(INT(I), INT(J))) = (__value__(X(XCOUNT)) *
     >  2.0D00)
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(XCOUNT)
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(I)
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(J)
          OpenAD_Symbol_34 = 0_w2f__i8
          DO K = 1, 2, 1
            OpenAD_Symbol_35 = 0_w2f__i8
            DO L = 1, 2, 1
              OpenAD_Symbol_36 = 0_w2f__i8
              DO M = 1, 2, 1
                __value__(TFLD(INT(I), INT(J), INT(K), INT(L), INT(M)))
     >  = (__value__(X(XCOUNT)) * 3.0D00)
                __value__(SFLD(INT(I), INT(J), INT(K), INT(L), INT(M)))
     >  = (__value__(X(XCOUNT)) * 4.0D00)
C               $OpenAD$ INLINE push_i_s0(subst)
                CALL push_i_s0(XCOUNT)
C               $OpenAD$ INLINE push_i_s0(subst)
                CALL push_i_s0(I)
C               $OpenAD$ INLINE push_i_s0(subst)
                CALL push_i_s0(J)
C               $OpenAD$ INLINE push_i_s0(subst)
                CALL push_i_s0(K)
C               $OpenAD$ INLINE push_i_s0(subst)
                CALL push_i_s0(L)
C               $OpenAD$ INLINE push_i_s0(subst)
                CALL push_i_s0(M)
                XCOUNT = (XCOUNT + 1)
                OpenAD_Symbol_36 = (INT(OpenAD_Symbol_36) + INT(
     > 1_w2f__i8))
              END DO
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_36)
              OpenAD_Symbol_35 = (INT(OpenAD_Symbol_35) + INT(1_w2f__i8
     > ))
            END DO
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_35)
            OpenAD_Symbol_34 = (INT(OpenAD_Symbol_34) + INT(1_w2f__i8))
          END DO
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_34)
          OpenAD_Symbol_33 = (INT(OpenAD_Symbol_33) + INT(1_w2f__i8))
        END DO
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_33)
        OpenAD_Symbol_32 = (INT(OpenAD_Symbol_32) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_32)
      CALL find_bulkmod(__deriv__(LOCPRES), __deriv__(TFLD), __deriv__(
     > SFLD), __deriv__(BULKMOD))
      OpenAD_Symbol_37 = 0_w2f__i8
      DO I = 1, 2, 1
        OpenAD_Symbol_38 = 0_w2f__i8
        DO J = 1, 2, 1
          __value__(Y(J + I * 2 + (-2))) = __value__(BULKMOD(I, J))
          OpenAD_Symbol_60 = (J + I * 2 +(-2))
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_60)
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(I)
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(J)
          OpenAD_Symbol_38 = (INT(OpenAD_Symbol_38) + INT(1_w2f__i8))
        END DO
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_38)
        OpenAD_Symbol_37 = (INT(OpenAD_Symbol_37) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_37)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_18)
      OpenAD_Symbol_19 = 1
      DO WHILE(INT(OpenAD_Symbol_19) .LE. INT(OpenAD_Symbol_18))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_20)
        OpenAD_Symbol_21 = 1
        DO WHILE(INT(OpenAD_Symbol_21) .LE. INT(OpenAD_Symbol_20))
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_106)
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_107)
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_108)
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(Y(OpenAD_Symbol_108)), __deriv__(
     > BULKMOD(OpenAD_Symbol_107, OpenAD_Symbol_106)))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(Y(OpenAD_Symbol_108)))
          OpenAD_Symbol_21 = INT(OpenAD_Symbol_21) + 1
        END DO
        OpenAD_Symbol_19 = INT(OpenAD_Symbol_19) + 1
      END DO
      CALL find_bulkmod(__deriv__(LOCPRES), __deriv__(TFLD), __deriv__(
     > SFLD), __deriv__(BULKMOD))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_22)
      OpenAD_Symbol_23 = 1
      DO WHILE(INT(OpenAD_Symbol_23) .LE. INT(OpenAD_Symbol_22))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_24)
        OpenAD_Symbol_25 = 1
        DO WHILE(INT(OpenAD_Symbol_25) .LE. INT(OpenAD_Symbol_24))
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_26)
          OpenAD_Symbol_27 = 1
          DO WHILE(INT(OpenAD_Symbol_27) .LE. INT(OpenAD_Symbol_26))
C           $OpenAD$ INLINE pop_i_s0(subst)
            CALL pop_i_s0(OpenAD_Symbol_28)
            OpenAD_Symbol_29 = 1
            DO WHILE(INT(OpenAD_Symbol_29) .LE. INT(OpenAD_Symbol_28))
C             $OpenAD$ INLINE pop_i_s0(subst)
              CALL pop_i_s0(OpenAD_Symbol_30)
              OpenAD_Symbol_31 = 1
              DO WHILE(INT(OpenAD_Symbol_31) .LE. INT(OpenAD_Symbol_30)
     > )
C               $OpenAD$ INLINE pop_i_s0(subst)
                CALL pop_i_s0(OpenAD_Symbol_100)
C               $OpenAD$ INLINE pop_i_s0(subst)
                CALL pop_i_s0(OpenAD_Symbol_101)
C               $OpenAD$ INLINE pop_i_s0(subst)
                CALL pop_i_s0(OpenAD_Symbol_102)
C               $OpenAD$ INLINE pop_i_s0(subst)
                CALL pop_i_s0(OpenAD_Symbol_103)
C               $OpenAD$ INLINE pop_i_s0(subst)
                CALL pop_i_s0(OpenAD_Symbol_104)
C               $OpenAD$ INLINE pop_i_s0(subst)
                CALL pop_i_s0(OpenAD_Symbol_105)
C               $OpenAD$ INLINE Saxpy(subst,subst,subst)
                CALL Saxpy(4.0D00, __deriv__(SFLD(OpenAD_Symbol_104,
     >  OpenAD_Symbol_103, OpenAD_Symbol_102, OpenAD_Symbol_101,
     >  OpenAD_Symbol_100)), __deriv__(X(OpenAD_Symbol_105)))
C               $OpenAD$ INLINE ZeroDeriv(subst)
                CALL ZeroDeriv(__deriv__(SFLD(OpenAD_Symbol_104,
     >  OpenAD_Symbol_103, OpenAD_Symbol_102, OpenAD_Symbol_101,
     >  OpenAD_Symbol_100)))
C               $OpenAD$ INLINE Saxpy(subst,subst,subst)
                CALL Saxpy(3.0D00, __deriv__(TFLD(OpenAD_Symbol_104,
     >  OpenAD_Symbol_103, OpenAD_Symbol_102, OpenAD_Symbol_101,
     >  OpenAD_Symbol_100)), __deriv__(X(OpenAD_Symbol_105)))
C               $OpenAD$ INLINE ZeroDeriv(subst)
                CALL ZeroDeriv(__deriv__(TFLD(OpenAD_Symbol_104,
     >  OpenAD_Symbol_103, OpenAD_Symbol_102, OpenAD_Symbol_101,
     >  OpenAD_Symbol_100)))
                OpenAD_Symbol_31 = INT(OpenAD_Symbol_31) + 1
              END DO
              OpenAD_Symbol_29 = INT(OpenAD_Symbol_29) + 1
            END DO
            OpenAD_Symbol_27 = INT(OpenAD_Symbol_27) + 1
          END DO
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_97)
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_98)
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_99)
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(2.0D00, __deriv__(LOCPRES(OpenAD_Symbol_98,
     >  OpenAD_Symbol_97)), __deriv__(X(OpenAD_Symbol_99)))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(LOCPRES(OpenAD_Symbol_98,
     >  OpenAD_Symbol_97)))
          OpenAD_Symbol_25 = INT(OpenAD_Symbol_25) + 1
        END DO
        OpenAD_Symbol_23 = INT(OpenAD_Symbol_23) + 1
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
      XCOUNT = 1
      OpenAD_Symbol_53 = 0_w2f__i8
      DO I = 1, 2, 1
        OpenAD_Symbol_54 = 0_w2f__i8
        DO J = 1, 2, 1
          __value__(LOCPRES(INT(I), INT(J))) = (__value__(X(XCOUNT)) *
     >  2.0D00)
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(XCOUNT)
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(I)
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(J)
          OpenAD_Symbol_55 = 0_w2f__i8
          DO K = 1, 2, 1
            OpenAD_Symbol_56 = 0_w2f__i8
            DO L = 1, 2, 1
              OpenAD_Symbol_57 = 0_w2f__i8
              DO M = 1, 2, 1
                __value__(TFLD(INT(I), INT(J), INT(K), INT(L), INT(M)))
     >  = (__value__(X(XCOUNT)) * 3.0D00)
                __value__(SFLD(INT(I), INT(J), INT(K), INT(L), INT(M)))
     >  = (__value__(X(XCOUNT)) * 4.0D00)
C               $OpenAD$ INLINE push_i_s0(subst)
                CALL push_i_s0(XCOUNT)
C               $OpenAD$ INLINE push_i_s0(subst)
                CALL push_i_s0(I)
C               $OpenAD$ INLINE push_i_s0(subst)
                CALL push_i_s0(J)
C               $OpenAD$ INLINE push_i_s0(subst)
                CALL push_i_s0(K)
C               $OpenAD$ INLINE push_i_s0(subst)
                CALL push_i_s0(L)
C               $OpenAD$ INLINE push_i_s0(subst)
                CALL push_i_s0(M)
                XCOUNT = (XCOUNT + 1)
                OpenAD_Symbol_57 = (INT(OpenAD_Symbol_57) + INT(
     > 1_w2f__i8))
              END DO
C             $OpenAD$ INLINE push_i_s0(subst)
              CALL push_i_s0(OpenAD_Symbol_57)
              OpenAD_Symbol_56 = (INT(OpenAD_Symbol_56) + INT(1_w2f__i8
     > ))
            END DO
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_56)
            OpenAD_Symbol_55 = (INT(OpenAD_Symbol_55) + INT(1_w2f__i8))
          END DO
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_55)
          OpenAD_Symbol_54 = (INT(OpenAD_Symbol_54) + INT(1_w2f__i8))
        END DO
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_54)
        OpenAD_Symbol_53 = (INT(OpenAD_Symbol_53) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_53)
      CALL find_bulkmod(__deriv__(LOCPRES), __deriv__(TFLD), __deriv__(
     > SFLD), __deriv__(BULKMOD))
      OpenAD_Symbol_58 = 0_w2f__i8
      DO I = 1, 2, 1
        OpenAD_Symbol_59 = 0_w2f__i8
        DO J = 1, 2, 1
          __value__(Y(J + I * 2 + (-2))) = __value__(BULKMOD(I, J))
          OpenAD_Symbol_60 = (J + I * 2 +(-2))
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_60)
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(I)
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(J)
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
      CALL pop_i_s0(OpenAD_Symbol_39)
      OpenAD_Symbol_40 = 1
      DO WHILE(INT(OpenAD_Symbol_40) .LE. INT(OpenAD_Symbol_39))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_41)
        OpenAD_Symbol_42 = 1
        DO WHILE(INT(OpenAD_Symbol_42) .LE. INT(OpenAD_Symbol_41))
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_106)
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_107)
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_108)
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(Y(OpenAD_Symbol_108)), __deriv__(
     > BULKMOD(OpenAD_Symbol_107, OpenAD_Symbol_106)))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(Y(OpenAD_Symbol_108)))
          OpenAD_Symbol_42 = INT(OpenAD_Symbol_42) + 1
        END DO
        OpenAD_Symbol_40 = INT(OpenAD_Symbol_40) + 1
      END DO
      CALL find_bulkmod(__deriv__(LOCPRES), __deriv__(TFLD), __deriv__(
     > SFLD), __deriv__(BULKMOD))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_43)
      OpenAD_Symbol_44 = 1
      DO WHILE(INT(OpenAD_Symbol_44) .LE. INT(OpenAD_Symbol_43))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_45)
        OpenAD_Symbol_46 = 1
        DO WHILE(INT(OpenAD_Symbol_46) .LE. INT(OpenAD_Symbol_45))
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_47)
          OpenAD_Symbol_48 = 1
          DO WHILE(INT(OpenAD_Symbol_48) .LE. INT(OpenAD_Symbol_47))
C           $OpenAD$ INLINE pop_i_s0(subst)
            CALL pop_i_s0(OpenAD_Symbol_49)
            OpenAD_Symbol_50 = 1
            DO WHILE(INT(OpenAD_Symbol_50) .LE. INT(OpenAD_Symbol_49))
C             $OpenAD$ INLINE pop_i_s0(subst)
              CALL pop_i_s0(OpenAD_Symbol_51)
              OpenAD_Symbol_52 = 1
              DO WHILE(INT(OpenAD_Symbol_52) .LE. INT(OpenAD_Symbol_51)
     > )
C               $OpenAD$ INLINE pop_i_s0(subst)
                CALL pop_i_s0(OpenAD_Symbol_100)
C               $OpenAD$ INLINE pop_i_s0(subst)
                CALL pop_i_s0(OpenAD_Symbol_101)
C               $OpenAD$ INLINE pop_i_s0(subst)
                CALL pop_i_s0(OpenAD_Symbol_102)
C               $OpenAD$ INLINE pop_i_s0(subst)
                CALL pop_i_s0(OpenAD_Symbol_103)
C               $OpenAD$ INLINE pop_i_s0(subst)
                CALL pop_i_s0(OpenAD_Symbol_104)
C               $OpenAD$ INLINE pop_i_s0(subst)
                CALL pop_i_s0(OpenAD_Symbol_105)
C               $OpenAD$ INLINE Saxpy(subst,subst,subst)
                CALL Saxpy(4.0D00, __deriv__(SFLD(OpenAD_Symbol_104,
     >  OpenAD_Symbol_103, OpenAD_Symbol_102, OpenAD_Symbol_101,
     >  OpenAD_Symbol_100)), __deriv__(X(OpenAD_Symbol_105)))
C               $OpenAD$ INLINE ZeroDeriv(subst)
                CALL ZeroDeriv(__deriv__(SFLD(OpenAD_Symbol_104,
     >  OpenAD_Symbol_103, OpenAD_Symbol_102, OpenAD_Symbol_101,
     >  OpenAD_Symbol_100)))
C               $OpenAD$ INLINE Saxpy(subst,subst,subst)
                CALL Saxpy(3.0D00, __deriv__(TFLD(OpenAD_Symbol_104,
     >  OpenAD_Symbol_103, OpenAD_Symbol_102, OpenAD_Symbol_101,
     >  OpenAD_Symbol_100)), __deriv__(X(OpenAD_Symbol_105)))
C               $OpenAD$ INLINE ZeroDeriv(subst)
                CALL ZeroDeriv(__deriv__(TFLD(OpenAD_Symbol_104,
     >  OpenAD_Symbol_103, OpenAD_Symbol_102, OpenAD_Symbol_101,
     >  OpenAD_Symbol_100)))
                OpenAD_Symbol_52 = INT(OpenAD_Symbol_52) + 1
              END DO
              OpenAD_Symbol_50 = INT(OpenAD_Symbol_50) + 1
            END DO
            OpenAD_Symbol_48 = INT(OpenAD_Symbol_48) + 1
          END DO
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_97)
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_98)
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_99)
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(2.0D00, __deriv__(LOCPRES(OpenAD_Symbol_98,
     >  OpenAD_Symbol_97)), __deriv__(X(OpenAD_Symbol_99)))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(LOCPRES(OpenAD_Symbol_98,
     >  OpenAD_Symbol_97)))
          OpenAD_Symbol_46 = INT(OpenAD_Symbol_46) + 1
        END DO
        OpenAD_Symbol_44 = INT(OpenAD_Symbol_44) + 1
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE
