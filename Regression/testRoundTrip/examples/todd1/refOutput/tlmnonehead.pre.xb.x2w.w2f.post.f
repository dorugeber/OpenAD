
C$OPENAD XXX File_start [head.f]
      SUBROUTINE head(OBJ, G_OBJ0, G_OBJ1, G_OBJ2, G_OBJ3, G_OBJ4, G_OBJ
     +5, X0, X1, X2, X3, X4, X5, H_OBJ0, H_OBJ1, H_OBJ2, H_OBJ3, H_OBJ4,
     + H_OBJ5, H_OBJ6, H_OBJ7, H_OBJ8, H_OBJ9, H_OBJ10, H_OBJ11, H_OBJ12
     +, H_OBJ13, H_OBJ14, H_OBJ15, H_OBJ16, H_OBJ17, H_OBJ18, H_OBJ19, H
     +_OBJ20)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: OBJ
      type(active) :: G_OBJ0
      type(active) :: G_OBJ1
      type(active) :: G_OBJ2
      type(active) :: G_OBJ3
      type(active) :: G_OBJ4
      type(active) :: G_OBJ5
      type(active) :: X0
      type(active) :: X1
      type(active) :: X2
      type(active) :: X3
      type(active) :: X4
      type(active) :: X5
      type(active) :: H_OBJ0
      type(active) :: H_OBJ1
      type(active) :: H_OBJ2
      type(active) :: H_OBJ3
      type(active) :: H_OBJ4
      type(active) :: H_OBJ5
      type(active) :: H_OBJ6
      type(active) :: H_OBJ7
      type(active) :: H_OBJ8
      type(active) :: H_OBJ9
      type(active) :: H_OBJ10
      type(active) :: H_OBJ11
      type(active) :: H_OBJ12
      type(active) :: H_OBJ13
      type(active) :: H_OBJ14
      type(active) :: H_OBJ15
      type(active) :: H_OBJ16
      type(active) :: H_OBJ17
      type(active) :: H_OBJ18
      type(active) :: H_OBJ19
      type(active) :: H_OBJ20
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) A
      REAL(w2f__8) AB
      type(active) :: ADJ_M0
      type(active) :: ADJ_M1
      type(active) :: ADJ_M2
      type(active) :: ADJ_M3
      REAL(w2f__8) B
      REAL(w2f__8) BM1
      type(active) :: D00
      type(active) :: D01
      type(active) :: D03
      type(active) :: D11
      type(active) :: D12
      type(active) :: F
      REAL(w2f__8) FOUR3RD
      type(active) :: G
      type(active) :: GG
      type(active) :: LOC1
      type(active) :: LOC2
      type(active) :: M00
      type(active) :: M01
      type(active) :: M02
      type(active) :: M03
      type(active) :: M11
      type(active) :: M12
      type(active) :: M13
      type(active) :: M22
      type(active) :: M23
      type(active) :: M33
      type(active) :: MATR0
      type(active) :: MATR1
      type(active) :: MATR2
      type(active) :: MATR3
      REAL(w2f__8) NORM
      type(active) :: R00
      type(active) :: R02
      type(active) :: R03
      type(active) :: R11
      type(active) :: R12
      type(active) :: R13
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
      type(active) :: OpenAD_prp_0
      type(active) :: OpenAD_prp_1
      type(active) :: OpenAD_prp_10
      type(active) :: OpenAD_prp_11
      type(active) :: OpenAD_prp_2
      type(active) :: OpenAD_prp_3
      type(active) :: OpenAD_prp_4
      type(active) :: OpenAD_prp_5
      type(active) :: OpenAD_prp_6
      type(active) :: OpenAD_prp_7
      type(active) :: OpenAD_prp_8
      type(active) :: OpenAD_prp_9
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
      MATR0%v = (X1%v-X0%v)
      OpenAD_aux_0 = (X2%v*2.0D00-X1%v-X0%v)
      OpenAD_lin_0 = SQRT3
      MATR1%v = (SQRT3*OpenAD_aux_0)
      MATR2%v = (X4%v-X3%v)
      OpenAD_aux_1 = (X5%v*2.0D00-X4%v-X3%v)
      OpenAD_lin_1 = SQRT3
      MATR3%v = (SQRT3*OpenAD_aux_1)
      OpenAD_lin_2 = MATR3%v
      OpenAD_lin_3 = MATR0%v
      M03%v = (MATR0%v*MATR3%v)
      OpenAD_lin_4 = MATR2%v
      OpenAD_lin_5 = MATR1%v
      M12%v = (MATR1%v*MATR2%v)
      G%v = (M03%v-M12%v)
      D00%v = (M03%v*2.0D00)
      D11%v = (M12%v*2.0D00)
      OpenAD_lin_6 = MATR0%v
      OpenAD_lin_7 = MATR0%v
      M00%v = (MATR0%v*MATR0%v)
      OpenAD_lin_8 = MATR1%v
      OpenAD_lin_9 = MATR1%v
      M11%v = (MATR1%v*MATR1%v)
      OpenAD_lin_10 = MATR2%v
      OpenAD_lin_11 = MATR2%v
      M22%v = (MATR2%v*MATR2%v)
      OpenAD_lin_12 = MATR3%v
      OpenAD_lin_13 = MATR3%v
      M33%v = (MATR3%v*MATR3%v)
      OpenAD_lin_14 = M33%v
      OpenAD_lin_15 = M00%v
      D12%v = (M00%v*M33%v)
      OpenAD_lin_16 = M22%v
      OpenAD_lin_17 = M11%v
      D03%v = (M11%v*M22%v)
      F%v = (D03%v+D12%v)
      NORM = F%v
      OpenAD_aux_2 = (1.0D00/G%v)
      OpenAD_lin_19 = (-(1.0D00/(G%v*G%v)))
      OpenAD_lin_18 = A
      LOC1%v = (A*OpenAD_aux_2)
      OpenAD_lin_20 = LOC1%v
      OpenAD_lin_21 = F%v
      OBJ%v = (F%v*LOC1%v)
      OpenAD_aux_4 = (G%v*G%v)
      OpenAD_aux_3 = (1.0D00/OpenAD_aux_4)
      OpenAD_lin_24 = G%v
      OpenAD_lin_25 = G%v
      OpenAD_lin_23 = (-(1.0D00/(OpenAD_aux_4*OpenAD_aux_4)))
      OpenAD_lin_22 = AB
      LOC2%v = (AB*OpenAD_aux_3)
      OpenAD_aux_5 = (AB*B)
      OpenAD_aux_8 = (G%v*G%v)
      OpenAD_aux_7 = (G%v*OpenAD_aux_8)
      OpenAD_aux_6 = (1.0D00/OpenAD_aux_7)
      OpenAD_lin_28 = OpenAD_aux_8
      OpenAD_lin_30 = G%v
      OpenAD_lin_31 = G%v
      OpenAD_lin_29 = G%v
      OpenAD_lin_27 = (-(1.0D00/(OpenAD_aux_7*OpenAD_aux_7)))
      OpenAD_lin_26 = OpenAD_aux_5
      GG%v = (OpenAD_aux_5*OpenAD_aux_6)
      OpenAD_acc_0 = (OpenAD_lin_19*OpenAD_lin_18)
      OpenAD_acc_1 = (OpenAD_lin_6+OpenAD_lin_7)
      OpenAD_acc_2 = (OpenAD_lin_8+OpenAD_lin_9)
      OpenAD_acc_3 = (OpenAD_lin_10+OpenAD_lin_11)
      OpenAD_acc_4 = (OpenAD_lin_12+OpenAD_lin_13)
      OpenAD_acc_5 = ((OpenAD_lin_28+(OpenAD_lin_30+OpenAD_lin_31)*OpenA
     +D_lin_29)*OpenAD_lin_27*OpenAD_lin_26)
      OpenAD_acc_6 = ((OpenAD_lin_24+OpenAD_lin_25)*OpenAD_lin_23*OpenAD
     +_lin_22)
      CALL setderiv(MATR0,X1)
      CALL dec_deriv(MATR0,X0)
      CALL setderiv(MATR2,X4)
      CALL dec_deriv(MATR2,X3)
      CALL sax(OpenAD_acc_1,MATR0,M00)
      CALL sax(OpenAD_acc_3,MATR2,M22)
      CALL set_neg_deriv(OpenAD_prp_0,X0)
      CALL dec_deriv(OpenAD_prp_0,X1)
      CALL saxpy(2.0D00,X2,OpenAD_prp_0)
      CALL set_neg_deriv(OpenAD_prp_1,X3)
      CALL dec_deriv(OpenAD_prp_1,X4)
      CALL saxpy(2.0D00,X5,OpenAD_prp_1)
      CALL sax(OpenAD_lin_0,OpenAD_prp_0,MATR1)
      CALL sax(OpenAD_lin_1,OpenAD_prp_1,MATR3)
      CALL sax(OpenAD_lin_2,MATR0,M03)
      CALL saxpy(OpenAD_lin_3,MATR3,M03)
      CALL sax(OpenAD_lin_4,MATR1,M12)
      CALL saxpy(OpenAD_lin_5,MATR2,M12)
      CALL setderiv(G,M03)
      CALL dec_deriv(G,M12)
      CALL sax(2.0D00,M03,D00)
      CALL sax(2.0D00,M12,D11)
      CALL sax(OpenAD_acc_2,MATR1,M11)
      CALL sax(OpenAD_acc_4,MATR3,M33)
      CALL sax(OpenAD_lin_14,M00,D12)
      CALL saxpy(OpenAD_lin_15,M33,D12)
      CALL sax(OpenAD_lin_16,M11,D03)
      CALL saxpy(OpenAD_lin_17,M22,D03)
      CALL setderiv(F,D03)
      CALL inc_deriv(F,D12)
      CALL sax(OpenAD_acc_0,G,LOC1)
      CALL sax(OpenAD_lin_20,F,OBJ)
      CALL saxpy(OpenAD_lin_21,LOC1,OBJ)
      CALL sax(OpenAD_acc_6,G,LOC2)
      CALL sax(OpenAD_acc_5,G,GG)
      OpenAD_lin_32 = LOC2%v
      OpenAD_lin_33 = F%v
      G%v = (F%v*LOC2%v)
      F%v = (LOC1%v*2.0D00)
      OpenAD_lin_34 = MATR0%v
      OpenAD_lin_35 = F%v
      OpenAD_lin_36 = MATR3%v
      OpenAD_lin_37 = G%v
      ADJ_M0%v = (F%v*MATR0%v+G%v*MATR3%v)
      OpenAD_lin_38 = MATR1%v
      OpenAD_lin_39 = F%v
      OpenAD_lin_40 = MATR2%v
      OpenAD_lin_41 = G%v
      ADJ_M1%v = (F%v*MATR1%v-G%v*MATR2%v)
      OpenAD_lin_42 = MATR2%v
      OpenAD_lin_43 = F%v
      OpenAD_lin_44 = MATR1%v
      OpenAD_lin_45 = G%v
      ADJ_M2%v = (F%v*MATR2%v-G%v*MATR1%v)
      OpenAD_lin_46 = MATR3%v
      OpenAD_lin_47 = F%v
      OpenAD_lin_48 = MATR0%v
      OpenAD_lin_49 = G%v
      ADJ_M3%v = (F%v*MATR3%v+G%v*MATR0%v)
      OpenAD_lin_50 = SQRT3
      LOC1%v = (ADJ_M1%v*SQRT3)
      G_OBJ0%v = (-(ADJ_M0%v+LOC1%v))
      G_OBJ1%v = (ADJ_M0%v-LOC1%v)
      G_OBJ2%v = (LOC1%v*2.0D00)
      OpenAD_acc_7 = (2.0D00*OpenAD_lin_34)
      OpenAD_acc_8 = (2.0D00*OpenAD_lin_38)
      OpenAD_acc_9 = (OpenAD_lin_40*INT((-1_w2f__i8)))
      OpenAD_acc_10 = (OpenAD_lin_41*INT((-1_w2f__i8)))
      OpenAD_acc_11 = (2.0D00*OpenAD_lin_42)
      OpenAD_acc_12 = (OpenAD_lin_44*INT((-1_w2f__i8)))
      OpenAD_acc_13 = (OpenAD_lin_45*INT((-1_w2f__i8)))
      OpenAD_acc_14 = (2.0D00*OpenAD_lin_46)
      CALL setderiv(OpenAD_prp_2,LOC1)
      CALL sax(OpenAD_lin_32,F,G)
      CALL saxpy(OpenAD_lin_33,LOC2,G)
      CALL sax(OpenAD_lin_35,MATR0,ADJ_M0)
      CALL saxpy(OpenAD_acc_7,OpenAD_prp_2,ADJ_M0)
      CALL saxpy(OpenAD_lin_36,G,ADJ_M0)
      CALL saxpy(OpenAD_lin_37,MATR3,ADJ_M0)
      CALL sax(OpenAD_lin_39,MATR1,ADJ_M1)
      CALL saxpy(OpenAD_acc_8,OpenAD_prp_2,ADJ_M1)
      CALL saxpy(OpenAD_acc_9,G,ADJ_M1)
      CALL saxpy(OpenAD_acc_10,MATR2,ADJ_M1)
      CALL sax(OpenAD_lin_43,MATR2,ADJ_M2)
      CALL saxpy(OpenAD_acc_11,OpenAD_prp_2,ADJ_M2)
      CALL saxpy(OpenAD_acc_12,G,ADJ_M2)
      CALL saxpy(OpenAD_acc_13,MATR1,ADJ_M2)
      CALL sax(OpenAD_lin_47,MATR3,ADJ_M3)
      CALL saxpy(OpenAD_acc_14,OpenAD_prp_2,ADJ_M3)
      CALL saxpy(OpenAD_lin_48,G,ADJ_M3)
      CALL saxpy(OpenAD_lin_49,MATR0,ADJ_M3)
      CALL sax(OpenAD_lin_50,ADJ_M1,LOC1)
      CALL setderiv(G_OBJ1,ADJ_M0)
      CALL dec_deriv(G_OBJ1,LOC1)
      CALL sax(2.0D00,LOC1,G_OBJ2)
      CALL set_neg_deriv(G_OBJ0,ADJ_M0)
      CALL dec_deriv(G_OBJ0,LOC1)
      OpenAD_lin_51 = SQRT3
      LOC1%v = (ADJ_M3%v*SQRT3)
      G_OBJ3%v = (-(ADJ_M2%v+LOC1%v))
      G_OBJ4%v = (ADJ_M2%v-LOC1%v)
      G_OBJ5%v = (LOC1%v*2.0D00)
      LOC2%v = (LOC2%v*2.0D00)
      OpenAD_lin_52 = LOC2%v
      OpenAD_lin_53 = D00%v
      OpenAD_lin_54 = M33%v
      OpenAD_lin_55 = GG%v
      R00%v = (D00%v*LOC2%v+GG%v*M33%v)
      OpenAD_lin_56 = LOC2%v
      OpenAD_lin_57 = D11%v
      OpenAD_lin_58 = M22%v
      OpenAD_lin_59 = GG%v
      R11%v = (D11%v*LOC2%v+GG%v*M22%v)
      R22 = (D11%v*LOC2%v+GG%v*M11%v)
      R33 = (D00%v*LOC2%v+GG%v*M00%v)
      OpenAD_lin_60 = MATR1%v
      OpenAD_lin_61 = MATR0%v
      M01%v = (MATR0%v*MATR1%v)
      OpenAD_lin_62 = MATR2%v
      OpenAD_lin_63 = MATR0%v
      M02%v = (MATR0%v*MATR2%v)
      OpenAD_lin_64 = MATR2%v
      OpenAD_lin_65 = MATR1%v
      M12%v = (MATR1%v*MATR2%v)
      OpenAD_lin_66 = MATR3%v
      OpenAD_lin_67 = MATR1%v
      M13%v = (MATR1%v*MATR3%v)
      OpenAD_lin_68 = MATR3%v
      OpenAD_lin_69 = MATR2%v
      M23%v = (MATR2%v*MATR3%v)
      OpenAD_aux_9 = (M02%v+M13%v)
      OpenAD_lin_70 = OpenAD_aux_9
      OpenAD_lin_71 = LOC2%v
      D01%v = (LOC2%v*OpenAD_aux_9)
      CALL setderiv(OpenAD_prp_3,LOC2)
      CALL sax(OpenAD_lin_51,ADJ_M3,LOC1)
      CALL setderiv(G_OBJ4,ADJ_M2)
      CALL dec_deriv(G_OBJ4,LOC1)
      CALL sax(2.0D00,LOC1,G_OBJ5)
      CALL sax(2.0D00,OpenAD_prp_3,LOC2)
      CALL sax(OpenAD_lin_60,MATR0,M01)
      CALL saxpy(OpenAD_lin_61,MATR1,M01)
      CALL sax(OpenAD_lin_62,MATR0,M02)
      CALL saxpy(OpenAD_lin_63,MATR2,M02)
      CALL sax(OpenAD_lin_64,MATR1,M12)
      CALL saxpy(OpenAD_lin_65,MATR2,M12)
      CALL sax(OpenAD_lin_66,MATR1,M13)
      CALL saxpy(OpenAD_lin_67,MATR3,M13)
      CALL sax(OpenAD_lin_68,MATR2,M23)
      CALL saxpy(OpenAD_lin_69,MATR3,M23)
      CALL setderiv(OpenAD_prp_4,M02)
      CALL inc_deriv(OpenAD_prp_4,M13)
      CALL set_neg_deriv(G_OBJ3,ADJ_M2)
      CALL dec_deriv(G_OBJ3,LOC1)
      CALL sax(OpenAD_lin_52,D00,R00)
      CALL saxpy(OpenAD_lin_53,LOC2,R00)
      CALL saxpy(OpenAD_lin_54,GG,R00)
      CALL saxpy(OpenAD_lin_55,M33,R00)
      CALL sax(OpenAD_lin_56,D11,R11)
      CALL saxpy(OpenAD_lin_57,LOC2,R11)
      CALL saxpy(OpenAD_lin_58,GG,R11)
      CALL saxpy(OpenAD_lin_59,M22,R11)
      CALL sax(OpenAD_lin_70,LOC2,D01)
      CALL saxpy(OpenAD_lin_71,OpenAD_prp_4,D01)
      OpenAD_lin_72 = THIRD
      LOC1%v = (R11%v*THIRD)
      OpenAD_aux_10 = (D01%v+GG%v*M23%v)
      OpenAD_lin_74 = M23%v
      OpenAD_lin_75 = GG%v
      OpenAD_lin_73 = TWOSQRT3
      F%v = (TWOSQRT3*OpenAD_aux_10)
      G%v = (LOC1%v+R00%v)
      H_OBJ1%v = (LOC1%v-R00%v)
      H_OBJ0%v = (F%v+G%v)
      H_OBJ3%v = (G%v-F%v)
      CALL sax(OpenAD_lin_72,R11,LOC1)
      CALL setderiv(G,LOC1)
      CALL inc_deriv(G,R00)
      CALL setderiv(H_OBJ1,LOC1)
      CALL dec_deriv(H_OBJ1,R00)
      CALL setderiv(OpenAD_prp_5,D01)
      CALL saxpy(OpenAD_lin_74,GG,OpenAD_prp_5)
      CALL saxpy(OpenAD_lin_75,M23,OpenAD_prp_5)
      CALL sax(OpenAD_lin_73,OpenAD_prp_5,F)
      CALL setderiv(H_OBJ0,F)
      CALL inc_deriv(H_OBJ0,G)
      CALL setderiv(H_OBJ3,G)
      CALL dec_deriv(H_OBJ3,F)
      OpenAD_lin_76 = TWO3RD
      LOC1%v = (-(R11%v*TWO3RD))
      H_OBJ2%v = (LOC1%v-F%v)
      H_OBJ4%v = (F%v+LOC1%v)
      OpenAD_lin_77 = FOUR3RD
      H_OBJ5%v = (FOUR3RD*R11%v)
      OpenAD_acc_15 = (OpenAD_lin_76*INT((-1_w2f__i8)))
      CALL sax(OpenAD_lin_77,R11,H_OBJ5)
      CALL sax(OpenAD_acc_15,R11,LOC1)
      CALL setderiv(H_OBJ2,LOC1)
      CALL dec_deriv(H_OBJ2,F)
      CALL setderiv(H_OBJ4,F)
      CALL inc_deriv(H_OBJ4,LOC1)
      OpenAD_lin_78 = THIRD
      LOC1%v = (R00%v*THIRD)
      OpenAD_aux_11 = (D01%v+GG%v*M01%v)
      OpenAD_lin_80 = M01%v
      OpenAD_lin_81 = GG%v
      OpenAD_lin_79 = TWOSQRT3
      F%v = (TWOSQRT3*OpenAD_aux_11)
      G%v = (LOC1%v+R11%v)
      H_OBJ16%v = (LOC1%v-R11%v)
      H_OBJ15%v = (F%v+G%v)
      H_OBJ18%v = (G%v-F%v)
      CALL sax(OpenAD_lin_78,R00,LOC1)
      CALL setderiv(G,LOC1)
      CALL inc_deriv(G,R11)
      CALL setderiv(H_OBJ16,LOC1)
      CALL dec_deriv(H_OBJ16,R11)
      CALL setderiv(OpenAD_prp_6,D01)
      CALL saxpy(OpenAD_lin_80,GG,OpenAD_prp_6)
      CALL saxpy(OpenAD_lin_81,M01,OpenAD_prp_6)
      CALL sax(OpenAD_lin_79,OpenAD_prp_6,F)
      CALL setderiv(H_OBJ15,F)
      CALL inc_deriv(H_OBJ15,G)
      CALL setderiv(H_OBJ18,G)
      CALL dec_deriv(H_OBJ18,F)
      OpenAD_lin_82 = TWO3RD
      LOC1%v = (-(R00%v*TWO3RD))
      H_OBJ17%v = (LOC1%v-F%v)
      H_OBJ19%v = (F%v+LOC1%v)
      OpenAD_lin_83 = FOUR3RD
      H_OBJ20%v = (FOUR3RD*R00%v)
      OpenAD_acc_16 = (OpenAD_lin_82*INT((-1_w2f__i8)))
      CALL sax(OpenAD_lin_83,R00,H_OBJ20)
      CALL sax(OpenAD_acc_16,R00,LOC1)
      CALL setderiv(H_OBJ17,LOC1)
      CALL dec_deriv(H_OBJ17,F)
      CALL setderiv(H_OBJ19,F)
      CALL inc_deriv(H_OBJ19,LOC1)
      OpenAD_aux_12 = (M01%v+M23%v)
      OpenAD_lin_84 = OpenAD_aux_12
      OpenAD_lin_85 = LOC2%v
      LOC1%v = (LOC2%v*OpenAD_aux_12)
      OpenAD_lin_86 = M02%v
      OpenAD_lin_87 = GG%v
      R13%v = (LOC1%v+GG%v*M02%v)
      OpenAD_lin_88 = M13%v
      OpenAD_lin_89 = GG%v
      R02%v = (LOC1%v+GG%v*M13%v)
      OpenAD_lin_90 = LOC2%v
      OpenAD_lin_91 = D03%v
      OpenAD_lin_92 = M03%v
      OpenAD_lin_93 = GG%v
      R03%v = (D03%v*LOC2%v+GG%v*M03%v)
      OpenAD_lin_94 = LOC2%v
      OpenAD_lin_95 = D12%v
      OpenAD_lin_96 = M12%v
      OpenAD_lin_97 = GG%v
      R12%v = (D12%v*LOC2%v+GG%v*M12%v)
      OpenAD_lin_98 = TWO3RD
      G%v = (-(R13%v*TWO3RD))
      OpenAD_lin_99 = TWOSQRT3
      F%v = (R03%v*TWOSQRT3)
      H_OBJ8%v = (G%v-F%v)
      H_OBJ11%v = (F%v+G%v)
      OpenAD_acc_17 = (OpenAD_lin_98*INT((-1_w2f__i8)))
      CALL setderiv(OpenAD_prp_7,M01)
      CALL inc_deriv(OpenAD_prp_7,M23)
      CALL sax(OpenAD_lin_84,LOC2,LOC1)
      CALL saxpy(OpenAD_lin_85,OpenAD_prp_7,LOC1)
      CALL setderiv(R13,LOC1)
      CALL saxpy(OpenAD_lin_86,GG,R13)
      CALL saxpy(OpenAD_lin_87,M02,R13)
      CALL setderiv(R02,LOC1)
      CALL saxpy(OpenAD_lin_88,GG,R02)
      CALL saxpy(OpenAD_lin_89,M13,R02)
      CALL sax(OpenAD_lin_90,D03,R03)
      CALL saxpy(OpenAD_lin_91,LOC2,R03)
      CALL saxpy(OpenAD_lin_92,GG,R03)
      CALL saxpy(OpenAD_lin_93,M03,R03)
      CALL sax(OpenAD_lin_94,D12,R12)
      CALL saxpy(OpenAD_lin_95,LOC2,R12)
      CALL saxpy(OpenAD_lin_96,GG,R12)
      CALL saxpy(OpenAD_lin_97,M12,R12)
      CALL sax(OpenAD_lin_99,R03,F)
      CALL sax(OpenAD_acc_17,R13,G)
      CALL setderiv(H_OBJ8,G)
      CALL dec_deriv(H_OBJ8,F)
      CALL setderiv(H_OBJ11,F)
      CALL inc_deriv(H_OBJ11,G)
      OpenAD_lin_100 = TWOSQRT3
      F%v = (R12%v*TWOSQRT3)
      H_OBJ12%v = (G%v-F%v)
      H_OBJ13%v = (F%v+G%v)
      H_OBJ14%v = (R13%v*1.2D+01)
      OpenAD_lin_101 = FOUR3RD
      G%v = (FOUR3RD*R02%v)
      CALL setderiv(OpenAD_prp_8,G)
      CALL sax(OpenAD_lin_100,R12,F)
      CALL setderiv(H_OBJ12,OpenAD_prp_8)
      CALL dec_deriv(H_OBJ12,F)
      CALL setderiv(H_OBJ13,F)
      CALL inc_deriv(H_OBJ13,OpenAD_prp_8)
      CALL sax(1.2D+01,R13,H_OBJ14)
      CALL sax(OpenAD_lin_101,R02,G)
      OpenAD_aux_13 = (R03%v+R12%v)
      OpenAD_lin_102 = SQRT3
      F%v = (SQRT3*OpenAD_aux_13)
      H_OBJ10%v = (G%v-F%v)
      H_OBJ6%v = (F%v+G%v)
      OpenAD_lin_103 = TWO3RD
      G%v = (R02%v*TWO3RD)
      CALL setderiv(OpenAD_prp_9,G)
      CALL setderiv(OpenAD_prp_10,R03)
      CALL inc_deriv(OpenAD_prp_10,R12)
      CALL sax(OpenAD_lin_103,R02,G)
      CALL sax(OpenAD_lin_102,OpenAD_prp_10,F)
      CALL setderiv(H_OBJ10,OpenAD_prp_9)
      CALL dec_deriv(H_OBJ10,F)
      CALL setderiv(H_OBJ6,F)
      CALL inc_deriv(H_OBJ6,OpenAD_prp_9)
      OpenAD_aux_14 = (R03%v-R12%v)
      OpenAD_lin_104 = SQRT3
      F%v = (SQRT3*OpenAD_aux_14)
      H_OBJ9%v = (G%v-F%v)
      H_OBJ7%v = (F%v+G%v)
      CALL setderiv(OpenAD_prp_11,R03)
      CALL dec_deriv(OpenAD_prp_11,R12)
      CALL sax(OpenAD_lin_104,OpenAD_prp_11,F)
      CALL setderiv(H_OBJ9,G)
      CALL dec_deriv(H_OBJ9,F)
      CALL setderiv(H_OBJ7,F)
      CALL inc_deriv(H_OBJ7,G)
      END SUBROUTINE
