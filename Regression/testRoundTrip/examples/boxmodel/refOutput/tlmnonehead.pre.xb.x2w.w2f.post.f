
C$OPENAD XXX File_start [head.f]
      SUBROUTINE box_forward(ILEV1)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      use all_globals_mod
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      INTEGER(w2f__i4) ILEV1
C
C     **** Local Variables and Functions ****
C
      EXTERNAL box_cycle_fields
      EXTERNAL box_density
      EXTERNAL box_robert_filter
      EXTERNAL box_timestep
      EXTERNAL box_transport
      INTEGER(w2f__i4) IKEY
      INTEGER(w2f__i4) ISBYTE
      PARAMETER ( ISBYTE = 8)
      INTEGER(w2f__i4) L
      INTEGER(w2f__i4) NLEV1
      PARAMETER ( NLEV1 = 73)
      INTEGER(w2f__i4) NLEV2
      PARAMETER ( NLEV2 = 50)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      IKEY = ILEV1
      CALL box_density(TNOW,SNOW,RHO)
      CALL box_transport(RHO,UVEL)
      CALL box_timestep(GAMMA_T,TSTAR,NULLFORCE,UVEL,TNOW,TOLD,TNEW)
      CALL box_timestep(GAMMA_S,SSTAR,FW,UVEL,SNOW,SOLD,SNEW)
      CALL box_robert_filter(TNOW,TOLD,TNEW)
      CALL box_robert_filter(SNOW,SOLD,SNEW)
      CALL box_cycle_fields()
      DO L = 1,3,1
        IF (TNOW(L)%v.LT.(-2.0D00)) THEN
          TNOW(INT(L))%v = 2.0D00
          CALL zero_deriv(TNOW(INT(L)))
        ENDIF
      END DO
      END SUBROUTINE

      SUBROUTINE box_final_state()
      use OAD_active
      use w2f__types
      use oad_intrinsics
      use all_globals_mod
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) ISBYTE
      PARAMETER ( ISBYTE = 8)
      INTEGER(w2f__i4) L
      INTEGER(w2f__i4) NLEV1
      PARAMETER ( NLEV1 = 73)
      INTEGER(w2f__i4) NLEV2
      PARAMETER ( NLEV2 = 50)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      DO L = 1, 6, 1
        TSVEC(INT(L)) = 0.0
      END DO
      DO L = 1, 3, 1
        TSVEC(INT(L)) = TNOW(L)%v
        TSVEC(L+3) = SNOW(L)%v
      END DO
      END SUBROUTINE

      SUBROUTINE box_ini_fields()
      use OAD_active
      use w2f__types
      use oad_intrinsics
      use all_globals_mod
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) ISBYTE
      PARAMETER ( ISBYTE = 8)
      INTEGER(w2f__i4) L
      INTEGER(w2f__i4) NLEV1
      PARAMETER ( NLEV1 = 73)
      INTEGER(w2f__i4) NLEV2
      PARAMETER ( NLEV2 = 50)
      type(active) :: OpenAD_prp_12
      type(active) :: OpenAD_prp_13
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      THC_TOT = 0.0D00
      THC_T = 0.0D00
      THC_S = 0.0D00
      METRIC1 = 0.0D00
      METRIC2 = 0.0D00
      METRIC = 0.0D00
      NULLFORCE(1) = 0.0D00
      NULLFORCE(2) = 0.0D00
      FW(1) = (AREA(1) *(HUNDRED / YEAR) * 3.5D+01)
      FW(2) = (- FW(1))
      TSTAR(1) = 2.2D+01
      TSTAR(2) = 0.0D00
      SSTAR(1) = 3.6D+01
      SSTAR(2) = 3.4D+01
      UBAR = (SV * 2.0D+01)
      T(1)%v = 2.0D+01
      T(2)%v = 1.0D00
      T(3)%v = 1.0D00
      S(1)%v = 3.55D+01
      S(2)%v = 3.45D+01
      S(3)%v = 3.45D+01
      CALL zero_deriv(T(1))
      CALL zero_deriv(T(2))
      CALL zero_deriv(T(3))
      CALL zero_deriv(S(1))
      CALL zero_deriv(S(2))
      CALL zero_deriv(S(3))
      DO L = 1,6,1
        TSVEC(INT(L)) = 0.0
      END DO
      DO L = 1,3,1
        T(INT(L))%v = (T(L)%v+XX(L)%v)
        S(INT(L))%v = (S(L)%v+XX(L+3)%v)
        CALL setderiv(OpenAD_prp_12,T(L))
        CALL setderiv(OpenAD_prp_13,S(L))
        CALL setderiv(T(L),OpenAD_prp_12)
        CALL inc_deriv(T(L),XX(L))
        CALL setderiv(S(L),OpenAD_prp_13)
        CALL inc_deriv(S(L),XX(L+3))
      END DO
      DO L = 1,3,1
        TNEW(INT(L))%v = T(L)%v
        CALL setderiv(TNEW(L),T(L))
        SNEW(INT(L))%v = S(L)%v
        TOLD(INT(L))%v = T(L)%v
        CALL setderiv(SNEW(L),S(L))
        CALL setderiv(TOLD(L),T(L))
        SOLD(INT(L))%v = S(L)%v
        TNOW(INT(L))%v = T(L)%v
        CALL setderiv(SOLD(L),S(L))
        CALL setderiv(TNOW(L),T(L))
        SNOW(INT(L))%v = S(L)%v
        CALL setderiv(SNOW(L),S(L))
      END DO
      UVEL%v = UBAR
      CALL zero_deriv(UVEL)
      END SUBROUTINE

      SUBROUTINE box_ini_params()
      use OAD_active
      use w2f__types
      use oad_intrinsics
      use all_globals_mod
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) ISBYTE
      PARAMETER ( ISBYTE = 8)
      INTEGER(w2f__i4) J
      INTEGER(w2f__i4) L
      INTEGER(w2f__i4) NLEV1
      PARAMETER ( NLEV1 = 73)
      INTEGER(w2f__i4) NLEV2
      PARAMETER ( NLEV2 = 50)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      VERBMODE = .FALSE.
      BLENGTH(1) = 5.0D+08
      BLENGTH(2) = 1.0D+08
      BLENGTH(3) = BLENGTH(1)
      BHEIGHT(1) = 1.0D+05
      BHEIGHT(3) = 4.0D+05
      BHEIGHT(2) = (BHEIGHT(1) + BHEIGHT(3))
      DELTA = (BHEIGHT(1) /(BHEIGHT(1) + BHEIGHT(3)))
      BWIDTH = 4.0D+08
      AREA(1) = (BLENGTH(1) * BWIDTH)
      AREA(2) = (BLENGTH(2) * BWIDTH)
      AREA(3) = (BLENGTH(3) * BWIDTH)
      VOL(1) = (AREA(1) * BHEIGHT(1))
      VOL(2) = (AREA(2) *(BHEIGHT(1) + BHEIGHT(3)))
      VOL(3) = (AREA(3) * BHEIGHT(3))
      ROBERT_FILTER_COEFF = 2.5D-01
      HUNDRED = 1.0D+02
      THOUSAND = 1.0D+03
      DAY = 8.64D+04
      YEAR = (DAY * 3.65D+02)
      SV = 1.0D+12
      U0 = ((SV * 1.6D+01) / 4.00000000000000019169D-04)
      ALPHA = 1.6679999999999998864D-04
      BETA = 7.81000000000000010186D-04
      DAYS_PER_50M_MIXED_LAYER = 5.0D+01
      GAMMA_T = (1.0D00 /(DAY * 3.0D+02))
      GAMMA_S = (0.0D00 /((BHEIGHT(1) / 5.0D+03) * DAY * DAYS_PER_50M_MI
     +XED_LAYER))
      EPSILON_IC = (-1.00000000000000004792D-04)
      NOISE_CORRELATION_TIME = (DAY * 1.5D+01)
      DELTA_T = (DAY * 5.0D00)
      INTEGRATION_TIME = (YEAR * 5.0D+01)
      N_MAX = INT(INTEGRATION_TIME / DELTA_T)
      FDEPS = 9.99999999999999954748D-07
      Y(1) = (THOUSAND / 3.2680000000000000715D-02)
      Y(2) = (THOUSAND / 7.9399999999999991418D-03)
      Y(3) = (THOUSAND / 1.39999999999999998578D-03)
      Y(4) = (THOUSAND / 1.41699999999999992628D-01)
      Y(5) = (THOUSAND / 1.28599999999999992095D-01)
      Y(6) = (THOUSAND / 8.7800000000000003042D-02)
      R(1) = (ALPHA * DELTA)
      R(2) = (- ALPHA)
      R(3) = (ALPHA *(1.0D00 - DELTA))
      R(4) = (-(BETA * DELTA))
      R(5) = BETA
      R(6) = (-(BETA *(1.0D00 - DELTA)))
      DO L = 1, 6, 1
        R(INT(L)) = ((R(L) * U0) / SV)
      END DO
      DO L = 1, 6, 1
        IF(L .LE. 3) THEN
          PROJ_T(INT(L)) = 1.0D00
          PROJ_S(INT(L)) = 0.0D00
        ELSE
          PROJ_T(INT(L)) = 0.0D00
          PROJ_S(INT(L)) = 1.0D00
        ENDIF
      END DO
      DO L = 1, 6, 1
        R_T(INT(L)) = (PROJ_T(L) * R(L))
        R_S(INT(L)) = (PROJ_S(L) * R(L))
      END DO
      R1(1) = 1.0D00
      R1(2) = (-1.0D00)
      R1(3) = 0.0D00
      R1(4) = 0.0D00
      R1(5) = 0.0D00
      R1(6) = 0.0D00
      DO J = 1, 6, 1
        DO I = 1, 6, 1
          X(INT(I), INT(J)) = (R(I) * R(J))
        END DO
      END DO
      EPSILON_REGULARIZE = 1.00000000000000002092D-08
      DO J = 1, 6, 1
        DO I = 1, 6, 1
          X(INT(I), INT(J)) = (X(I, J) + EPSILON_REGULARIZE)
        END DO
      END DO
      END SUBROUTINE

      SUBROUTINE box_model_body()
      use OAD_active
      use w2f__types
      use oad_intrinsics
      use all_globals_mod
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      EXTERNAL box_final_state
      EXTERNAL box_forward
      EXTERNAL box_ini_fields
      INTEGER(w2f__i4) ILEV1
      INTEGER(w2f__i4) ILEV2
      INTEGER(w2f__i4) ILOOP
      INTEGER(w2f__i4) ISBYTE
      PARAMETER ( ISBYTE = 8)
      INTEGER(w2f__i4) MAXLEV2
      INTEGER(w2f__i4) NLEV1
      PARAMETER ( NLEV1 = 73)
      INTEGER(w2f__i4) NLEV2
      PARAMETER ( NLEV2 = 50)
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(XX)
C$OPENAD DEPENDENT(TNEW)
C$OPENAD DEPENDENT(SNEW)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      CALL box_ini_fields()
      MAXLEV2 = ((N_MAX / 73) + 1)
      IF(N_MAX .GT. 3650) THEN
        WRITE(*,*) 'NEED TO SET nlev1*nlev2 >= n_max '
      ELSE
        DO ILEV2 = 1, 50, 1
          IF(ILEV2 .LE. MAXLEV2) THEN
            DO ILEV1 = 1, 73, 1
              ILOOP = (ILEV1 + ILEV2 * 73 +(-73))
              IF(ILOOP .LE. N_MAX) THEN
                CALL box_forward(ILEV1)
              ENDIF
            END DO
          ENDIF
        END DO
        CALL box_final_state()
      ENDIF
      END SUBROUTINE

      SUBROUTINE box_density(TLOC, SLOC, RHOLOC)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      use all_globals_mod
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: TLOC(1:3)
      type(active) :: SLOC(1:3)
      type(active) :: RHOLOC(1:3)
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) ISBYTE
      PARAMETER ( ISBYTE = 8)
      INTEGER(w2f__i4) L
      INTEGER(w2f__i4) NLEV1
      PARAMETER ( NLEV1 = 73)
      INTEGER(w2f__i4) NLEV2
      PARAMETER ( NLEV2 = 50)
      REAL(w2f__8) OpenAD_acc_0
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      DO L = 1, 3, 1
        OpenAD_lin_0 = BETA
        OpenAD_lin_1 = ALPHA
        RHOLOC(INT(L))%v = (SLOC(L)%v*BETA-TLOC(L)%v*ALPHA)
        OpenAD_acc_0 = (OpenAD_lin_1*INT((-1_w2f__i8)))
        CALL sax(OpenAD_lin_0,SLOC(L),RHOLOC(L))
        CALL saxpy(OpenAD_acc_0,TLOC(L),RHOLOC(L))
      END DO
      END SUBROUTINE

      SUBROUTINE box_transport(RHOLOC, UVELLOC)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      use all_globals_mod
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: RHOLOC(1:3)
      type(active) :: UVELLOC
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) ISBYTE
      PARAMETER ( ISBYTE = 8)
      INTEGER(w2f__i4) NLEV1
      PARAMETER ( NLEV1 = 73)
      INTEGER(w2f__i4) NLEV2
      PARAMETER ( NLEV2 = 50)
      REAL(w2f__8) OpenAD_acc_1
      REAL(w2f__8) OpenAD_aux_0
      REAL(w2f__8) OpenAD_aux_1
      REAL(w2f__8) OpenAD_lin_2
      REAL(w2f__8) OpenAD_lin_3
      REAL(w2f__8) OpenAD_lin_4
      type(active) :: OpenAD_prp_0
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      OpenAD_aux_1 = (1.0D00 - DELTA)
      OpenAD_aux_0 = (RHOLOC(1)%v*DELTA+RHOLOC(3)%v*OpenAD_aux_1-RHOLOC(
     +2)%v)
      OpenAD_lin_3 = DELTA
      OpenAD_lin_4 = OpenAD_aux_1
      OpenAD_lin_2 = U0
      UVELLOC%v = (-(U0*OpenAD_aux_0))
      OpenAD_acc_1 = (OpenAD_lin_2*INT((-1_w2f__i8)))
      CALL set_neg_deriv(OpenAD_prp_0,RHOLOC(2))
      CALL saxpy(OpenAD_lin_3,RHOLOC(1),OpenAD_prp_0)
      CALL saxpy(OpenAD_lin_4,RHOLOC(3),OpenAD_prp_0)
      CALL sax(OpenAD_acc_1,OpenAD_prp_0,UVELLOC)
      END SUBROUTINE

      SUBROUTINE box_robert_filter(FLDNOW, FLDOLD, FLDNEW)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      use all_globals_mod
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: FLDNOW(1:3)
      type(active) :: FLDOLD(1:3)
      type(active) :: FLDNEW(1:3)
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) ISBYTE
      PARAMETER ( ISBYTE = 8)
      INTEGER(w2f__i4) L
      INTEGER(w2f__i4) NLEV1
      PARAMETER ( NLEV1 = 73)
      INTEGER(w2f__i4) NLEV2
      PARAMETER ( NLEV2 = 50)
      REAL(w2f__8) OpenAD_aux_22
      REAL(w2f__8) OpenAD_lin_31
      type(active) :: OpenAD_prp_7
      type(active) :: OpenAD_prp_8
      type(active) :: OpenAD_prp_9
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      DO L = 1, 3, 1
        OpenAD_aux_22 = (FLDOLD(L)%v+FLDNEW(L)%v-FLDNOW(L)%v*2.0D00)
        OpenAD_lin_31 = ROBERT_FILTER_COEFF
        FLDNOW(INT(L))%v = (FLDNOW(L)%v+ROBERT_FILTER_COEFF*OpenAD_aux_2
     +2)
        CALL setderiv(OpenAD_prp_7,FLDNOW(L))
        CALL setderiv(OpenAD_prp_8,FLDNOW(L))
        CALL setderiv(OpenAD_prp_9,FLDOLD(L))
        CALL inc_deriv(OpenAD_prp_9,FLDNEW(L))
        CALL saxpy(-2.0D00,OpenAD_prp_8,OpenAD_prp_9)
        CALL setderiv(FLDNOW(L),OpenAD_prp_7)
        CALL saxpy(OpenAD_lin_31,OpenAD_prp_9,FLDNOW(L))
      END DO
      END SUBROUTINE

      SUBROUTINE box_cycle_fields()
      use OAD_active
      use w2f__types
      use oad_intrinsics
      use all_globals_mod
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) ISBYTE
      PARAMETER ( ISBYTE = 8)
      INTEGER(w2f__i4) L
      INTEGER(w2f__i4) NLEV1
      PARAMETER ( NLEV1 = 73)
      INTEGER(w2f__i4) NLEV2
      PARAMETER ( NLEV2 = 50)
      type(active) :: OpenAD_prp_10
      type(active) :: OpenAD_prp_11
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      DO L = 1, 3, 1
        TOLD(INT(L))%v = TNOW(L)%v
        TNOW(INT(L))%v = TNEW(L)%v
        CALL setderiv(OpenAD_prp_10,TNOW(L))
        CALL setderiv(TOLD(L),OpenAD_prp_10)
        CALL setderiv(TNOW(L),TNEW(L))
        SOLD(INT(L))%v = SNOW(L)%v
        SNOW(INT(L))%v = SNEW(L)%v
        CALL setderiv(OpenAD_prp_11,SNOW(L))
        CALL setderiv(SOLD(L),OpenAD_prp_11)
        CALL setderiv(SNOW(L),SNEW(L))
      END DO
      END SUBROUTINE

      SUBROUTINE box_update(FLDNEW, FLDOLD, DFLDDT)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      use all_globals_mod
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: FLDNEW(1:3)
      type(active) :: FLDOLD(1:3)
      type(active) :: DFLDDT(1:3)
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) ISBYTE
      PARAMETER ( ISBYTE = 8)
      INTEGER(w2f__i4) L
      INTEGER(w2f__i4) NLEV1
      PARAMETER ( NLEV1 = 73)
      INTEGER(w2f__i4) NLEV2
      PARAMETER ( NLEV2 = 50)
      REAL(w2f__8) OpenAD_aux_23
      REAL(w2f__8) OpenAD_lin_32
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      DO L = 1, 3, 1
        OpenAD_aux_23 = (DELTA_T * 2.0D00)
        OpenAD_lin_32 = OpenAD_aux_23
        FLDNEW(INT(L))%v = (FLDOLD(L)%v+DFLDDT(L)%v*OpenAD_aux_23)
        CALL setderiv(FLDNEW(L),FLDOLD(L))
        CALL saxpy(OpenAD_lin_32,DFLDDT(L),FLDNEW(L))
      END DO
      END SUBROUTINE

      SUBROUTINE box_timestep(GAMMALOC, FLDSTAR, EXTFORLOC, UVELLOC, FLD
     +NOW, FLDOLD, FLDNEW)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      use all_globals_mod
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      REAL(w2f__8) GAMMALOC
      REAL(w2f__8) FLDSTAR(1 : 2)
      REAL(w2f__8) EXTFORLOC(1 : 2)
      type(active) :: UVELLOC
      type(active) :: FLDNOW(1:3)
      type(active) :: FLDOLD(1:3)
      type(active) :: FLDNEW(1:3)
