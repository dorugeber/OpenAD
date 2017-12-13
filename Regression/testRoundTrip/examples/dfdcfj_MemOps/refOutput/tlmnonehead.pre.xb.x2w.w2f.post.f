
C$OPENAD XXX File_start [head.f]
      SUBROUTINE head(NX, NY, X, FVEC, R)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      INTEGER(w2f__i4) NX
      INTEGER(w2f__i4) NY
      type(active) :: X(1:INT((NX*NY)))
      type(active) :: FVEC(1:INT((NX*NY)))
      REAL(w2f__8) R
C
C     **** Local Variables and Functions ****
C
      type(active) :: DPDX
      type(active) :: DPDY
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
      type(active) :: P
      type(active) :: PB
      type(active) :: PBB
      type(active) :: PBL
      type(active) :: PBLAP
      type(active) :: PBR
      type(active) :: PL
      type(active) :: PLAP
      type(active) :: PLL
      type(active) :: PLLAP
      type(active) :: PR
      type(active) :: PRLAP
      type(active) :: PRR
      type(active) :: PT
      type(active) :: PTL
      type(active) :: PTLAP
      type(active) :: PTR
      type(active) :: PTT
      REAL(w2f__8) THREE
      PARAMETER ( THREE = 3.0D00)
      REAL(w2f__8) TWO
      PARAMETER ( TWO = 2.0D00)
      REAL(w2f__8) ZERO
      PARAMETER ( ZERO = 0.0D00)
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
      DO I = 1, NY, 1
        DO J = 1, NX, 1
          K = (J + NX *(I +(-1)))
          IF((I .eq. 1) .OR.(J .eq. 1)) THEN
            PBL%v = 0.0D00
            CALL zero_deriv(PBL)
          ELSE
            PBL%v = X(K-NX+(-1))%v
            CALL setderiv(PBL,X(K-NX+(-1)))
          ENDIF
          IF (I.eq.1) THEN
            PB%v = 0.0D00
            PBB%v = X(K)%v
            CALL zero_deriv(PB)
            CALL setderiv(PBB,X(K))
          ELSE
            IF (I.eq.2) THEN
              PB%v = X(K-NX)%v
              PBB%v = 0.0D00
              CALL zero_deriv(PBB)
              CALL setderiv(PB,X(K-NX))
            ELSE
              PB%v = X(K-NX)%v
              PBB%v = X(K-NX*2)%v
              CALL setderiv(PB,X(K-NX))
              CALL setderiv(PBB,X(K-NX*2))
            ENDIF
          ENDIF
          IF ((NX.eq.J).OR.(I.eq.1)) THEN
            PBR%v = 0.0D00
            CALL zero_deriv(PBR)
          ELSE
            PBR%v = X(K-NX+1)%v
            CALL setderiv(PBR,X(K-NX+1))
          ENDIF
          IF (J.eq.1) THEN
            PL%v = 0.0D00
            PLL%v = X(K)%v
            CALL zero_deriv(PL)
            CALL setderiv(PLL,X(K))
          ELSE
            IF (J.eq.2) THEN
              PL%v = X(K+(-1))%v
              PLL%v = 0.0D00
              CALL zero_deriv(PLL)
              CALL setderiv(PL,X(K+(-1)))
            ELSE
              PL%v = X(K+(-1))%v
              PLL%v = X(K+(-2))%v
              CALL setderiv(PL,X(K+(-1)))
              CALL setderiv(PLL,X(K+(-2)))
            ENDIF
          ENDIF
          P%v = X(K)%v
          CALL setderiv(P,X(K))
          IF (J.eq.(NX+(-1))) THEN
            PR%v = X(K+1)%v
            PRR%v = 0.0D00
            CALL zero_deriv(PRR)
            CALL setderiv(PR,X(K+1))
          ELSE
            IF (NX.eq.J) THEN
              PR%v = 0.0D00
              PRR%v = X(K)%v
              CALL zero_deriv(PR)
              CALL setderiv(PRR,X(K))
            ELSE
              PR%v = X(K+1)%v
              PRR%v = X(K+2)%v
              CALL setderiv(PR,X(K+1))
              CALL setderiv(PRR,X(K+2))
            ENDIF
          ENDIF
          IF ((NY.eq.I).OR.(J.eq.1)) THEN
            PTL%v = 0.0D00
            CALL zero_deriv(PTL)
          ELSE
            PTL%v = X(NX+K+(-1))%v
            CALL setderiv(PTL,X(NX+K+(-1)))
          ENDIF
          IF (I.eq.(NY+(-1))) THEN
            PT%v = X(NX+K)%v
            PTT%v = 0.0D00
            CALL zero_deriv(PTT)
            CALL setderiv(PT,X(NX+K))
          ELSE
            IF (NY.eq.I) THEN
              PT%v = 0.0D00
              PTT%v = X(K)%v
              CALL zero_deriv(PT)
              CALL setderiv(PTT,X(K))
            ELSE
              PT%v = X(NX+K)%v
              PTT%v = X(K+NX*2)%v
              CALL setderiv(PT,X(NX+K))
              CALL setderiv(PTT,X(K+NX*2))
            ENDIF
          ENDIF
          IF ((NX.eq.J).OR.(NY.eq.I)) THEN
            PTR%v = 0.0D00
            CALL zero_deriv(PTR)
          ELSE
            PTR%v = X(NX+K+1)%v
            CALL setderiv(PTR,X(NX+K+1))
          ENDIF
          OpenAD_aux_0 = (PT%v-PB%v)
          OpenAD_aux_1 = (HY*2.0D00)
          OpenAD_lin_0 = (INT(1_w2f__i8)/OpenAD_aux_1)
          DPDY%v = (OpenAD_aux_0/OpenAD_aux_1)
          OpenAD_aux_2 = (PR%v-PL%v)
          OpenAD_aux_3 = (HX*2.0D00)
          OpenAD_lin_1 = (INT(1_w2f__i8)/OpenAD_aux_3)
          DPDX%v = (OpenAD_aux_2/OpenAD_aux_3)
          OpenAD_aux_4 = (PBB%v+P%v-PB%v*2.0D00)
          OpenAD_aux_5 = (PBL%v+PBR%v-PB%v*2.0D00)
          OpenAD_lin_2 = (INT(1_w2f__i8)/HY2)
          OpenAD_lin_3 = (INT(1_w2f__i8)/HX2)
          PBLAP%v = ((OpenAD_aux_4/HY2)+(OpenAD_aux_5/HX2))
          OpenAD_aux_6 = (PBL%v+PTL%v-PL%v*2.0D00)
          OpenAD_aux_7 = (PLL%v+P%v-PL%v*2.0D00)
          OpenAD_lin_4 = (INT(1_w2f__i8)/HY2)
          OpenAD_lin_5 = (INT(1_w2f__i8)/HX2)
          PLLAP%v = ((OpenAD_aux_6/HY2)+(OpenAD_aux_7/HX2))
          OpenAD_aux_8 = (PB%v+PT%v-P%v*2.0D00)
          OpenAD_aux_9 = (PL%v+PR%v-P%v*2.0D00)
          OpenAD_lin_6 = (INT(1_w2f__i8)/HY2)
          OpenAD_lin_7 = (INT(1_w2f__i8)/HX2)
          PLAP%v = ((OpenAD_aux_8/HY2)+(OpenAD_aux_9/HX2))
          OpenAD_aux_10 = (P%v+PRR%v-PR%v*2.0D00)
          OpenAD_aux_11 = (PBR%v+PTR%v-PR%v*2.0D00)
          OpenAD_lin_8 = (INT(1_w2f__i8)/HX2)
          OpenAD_lin_9 = (INT(1_w2f__i8)/HY2)
          PRLAP%v = ((OpenAD_aux_10/HX2)+(OpenAD_aux_11/HY2))
          OpenAD_aux_12 = (P%v+PTT%v-PT%v*2.0D00)
          OpenAD_aux_13 = (PTL%v+PTR%v-PT%v*2.0D00)
          OpenAD_lin_10 = (INT(1_w2f__i8)/HY2)
          OpenAD_lin_11 = (INT(1_w2f__i8)/HX2)
          PTLAP%v = ((OpenAD_aux_12/HY2)+(OpenAD_aux_13/HX2))
          OpenAD_aux_14 = (PBLAP%v+PTLAP%v-PLAP%v*2.0D00)
          OpenAD_aux_15 = (PLLAP%v+PRLAP%v-PLAP%v*2.0D00)
          OpenAD_aux_19 = (PRLAP%v-PLLAP%v)
          OpenAD_aux_17 = (DPDY%v*OpenAD_aux_19)
          OpenAD_aux_18 = (HX*2.0D00)
          OpenAD_aux_22 = (PTLAP%v-PBLAP%v)
          OpenAD_aux_20 = (DPDX%v*OpenAD_aux_22)
          OpenAD_aux_21 = (HY*2.0D00)
          OpenAD_aux_16 = ((OpenAD_aux_17/OpenAD_aux_18)-(OpenAD_aux_20/
     +OpenAD_aux_21))
          OpenAD_lin_12 = (INT(1_w2f__i8)/HY2)
          OpenAD_lin_13 = (INT(1_w2f__i8)/HX2)
          OpenAD_lin_16 = OpenAD_aux_19
          OpenAD_lin_17 = DPDY%v
          OpenAD_lin_15 = (INT(1_w2f__i8)/OpenAD_aux_18)
          OpenAD_lin_19 = OpenAD_aux_22
          OpenAD_lin_20 = DPDX%v
          OpenAD_lin_18 = (INT(1_w2f__i8)/OpenAD_aux_21)
          OpenAD_lin_14 = R
          FVEC(INT(K))%v = ((OpenAD_aux_14/HY2)+(OpenAD_aux_15/HX2)-R*Op
     +enAD_aux_16)
          OpenAD_acc_0 = (OpenAD_lin_14*INT((-1_w2f__i8)))
          OpenAD_acc_1 = (INT((-1_w2f__i8))*OpenAD_acc_0)
          OpenAD_acc_2 = (OpenAD_lin_20*OpenAD_lin_18*OpenAD_acc_1)
          OpenAD_acc_3 = (OpenAD_lin_1*OpenAD_lin_19*OpenAD_lin_18*OpenA
     +D_acc_1)
          OpenAD_acc_4 = (OpenAD_lin_17*OpenAD_lin_15*OpenAD_acc_0)
          OpenAD_acc_5 = (OpenAD_lin_0*OpenAD_lin_16*OpenAD_lin_15*OpenA
     +D_acc_0)
          OpenAD_acc_6 = (OpenAD_lin_13+OpenAD_acc_4)
          OpenAD_acc_7 = (OpenAD_lin_13+INT((-1_w2f__i8))*OpenAD_acc_4)
          OpenAD_acc_8 = (2.0D00*INT((-1_w2f__i8))*OpenAD_lin_13+2.0D00*
     +INT((-1_w2f__i8))*OpenAD_lin_12)
          OpenAD_acc_9 = (OpenAD_lin_12+OpenAD_acc_2)
          OpenAD_acc_10 = (OpenAD_lin_12+INT((-1_w2f__i8))*OpenAD_acc_2)
          OpenAD_acc_11 = (OpenAD_lin_11*OpenAD_acc_9)
          OpenAD_acc_12 = (OpenAD_lin_10*OpenAD_acc_9)
          OpenAD_acc_13 = (OpenAD_lin_9*OpenAD_acc_6)
          OpenAD_acc_14 = (OpenAD_lin_8*OpenAD_acc_6)
          OpenAD_acc_15 = (OpenAD_lin_7*OpenAD_acc_8)
          OpenAD_acc_16 = (OpenAD_lin_6*OpenAD_acc_8)
          OpenAD_acc_17 = (OpenAD_lin_5*OpenAD_acc_7)
          OpenAD_acc_18 = (OpenAD_lin_4*OpenAD_acc_7)
          OpenAD_acc_19 = (OpenAD_lin_3*OpenAD_acc_10)
          OpenAD_acc_20 = (OpenAD_lin_2*OpenAD_acc_10)
          CALL setderiv(OpenAD_prp_0,PT)
          CALL dec_deriv(OpenAD_prp_0,PB)
          CALL setderiv(OpenAD_prp_1,PR)
          CALL dec_deriv(OpenAD_prp_1,PL)
          CALL setderiv(OpenAD_prp_2,PBB)
          CALL inc_deriv(OpenAD_prp_2,P)
          CALL saxpy(-2.0D00,PB,OpenAD_prp_2)
          CALL setderiv(OpenAD_prp_3,PBL)
          CALL inc_deriv(OpenAD_prp_3,PBR)
          CALL saxpy(-2.0D00,PB,OpenAD_prp_3)
          CALL setderiv(OpenAD_prp_4,PBL)
          CALL inc_deriv(OpenAD_prp_4,PTL)
          CALL saxpy(-2.0D00,PL,OpenAD_prp_4)
          CALL setderiv(OpenAD_prp_5,PLL)
          CALL inc_deriv(OpenAD_prp_5,P)
          CALL saxpy(-2.0D00,PL,OpenAD_prp_5)
          CALL setderiv(OpenAD_prp_6,PB)
          CALL inc_deriv(OpenAD_prp_6,PT)
          CALL saxpy(-2.0D00,P,OpenAD_prp_6)
          CALL setderiv(OpenAD_prp_7,PL)
          CALL inc_deriv(OpenAD_prp_7,PR)
          CALL saxpy(-2.0D00,P,OpenAD_prp_7)
          CALL setderiv(OpenAD_prp_8,P)
          CALL inc_deriv(OpenAD_prp_8,PRR)
          CALL saxpy(-2.0D00,PR,OpenAD_prp_8)
          CALL setderiv(OpenAD_prp_9,PBR)
          CALL inc_deriv(OpenAD_prp_9,PTR)
          CALL saxpy(-2.0D00,PR,OpenAD_prp_9)
          CALL setderiv(OpenAD_prp_10,P)
          CALL inc_deriv(OpenAD_prp_10,PTT)
          CALL saxpy(-2.0D00,PT,OpenAD_prp_10)
          CALL setderiv(OpenAD_prp_11,PTL)
          CALL inc_deriv(OpenAD_prp_11,PTR)
          CALL saxpy(-2.0D00,PT,OpenAD_prp_11)
          CALL sax(OpenAD_acc_3,OpenAD_prp_1,FVEC(K))
          CALL saxpy(OpenAD_acc_5,OpenAD_prp_0,FVEC(K))
          CALL saxpy(OpenAD_acc_11,OpenAD_prp_11,FVEC(K))
          CALL saxpy(OpenAD_acc_12,OpenAD_prp_10,FVEC(K))
          CALL saxpy(OpenAD_acc_13,OpenAD_prp_9,FVEC(K))
          CALL saxpy(OpenAD_acc_14,OpenAD_prp_8,FVEC(K))
          CALL saxpy(OpenAD_acc_15,OpenAD_prp_7,FVEC(K))
          CALL saxpy(OpenAD_acc_16,OpenAD_prp_6,FVEC(K))
          CALL saxpy(OpenAD_acc_17,OpenAD_prp_5,FVEC(K))
          CALL saxpy(OpenAD_acc_18,OpenAD_prp_4,FVEC(K))
          CALL saxpy(OpenAD_acc_19,OpenAD_prp_3,FVEC(K))
          CALL saxpy(OpenAD_acc_20,OpenAD_prp_2,FVEC(K))
        END DO
      END DO
      END SUBROUTINE
