
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
      SUBROUTINE box_timestep(GAMMALOC, FLDSTAR, EXTFORLOC, UVELLOC, FLD
     +NOW, FLDOLD, FLDNEW)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      REAL(w2f__8) GAMMALOC
      REAL(w2f__8) FLDSTAR(1 : 2)
      REAL(w2f__8) EXTFORLOC(1 : 2)
      REAL(w2f__8) UVELLOC
      type(active) :: FLDNOW(1:3)
      REAL(w2f__8) FLDOLD(1 : 3)
      type(active) :: FLDNEW(1:3)
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) DELTA_T
      type(active) :: DFLDDT(1:3)
      INTEGER(w2f__i4) L
      REAL(w2f__8) VOL(1 : 3)
      REAL(w2f__8) OpenAD_acc_0
      REAL(w2f__8) OpenAD_acc_1
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
      REAL(w2f__8) OpenAD_aux_15
      REAL(w2f__8) OpenAD_aux_16
      REAL(w2f__8) OpenAD_aux_17
      REAL(w2f__8) OpenAD_aux_18
      REAL(w2f__8) OpenAD_aux_19
      REAL(w2f__8) OpenAD_aux_2
      REAL(w2f__8) OpenAD_aux_20
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
      type(active) :: OpenAD_prp_0
      type(active) :: OpenAD_prp_1
      type(active) :: OpenAD_prp_2
      type(active) :: OpenAD_prp_3
      type(active) :: OpenAD_prp_4
      type(active) :: OpenAD_prp_5
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(FLDNOW)
C$OPENAD DEPENDENT(FLDNEW)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      VOL(1) = 1.0D+01
      VOL(2) = 1.2D+01
      VOL(3) = 1.4D+01
      DELTA_T = 2.0D+01
      IF(UVELLOC .GE. 0.0D00) THEN
        OpenAD_aux_2 = (FLDSTAR(1)-FLDNOW(1)%v)
        OpenAD_aux_1 = (GAMMALOC*OpenAD_aux_2)
        OpenAD_aux_3 = (FLDNOW(3)%v-FLDNOW(1)%v)
        OpenAD_aux_0 = (EXTFORLOC(1)+VOL(1)*OpenAD_aux_1+UVELLOC*OpenAD_
     +aux_3)
        OpenAD_lin_2 = GAMMALOC
        OpenAD_lin_1 = VOL(1)
        OpenAD_lin_3 = UVELLOC
        OpenAD_lin_0 = (INT(1_w2f__i8)/VOL(1))
        DFLDDT(1)%v = (OpenAD_aux_0/VOL(1))
        OpenAD_acc_0 = (OpenAD_lin_3*OpenAD_lin_0)
        OpenAD_acc_1 = (INT((-1_w2f__i8))*OpenAD_lin_2*OpenAD_lin_1*Open
     +AD_lin_0)
        CALL setderiv(OpenAD_prp_0,FLDNOW(3))
        CALL dec_deriv(OpenAD_prp_0,FLDNOW(1))
        CALL sax(OpenAD_acc_0,OpenAD_prp_0,DFLDDT(1))
        CALL saxpy(OpenAD_acc_1,FLDNOW(1),DFLDDT(1))
        OpenAD_aux_6 = (FLDSTAR(2)-FLDNOW(2)%v)
        OpenAD_aux_5 = (GAMMALOC*OpenAD_aux_6)
        OpenAD_aux_7 = (FLDNOW(1)%v-FLDNOW(2)%v)
        OpenAD_aux_4 = (EXTFORLOC(2)+VOL(2)*OpenAD_aux_5+UVELLOC*OpenAD_
     +aux_7)
        OpenAD_lin_6 = GAMMALOC
        OpenAD_lin_5 = VOL(2)
        OpenAD_lin_7 = UVELLOC
        OpenAD_lin_4 = (INT(1_w2f__i8)/VOL(2))
        DFLDDT(2)%v = (OpenAD_aux_4/VOL(2))
        OpenAD_acc_2 = (OpenAD_lin_7*OpenAD_lin_4)
        OpenAD_acc_3 = (INT((-1_w2f__i8))*OpenAD_lin_6*OpenAD_lin_5*Open
     +AD_lin_4)
        CALL setderiv(OpenAD_prp_1,FLDNOW(1))
        CALL dec_deriv(OpenAD_prp_1,FLDNOW(2))
        CALL sax(OpenAD_acc_2,OpenAD_prp_1,DFLDDT(2))
        CALL saxpy(OpenAD_acc_3,FLDNOW(2),DFLDDT(2))
        OpenAD_aux_9 = (FLDNOW(2)%v-FLDNOW(3)%v)
        OpenAD_aux_8 = (UVELLOC*OpenAD_aux_9)
        OpenAD_lin_9 = UVELLOC
        OpenAD_lin_8 = (INT(1_w2f__i8)/VOL(3))
        DFLDDT(3)%v = (OpenAD_aux_8/VOL(3))
        OpenAD_acc_4 = (OpenAD_lin_9*OpenAD_lin_8)
        CALL setderiv(OpenAD_prp_2,FLDNOW(2))
        CALL dec_deriv(OpenAD_prp_2,FLDNOW(3))
        CALL sax(OpenAD_acc_4,OpenAD_prp_2,DFLDDT(3))
      ELSE
        OpenAD_aux_13 = (FLDSTAR(1)-FLDNOW(1)%v)
        OpenAD_aux_12 = (GAMMALOC*OpenAD_aux_13)
        OpenAD_aux_14 = (FLDNOW(2)%v-FLDNOW(1)%v)
        OpenAD_aux_11 = (EXTFORLOC(1)+VOL(1)*OpenAD_aux_12-UVELLOC*OpenA
     +D_aux_14)
        OpenAD_lin_13 = GAMMALOC
        OpenAD_lin_12 = VOL(1)
        OpenAD_lin_14 = UVELLOC
        OpenAD_lin_11 = (INT(1_w2f__i8)/VOL(1))
        DFLDDT(1)%v = (OpenAD_aux_11/VOL(1))
        OpenAD_acc_5 = (OpenAD_lin_14*INT((-1_w2f__i8))*OpenAD_lin_11)
        OpenAD_acc_6 = (INT((-1_w2f__i8))*OpenAD_lin_13*OpenAD_lin_12*Op
     +enAD_lin_11)
        CALL setderiv(OpenAD_prp_3,FLDNOW(2))
        CALL dec_deriv(OpenAD_prp_3,FLDNOW(1))
        CALL sax(OpenAD_acc_5,OpenAD_prp_3,DFLDDT(1))
        CALL saxpy(OpenAD_acc_6,FLDNOW(1),DFLDDT(1))
        OpenAD_aux_17 = (FLDSTAR(2)-FLDNOW(2)%v)
        OpenAD_aux_16 = (GAMMALOC*OpenAD_aux_17)
        OpenAD_aux_18 = (FLDNOW(3)%v-FLDNOW(2)%v)
        OpenAD_aux_15 = (EXTFORLOC(2)+VOL(2)*OpenAD_aux_16-UVELLOC*OpenA
     +D_aux_18)
        OpenAD_lin_17 = GAMMALOC
        OpenAD_lin_16 = VOL(2)
        OpenAD_lin_18 = UVELLOC
        OpenAD_lin_15 = (INT(1_w2f__i8)/VOL(2))
        DFLDDT(2)%v = (OpenAD_aux_15/VOL(2))
        OpenAD_acc_7 = (OpenAD_lin_18*INT((-1_w2f__i8))*OpenAD_lin_15)
        OpenAD_acc_8 = (INT((-1_w2f__i8))*OpenAD_lin_17*OpenAD_lin_16*Op
     +enAD_lin_15)
        CALL setderiv(OpenAD_prp_4,FLDNOW(3))
        CALL dec_deriv(OpenAD_prp_4,FLDNOW(2))
        CALL sax(OpenAD_acc_7,OpenAD_prp_4,DFLDDT(2))
        CALL saxpy(OpenAD_acc_8,FLDNOW(2),DFLDDT(2))
        OpenAD_aux_20 = (FLDNOW(1)%v-FLDNOW(3)%v)
        OpenAD_aux_19 = (UVELLOC*OpenAD_aux_20)
        OpenAD_lin_20 = UVELLOC
        OpenAD_lin_19 = (INT(1_w2f__i8)/VOL(3))
        DFLDDT(3)%v = (-(OpenAD_aux_19/VOL(3)))
        OpenAD_acc_9 = (OpenAD_lin_20*OpenAD_lin_19*INT((-1_w2f__i8)))
        CALL setderiv(OpenAD_prp_5,FLDNOW(1))
        CALL dec_deriv(OpenAD_prp_5,FLDNOW(3))
        CALL sax(OpenAD_acc_9,OpenAD_prp_5,DFLDDT(3))
      ENDIF
      DO L = 1,3,1
        OpenAD_aux_10 = (DELTA_T*2.0D00)
        OpenAD_lin_10 = OpenAD_aux_10
        FLDNEW(INT(L))%v = (FLDOLD(L)+DFLDDT(L)%v*OpenAD_aux_10)
        CALL sax(OpenAD_lin_10,DFLDDT(L),FLDNEW(L))
      END DO
      END SUBROUTINE