C
C     **** Local Variables and Functions ****
C
      EXTERNAL box_update
      type(active) :: DFLDDT(1:3)
      INTEGER(w2f__i4) ISBYTE
      PARAMETER ( ISBYTE = 8)
      INTEGER(w2f__i4) NLEV1
      PARAMETER ( NLEV1 = 73)
      INTEGER(w2f__i4) NLEV2
      PARAMETER ( NLEV2 = 50)
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
      REAL(w2f__8) OpenAD_acc_3
      REAL(w2f__8) OpenAD_acc_4
      REAL(w2f__8) OpenAD_acc_5
      REAL(w2f__8) OpenAD_acc_6
      REAL(w2f__8) OpenAD_acc_7
      REAL(w2f__8) OpenAD_acc_8
      REAL(w2f__8) OpenAD_acc_9
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
      REAL(w2f__8) OpenAD_aux_3
      REAL(w2f__8) OpenAD_aux_4
      REAL(w2f__8) OpenAD_aux_5
      REAL(w2f__8) OpenAD_aux_6
      REAL(w2f__8) OpenAD_aux_7
      REAL(w2f__8) OpenAD_aux_8
      REAL(w2f__8) OpenAD_aux_9
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
      REAL(w2f__8) OpenAD_lin_30
      REAL(w2f__8) OpenAD_lin_5
      REAL(w2f__8) OpenAD_lin_6
      REAL(w2f__8) OpenAD_lin_7
      REAL(w2f__8) OpenAD_lin_8
      REAL(w2f__8) OpenAD_lin_9
      type(active) :: OpenAD_prp_1
      type(active) :: OpenAD_prp_2
      type(active) :: OpenAD_prp_3
      type(active) :: OpenAD_prp_4
      type(active) :: OpenAD_prp_5
      type(active) :: OpenAD_prp_6
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      IF (UVELLOC%v.GE.0.0D00) THEN
        OpenAD_aux_4 = (FLDSTAR(1)-FLDNOW(1)%v)
        OpenAD_aux_3 = (GAMMALOC*OpenAD_aux_4)
        OpenAD_aux_5 = (FLDNOW(3)%v-FLDNOW(1)%v)
        OpenAD_aux_2 = (EXTFORLOC(1)+VOL(1)*OpenAD_aux_3+UVELLOC%v*OpenA
     +D_aux_5)
        OpenAD_lin_7 = GAMMALOC
        OpenAD_lin_6 = VOL(1)
        OpenAD_lin_8 = OpenAD_aux_5
        OpenAD_lin_9 = UVELLOC%v
        OpenAD_lin_5 = (INT(1_w2f__i8)/VOL(1))
        DFLDDT(1)%v = (OpenAD_aux_2/VOL(1))
        OpenAD_acc_2 = (OpenAD_lin_8*OpenAD_lin_5)
        OpenAD_acc_3 = (OpenAD_lin_9*OpenAD_lin_5)
        OpenAD_acc_4 = (INT((-1_w2f__i8))*OpenAD_lin_7*OpenAD_lin_6*Open
     +AD_lin_5)
        CALL setderiv(OpenAD_prp_1,FLDNOW(3))
        CALL dec_deriv(OpenAD_prp_1,FLDNOW(1))
        CALL sax(OpenAD_acc_2,UVELLOC,DFLDDT(1))
        CALL saxpy(OpenAD_acc_3,OpenAD_prp_1,DFLDDT(1))
        CALL saxpy(OpenAD_acc_4,FLDNOW(1),DFLDDT(1))
        OpenAD_aux_8 = (FLDSTAR(2)-FLDNOW(2)%v)
        OpenAD_aux_7 = (GAMMALOC*OpenAD_aux_8)
        OpenAD_aux_9 = (FLDNOW(1)%v-FLDNOW(2)%v)
        OpenAD_aux_6 = (EXTFORLOC(2)+VOL(2)*OpenAD_aux_7+UVELLOC%v*OpenA
     +D_aux_9)
        OpenAD_lin_12 = GAMMALOC
        OpenAD_lin_11 = VOL(2)
        OpenAD_lin_13 = OpenAD_aux_9
        OpenAD_lin_14 = UVELLOC%v
        OpenAD_lin_10 = (INT(1_w2f__i8)/VOL(2))
        DFLDDT(2)%v = (OpenAD_aux_6/VOL(2))
        OpenAD_acc_5 = (OpenAD_lin_13*OpenAD_lin_10)
        OpenAD_acc_6 = (OpenAD_lin_14*OpenAD_lin_10)
        OpenAD_acc_7 = (INT((-1_w2f__i8))*OpenAD_lin_12*OpenAD_lin_11*Op
     +enAD_lin_10)
        CALL setderiv(OpenAD_prp_2,FLDNOW(1))
        CALL dec_deriv(OpenAD_prp_2,FLDNOW(2))
        CALL sax(OpenAD_acc_5,UVELLOC,DFLDDT(2))
        CALL saxpy(OpenAD_acc_6,OpenAD_prp_2,DFLDDT(2))
        CALL saxpy(OpenAD_acc_7,FLDNOW(2),DFLDDT(2))
        OpenAD_aux_11 = (FLDNOW(2)%v-FLDNOW(3)%v)
        OpenAD_aux_10 = (UVELLOC%v*OpenAD_aux_11)
        OpenAD_lin_16 = OpenAD_aux_11
        OpenAD_lin_17 = UVELLOC%v
        OpenAD_lin_15 = (INT(1_w2f__i8)/VOL(3))
        DFLDDT(3)%v = (OpenAD_aux_10/VOL(3))
        OpenAD_acc_8 = (OpenAD_lin_16*OpenAD_lin_15)
        OpenAD_acc_9 = (OpenAD_lin_17*OpenAD_lin_15)
        CALL setderiv(OpenAD_prp_3,FLDNOW(2))
        CALL dec_deriv(OpenAD_prp_3,FLDNOW(3))
        CALL sax(OpenAD_acc_8,UVELLOC,DFLDDT(3))
        CALL saxpy(OpenAD_acc_9,OpenAD_prp_3,DFLDDT(3))
      ELSE
        OpenAD_aux_14 = (FLDSTAR(1)-FLDNOW(1)%v)
        OpenAD_aux_13 = (GAMMALOC*OpenAD_aux_14)
        OpenAD_aux_15 = (FLDNOW(2)%v-FLDNOW(1)%v)
        OpenAD_aux_12 = (EXTFORLOC(1)+VOL(1)*OpenAD_aux_13-UVELLOC%v*Ope
     +nAD_aux_15)
        OpenAD_lin_20 = GAMMALOC
        OpenAD_lin_19 = VOL(1)
        OpenAD_lin_21 = OpenAD_aux_15
        OpenAD_lin_22 = UVELLOC%v
        OpenAD_lin_18 = (INT(1_w2f__i8)/VOL(1))
        DFLDDT(1)%v = (OpenAD_aux_12/VOL(1))
        OpenAD_acc_10 = (INT((-1_w2f__i8))*OpenAD_lin_18)
        OpenAD_acc_11 = (OpenAD_lin_21*OpenAD_acc_10)
        OpenAD_acc_12 = (OpenAD_lin_22*OpenAD_acc_10)
        OpenAD_acc_13 = (INT((-1_w2f__i8))*OpenAD_lin_20*OpenAD_lin_19*O
     +penAD_lin_18)
        CALL setderiv(OpenAD_prp_4,FLDNOW(2))
        CALL dec_deriv(OpenAD_prp_4,FLDNOW(1))
        CALL sax(OpenAD_acc_11,UVELLOC,DFLDDT(1))
        CALL saxpy(OpenAD_acc_12,OpenAD_prp_4,DFLDDT(1))
        CALL saxpy(OpenAD_acc_13,FLDNOW(1),DFLDDT(1))
        OpenAD_aux_18 = (FLDSTAR(2)-FLDNOW(2)%v)
        OpenAD_aux_17 = (GAMMALOC*OpenAD_aux_18)
        OpenAD_aux_19 = (FLDNOW(3)%v-FLDNOW(2)%v)
        OpenAD_aux_16 = (EXTFORLOC(2)+VOL(2)*OpenAD_aux_17-UVELLOC%v*Ope
     +nAD_aux_19)
        OpenAD_lin_25 = GAMMALOC
        OpenAD_lin_24 = VOL(2)
        OpenAD_lin_26 = OpenAD_aux_19
        OpenAD_lin_27 = UVELLOC%v
        OpenAD_lin_23 = (INT(1_w2f__i8)/VOL(2))
        DFLDDT(2)%v = (OpenAD_aux_16/VOL(2))
        OpenAD_acc_14 = (INT((-1_w2f__i8))*OpenAD_lin_23)
        OpenAD_acc_15 = (OpenAD_lin_26*OpenAD_acc_14)
        OpenAD_acc_16 = (OpenAD_lin_27*OpenAD_acc_14)
        OpenAD_acc_17 = (INT((-1_w2f__i8))*OpenAD_lin_25*OpenAD_lin_24*O
     +penAD_lin_23)
        CALL setderiv(OpenAD_prp_5,FLDNOW(3))
        CALL dec_deriv(OpenAD_prp_5,FLDNOW(2))
        CALL sax(OpenAD_acc_15,UVELLOC,DFLDDT(2))
        CALL saxpy(OpenAD_acc_16,OpenAD_prp_5,DFLDDT(2))
        CALL saxpy(OpenAD_acc_17,FLDNOW(2),DFLDDT(2))
        OpenAD_aux_21 = (FLDNOW(1)%v-FLDNOW(3)%v)
        OpenAD_aux_20 = (UVELLOC%v*OpenAD_aux_21)
        OpenAD_lin_29 = OpenAD_aux_21
        OpenAD_lin_30 = UVELLOC%v
        OpenAD_lin_28 = (INT(1_w2f__i8)/VOL(3))
        DFLDDT(3)%v = (-(OpenAD_aux_20/VOL(3)))
        OpenAD_acc_18 = (OpenAD_lin_29*OpenAD_lin_28*INT((-1_w2f__i8)))
        OpenAD_acc_19 = (OpenAD_lin_30*OpenAD_lin_28*INT((-1_w2f__i8)))
        CALL setderiv(OpenAD_prp_6,FLDNOW(1))
        CALL dec_deriv(OpenAD_prp_6,FLDNOW(3))
        CALL sax(OpenAD_acc_18,UVELLOC,DFLDDT(3))
        CALL saxpy(OpenAD_acc_19,OpenAD_prp_6,DFLDDT(3))
      ENDIF
      CALL box_update(FLDNEW,FLDOLD,DFLDDT)
      END SUBROUTINE
