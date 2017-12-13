
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
      SUBROUTINE head(OBJ, G_OBJ0, G_OBJ1, G_OBJ2, G_OBJ3, G_OBJ4,
     >  G_OBJ5, X0, X1, X2, X3, X4, X5, H_OBJ0, H_OBJ1, H_OBJ2, H_OBJ3,
     >  H_OBJ4, H_OBJ5, H_OBJ6, H_OBJ7, H_OBJ8, H_OBJ9, H_OBJ10,
     >  H_OBJ11, H_OBJ12, H_OBJ13, H_OBJ14, H_OBJ15, H_OBJ16, H_OBJ17,
     >  H_OBJ18, H_OBJ19, H_OBJ20)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) OBJ
      TYPE (OpenADTy_active) G_OBJ0
      TYPE (OpenADTy_active) G_OBJ1
      TYPE (OpenADTy_active) G_OBJ2
      TYPE (OpenADTy_active) G_OBJ3
      TYPE (OpenADTy_active) G_OBJ4
      TYPE (OpenADTy_active) G_OBJ5
      TYPE (OpenADTy_active) X0
      TYPE (OpenADTy_active) X1
      TYPE (OpenADTy_active) X2
      TYPE (OpenADTy_active) X3
      TYPE (OpenADTy_active) X4
      TYPE (OpenADTy_active) X5
      TYPE (OpenADTy_active) H_OBJ0
      TYPE (OpenADTy_active) H_OBJ1
      TYPE (OpenADTy_active) H_OBJ2
      TYPE (OpenADTy_active) H_OBJ3
      TYPE (OpenADTy_active) H_OBJ4
      TYPE (OpenADTy_active) H_OBJ5
      TYPE (OpenADTy_active) H_OBJ6
      TYPE (OpenADTy_active) H_OBJ7
      TYPE (OpenADTy_active) H_OBJ8
      TYPE (OpenADTy_active) H_OBJ9
      TYPE (OpenADTy_active) H_OBJ10
      TYPE (OpenADTy_active) H_OBJ11
      TYPE (OpenADTy_active) H_OBJ12
      TYPE (OpenADTy_active) H_OBJ13
      TYPE (OpenADTy_active) H_OBJ14
      TYPE (OpenADTy_active) H_OBJ15
      TYPE (OpenADTy_active) H_OBJ16
      TYPE (OpenADTy_active) H_OBJ17
      TYPE (OpenADTy_active) H_OBJ18
      TYPE (OpenADTy_active) H_OBJ19
      TYPE (OpenADTy_active) H_OBJ20
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) A
      REAL(w2f__8) AB
      TYPE (OpenADTy_active) ADJ_M0
      TYPE (OpenADTy_active) ADJ_M1
      TYPE (OpenADTy_active) ADJ_M2
      TYPE (OpenADTy_active) ADJ_M3
      REAL(w2f__8) B
      REAL(w2f__8) BM1
      TYPE (OpenADTy_active) D00
      TYPE (OpenADTy_active) D01
      TYPE (OpenADTy_active) D03
      TYPE (OpenADTy_active) D11
      TYPE (OpenADTy_active) D12
      TYPE (OpenADTy_active) F
      REAL(w2f__8) FOUR3RD
      TYPE (OpenADTy_active) G
      TYPE (OpenADTy_active) GG
      TYPE (OpenADTy_active) LOC1
      TYPE (OpenADTy_active) LOC2
      TYPE (OpenADTy_active) M00
      TYPE (OpenADTy_active) M01
      TYPE (OpenADTy_active) M02
      TYPE (OpenADTy_active) M03
      TYPE (OpenADTy_active) M11
      TYPE (OpenADTy_active) M12
      TYPE (OpenADTy_active) M13
      TYPE (OpenADTy_active) M22
      TYPE (OpenADTy_active) M23
      TYPE (OpenADTy_active) M33
      TYPE (OpenADTy_active) MATR0
      TYPE (OpenADTy_active) MATR1
      TYPE (OpenADTy_active) MATR2
      TYPE (OpenADTy_active) MATR3
      REAL(w2f__8) NORM
      TYPE (OpenADTy_active) R00
      TYPE (OpenADTy_active) R02
      TYPE (OpenADTy_active) R03
      TYPE (OpenADTy_active) R11
      TYPE (OpenADTy_active) R12
      TYPE (OpenADTy_active) R13
      REAL(w2f__8) R22
      REAL(w2f__8) R33
      REAL(w2f__8) SQRT3
      REAL(w2f__8) THIRD
      REAL(w2f__8) TSQRT3
      REAL(w2f__8) TWO3RD
      REAL(w2f__8) TWOSQRT3
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
      REAL(w2f__8) OpenAD_acc_2
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
      REAL(w2f__8) OpenAD_lin_100
      REAL(w2f__8) OpenAD_lin_101
      REAL(w2f__8) OpenAD_lin_102
      REAL(w2f__8) OpenAD_lin_103
      REAL(w2f__8) OpenAD_lin_104
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
      REAL(w2f__8) OpenAD_lin_49
      REAL(w2f__8) OpenAD_lin_5
      REAL(w2f__8) OpenAD_lin_50
      REAL(w2f__8) OpenAD_lin_51
      REAL(w2f__8) OpenAD_lin_52
      REAL(w2f__8) OpenAD_lin_53
      REAL(w2f__8) OpenAD_lin_54
      REAL(w2f__8) OpenAD_lin_55
      REAL(w2f__8) OpenAD_lin_56
      REAL(w2f__8) OpenAD_lin_57
      REAL(w2f__8) OpenAD_lin_58
      REAL(w2f__8) OpenAD_lin_59
      REAL(w2f__8) OpenAD_lin_6
      REAL(w2f__8) OpenAD_lin_60
      REAL(w2f__8) OpenAD_lin_61
      REAL(w2f__8) OpenAD_lin_62
      REAL(w2f__8) OpenAD_lin_63
      REAL(w2f__8) OpenAD_lin_64
      REAL(w2f__8) OpenAD_lin_65
      REAL(w2f__8) OpenAD_lin_66
      REAL(w2f__8) OpenAD_lin_67
      REAL(w2f__8) OpenAD_lin_68
      REAL(w2f__8) OpenAD_lin_69
      REAL(w2f__8) OpenAD_lin_7
      REAL(w2f__8) OpenAD_lin_70
      REAL(w2f__8) OpenAD_lin_71
      REAL(w2f__8) OpenAD_lin_72
      REAL(w2f__8) OpenAD_lin_73
      REAL(w2f__8) OpenAD_lin_74
      REAL(w2f__8) OpenAD_lin_75
      REAL(w2f__8) OpenAD_lin_76
      REAL(w2f__8) OpenAD_lin_77
      REAL(w2f__8) OpenAD_lin_78
      REAL(w2f__8) OpenAD_lin_79
      REAL(w2f__8) OpenAD_lin_8
      REAL(w2f__8) OpenAD_lin_80
      REAL(w2f__8) OpenAD_lin_81
      REAL(w2f__8) OpenAD_lin_82
      REAL(w2f__8) OpenAD_lin_83
      REAL(w2f__8) OpenAD_lin_84
      REAL(w2f__8) OpenAD_lin_85
      REAL(w2f__8) OpenAD_lin_86
      REAL(w2f__8) OpenAD_lin_87
      REAL(w2f__8) OpenAD_lin_88
      REAL(w2f__8) OpenAD_lin_89
      REAL(w2f__8) OpenAD_lin_9
      REAL(w2f__8) OpenAD_lin_90
      REAL(w2f__8) OpenAD_lin_91
      REAL(w2f__8) OpenAD_lin_92
      REAL(w2f__8) OpenAD_lin_93
      REAL(w2f__8) OpenAD_lin_94
      REAL(w2f__8) OpenAD_lin_95
      REAL(w2f__8) OpenAD_lin_96
      REAL(w2f__8) OpenAD_lin_97
      REAL(w2f__8) OpenAD_lin_98
      REAL(w2f__8) OpenAD_lin_99
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
C$OPENAD INDEPENDENT(X0)
C$OPENAD INDEPENDENT(X1)
C$OPENAD INDEPENDENT(X2)
C$OPENAD INDEPENDENT(X3)
C$OPENAD INDEPENDENT(X4)
C$OPENAD INDEPENDENT(X5)
C$OPENAD DEPENDENT(OBJ)
C$OPENAD DEPENDENT(G_OBJ0)
C$OPENAD DEPENDENT(G_OBJ1)
C$OPENAD DEPENDENT(G_OBJ2)
C$OPENAD DEPENDENT(G_OBJ3)
C$OPENAD DEPENDENT(G_OBJ4)
C$OPENAD DEPENDENT(G_OBJ5)
C$OPENAD DEPENDENT(H_OBJ0)
C$OPENAD DEPENDENT(H_OBJ1)
C$OPENAD DEPENDENT(H_OBJ2)
C$OPENAD DEPENDENT(H_OBJ3)
C$OPENAD DEPENDENT(H_OBJ4)
C$OPENAD DEPENDENT(H_OBJ5)
C$OPENAD DEPENDENT(H_OBJ6)
C$OPENAD DEPENDENT(H_OBJ7)
C$OPENAD DEPENDENT(H_OBJ8)
C$OPENAD DEPENDENT(H_OBJ9)
C$OPENAD DEPENDENT(H_OBJ10)
C$OPENAD DEPENDENT(H_OBJ11)
C$OPENAD DEPENDENT(H_OBJ12)
C$OPENAD DEPENDENT(H_OBJ13)
C$OPENAD DEPENDENT(H_OBJ14)
C$OPENAD DEPENDENT(H_OBJ15)
C$OPENAD DEPENDENT(H_OBJ16)
C$OPENAD DEPENDENT(H_OBJ17)
C$OPENAD DEPENDENT(H_OBJ18)
C$OPENAD DEPENDENT(H_OBJ19)
C$OPENAD DEPENDENT(H_OBJ20)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      A = 5.0D-01
      B = (-1.0D00)
      AB = (A * B)
      SQRT3 = 5.77350269189625842081D-01
      TSQRT3 = 1.15470053837925168416D00
      TWOSQRT3 = 1.15470053837925168416D00
      THIRD = 3.3333333333333331483D-01
      TWO3RD = 6.66666666666666629659D-01
      FOUR3RD = 1.33333333333333325932D00
      BM1 = (-2.0D00)
      __value__(MATR0) = (__value__(X1) - __value__(X0))
      OpenAD_aux_0 = (__value__(X2) * 2.0D00 - __value__(X1) -
     >  __value__(X0))
      OpenAD_lin_0 = SQRT3
      __value__(MATR1) = (SQRT3 * OpenAD_aux_0)
      __value__(MATR2) = (__value__(X4) - __value__(X3))
      OpenAD_aux_1 = (__value__(X5) * 2.0D00 - __value__(X4) -
     >  __value__(X3))
      OpenAD_lin_1 = SQRT3
      __value__(MATR3) = (SQRT3 * OpenAD_aux_1)
      OpenAD_lin_2 = __value__(MATR3)
      OpenAD_lin_3 = __value__(MATR0)
      __value__(M03) = (__value__(MATR0) * __value__(MATR3))
      OpenAD_lin_4 = __value__(MATR2)
      OpenAD_lin_5 = __value__(MATR1)
      __value__(M12) = (__value__(MATR1) * __value__(MATR2))
      __value__(G) = (__value__(M03) - __value__(M12))
      __value__(D00) = (__value__(M03) * 2.0D00)
      __value__(D11) = (__value__(M12) * 2.0D00)
      OpenAD_lin_6 = __value__(MATR0)
      OpenAD_lin_7 = __value__(MATR0)
      __value__(M00) = (__value__(MATR0) * __value__(MATR0))
      OpenAD_lin_8 = __value__(MATR1)
      OpenAD_lin_9 = __value__(MATR1)
      __value__(M11) = (__value__(MATR1) * __value__(MATR1))
      OpenAD_lin_10 = __value__(MATR2)
      OpenAD_lin_11 = __value__(MATR2)
      __value__(M22) = (__value__(MATR2) * __value__(MATR2))
      OpenAD_lin_12 = __value__(MATR3)
      OpenAD_lin_13 = __value__(MATR3)
      __value__(M33) = (__value__(MATR3) * __value__(MATR3))
      OpenAD_lin_14 = __value__(M33)
      OpenAD_lin_15 = __value__(M00)
      __value__(D12) = (__value__(M00) * __value__(M33))
      OpenAD_lin_16 = __value__(M22)
      OpenAD_lin_17 = __value__(M11)
      __value__(D03) = (__value__(M11) * __value__(M22))
      __value__(F) = (__value__(D03) + __value__(D12))
      NORM = __value__(F)
      OpenAD_aux_2 = (1.0D00 / __value__(G))
      OpenAD_lin_19 = (-(1.0D00 /(__value__(G) * __value__(G))))
      OpenAD_lin_18 = A
      __value__(LOC1) = (A * OpenAD_aux_2)
      OpenAD_lin_20 = __value__(LOC1)
      OpenAD_lin_21 = __value__(F)
      __value__(OBJ) = (__value__(F) * __value__(LOC1))
      OpenAD_aux_4 = (__value__(G) * __value__(G))
      OpenAD_aux_3 = (1.0D00 / OpenAD_aux_4)
      OpenAD_lin_24 = __value__(G)
      OpenAD_lin_25 = __value__(G)
      OpenAD_lin_23 = (-(1.0D00 /(OpenAD_aux_4 * OpenAD_aux_4)))
      OpenAD_lin_22 = AB
      __value__(LOC2) = (AB * OpenAD_aux_3)
      OpenAD_aux_5 = (AB * B)
      OpenAD_aux_8 = (__value__(G) * __value__(G))
      OpenAD_aux_7 = (__value__(G) * OpenAD_aux_8)
      OpenAD_aux_6 = (1.0D00 / OpenAD_aux_7)
      OpenAD_lin_28 = OpenAD_aux_8
      OpenAD_lin_30 = __value__(G)
      OpenAD_lin_31 = __value__(G)
      OpenAD_lin_29 = __value__(G)
      OpenAD_lin_27 = (-(1.0D00 /(OpenAD_aux_7 * OpenAD_aux_7)))
      OpenAD_lin_26 = OpenAD_aux_5
      __value__(GG) = (OpenAD_aux_5 * OpenAD_aux_6)
      OpenAD_acc_0 = (OpenAD_lin_19 * OpenAD_lin_18)
      OpenAD_acc_1 = (OpenAD_lin_6 + OpenAD_lin_7)
      OpenAD_acc_2 = (OpenAD_lin_8 + OpenAD_lin_9)
      OpenAD_acc_3 = (OpenAD_lin_10 + OpenAD_lin_11)
      OpenAD_acc_4 = (OpenAD_lin_12 + OpenAD_lin_13)
      OpenAD_acc_5 = ((OpenAD_lin_28 +(OpenAD_lin_30 + OpenAD_lin_31) *
     >  OpenAD_lin_29) * OpenAD_lin_27 * OpenAD_lin_26)
      OpenAD_acc_6 = ((OpenAD_lin_24 + OpenAD_lin_25) * OpenAD_lin_23 *
     >  OpenAD_lin_22)
      CALL setderiv(__deriv__(MATR0), __deriv__(X1))
      CALL dec_deriv(__deriv__(MATR0), __deriv__(X0))
      CALL setderiv(__deriv__(MATR2), __deriv__(X4))
      CALL dec_deriv(__deriv__(MATR2), __deriv__(X3))
      CALL sax(OpenAD_acc_1, __deriv__(MATR0), __deriv__(M00))
      CALL sax(OpenAD_acc_3, __deriv__(MATR2), __deriv__(M22))
      CALL set_neg_deriv(__deriv__(OpenAD_prp_0), __deriv__(X0))
      CALL dec_deriv(__deriv__(OpenAD_prp_0), __deriv__(X1))
      CALL saxpy(2.0D00, __deriv__(X2), __deriv__(OpenAD_prp_0))
      CALL set_neg_deriv(__deriv__(OpenAD_prp_1), __deriv__(X3))
      CALL dec_deriv(__deriv__(OpenAD_prp_1), __deriv__(X4))
      CALL saxpy(2.0D00, __deriv__(X5), __deriv__(OpenAD_prp_1))
      CALL sax(OpenAD_lin_0, __deriv__(OpenAD_prp_0), __deriv__(MATR1))
      CALL sax(OpenAD_lin_1, __deriv__(OpenAD_prp_1), __deriv__(MATR3))
      CALL sax(OpenAD_lin_2, __deriv__(MATR0), __deriv__(M03))
      CALL saxpy(OpenAD_lin_3, __deriv__(MATR3), __deriv__(M03))
      CALL sax(OpenAD_lin_4, __deriv__(MATR1), __deriv__(M12))
      CALL saxpy(OpenAD_lin_5, __deriv__(MATR2), __deriv__(M12))
      CALL setderiv(__deriv__(G), __deriv__(M03))
      CALL dec_deriv(__deriv__(G), __deriv__(M12))
      CALL sax(2.0D00, __deriv__(M03), __deriv__(D00))
      CALL sax(2.0D00, __deriv__(M12), __deriv__(D11))
      CALL sax(OpenAD_acc_2, __deriv__(MATR1), __deriv__(M11))
      CALL sax(OpenAD_acc_4, __deriv__(MATR3), __deriv__(M33))
      CALL sax(OpenAD_lin_14, __deriv__(M00), __deriv__(D12))
      CALL saxpy(OpenAD_lin_15, __deriv__(M33), __deriv__(D12))
      CALL sax(OpenAD_lin_16, __deriv__(M11), __deriv__(D03))
      CALL saxpy(OpenAD_lin_17, __deriv__(M22), __deriv__(D03))
      CALL setderiv(__deriv__(F), __deriv__(D03))
      CALL inc_deriv(__deriv__(F), __deriv__(D12))
      CALL sax(OpenAD_acc_0, __deriv__(G), __deriv__(LOC1))
      CALL sax(OpenAD_lin_20, __deriv__(F), __deriv__(OBJ))
      CALL saxpy(OpenAD_lin_21, __deriv__(LOC1), __deriv__(OBJ))
      CALL sax(OpenAD_acc_6, __deriv__(G), __deriv__(LOC2))
      CALL sax(OpenAD_acc_5, __deriv__(G), __deriv__(GG))
      OpenAD_lin_32 = __value__(LOC2)
      OpenAD_lin_33 = __value__(F)
      __value__(G) = (__value__(F) * __value__(LOC2))
      __value__(F) = (__value__(LOC1) * 2.0D00)
      OpenAD_lin_34 = __value__(MATR0)
      OpenAD_lin_35 = __value__(F)
      OpenAD_lin_36 = __value__(MATR3)
      OpenAD_lin_37 = __value__(G)
      __value__(ADJ_M0) = (__value__(F) * __value__(MATR0) + __value__(
     > G) * __value__(MATR3))
      OpenAD_lin_38 = __value__(MATR1)
      OpenAD_lin_39 = __value__(F)
      OpenAD_lin_40 = __value__(MATR2)
      OpenAD_lin_41 = __value__(G)
      __value__(ADJ_M1) = (__value__(F) * __value__(MATR1) - __value__(
     > G) * __value__(MATR2))
      OpenAD_lin_42 = __value__(MATR2)
      OpenAD_lin_43 = __value__(F)
      OpenAD_lin_44 = __value__(MATR1)
      OpenAD_lin_45 = __value__(G)
      __value__(ADJ_M2) = (__value__(F) * __value__(MATR2) - __value__(
     > G) * __value__(MATR1))
      OpenAD_lin_46 = __value__(MATR3)
      OpenAD_lin_47 = __value__(F)
      OpenAD_lin_48 = __value__(MATR0)
      OpenAD_lin_49 = __value__(G)
      __value__(ADJ_M3) = (__value__(F) * __value__(MATR3) + __value__(
     > G) * __value__(MATR0))
      OpenAD_lin_50 = SQRT3
      __value__(LOC1) = (__value__(ADJ_M1) * SQRT3)
      __value__(G_OBJ0) = (-(__value__(ADJ_M0) + __value__(LOC1)))
      __value__(G_OBJ1) = (__value__(ADJ_M0) - __value__(LOC1))
      __value__(G_OBJ2) = (__value__(LOC1) * 2.0D00)
      OpenAD_acc_7 = (2.0D00 * OpenAD_lin_34)
      OpenAD_acc_8 = (2.0D00 * OpenAD_lin_38)
      OpenAD_acc_9 = (OpenAD_lin_40 * INT((-1_w2f__i8)))
      OpenAD_acc_10 = (OpenAD_lin_41 * INT((-1_w2f__i8)))
      OpenAD_acc_11 = (2.0D00 * OpenAD_lin_42)
      OpenAD_acc_12 = (OpenAD_lin_44 * INT((-1_w2f__i8)))
      OpenAD_acc_13 = (OpenAD_lin_45 * INT((-1_w2f__i8)))
      OpenAD_acc_14 = (2.0D00 * OpenAD_lin_46)
      CALL setderiv(__deriv__(OpenAD_prp_2), __deriv__(LOC1))
      CALL sax(OpenAD_lin_32, __deriv__(F), __deriv__(G))
      CALL saxpy(OpenAD_lin_33, __deriv__(LOC2), __deriv__(G))
      CALL sax(OpenAD_lin_35, __deriv__(MATR0), __deriv__(ADJ_M0))
      CALL saxpy(OpenAD_acc_7, __deriv__(OpenAD_prp_2), __deriv__(
     > ADJ_M0))
      CALL saxpy(OpenAD_lin_36, __deriv__(G), __deriv__(ADJ_M0))
      CALL saxpy(OpenAD_lin_37, __deriv__(MATR3), __deriv__(ADJ_M0))
      CALL sax(OpenAD_lin_39, __deriv__(MATR1), __deriv__(ADJ_M1))
      CALL saxpy(OpenAD_acc_8, __deriv__(OpenAD_prp_2), __deriv__(
     > ADJ_M1))
      CALL saxpy(OpenAD_acc_9, __deriv__(G), __deriv__(ADJ_M1))
      CALL saxpy(OpenAD_acc_10, __deriv__(MATR2), __deriv__(ADJ_M1))
      CALL sax(OpenAD_lin_43, __deriv__(MATR2), __deriv__(ADJ_M2))
      CALL saxpy(OpenAD_acc_11, __deriv__(OpenAD_prp_2), __deriv__(
     > ADJ_M2))
      CALL saxpy(OpenAD_acc_12, __deriv__(G), __deriv__(ADJ_M2))
      CALL saxpy(OpenAD_acc_13, __deriv__(MATR1), __deriv__(ADJ_M2))
      CALL sax(OpenAD_lin_47, __deriv__(MATR3), __deriv__(ADJ_M3))
      CALL saxpy(OpenAD_acc_14, __deriv__(OpenAD_prp_2), __deriv__(
     > ADJ_M3))
      CALL saxpy(OpenAD_lin_48, __deriv__(G), __deriv__(ADJ_M3))
      CALL saxpy(OpenAD_lin_49, __deriv__(MATR0), __deriv__(ADJ_M3))
      CALL sax(OpenAD_lin_50, __deriv__(ADJ_M1), __deriv__(LOC1))
      CALL setderiv(__deriv__(G_OBJ1), __deriv__(ADJ_M0))
      CALL dec_deriv(__deriv__(G_OBJ1), __deriv__(LOC1))
      CALL sax(2.0D00, __deriv__(LOC1), __deriv__(G_OBJ2))
      CALL set_neg_deriv(__deriv__(G_OBJ0), __deriv__(ADJ_M0))
      CALL dec_deriv(__deriv__(G_OBJ0), __deriv__(LOC1))
      OpenAD_lin_51 = SQRT3
      __value__(LOC1) = (__value__(ADJ_M3) * SQRT3)
      __value__(G_OBJ3) = (-(__value__(ADJ_M2) + __value__(LOC1)))
      __value__(G_OBJ4) = (__value__(ADJ_M2) - __value__(LOC1))
      __value__(G_OBJ5) = (__value__(LOC1) * 2.0D00)
      __value__(LOC2) = (__value__(LOC2) * 2.0D00)
      OpenAD_lin_52 = __value__(LOC2)
      OpenAD_lin_53 = __value__(D00)
      OpenAD_lin_54 = __value__(M33)
      OpenAD_lin_55 = __value__(GG)
      __value__(R00) = (__value__(D00) * __value__(LOC2) + __value__(GG
     > ) * __value__(M33))
      OpenAD_lin_56 = __value__(LOC2)
      OpenAD_lin_57 = __value__(D11)
      OpenAD_lin_58 = __value__(M22)
      OpenAD_lin_59 = __value__(GG)
      __value__(R11) = (__value__(D11) * __value__(LOC2) + __value__(GG
     > ) * __value__(M22))
      R22 = (__value__(D11) * __value__(LOC2) + __value__(GG) *
     >  __value__(M11))
      R33 = (__value__(D00) * __value__(LOC2) + __value__(GG) *
     >  __value__(M00))
      OpenAD_lin_60 = __value__(MATR1)
      OpenAD_lin_61 = __value__(MATR0)
      __value__(M01) = (__value__(MATR0) * __value__(MATR1))
      OpenAD_lin_62 = __value__(MATR2)
      OpenAD_lin_63 = __value__(MATR0)
      __value__(M02) = (__value__(MATR0) * __value__(MATR2))
      OpenAD_lin_64 = __value__(MATR2)
      OpenAD_lin_65 = __value__(MATR1)
      __value__(M12) = (__value__(MATR1) * __value__(MATR2))
      OpenAD_lin_66 = __value__(MATR3)
      OpenAD_lin_67 = __value__(MATR1)
      __value__(M13) = (__value__(MATR1) * __value__(MATR3))
      OpenAD_lin_68 = __value__(MATR3)
      OpenAD_lin_69 = __value__(MATR2)
      __value__(M23) = (__value__(MATR2) * __value__(MATR3))
      OpenAD_aux_9 = (__value__(M02) + __value__(M13))
      OpenAD_lin_70 = OpenAD_aux_9
      OpenAD_lin_71 = __value__(LOC2)
      __value__(D01) = (__value__(LOC2) * OpenAD_aux_9)
      CALL setderiv(__deriv__(OpenAD_prp_3), __deriv__(LOC2))
      CALL sax(OpenAD_lin_51, __deriv__(ADJ_M3), __deriv__(LOC1))
      CALL setderiv(__deriv__(G_OBJ4), __deriv__(ADJ_M2))
      CALL dec_deriv(__deriv__(G_OBJ4), __deriv__(LOC1))
      CALL sax(2.0D00, __deriv__(LOC1), __deriv__(G_OBJ5))
      CALL sax(2.0D00, __deriv__(OpenAD_prp_3), __deriv__(LOC2))
      CALL sax(OpenAD_lin_60, __deriv__(MATR0), __deriv__(M01))
      CALL saxpy(OpenAD_lin_61, __deriv__(MATR1), __deriv__(M01))
      CALL sax(OpenAD_lin_62, __deriv__(MATR0), __deriv__(M02))
      CALL saxpy(OpenAD_lin_63, __deriv__(MATR2), __deriv__(M02))
      CALL sax(OpenAD_lin_64, __deriv__(MATR1), __deriv__(M12))
      CALL saxpy(OpenAD_lin_65, __deriv__(MATR2), __deriv__(M12))
      CALL sax(OpenAD_lin_66, __deriv__(MATR1), __deriv__(M13))
      CALL saxpy(OpenAD_lin_67, __deriv__(MATR3), __deriv__(M13))
      CALL sax(OpenAD_lin_68, __deriv__(MATR2), __deriv__(M23))
      CALL saxpy(OpenAD_lin_69, __deriv__(MATR3), __deriv__(M23))
      CALL setderiv(__deriv__(OpenAD_prp_4), __deriv__(M02))
      CALL inc_deriv(__deriv__(OpenAD_prp_4), __deriv__(M13))
      CALL set_neg_deriv(__deriv__(G_OBJ3), __deriv__(ADJ_M2))
      CALL dec_deriv(__deriv__(G_OBJ3), __deriv__(LOC1))
      CALL sax(OpenAD_lin_52, __deriv__(D00), __deriv__(R00))
      CALL saxpy(OpenAD_lin_53, __deriv__(LOC2), __deriv__(R00))
      CALL saxpy(OpenAD_lin_54, __deriv__(GG), __deriv__(R00))
      CALL saxpy(OpenAD_lin_55, __deriv__(M33), __deriv__(R00))
      CALL sax(OpenAD_lin_56, __deriv__(D11), __deriv__(R11))
      CALL saxpy(OpenAD_lin_57, __deriv__(LOC2), __deriv__(R11))
      CALL saxpy(OpenAD_lin_58, __deriv__(GG), __deriv__(R11))
      CALL saxpy(OpenAD_lin_59, __deriv__(M22), __deriv__(R11))
      CALL sax(OpenAD_lin_70, __deriv__(LOC2), __deriv__(D01))
      CALL saxpy(OpenAD_lin_71, __deriv__(OpenAD_prp_4), __deriv__(D01)
     > )
      OpenAD_lin_72 = THIRD
      __value__(LOC1) = (__value__(R11) * THIRD)
      OpenAD_aux_10 = (__value__(D01) + __value__(GG) * __value__(M23))
      OpenAD_lin_74 = __value__(M23)
      OpenAD_lin_75 = __value__(GG)
      OpenAD_lin_73 = TWOSQRT3
      __value__(F) = (TWOSQRT3 * OpenAD_aux_10)
      __value__(G) = (__value__(LOC1) + __value__(R00))
      __value__(H_OBJ1) = (__value__(LOC1) - __value__(R00))
      __value__(H_OBJ0) = (__value__(F) + __value__(G))
      __value__(H_OBJ3) = (__value__(G) - __value__(F))
      CALL sax(OpenAD_lin_72, __deriv__(R11), __deriv__(LOC1))
      CALL setderiv(__deriv__(G), __deriv__(LOC1))
      CALL inc_deriv(__deriv__(G), __deriv__(R00))
      CALL setderiv(__deriv__(H_OBJ1), __deriv__(LOC1))
      CALL dec_deriv(__deriv__(H_OBJ1), __deriv__(R00))
      CALL setderiv(__deriv__(OpenAD_prp_5), __deriv__(D01))
      CALL saxpy(OpenAD_lin_74, __deriv__(GG), __deriv__(OpenAD_prp_5))
      CALL saxpy(OpenAD_lin_75, __deriv__(M23), __deriv__(OpenAD_prp_5)
     > )
      CALL sax(OpenAD_lin_73, __deriv__(OpenAD_prp_5), __deriv__(F))
      CALL setderiv(__deriv__(H_OBJ0), __deriv__(F))
      CALL inc_deriv(__deriv__(H_OBJ0), __deriv__(G))
      CALL setderiv(__deriv__(H_OBJ3), __deriv__(G))
      CALL dec_deriv(__deriv__(H_OBJ3), __deriv__(F))
      OpenAD_lin_76 = TWO3RD
      __value__(LOC1) = (-(__value__(R11) * TWO3RD))
      __value__(H_OBJ2) = (__value__(LOC1) - __value__(F))
      __value__(H_OBJ4) = (__value__(F) + __value__(LOC1))
      OpenAD_lin_77 = FOUR3RD
      __value__(H_OBJ5) = (FOUR3RD * __value__(R11))
      OpenAD_acc_15 = (OpenAD_lin_76 * INT((-1_w2f__i8)))
      CALL sax(OpenAD_lin_77, __deriv__(R11), __deriv__(H_OBJ5))
      CALL sax(OpenAD_acc_15, __deriv__(R11), __deriv__(LOC1))
      CALL setderiv(__deriv__(H_OBJ2), __deriv__(LOC1))
      CALL dec_deriv(__deriv__(H_OBJ2), __deriv__(F))
      CALL setderiv(__deriv__(H_OBJ4), __deriv__(F))
      CALL inc_deriv(__deriv__(H_OBJ4), __deriv__(LOC1))
      OpenAD_lin_78 = THIRD
      __value__(LOC1) = (__value__(R00) * THIRD)
      OpenAD_aux_11 = (__value__(D01) + __value__(GG) * __value__(M01))
      OpenAD_lin_80 = __value__(M01)
      OpenAD_lin_81 = __value__(GG)
      OpenAD_lin_79 = TWOSQRT3
      __value__(F) = (TWOSQRT3 * OpenAD_aux_11)
      __value__(G) = (__value__(LOC1) + __value__(R11))
      __value__(H_OBJ16) = (__value__(LOC1) - __value__(R11))
      __value__(H_OBJ15) = (__value__(F) + __value__(G))
      __value__(H_OBJ18) = (__value__(G) - __value__(F))
      CALL sax(OpenAD_lin_78, __deriv__(R00), __deriv__(LOC1))
      CALL setderiv(__deriv__(G), __deriv__(LOC1))
      CALL inc_deriv(__deriv__(G), __deriv__(R11))
      CALL setderiv(__deriv__(H_OBJ16), __deriv__(LOC1))
      CALL dec_deriv(__deriv__(H_OBJ16), __deriv__(R11))
      CALL setderiv(__deriv__(OpenAD_prp_6), __deriv__(D01))
      CALL saxpy(OpenAD_lin_80, __deriv__(GG), __deriv__(OpenAD_prp_6))
      CALL saxpy(OpenAD_lin_81, __deriv__(M01), __deriv__(OpenAD_prp_6)
     > )
      CALL sax(OpenAD_lin_79, __deriv__(OpenAD_prp_6), __deriv__(F))
      CALL setderiv(__deriv__(H_OBJ15), __deriv__(F))
      CALL inc_deriv(__deriv__(H_OBJ15), __deriv__(G))
      CALL setderiv(__deriv__(H_OBJ18), __deriv__(G))
      CALL dec_deriv(__deriv__(H_OBJ18), __deriv__(F))
      OpenAD_lin_82 = TWO3RD
      __value__(LOC1) = (-(__value__(R00) * TWO3RD))
      __value__(H_OBJ17) = (__value__(LOC1) - __value__(F))
      __value__(H_OBJ19) = (__value__(F) + __value__(LOC1))
      OpenAD_lin_83 = FOUR3RD
      __value__(H_OBJ20) = (FOUR3RD * __value__(R00))
      OpenAD_acc_16 = (OpenAD_lin_82 * INT((-1_w2f__i8)))
      CALL sax(OpenAD_lin_83, __deriv__(R00), __deriv__(H_OBJ20))
      CALL sax(OpenAD_acc_16, __deriv__(R00), __deriv__(LOC1))
      CALL setderiv(__deriv__(H_OBJ17), __deriv__(LOC1))
      CALL dec_deriv(__deriv__(H_OBJ17), __deriv__(F))
      CALL setderiv(__deriv__(H_OBJ19), __deriv__(F))
      CALL inc_deriv(__deriv__(H_OBJ19), __deriv__(LOC1))
      OpenAD_aux_12 = (__value__(M01) + __value__(M23))
      OpenAD_lin_84 = OpenAD_aux_12
      OpenAD_lin_85 = __value__(LOC2)
      __value__(LOC1) = (__value__(LOC2) * OpenAD_aux_12)
      OpenAD_lin_86 = __value__(M02)
      OpenAD_lin_87 = __value__(GG)
      __value__(R13) = (__value__(LOC1) + __value__(GG) * __value__(M02
     > ))
      OpenAD_lin_88 = __value__(M13)
      OpenAD_lin_89 = __value__(GG)
      __value__(R02) = (__value__(LOC1) + __value__(GG) * __value__(M13
     > ))
      OpenAD_lin_90 = __value__(LOC2)
      OpenAD_lin_91 = __value__(D03)
      OpenAD_lin_92 = __value__(M03)
      OpenAD_lin_93 = __value__(GG)
      __value__(R03) = (__value__(D03) * __value__(LOC2) + __value__(GG
     > ) * __value__(M03))
      OpenAD_lin_94 = __value__(LOC2)
      OpenAD_lin_95 = __value__(D12)
      OpenAD_lin_96 = __value__(M12)
      OpenAD_lin_97 = __value__(GG)
      __value__(R12) = (__value__(D12) * __value__(LOC2) + __value__(GG
     > ) * __value__(M12))
      OpenAD_lin_98 = TWO3RD
      __value__(G) = (-(__value__(R13) * TWO3RD))
      OpenAD_lin_99 = TWOSQRT3
      __value__(F) = (__value__(R03) * TWOSQRT3)
      __value__(H_OBJ8) = (__value__(G) - __value__(F))
      __value__(H_OBJ11) = (__value__(F) + __value__(G))
      OpenAD_acc_17 = (OpenAD_lin_98 * INT((-1_w2f__i8)))
      CALL setderiv(__deriv__(OpenAD_prp_7), __deriv__(M01))
      CALL inc_deriv(__deriv__(OpenAD_prp_7), __deriv__(M23))
      CALL sax(OpenAD_lin_84, __deriv__(LOC2), __deriv__(LOC1))
      CALL saxpy(OpenAD_lin_85, __deriv__(OpenAD_prp_7), __deriv__(LOC1
     > ))
      CALL setderiv(__deriv__(R13), __deriv__(LOC1))
      CALL saxpy(OpenAD_lin_86, __deriv__(GG), __deriv__(R13))
      CALL saxpy(OpenAD_lin_87, __deriv__(M02), __deriv__(R13))
      CALL setderiv(__deriv__(R02), __deriv__(LOC1))
      CALL saxpy(OpenAD_lin_88, __deriv__(GG), __deriv__(R02))
      CALL saxpy(OpenAD_lin_89, __deriv__(M13), __deriv__(R02))
      CALL sax(OpenAD_lin_90, __deriv__(D03), __deriv__(R03))
      CALL saxpy(OpenAD_lin_91, __deriv__(LOC2), __deriv__(R03))
      CALL saxpy(OpenAD_lin_92, __deriv__(GG), __deriv__(R03))
      CALL saxpy(OpenAD_lin_93, __deriv__(M03), __deriv__(R03))
      CALL sax(OpenAD_lin_94, __deriv__(D12), __deriv__(R12))
      CALL saxpy(OpenAD_lin_95, __deriv__(LOC2), __deriv__(R12))
      CALL saxpy(OpenAD_lin_96, __deriv__(GG), __deriv__(R12))
      CALL saxpy(OpenAD_lin_97, __deriv__(M12), __deriv__(R12))
      CALL sax(OpenAD_lin_99, __deriv__(R03), __deriv__(F))
      CALL sax(OpenAD_acc_17, __deriv__(R13), __deriv__(G))
      CALL setderiv(__deriv__(H_OBJ8), __deriv__(G))
      CALL dec_deriv(__deriv__(H_OBJ8), __deriv__(F))
      CALL setderiv(__deriv__(H_OBJ11), __deriv__(F))
      CALL inc_deriv(__deriv__(H_OBJ11), __deriv__(G))
      OpenAD_lin_100 = TWOSQRT3
      __value__(F) = (__value__(R12) * TWOSQRT3)
      __value__(H_OBJ12) = (__value__(G) - __value__(F))
      __value__(H_OBJ13) = (__value__(F) + __value__(G))
      __value__(H_OBJ14) = (__value__(R13) * 1.2D+01)
      OpenAD_lin_101 = FOUR3RD
      __value__(G) = (FOUR3RD * __value__(R02))
      CALL setderiv(__deriv__(OpenAD_prp_8), __deriv__(G))
      CALL sax(OpenAD_lin_100, __deriv__(R12), __deriv__(F))
      CALL setderiv(__deriv__(H_OBJ12), __deriv__(OpenAD_prp_8))
      CALL dec_deriv(__deriv__(H_OBJ12), __deriv__(F))
      CALL setderiv(__deriv__(H_OBJ13), __deriv__(F))
      CALL inc_deriv(__deriv__(H_OBJ13), __deriv__(OpenAD_prp_8))
      CALL sax(1.2D+01, __deriv__(R13), __deriv__(H_OBJ14))
      CALL sax(OpenAD_lin_101, __deriv__(R02), __deriv__(G))
      OpenAD_aux_13 = (__value__(R03) + __value__(R12))
      OpenAD_lin_102 = SQRT3
      __value__(F) = (SQRT3 * OpenAD_aux_13)
      __value__(H_OBJ10) = (__value__(G) - __value__(F))
      __value__(H_OBJ6) = (__value__(F) + __value__(G))
      OpenAD_lin_103 = TWO3RD
      __value__(G) = (__value__(R02) * TWO3RD)
      CALL setderiv(__deriv__(OpenAD_prp_9), __deriv__(G))
      CALL setderiv(__deriv__(OpenAD_prp_10), __deriv__(R03))
      CALL inc_deriv(__deriv__(OpenAD_prp_10), __deriv__(R12))
      CALL sax(OpenAD_lin_103, __deriv__(R02), __deriv__(G))
      CALL sax(OpenAD_lin_102, __deriv__(OpenAD_prp_10), __deriv__(F))
      CALL setderiv(__deriv__(H_OBJ10), __deriv__(OpenAD_prp_9))
      CALL dec_deriv(__deriv__(H_OBJ10), __deriv__(F))
      CALL setderiv(__deriv__(H_OBJ6), __deriv__(F))
      CALL inc_deriv(__deriv__(H_OBJ6), __deriv__(OpenAD_prp_9))
      OpenAD_aux_14 = (__value__(R03) - __value__(R12))
      OpenAD_lin_104 = SQRT3
      __value__(F) = (SQRT3 * OpenAD_aux_14)
      __value__(H_OBJ9) = (__value__(G) - __value__(F))
      __value__(H_OBJ7) = (__value__(F) + __value__(G))
      CALL setderiv(__deriv__(OpenAD_prp_11), __deriv__(R03))
      CALL dec_deriv(__deriv__(OpenAD_prp_11), __deriv__(R12))
      CALL sax(OpenAD_lin_104, __deriv__(OpenAD_prp_11), __deriv__(F))
      CALL setderiv(__deriv__(H_OBJ9), __deriv__(G))
      CALL dec_deriv(__deriv__(H_OBJ9), __deriv__(F))
      CALL setderiv(__deriv__(H_OBJ7), __deriv__(F))
      CALL inc_deriv(__deriv__(H_OBJ7), __deriv__(G))
      END SUBROUTINE
