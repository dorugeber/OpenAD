
!$OPENAD XXX File_start [OAD_intrinsics.f90]
MODULE oad_intrinsics
use OAD_active
use w2f__types
IMPLICIT NONE
SAVE
!
!     **** Statements ****
!
END MODULE

C$OPENAD XXX File_start [all_globals_mod.f]
      MODULE all_globals_mod
      use OAD_active
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Statements ****
C
      END MODULE

C$OPENAD XXX File_start [head.f]
      SUBROUTINE head(X, F)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X(1:9)
      type(active) :: F(1:7)
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) H
      INTEGER(w2f__i4) I
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
      REAL(w2f__8) OpenAD_acc_23
      REAL(w2f__8) OpenAD_acc_24
      REAL(w2f__8) OpenAD_acc_25
      REAL(w2f__8) OpenAD_acc_26
      REAL(w2f__8) OpenAD_acc_27
      REAL(w2f__8) OpenAD_acc_28
      REAL(w2f__8) OpenAD_acc_29
      REAL(w2f__8) OpenAD_acc_3
      REAL(w2f__8) OpenAD_acc_30
      REAL(w2f__8) OpenAD_acc_31
      REAL(w2f__8) OpenAD_acc_32
      REAL(w2f__8) OpenAD_acc_33
      REAL(w2f__8) OpenAD_acc_34
      REAL(w2f__8) OpenAD_acc_35
      REAL(w2f__8) OpenAD_acc_36
      REAL(w2f__8) OpenAD_acc_37
      REAL(w2f__8) OpenAD_acc_38
      REAL(w2f__8) OpenAD_acc_39
      REAL(w2f__8) OpenAD_acc_4
      REAL(w2f__8) OpenAD_acc_40
      REAL(w2f__8) OpenAD_acc_41
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
      REAL(w2f__8) OpenAD_aux_23
      REAL(w2f__8) OpenAD_aux_24
      REAL(w2f__8) OpenAD_aux_25
      REAL(w2f__8) OpenAD_aux_26
      REAL(w2f__8) OpenAD_aux_27
      REAL(w2f__8) OpenAD_aux_28
      REAL(w2f__8) OpenAD_aux_29
      REAL(w2f__8) OpenAD_aux_3
      REAL(w2f__8) OpenAD_aux_30
      REAL(w2f__8) OpenAD_aux_31
      REAL(w2f__8) OpenAD_aux_32
      REAL(w2f__8) OpenAD_aux_33
      REAL(w2f__8) OpenAD_aux_34
      REAL(w2f__8) OpenAD_aux_35
      REAL(w2f__8) OpenAD_aux_36
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
      REAL(w2f__8) OpenAD_lin_7
      REAL(w2f__8) OpenAD_lin_8
      REAL(w2f__8) OpenAD_lin_9
      type(active) :: OpenAD_prp_0
      type(active) :: OpenAD_prp_1
      type(active) :: OpenAD_prp_2
      type(active) :: OpenAD_prp_3
      type(active) :: OpenAD_prp_4
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(F)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      H = 2.5D-01
      OpenAD_aux_3 = (H * H)
      OpenAD_aux_2 = (X(8)%v*OpenAD_aux_3)
      OpenAD_aux_0 = (OpenAD_aux_2/1.2D+01)
      OpenAD_aux_5 = (X(1)%v*X(9)%v+1.0D00)
      OpenAD_aux_4 = EXP(X(1)%v/OpenAD_aux_5)
      OpenAD_aux_1 = (OpenAD_aux_4*1.0D+01+1.0D00)
      OpenAD_lin_3 = OpenAD_aux_3
      OpenAD_lin_2 = (INT(1_w2f__i8)/1.2D+01)
      OpenAD_lin_0 = OpenAD_aux_1
      OpenAD_lin_5 = (INT(1_w2f__i8)/OpenAD_aux_5)
      OpenAD_lin_7 = X(9)%v
      OpenAD_lin_8 = X(1)%v
      OpenAD_lin_6 = (-(X(1)%v/(OpenAD_aux_5*OpenAD_aux_5)))
      OpenAD_lin_4 = OpenAD_aux_4
      OpenAD_lin_1 = OpenAD_aux_0
      F(1)%v = (X(1)%v*(-2.0D00)+OpenAD_aux_0*OpenAD_aux_1)
      OpenAD_acc_0 = (OpenAD_lin_4*1.0D+01)
      OpenAD_acc_1 = (OpenAD_lin_3*OpenAD_lin_2*OpenAD_lin_0)
      OpenAD_acc_2 = (OpenAD_lin_5*OpenAD_acc_0*OpenAD_lin_1)
      OpenAD_acc_3 = (OpenAD_lin_6*OpenAD_acc_0*OpenAD_lin_1)
      OpenAD_acc_4 = (OpenAD_lin_7*OpenAD_acc_3)
      OpenAD_acc_5 = (OpenAD_lin_8*OpenAD_acc_3)
      CALL sax(-2.0D00,X(1),F(1))
      CALL saxpy(OpenAD_acc_1,X(8),F(1))
      CALL saxpy(OpenAD_acc_2,X(1),F(1))
      CALL saxpy(OpenAD_acc_4,X(1),F(1))
      CALL saxpy(OpenAD_acc_5,X(9),F(1))
      OpenAD_aux_8 = (X(1)%v*X(9)%v+1.0D00)
      OpenAD_aux_6 = EXP(X(1)%v/OpenAD_aux_8)
      OpenAD_aux_10 = (H*H)
      OpenAD_aux_9 = (X(8)%v*OpenAD_aux_10)
      OpenAD_aux_7 = (OpenAD_aux_9/1.2D+01)
      OpenAD_lin_12 = (INT(1_w2f__i8)/OpenAD_aux_8)
      OpenAD_lin_14 = X(9)%v
      OpenAD_lin_15 = X(1)%v
      OpenAD_lin_13 = (-(X(1)%v/(OpenAD_aux_8*OpenAD_aux_8)))
      OpenAD_lin_11 = OpenAD_aux_6
      OpenAD_lin_9 = OpenAD_aux_7
      OpenAD_lin_17 = OpenAD_aux_10
      OpenAD_lin_16 = (INT(1_w2f__i8)/1.2D+01)
      OpenAD_lin_10 = OpenAD_aux_6
      F(2)%v = (X(1)%v+OpenAD_aux_6*OpenAD_aux_7)
      OpenAD_acc_6 = (OpenAD_lin_11*OpenAD_lin_9)
      OpenAD_acc_7 = (OpenAD_lin_17*OpenAD_lin_16*OpenAD_lin_10)
      OpenAD_acc_8 = (OpenAD_lin_12*OpenAD_acc_6)
      OpenAD_acc_9 = (OpenAD_lin_13*OpenAD_acc_6)
      OpenAD_acc_10 = (OpenAD_lin_14*OpenAD_acc_9)
      OpenAD_acc_11 = (OpenAD_lin_15*OpenAD_acc_9)
      CALL setderiv(F(2),X(1))
      CALL saxpy(OpenAD_acc_7,X(8),F(2))
      CALL saxpy(OpenAD_acc_8,X(1),F(2))
      CALL saxpy(OpenAD_acc_10,X(1),F(2))
      CALL saxpy(OpenAD_acc_11,X(9),F(2))
      DO I = 2,6,1
        OpenAD_aux_13 = (X(I)%v*X(9)%v+1.0D00)
        OpenAD_aux_11 = EXP(X(I)%v/OpenAD_aux_13)
        OpenAD_aux_15 = (H*H)
        OpenAD_aux_14 = (X(8)%v*OpenAD_aux_15)
        OpenAD_aux_12 = (OpenAD_aux_14/1.2D+01)
        OpenAD_lin_21 = (INT(1_w2f__i8)/OpenAD_aux_13)
        OpenAD_lin_23 = X(9)%v
        OpenAD_lin_24 = X(I)%v
        OpenAD_lin_22 = (-(X(I)%v/(OpenAD_aux_13*OpenAD_aux_13)))
        OpenAD_lin_20 = OpenAD_aux_11
        OpenAD_lin_18 = OpenAD_aux_12
        OpenAD_lin_26 = OpenAD_aux_15
        OpenAD_lin_25 = (INT(1_w2f__i8)/1.2D+01)
        OpenAD_lin_19 = OpenAD_aux_11
        F(I+(-1))%v = (X(I)%v+F(I+(-1))%v+OpenAD_aux_11*OpenAD_aux_12)
        OpenAD_acc_12 = (OpenAD_lin_20*OpenAD_lin_18)
        OpenAD_acc_13 = (OpenAD_lin_26*OpenAD_lin_25*OpenAD_lin_19)
        OpenAD_acc_14 = (OpenAD_lin_21*OpenAD_acc_12)
        OpenAD_acc_15 = (OpenAD_lin_22*OpenAD_acc_12)
        OpenAD_acc_16 = (OpenAD_lin_23*OpenAD_acc_15)
        OpenAD_acc_17 = (OpenAD_lin_24*OpenAD_acc_15)
        CALL setderiv(OpenAD_prp_0,F(I+(-1)))
        CALL setderiv(F(I+(-1)),X(I))
        CALL inc_deriv(F(I+(-1)),OpenAD_prp_0)
        CALL saxpy(OpenAD_acc_13,X(8),F(I+(-1)))
        CALL saxpy(OpenAD_acc_14,X(I),F(I+(-1)))
        CALL saxpy(OpenAD_acc_16,X(I),F(I+(-1)))
        CALL saxpy(OpenAD_acc_17,X(9),F(I+(-1)))
        OpenAD_aux_18 = (X(I)%v*X(9)%v+1.0D00)
        OpenAD_aux_16 = EXP(X(I)%v/OpenAD_aux_18)
        OpenAD_aux_20 = (H*H)
        OpenAD_aux_19 = (X(8)%v*OpenAD_aux_20)
        OpenAD_aux_17 = (OpenAD_aux_19/1.19999999999999995559D00)
        OpenAD_lin_30 = (INT(1_w2f__i8)/OpenAD_aux_18)
        OpenAD_lin_32 = X(9)%v
        OpenAD_lin_33 = X(I)%v
        OpenAD_lin_31 = (-(X(I)%v/(OpenAD_aux_18*OpenAD_aux_18)))
        OpenAD_lin_29 = OpenAD_aux_16
        OpenAD_lin_27 = OpenAD_aux_17
        OpenAD_lin_35 = OpenAD_aux_20
        OpenAD_lin_34 = (INT(1_w2f__i8)/1.19999999999999995559D00)
        OpenAD_lin_28 = OpenAD_aux_16
        F(INT(I))%v = (OpenAD_aux_16*OpenAD_aux_17+F(I)%v-X(I)%v*2.0D00)
        OpenAD_acc_18 = (OpenAD_lin_29*OpenAD_lin_27)
        OpenAD_acc_19 = (OpenAD_lin_35*OpenAD_lin_34*OpenAD_lin_28)
        OpenAD_acc_20 = (OpenAD_lin_30*OpenAD_acc_18)
        OpenAD_acc_21 = (OpenAD_lin_31*OpenAD_acc_18)
        OpenAD_acc_22 = (OpenAD_lin_32*OpenAD_acc_21)
        OpenAD_acc_23 = (OpenAD_lin_33*OpenAD_acc_21)
        CALL setderiv(OpenAD_prp_1,F(I))
        CALL setderiv(F(I),OpenAD_prp_1)
        CALL saxpy(-2.0D00,X(I),F(I))
        CALL saxpy(OpenAD_acc_19,X(8),F(I))
        CALL saxpy(OpenAD_acc_20,X(I),F(I))
        CALL saxpy(OpenAD_acc_22,X(I),F(I))
        CALL saxpy(OpenAD_acc_23,X(9),F(I))
        OpenAD_aux_23 = (X(I)%v*X(9)%v+1.0D00)
        OpenAD_aux_21 = EXP(X(I)%v/OpenAD_aux_23)
        OpenAD_aux_25 = (H*H)
        OpenAD_aux_24 = (X(8)%v*OpenAD_aux_25)
        OpenAD_aux_22 = (OpenAD_aux_24/1.2D+01)
        OpenAD_lin_39 = (INT(1_w2f__i8)/OpenAD_aux_23)
        OpenAD_lin_41 = X(9)%v
        OpenAD_lin_42 = X(I)%v
        OpenAD_lin_40 = (-(X(I)%v/(OpenAD_aux_23*OpenAD_aux_23)))
        OpenAD_lin_38 = OpenAD_aux_21
        OpenAD_lin_36 = OpenAD_aux_22
        OpenAD_lin_44 = OpenAD_aux_25
        OpenAD_lin_43 = (INT(1_w2f__i8)/1.2D+01)
        OpenAD_lin_37 = OpenAD_aux_21
        F(I+1)%v = (X(I)%v+OpenAD_aux_21*OpenAD_aux_22)
        OpenAD_acc_24 = (OpenAD_lin_38*OpenAD_lin_36)
        OpenAD_acc_25 = (OpenAD_lin_44*OpenAD_lin_43*OpenAD_lin_37)
        OpenAD_acc_26 = (OpenAD_lin_39*OpenAD_acc_24)
        OpenAD_acc_27 = (OpenAD_lin_40*OpenAD_acc_24)
        OpenAD_acc_28 = (OpenAD_lin_41*OpenAD_acc_27)
        OpenAD_acc_29 = (OpenAD_lin_42*OpenAD_acc_27)
        CALL setderiv(F(I+1),X(I))
        CALL saxpy(OpenAD_acc_25,X(8),F(I+1))
        CALL saxpy(OpenAD_acc_26,X(I),F(I+1))
        CALL saxpy(OpenAD_acc_28,X(I),F(I+1))
        CALL saxpy(OpenAD_acc_29,X(9),F(I+1))
      END DO
      OpenAD_aux_28 = (X(7)%v*X(9)%v+1.0D00)
      OpenAD_aux_26 = EXP(X(7)%v/OpenAD_aux_28)
      OpenAD_aux_30 = (H*H)
      OpenAD_aux_29 = (X(8)%v*OpenAD_aux_30)
      OpenAD_aux_27 = (OpenAD_aux_29/1.2D+01)
      OpenAD_lin_48 = (INT(1_w2f__i8)/OpenAD_aux_28)
      OpenAD_lin_50 = X(9)%v
      OpenAD_lin_51 = X(7)%v
      OpenAD_lin_49 = (-(X(7)%v/(OpenAD_aux_28*OpenAD_aux_28)))
      OpenAD_lin_47 = OpenAD_aux_26
      OpenAD_lin_45 = OpenAD_aux_27
      OpenAD_lin_53 = OpenAD_aux_30
      OpenAD_lin_52 = (INT(1_w2f__i8)/1.2D+01)
      OpenAD_lin_46 = OpenAD_aux_26
      F(6)%v = (X(7)%v+F(6)%v+OpenAD_aux_26*OpenAD_aux_27)
      OpenAD_acc_30 = (OpenAD_lin_47*OpenAD_lin_45)
      OpenAD_acc_31 = (OpenAD_lin_53*OpenAD_lin_52*OpenAD_lin_46)
      OpenAD_acc_32 = (OpenAD_lin_48*OpenAD_acc_30)
      OpenAD_acc_33 = (OpenAD_lin_49*OpenAD_acc_30)
      OpenAD_acc_34 = (OpenAD_lin_50*OpenAD_acc_33)
      OpenAD_acc_35 = (OpenAD_lin_51*OpenAD_acc_33)
      CALL setderiv(OpenAD_prp_2,F(6))
      CALL setderiv(F(6),X(7))
      CALL inc_deriv(F(6),OpenAD_prp_2)
      CALL saxpy(OpenAD_acc_31,X(8),F(6))
      CALL saxpy(OpenAD_acc_32,X(7),F(6))
      CALL saxpy(OpenAD_acc_34,X(7),F(6))
      CALL saxpy(OpenAD_acc_35,X(9),F(6))
      F(7)%v = (F(7)%v-X(7)%v*2.0D00)
      CALL setderiv(OpenAD_prp_3,F(7))
      CALL setderiv(F(7),OpenAD_prp_3)
      CALL saxpy(-2.0D00,X(7),F(7))
      OpenAD_aux_34 = (H*H)
      OpenAD_aux_33 = (X(8)%v*OpenAD_aux_34)
      OpenAD_aux_31 = (OpenAD_aux_33/1.2D+01)
      OpenAD_aux_36 = (X(7)%v*X(9)%v+1.0D00)
      OpenAD_aux_35 = EXP(X(7)%v/OpenAD_aux_36)
      OpenAD_aux_32 = (OpenAD_aux_35*1.0D+01+1.0D00)
      OpenAD_lin_57 = OpenAD_aux_34
      OpenAD_lin_56 = (INT(1_w2f__i8)/1.2D+01)
      OpenAD_lin_54 = OpenAD_aux_32
      OpenAD_lin_59 = (INT(1_w2f__i8)/OpenAD_aux_36)
      OpenAD_lin_61 = X(9)%v
      OpenAD_lin_62 = X(7)%v
      OpenAD_lin_60 = (-(X(7)%v/(OpenAD_aux_36*OpenAD_aux_36)))
      OpenAD_lin_58 = OpenAD_aux_35
      OpenAD_lin_55 = OpenAD_aux_31
      F(7)%v = (F(7)%v+OpenAD_aux_31*OpenAD_aux_32)
      OpenAD_acc_36 = (OpenAD_lin_58*1.0D+01)
      OpenAD_acc_37 = (OpenAD_lin_57*OpenAD_lin_56*OpenAD_lin_54)
      OpenAD_acc_38 = (OpenAD_lin_59*OpenAD_acc_36*OpenAD_lin_55)
      OpenAD_acc_39 = (OpenAD_lin_60*OpenAD_acc_36*OpenAD_lin_55)
      OpenAD_acc_40 = (OpenAD_lin_61*OpenAD_acc_39)
      OpenAD_acc_41 = (OpenAD_lin_62*OpenAD_acc_39)
      CALL setderiv(OpenAD_prp_4,F(7))
      CALL setderiv(F(7),OpenAD_prp_4)
      CALL saxpy(OpenAD_acc_37,X(8),F(7))
      CALL saxpy(OpenAD_acc_38,X(7),F(7))
      CALL saxpy(OpenAD_acc_40,X(7),F(7))
      CALL saxpy(OpenAD_acc_41,X(9),F(7))
      END SUBROUTINE
