
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
C     **** Global Variables & Derived Type Definitions ****
C
      REAL(w2f__8) ALPHA
      REAL(w2f__8) AREA(1 : 3)
      REAL(w2f__8) BETA
      REAL(w2f__8) BHEIGHT(1 : 3)
      REAL(w2f__8) BLENGTH(1 : 3)
      REAL(w2f__8) BWIDTH
      REAL(w2f__8) DAY
      REAL(w2f__8) DAYS_PER_50M_MIXED_LAYER
      REAL(w2f__8) DELTA
      REAL(w2f__8) DELTA_T
      REAL(w2f__8) EPSILON_IC
      REAL(w2f__8) EPSILON_REGULARIZE
      REAL(w2f__8) FDEPS
      REAL(w2f__8) FW(1 : 2)
      REAL(w2f__8) GAMMA_S
      REAL(w2f__8) GAMMA_T
      REAL(w2f__8) HUNDRED
      REAL(w2f__8) INTEGRATION_TIME
      REAL(w2f__8) METRIC
      REAL(w2f__8) METRIC1
      REAL(w2f__8) METRIC2
      REAL(w2f__8) NOISE_CORRELATION_TIME
      REAL(w2f__8) NULLFORCE(1 : 2)
      INTEGER(w2f__i4) N_MAX
      REAL(w2f__8) PROJ_S(1 : 6)
      REAL(w2f__8) PROJ_T(1 : 6)
      REAL(w2f__8) R(1 : 6)
      REAL(w2f__8) R1(1 : 6)
      TYPE (OpenADTy_active) RHO(1 : 3)
      REAL(w2f__8) ROBERT_FILTER_COEFF
      REAL(w2f__8) R_S(1 : 6)
      REAL(w2f__8) R_T(1 : 6)
      TYPE (OpenADTy_active) S(1 : 3)
      TYPE (OpenADTy_active) SNEW(1 : 3)
      TYPE (OpenADTy_active) SNOW(1 : 3)
      TYPE (OpenADTy_active) SOLD(1 : 3)
      REAL(w2f__8) SSTAR(1 : 2)
      REAL(w2f__8) SV
      TYPE (OpenADTy_active) T(1 : 3)
      REAL(w2f__8) THC_S
      REAL(w2f__8) THC_T
      REAL(w2f__8) THC_TOT
      REAL(w2f__8) THOUSAND
      TYPE (OpenADTy_active) TNEW(1 : 3)
      TYPE (OpenADTy_active) TNOW(1 : 3)
      TYPE (OpenADTy_active) TOLD(1 : 3)
      REAL(w2f__8) TSTAR(1 : 2)
      REAL(w2f__8) TSVEC(1 : 6)
      REAL(w2f__8) U0
      REAL(w2f__8) UBAR
      TYPE (OpenADTy_active) UVEL
      LOGICAL(w2f__i4) VERBMODE
      REAL(w2f__8) VOL(1 : 3)
      REAL(w2f__8) X(1 : 6, 1 : 6)
      TYPE (OpenADTy_active) XX(1 : 6)
      REAL(w2f__8) Y(1 : 6)
      REAL(w2f__8) YEAR
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) NDIM
      PARAMETER ( NDIM = 3)
C
C     **** Statements ****
C
      END MODULE

C$OPENAD XXX File_start [head.f]
      SUBROUTINE box_forward(ILEV1)
      use w2f__types
      use oad_intrinsics
      use all_globals_mod
      use oad_intrinsics
      use all_globals_mod
      use oad_intrinsics
      use all_globals_mod
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      INTEGER(w2f__i8) OpenAD_Symbol_0
      INTEGER(w2f__i8) OpenAD_Symbol_1
      INTEGER(w2f__i8) OpenAD_Symbol_10
      INTEGER(w2f__i8) OpenAD_Symbol_11
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
      INTEGER(w2f__i4) OpenAD_Symbol_151
C
C     **** Statements ****
C
C     $OpenAD$ BEGIN REPLACEMENT 1
C$OPENAD XXX Template ad_template.f
      IKEY = ILEV1
      CALL box_density(__deriv__(TNOW), __deriv__(SNOW), __deriv__(RHO)
     > )
      CALL box_transport(__deriv__(RHO), __deriv__(UVEL))
      CALL box_timestep(GAMMA_T, TSTAR, NULLFORCE, __deriv__(UVEL),
     >  __deriv__(TNOW), __deriv__(TOLD), __deriv__(TNEW))
      CALL box_timestep(GAMMA_S, SSTAR, FW, __deriv__(UVEL), __deriv__(
     > SNOW), __deriv__(SOLD), __deriv__(SNEW))
      CALL box_robert_filter(__deriv__(TNOW), __deriv__(TOLD),
     >  __deriv__(TNEW))
      CALL box_robert_filter(__deriv__(SNOW), __deriv__(SOLD),
     >  __deriv__(SNEW))
      CALL box_cycle_fields()
      DO L = 1, 3, 1
        IF(__value__(TNOW(L)) .LT.(-2.0D00)) THEN
          __value__(TNOW(INT(L))) = 2.0D00
        ENDIF
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      IKEY = ILEV1
      CALL box_density(__deriv__(TNOW), __deriv__(SNOW), __deriv__(RHO)
     > )
      CALL box_transport(__deriv__(RHO), __deriv__(UVEL))
      CALL box_timestep(GAMMA_T, TSTAR, NULLFORCE, __deriv__(UVEL),
     >  __deriv__(TNOW), __deriv__(TOLD), __deriv__(TNEW))
      CALL box_timestep(GAMMA_S, SSTAR, FW, __deriv__(UVEL), __deriv__(
     > SNOW), __deriv__(SOLD), __deriv__(SNEW))
      CALL box_robert_filter(__deriv__(TNOW), __deriv__(TOLD),
     >  __deriv__(TNEW))
      CALL box_robert_filter(__deriv__(SNOW), __deriv__(SOLD),
     >  __deriv__(SNEW))
      CALL box_cycle_fields()
      OpenAD_Symbol_3 = 0_w2f__i8
      DO L = 1, 3, 1
        IF(__value__(TNOW(L)) .LT.(-2.0D00)) THEN
          __value__(TNOW(INT(L))) = 2.0D00
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(L)
          OpenAD_Symbol_4 = 1_w2f__i8
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_4)
        ELSE
          OpenAD_Symbol_5 = 0_w2f__i8
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_5)
        ENDIF
        OpenAD_Symbol_3 = (INT(OpenAD_Symbol_3) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_3)
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
          CALL pop_i_s0(OpenAD_Symbol_151)
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(TNOW(OpenAD_Symbol_151)))
        ENDIF
        OpenAD_Symbol_1 = INT(OpenAD_Symbol_1) + 1
      END DO
      CALL box_cycle_fields()
      CALL box_robert_filter(__deriv__(SNOW), __deriv__(SOLD),
     >  __deriv__(SNEW))
      CALL box_robert_filter(__deriv__(TNOW), __deriv__(TOLD),
     >  __deriv__(TNEW))
      CALL box_timestep(GAMMA_S, SSTAR, FW, __deriv__(UVEL), __deriv__(
     > SNOW), __deriv__(SOLD), __deriv__(SNEW))
      CALL box_timestep(GAMMA_T, TSTAR, NULLFORCE, __deriv__(UVEL),
     >  __deriv__(TNOW), __deriv__(TOLD), __deriv__(TNEW))
      CALL box_transport(__deriv__(RHO), __deriv__(UVEL))
      CALL box_density(__deriv__(TNOW), __deriv__(SNOW), __deriv__(RHO)
     > )
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 4
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(ALPHA)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(BETA)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(DELTA)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(DELTA_T)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(GAMMA_S)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(GAMMA_T)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(ROBERT_FILTER_COEFF)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(U0)
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(UVEL))
C     $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
      CALL cp_arg_store_integer_scalar(ILEV1)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(FW)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(NULLFORCE)
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(RHO))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(SNEW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(SNOW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(SOLD))
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(SSTAR)
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(TNEW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(TNOW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(TOLD))
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(TSTAR)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(VOL)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 5
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 6
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(VOL)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(TSTAR)
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(TOLD))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(TNOW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(TNEW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(SSTAR)
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(SOLD))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(SNOW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(SNEW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(RHO))
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(NULLFORCE)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(FW)
C     $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
      CALL cp_arg_restore_integer_scalar(ILEV1)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(UVEL))
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(U0)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(ROBERT_FILTER_COEFF)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(GAMMA_T)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(GAMMA_S)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(DELTA_T)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(DELTA)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(BETA)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(ALPHA)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 7
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 8
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(UVEL))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(RHO))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(SNEW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(SNOW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(SOLD))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(TNEW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(TNOW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(TOLD))
C     $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
      CALL cp_arg_store_integer_scalar(ILEV1)
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(SNOW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(TNOW))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 9
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(TNOW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(SNOW))
C     $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
      CALL cp_arg_restore_integer_scalar(ILEV1)
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(TOLD))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(TNOW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(TNEW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(SOLD))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(SNOW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(SNEW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(RHO))
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(UVEL))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 10
C$OPENAD XXX Template ad_template.f
      IKEY = ILEV1
      CALL box_density(__deriv__(TNOW), __deriv__(SNOW), __deriv__(RHO)
     > )
      CALL box_transport(__deriv__(RHO), __deriv__(UVEL))
      CALL box_timestep(GAMMA_T, TSTAR, NULLFORCE, __deriv__(UVEL),
     >  __deriv__(TNOW), __deriv__(TOLD), __deriv__(TNEW))
      CALL box_timestep(GAMMA_S, SSTAR, FW, __deriv__(UVEL), __deriv__(
     > SNOW), __deriv__(SOLD), __deriv__(SNEW))
      CALL box_robert_filter(__deriv__(TNOW), __deriv__(TOLD),
     >  __deriv__(TNEW))
      CALL box_robert_filter(__deriv__(SNOW), __deriv__(SOLD),
     >  __deriv__(SNEW))
      CALL box_cycle_fields()
      OpenAD_Symbol_9 = 0_w2f__i8
      DO L = 1, 3, 1
        IF(__value__(TNOW(L)) .LT.(-2.0D00)) THEN
          __value__(TNOW(INT(L))) = 2.0D00
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(L)
          OpenAD_Symbol_10 = 1_w2f__i8
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_10)
        ELSE
          OpenAD_Symbol_11 = 0_w2f__i8
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_11)
        ENDIF
        OpenAD_Symbol_9 = (INT(OpenAD_Symbol_9) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_9)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_6)
      OpenAD_Symbol_7 = 1
      DO WHILE(INT(OpenAD_Symbol_7) .LE. INT(OpenAD_Symbol_6))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_8)
        IF(OpenAD_Symbol_8 .ne. 0) THEN
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_151)
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(TNOW(OpenAD_Symbol_151)))
        ENDIF
        OpenAD_Symbol_7 = INT(OpenAD_Symbol_7) + 1
      END DO
      CALL box_cycle_fields()
      CALL box_robert_filter(__deriv__(SNOW), __deriv__(SOLD),
     >  __deriv__(SNEW))
      CALL box_robert_filter(__deriv__(TNOW), __deriv__(TOLD),
     >  __deriv__(TNEW))
      CALL box_timestep(GAMMA_S, SSTAR, FW, __deriv__(UVEL), __deriv__(
     > SNOW), __deriv__(SOLD), __deriv__(SNEW))
      CALL box_timestep(GAMMA_T, TSTAR, NULLFORCE, __deriv__(UVEL),
     >  __deriv__(TNOW), __deriv__(TOLD), __deriv__(TNEW))
      CALL box_transport(__deriv__(RHO), __deriv__(UVEL))
      CALL box_density(__deriv__(TNOW), __deriv__(SNOW), __deriv__(RHO)
     > )
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a_d(subst)
      CALL cp_arg_store_real_scalar_a_d(__deriv__(UVEL))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
      CALL cp_arg_store_real_vector_a_d(__deriv__(RHO))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
      CALL cp_arg_store_real_vector_a_d(__deriv__(SNEW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
      CALL cp_arg_store_real_vector_a_d(__deriv__(SNOW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
      CALL cp_arg_store_real_vector_a_d(__deriv__(SOLD))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
      CALL cp_arg_store_real_vector_a_d(__deriv__(TNEW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
      CALL cp_arg_store_real_vector_a_d(__deriv__(TNOW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
      CALL cp_arg_store_real_vector_a_d(__deriv__(TOLD))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
      CALL cp_arg_restore_real_vector_a_d(__deriv__(TOLD))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
      CALL cp_arg_restore_real_vector_a_d(__deriv__(TNOW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
      CALL cp_arg_restore_real_vector_a_d(__deriv__(TNEW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
      CALL cp_arg_restore_real_vector_a_d(__deriv__(SOLD))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
      CALL cp_arg_restore_real_vector_a_d(__deriv__(SNOW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
      CALL cp_arg_restore_real_vector_a_d(__deriv__(SNEW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
      CALL cp_arg_restore_real_vector_a_d(__deriv__(RHO))
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a_d(subst)
      CALL cp_arg_restore_real_scalar_a_d(__deriv__(UVEL))
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE

      SUBROUTINE box_final_state()
      use w2f__types
      use oad_intrinsics
      use all_globals_mod
      use oad_intrinsics
      use all_globals_mod
      use oad_intrinsics
      use all_globals_mod
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
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
C     $OpenAD$ BEGIN REPLACEMENT 1
C$OPENAD XXX Template ad_template.f
      DO L = 1, 6, 1
        TSVEC(INT(L)) = 0.0
      END DO
      DO L = 1, 3, 1
        TSVEC(INT(L)) = __value__(TNOW(L))
        TSVEC(L + 3) = __value__(SNOW(L))
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      OpenAD_Symbol_40 = 0_w2f__i8
      DO L = 1, 6, 1
        TSVEC(INT(L)) = 0.0
        OpenAD_Symbol_40 = (INT(OpenAD_Symbol_40) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_40)
      OpenAD_Symbol_41 = 0_w2f__i8
      DO L = 1, 3, 1
        TSVEC(INT(L)) = __value__(TNOW(L))
        TSVEC(L + 3) = __value__(SNOW(L))
        OpenAD_Symbol_41 = (INT(OpenAD_Symbol_41) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_41)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_36)
      OpenAD_Symbol_37 = 1
      DO WHILE(INT(OpenAD_Symbol_37) .LE. INT(OpenAD_Symbol_36))
        OpenAD_Symbol_37 = INT(OpenAD_Symbol_37) + 1
      END DO
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_38)
      OpenAD_Symbol_39 = 1
      DO WHILE(INT(OpenAD_Symbol_39) .LE. INT(OpenAD_Symbol_38))
        OpenAD_Symbol_39 = INT(OpenAD_Symbol_39) + 1
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 4
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(SNOW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(TNOW))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 5
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 6
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(TNOW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(SNOW))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 7
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 8
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(TSVEC)
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(SNOW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(TNOW))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 9
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(TNOW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(SNOW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(TSVEC)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 10
C$OPENAD XXX Template ad_template.f
      OpenAD_Symbol_46 = 0_w2f__i8
      DO L = 1, 6, 1
        TSVEC(INT(L)) = 0.0
        OpenAD_Symbol_46 = (INT(OpenAD_Symbol_46) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_46)
      OpenAD_Symbol_47 = 0_w2f__i8
      DO L = 1, 3, 1
        TSVEC(INT(L)) = __value__(TNOW(L))
        TSVEC(L + 3) = __value__(SNOW(L))
        OpenAD_Symbol_47 = (INT(OpenAD_Symbol_47) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_47)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_42)
      OpenAD_Symbol_43 = 1
      DO WHILE(INT(OpenAD_Symbol_43) .LE. INT(OpenAD_Symbol_42))
        OpenAD_Symbol_43 = INT(OpenAD_Symbol_43) + 1
      END DO
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_44)
      OpenAD_Symbol_45 = 1
      DO WHILE(INT(OpenAD_Symbol_45) .LE. INT(OpenAD_Symbol_44))
        OpenAD_Symbol_45 = INT(OpenAD_Symbol_45) + 1
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE

      SUBROUTINE box_ini_fields()
      use w2f__types
      use oad_intrinsics
      use all_globals_mod
      use oad_intrinsics
      use all_globals_mod
      use oad_intrinsics
      use all_globals_mod
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
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
      INTEGER(w2f__i8) OpenAD_Symbol_60
      INTEGER(w2f__i8) OpenAD_Symbol_61
      INTEGER(w2f__i8) OpenAD_Symbol_62
      INTEGER(w2f__i8) OpenAD_Symbol_63
      INTEGER(w2f__i8) OpenAD_Symbol_64
      INTEGER(w2f__i8) OpenAD_Symbol_65
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
      INTEGER(w2f__i4) OpenAD_Symbol_178
      INTEGER(w2f__i4) OpenAD_Symbol_179
      INTEGER(w2f__i4) OpenAD_Symbol_180
      INTEGER(w2f__i4) OpenAD_Symbol_181
      INTEGER(w2f__i4) OpenAD_Symbol_182
      INTEGER(w2f__i4) OpenAD_Symbol_183
      INTEGER(w2f__i4) OpenAD_Symbol_66
      TYPE (OpenADTy_active) OpenAD_prp_12
      TYPE (OpenADTy_active) OpenAD_prp_13
C
C     **** Statements ****
C
C     $OpenAD$ BEGIN REPLACEMENT 1
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
      __value__(T(1)) = 2.0D+01
      __value__(T(2)) = 1.0D00
      __value__(T(3)) = 1.0D00
      __value__(S(1)) = 3.55D+01
      __value__(S(2)) = 3.45D+01
      __value__(S(3)) = 3.45D+01
      DO L = 1, 6, 1
        TSVEC(INT(L)) = 0.0
      END DO
      DO L = 1, 3, 1
        __value__(T(INT(L))) = (__value__(T(L)) + __value__(XX(L)))
        __value__(S(INT(L))) = (__value__(S(L)) + __value__(XX(L + 3)))
      END DO
      DO L = 1, 3, 1
        __value__(TNEW(INT(L))) = __value__(T(L))
        __value__(SNEW(INT(L))) = __value__(S(L))
        __value__(TOLD(INT(L))) = __value__(T(L))
        __value__(SOLD(INT(L))) = __value__(S(L))
        __value__(TNOW(INT(L))) = __value__(T(L))
        __value__(SNOW(INT(L))) = __value__(S(L))
      END DO
      __value__(UVEL) = UBAR
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
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
      __value__(T(1)) = 2.0D+01
      __value__(T(2)) = 1.0D00
      __value__(T(3)) = 1.0D00
      __value__(S(1)) = 3.55D+01
      __value__(S(2)) = 3.45D+01
      __value__(S(3)) = 3.45D+01
      OpenAD_Symbol_54 = 0_w2f__i8
      DO L = 1, 6, 1
        TSVEC(INT(L)) = 0.0
        OpenAD_Symbol_54 = (INT(OpenAD_Symbol_54) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_54)
      OpenAD_Symbol_55 = 0_w2f__i8
      DO L = 1, 3, 1
        __value__(T(INT(L))) = (__value__(T(L)) + __value__(XX(L)))
        __value__(S(INT(L))) = (__value__(S(L)) + __value__(XX(L + 3)))
        OpenAD_Symbol_66 = (L + 3)
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_66)
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(L)
        OpenAD_Symbol_55 = (INT(OpenAD_Symbol_55) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_55)
      OpenAD_Symbol_56 = 0_w2f__i8
      DO L = 1, 3, 1
        __value__(TNEW(INT(L))) = __value__(T(L))
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(L)
        __value__(SNEW(INT(L))) = __value__(S(L))
        __value__(TOLD(INT(L))) = __value__(T(L))
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(L)
        __value__(SOLD(INT(L))) = __value__(S(L))
        __value__(TNOW(INT(L))) = __value__(T(L))
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(L)
        __value__(SNOW(INT(L))) = __value__(S(L))
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(L)
        OpenAD_Symbol_56 = (INT(OpenAD_Symbol_56) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_56)
      __value__(UVEL) = UBAR
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(UVEL))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_48)
      OpenAD_Symbol_49 = 1
      DO WHILE(INT(OpenAD_Symbol_49) .LE. INT(OpenAD_Symbol_48))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_180)
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(SNOW(OpenAD_Symbol_180)), __deriv__(S(
     > OpenAD_Symbol_180)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(SNOW(OpenAD_Symbol_180)))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_181)
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(TNOW(OpenAD_Symbol_181)), __deriv__(T(
     > OpenAD_Symbol_181)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(TNOW(OpenAD_Symbol_181)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(SOLD(OpenAD_Symbol_181)), __deriv__(S(
     > OpenAD_Symbol_181)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(SOLD(OpenAD_Symbol_181)))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_182)
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(TOLD(OpenAD_Symbol_182)), __deriv__(T(
     > OpenAD_Symbol_182)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(TOLD(OpenAD_Symbol_182)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(SNEW(OpenAD_Symbol_182)), __deriv__(S(
     > OpenAD_Symbol_182)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(SNEW(OpenAD_Symbol_182)))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_183)
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(TNEW(OpenAD_Symbol_183)), __deriv__(T(
     > OpenAD_Symbol_183)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(TNEW(OpenAD_Symbol_183)))
        OpenAD_Symbol_49 = INT(OpenAD_Symbol_49) + 1
      END DO
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_50)
      OpenAD_Symbol_51 = 1
      DO WHILE(INT(OpenAD_Symbol_51) .LE. INT(OpenAD_Symbol_50))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_178)
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_179)
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(S(OpenAD_Symbol_178)), __deriv__(XX(
     > OpenAD_Symbol_179)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(S(OpenAD_Symbol_178)), __deriv__(
     > OpenAD_prp_13))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(S(OpenAD_Symbol_178)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(T(OpenAD_Symbol_178)), __deriv__(XX(
     > OpenAD_Symbol_178)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(T(OpenAD_Symbol_178)), __deriv__(
     > OpenAD_prp_12))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(T(OpenAD_Symbol_178)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_13), __deriv__(S(
     > OpenAD_Symbol_178)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_13))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_12), __deriv__(T(
     > OpenAD_Symbol_178)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_12))
        OpenAD_Symbol_51 = INT(OpenAD_Symbol_51) + 1
      END DO
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_52)
      OpenAD_Symbol_53 = 1
      DO WHILE(INT(OpenAD_Symbol_53) .LE. INT(OpenAD_Symbol_52))
        OpenAD_Symbol_53 = INT(OpenAD_Symbol_53) + 1
      END DO
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(S(3)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(S(2)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(S(1)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(T(3)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(T(2)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(T(1)))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 4
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(HUNDRED)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(SV)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(UBAR)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(YEAR)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(AREA)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(FW)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(NULLFORCE)
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(S))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(T))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(XX))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 5
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 6
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(XX))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(T))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(S))
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(NULLFORCE)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(FW)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(AREA)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(YEAR)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(UBAR)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(SV)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(HUNDRED)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 7
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 8
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(METRIC)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(METRIC1)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(METRIC2)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(THC_S)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(THC_T)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(THC_TOT)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(UBAR)
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(UVEL))
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(FW)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(NULLFORCE)
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(S))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(SNEW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(SNOW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(SOLD))
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(SSTAR)
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(T))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(TNEW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(TNOW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(TOLD))
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(TSTAR)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(TSVEC)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(HUNDRED)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(SV)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(UBAR)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(YEAR)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(AREA)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(FW)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(NULLFORCE)
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(S))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(T))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(XX))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 9
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(XX))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(T))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(S))
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(NULLFORCE)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(FW)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(AREA)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(YEAR)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(UBAR)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(SV)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(HUNDRED)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(TSVEC)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(TSTAR)
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(TOLD))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(TNOW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(TNEW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(T))
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(SSTAR)
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(SOLD))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(SNOW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(SNEW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(S))
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(NULLFORCE)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(FW)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(UVEL))
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(UBAR)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(THC_TOT)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(THC_T)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(THC_S)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(METRIC2)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(METRIC1)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(METRIC)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 10
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
      __value__(T(1)) = 2.0D+01
      __value__(T(2)) = 1.0D00
      __value__(T(3)) = 1.0D00
      __value__(S(1)) = 3.55D+01
      __value__(S(2)) = 3.45D+01
      __value__(S(3)) = 3.45D+01
      OpenAD_Symbol_63 = 0_w2f__i8
      DO L = 1, 6, 1
        TSVEC(INT(L)) = 0.0
        OpenAD_Symbol_63 = (INT(OpenAD_Symbol_63) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_63)
      OpenAD_Symbol_64 = 0_w2f__i8
      DO L = 1, 3, 1
        __value__(T(INT(L))) = (__value__(T(L)) + __value__(XX(L)))
        __value__(S(INT(L))) = (__value__(S(L)) + __value__(XX(L + 3)))
        OpenAD_Symbol_66 = (L + 3)
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_66)
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(L)
        OpenAD_Symbol_64 = (INT(OpenAD_Symbol_64) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_64)
      OpenAD_Symbol_65 = 0_w2f__i8
      DO L = 1, 3, 1
        __value__(TNEW(INT(L))) = __value__(T(L))
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(L)
        __value__(SNEW(INT(L))) = __value__(S(L))
        __value__(TOLD(INT(L))) = __value__(T(L))
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(L)
        __value__(SOLD(INT(L))) = __value__(S(L))
        __value__(TNOW(INT(L))) = __value__(T(L))
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(L)
        __value__(SNOW(INT(L))) = __value__(S(L))
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(L)
        OpenAD_Symbol_65 = (INT(OpenAD_Symbol_65) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_65)
      __value__(UVEL) = UBAR
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(UVEL))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_57)
      OpenAD_Symbol_58 = 1
      DO WHILE(INT(OpenAD_Symbol_58) .LE. INT(OpenAD_Symbol_57))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_180)
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(SNOW(OpenAD_Symbol_180)), __deriv__(S(
     > OpenAD_Symbol_180)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(SNOW(OpenAD_Symbol_180)))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_181)
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(TNOW(OpenAD_Symbol_181)), __deriv__(T(
     > OpenAD_Symbol_181)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(TNOW(OpenAD_Symbol_181)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(SOLD(OpenAD_Symbol_181)), __deriv__(S(
     > OpenAD_Symbol_181)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(SOLD(OpenAD_Symbol_181)))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_182)
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(TOLD(OpenAD_Symbol_182)), __deriv__(T(
     > OpenAD_Symbol_182)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(TOLD(OpenAD_Symbol_182)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(SNEW(OpenAD_Symbol_182)), __deriv__(S(
     > OpenAD_Symbol_182)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(SNEW(OpenAD_Symbol_182)))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_183)
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(TNEW(OpenAD_Symbol_183)), __deriv__(T(
     > OpenAD_Symbol_183)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(TNEW(OpenAD_Symbol_183)))
        OpenAD_Symbol_58 = INT(OpenAD_Symbol_58) + 1
      END DO
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_59)
      OpenAD_Symbol_60 = 1
      DO WHILE(INT(OpenAD_Symbol_60) .LE. INT(OpenAD_Symbol_59))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_178)
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_179)
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(S(OpenAD_Symbol_178)), __deriv__(XX(
     > OpenAD_Symbol_179)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(S(OpenAD_Symbol_178)), __deriv__(
     > OpenAD_prp_13))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(S(OpenAD_Symbol_178)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(T(OpenAD_Symbol_178)), __deriv__(XX(
     > OpenAD_Symbol_178)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(T(OpenAD_Symbol_178)), __deriv__(
     > OpenAD_prp_12))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(T(OpenAD_Symbol_178)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_13), __deriv__(S(
     > OpenAD_Symbol_178)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_13))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_12), __deriv__(T(
     > OpenAD_Symbol_178)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_12))
        OpenAD_Symbol_60 = INT(OpenAD_Symbol_60) + 1
      END DO
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_61)
      OpenAD_Symbol_62 = 1
      DO WHILE(INT(OpenAD_Symbol_62) .LE. INT(OpenAD_Symbol_61))
        OpenAD_Symbol_62 = INT(OpenAD_Symbol_62) + 1
      END DO
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(S(3)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(S(2)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(S(1)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(T(3)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(T(2)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(T(1)))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a_d(subst)
      CALL cp_arg_store_real_scalar_a_d(__deriv__(UVEL))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
      CALL cp_arg_store_real_vector_a_d(__deriv__(S))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
      CALL cp_arg_store_real_vector_a_d(__deriv__(SNEW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
      CALL cp_arg_store_real_vector_a_d(__deriv__(SNOW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
      CALL cp_arg_store_real_vector_a_d(__deriv__(SOLD))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
      CALL cp_arg_store_real_vector_a_d(__deriv__(T))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
      CALL cp_arg_store_real_vector_a_d(__deriv__(TNEW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
      CALL cp_arg_store_real_vector_a_d(__deriv__(TNOW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
      CALL cp_arg_store_real_vector_a_d(__deriv__(TOLD))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
      CALL cp_arg_restore_real_vector_a_d(__deriv__(TOLD))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
      CALL cp_arg_restore_real_vector_a_d(__deriv__(TNOW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
      CALL cp_arg_restore_real_vector_a_d(__deriv__(TNEW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
      CALL cp_arg_restore_real_vector_a_d(__deriv__(T))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
      CALL cp_arg_restore_real_vector_a_d(__deriv__(SOLD))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
      CALL cp_arg_restore_real_vector_a_d(__deriv__(SNOW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
      CALL cp_arg_restore_real_vector_a_d(__deriv__(SNEW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
      CALL cp_arg_restore_real_vector_a_d(__deriv__(S))
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a_d(subst)
      CALL cp_arg_restore_real_scalar_a_d(__deriv__(UVEL))
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE

      SUBROUTINE box_ini_params()
      use w2f__types
      use oad_intrinsics
      use all_globals_mod
      use oad_intrinsics
      use all_globals_mod
      use oad_intrinsics
      use all_globals_mod
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      INTEGER(w2f__i8) OpenAD_Symbol_100
      INTEGER(w2f__i8) OpenAD_Symbol_101
      INTEGER(w2f__i8) OpenAD_Symbol_102
      INTEGER(w2f__i8) OpenAD_Symbol_103
      INTEGER(w2f__i8) OpenAD_Symbol_104
      INTEGER(w2f__i8) OpenAD_Symbol_105
      INTEGER(w2f__i8) OpenAD_Symbol_106
      INTEGER(w2f__i8) OpenAD_Symbol_107
      INTEGER(w2f__i8) OpenAD_Symbol_108
      INTEGER(w2f__i8) OpenAD_Symbol_109
      INTEGER(w2f__i8) OpenAD_Symbol_110
      INTEGER(w2f__i8) OpenAD_Symbol_111
      INTEGER(w2f__i8) OpenAD_Symbol_112
      INTEGER(w2f__i8) OpenAD_Symbol_113
      INTEGER(w2f__i8) OpenAD_Symbol_114
      INTEGER(w2f__i8) OpenAD_Symbol_67
      INTEGER(w2f__i8) OpenAD_Symbol_68
      INTEGER(w2f__i8) OpenAD_Symbol_69
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
      INTEGER(w2f__i8) OpenAD_Symbol_80
      INTEGER(w2f__i8) OpenAD_Symbol_81
      INTEGER(w2f__i8) OpenAD_Symbol_82
      INTEGER(w2f__i8) OpenAD_Symbol_83
      INTEGER(w2f__i8) OpenAD_Symbol_84
      INTEGER(w2f__i8) OpenAD_Symbol_85
      INTEGER(w2f__i8) OpenAD_Symbol_86
      INTEGER(w2f__i8) OpenAD_Symbol_87
      INTEGER(w2f__i8) OpenAD_Symbol_88
      INTEGER(w2f__i8) OpenAD_Symbol_89
      INTEGER(w2f__i8) OpenAD_Symbol_90
      INTEGER(w2f__i8) OpenAD_Symbol_91
      INTEGER(w2f__i8) OpenAD_Symbol_92
      INTEGER(w2f__i8) OpenAD_Symbol_93
      INTEGER(w2f__i8) OpenAD_Symbol_94
      INTEGER(w2f__i8) OpenAD_Symbol_95
      INTEGER(w2f__i8) OpenAD_Symbol_96
      INTEGER(w2f__i8) OpenAD_Symbol_97
      INTEGER(w2f__i8) OpenAD_Symbol_98
      INTEGER(w2f__i8) OpenAD_Symbol_99
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
C     $OpenAD$ BEGIN REPLACEMENT 1
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
      GAMMA_S = (0.0D00 /((BHEIGHT(1) / 5.0D+03) * DAY *
     >  DAYS_PER_50M_MIXED_LAYER))
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
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
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
      GAMMA_S = (0.0D00 /((BHEIGHT(1) / 5.0D+03) * DAY *
     >  DAYS_PER_50M_MIXED_LAYER))
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
      OpenAD_Symbol_82 = 0_w2f__i8
      DO L = 1, 6, 1
        R(INT(L)) = ((R(L) * U0) / SV)
        OpenAD_Symbol_82 = (INT(OpenAD_Symbol_82) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_82)
      OpenAD_Symbol_83 = 0_w2f__i8
      DO L = 1, 6, 1
        IF(L .LE. 3) THEN
          PROJ_T(INT(L)) = 1.0D00
          PROJ_S(INT(L)) = 0.0D00
          OpenAD_Symbol_84 = 1_w2f__i8
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_84)
        ELSE
          PROJ_T(INT(L)) = 0.0D00
          PROJ_S(INT(L)) = 1.0D00
          OpenAD_Symbol_85 = 0_w2f__i8
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_85)
        ENDIF
        OpenAD_Symbol_83 = (INT(OpenAD_Symbol_83) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_83)
      OpenAD_Symbol_86 = 0_w2f__i8
      DO L = 1, 6, 1
        R_T(INT(L)) = (PROJ_T(L) * R(L))
        R_S(INT(L)) = (PROJ_S(L) * R(L))
        OpenAD_Symbol_86 = (INT(OpenAD_Symbol_86) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_86)
      R1(1) = 1.0D00
      R1(2) = (-1.0D00)
      R1(3) = 0.0D00
      R1(4) = 0.0D00
      R1(5) = 0.0D00
      R1(6) = 0.0D00
      OpenAD_Symbol_87 = 0_w2f__i8
      DO J = 1, 6, 1
        OpenAD_Symbol_88 = 0_w2f__i8
        DO I = 1, 6, 1
          X(INT(I), INT(J)) = (R(I) * R(J))
          OpenAD_Symbol_88 = (INT(OpenAD_Symbol_88) + INT(1_w2f__i8))
        END DO
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_88)
        OpenAD_Symbol_87 = (INT(OpenAD_Symbol_87) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_87)
      EPSILON_REGULARIZE = 1.00000000000000002092D-08
      OpenAD_Symbol_89 = 0_w2f__i8
      DO J = 1, 6, 1
        OpenAD_Symbol_90 = 0_w2f__i8
        DO I = 1, 6, 1
          X(INT(I), INT(J)) = (X(I, J) + EPSILON_REGULARIZE)
          OpenAD_Symbol_90 = (INT(OpenAD_Symbol_90) + INT(1_w2f__i8))
        END DO
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_90)
        OpenAD_Symbol_89 = (INT(OpenAD_Symbol_89) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_89)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_67)
      OpenAD_Symbol_68 = 1
      DO WHILE(INT(OpenAD_Symbol_68) .LE. INT(OpenAD_Symbol_67))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_69)
        OpenAD_Symbol_70 = 1
        DO WHILE(INT(OpenAD_Symbol_70) .LE. INT(OpenAD_Symbol_69))
          OpenAD_Symbol_70 = INT(OpenAD_Symbol_70) + 1
        END DO
        OpenAD_Symbol_68 = INT(OpenAD_Symbol_68) + 1
      END DO
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_71)
      OpenAD_Symbol_72 = 1
      DO WHILE(INT(OpenAD_Symbol_72) .LE. INT(OpenAD_Symbol_71))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_73)
        OpenAD_Symbol_74 = 1
        DO WHILE(INT(OpenAD_Symbol_74) .LE. INT(OpenAD_Symbol_73))
          OpenAD_Symbol_74 = INT(OpenAD_Symbol_74) + 1
        END DO
        OpenAD_Symbol_72 = INT(OpenAD_Symbol_72) + 1
      END DO
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_75)
      OpenAD_Symbol_76 = 1
      DO WHILE(INT(OpenAD_Symbol_76) .LE. INT(OpenAD_Symbol_75))
        OpenAD_Symbol_76 = INT(OpenAD_Symbol_76) + 1
      END DO
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_77)
      OpenAD_Symbol_78 = 1
      DO WHILE(INT(OpenAD_Symbol_78) .LE. INT(OpenAD_Symbol_77))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_79)
        IF(OpenAD_Symbol_79 .ne. 0) THEN
        ENDIF
        OpenAD_Symbol_78 = INT(OpenAD_Symbol_78) + 1
      END DO
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_80)
      OpenAD_Symbol_81 = 1
      DO WHILE(INT(OpenAD_Symbol_81) .LE. INT(OpenAD_Symbol_80))
        OpenAD_Symbol_81 = INT(OpenAD_Symbol_81) + 1
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 4
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(ALPHA)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(BETA)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(BWIDTH)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(DAY)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(DAYS_PER_50M_MIXED_LAYER)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(DELTA)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(DELTA_T)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(EPSILON_REGULARIZE)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(INTEGRATION_TIME)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(SV)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(THOUSAND)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(U0)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(YEAR)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(AREA)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(BHEIGHT)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(BLENGTH)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(PROJ_S)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(PROJ_T)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(R)
C     $OpenAD$ INLINE cp_arg_store_real_matrix(subst)
      CALL cp_arg_store_real_matrix(X)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 5
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 6
C     $OpenAD$ INLINE cp_arg_restore_real_matrix(subst)
      CALL cp_arg_restore_real_matrix(X)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(R)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(PROJ_T)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(PROJ_S)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(BLENGTH)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(BHEIGHT)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(AREA)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(YEAR)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(U0)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(THOUSAND)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(SV)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(INTEGRATION_TIME)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(EPSILON_REGULARIZE)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(DELTA_T)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(DELTA)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(DAYS_PER_50M_MIXED_LAYER)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(DAY)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(BWIDTH)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(BETA)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(ALPHA)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 7
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 8
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(ALPHA)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(BETA)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(BWIDTH)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(DAY)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(DAYS_PER_50M_MIXED_LAYER)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(DELTA)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(DELTA_T)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(EPSILON_IC)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(EPSILON_REGULARIZE)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(FDEPS)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(GAMMA_S)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(GAMMA_T)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(HUNDRED)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(INTEGRATION_TIME)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(NOISE_CORRELATION_TIME)
C     $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
      CALL cp_arg_store_integer_scalar(N_MAX)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(ROBERT_FILTER_COEFF)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(SV)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(THOUSAND)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(U0)
C     $OpenAD$ INLINE cp_arg_store_bool_scalar(subst)
      CALL cp_arg_store_bool_scalar(VERBMODE)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(YEAR)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(AREA)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(BHEIGHT)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(BLENGTH)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(PROJ_S)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(PROJ_T)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(R)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(R1)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(R_S)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(R_T)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(VOL)
C     $OpenAD$ INLINE cp_arg_store_real_matrix(subst)
      CALL cp_arg_store_real_matrix(X)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(Y)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(ALPHA)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(BETA)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(BWIDTH)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(DAY)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(DAYS_PER_50M_MIXED_LAYER)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(DELTA)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(DELTA_T)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(EPSILON_REGULARIZE)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(INTEGRATION_TIME)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(SV)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(THOUSAND)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(U0)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(YEAR)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(AREA)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(BHEIGHT)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(BLENGTH)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(PROJ_S)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(PROJ_T)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(R)
C     $OpenAD$ INLINE cp_arg_store_real_matrix(subst)
      CALL cp_arg_store_real_matrix(X)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 9
C     $OpenAD$ INLINE cp_arg_restore_real_matrix(subst)
      CALL cp_arg_restore_real_matrix(X)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(R)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(PROJ_T)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(PROJ_S)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(BLENGTH)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(BHEIGHT)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(AREA)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(YEAR)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(U0)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(THOUSAND)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(SV)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(INTEGRATION_TIME)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(EPSILON_REGULARIZE)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(DELTA_T)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(DELTA)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(DAYS_PER_50M_MIXED_LAYER)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(DAY)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(BWIDTH)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(BETA)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(ALPHA)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(Y)
C     $OpenAD$ INLINE cp_arg_restore_real_matrix(subst)
      CALL cp_arg_restore_real_matrix(X)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(VOL)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(R_T)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(R_S)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(R1)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(R)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(PROJ_T)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(PROJ_S)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(BLENGTH)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(BHEIGHT)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(AREA)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(YEAR)
C     $OpenAD$ INLINE cp_arg_restore_bool_scalar(subst)
      CALL cp_arg_restore_bool_scalar(VERBMODE)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(U0)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(THOUSAND)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(SV)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(ROBERT_FILTER_COEFF)
C     $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
      CALL cp_arg_restore_integer_scalar(N_MAX)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(NOISE_CORRELATION_TIME)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(INTEGRATION_TIME)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(HUNDRED)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(GAMMA_T)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(GAMMA_S)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(FDEPS)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(EPSILON_REGULARIZE)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(EPSILON_IC)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(DELTA_T)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(DELTA)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(DAYS_PER_50M_MIXED_LAYER)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(DAY)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(BWIDTH)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(BETA)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(ALPHA)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 10
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
      GAMMA_S = (0.0D00 /((BHEIGHT(1) / 5.0D+03) * DAY *
     >  DAYS_PER_50M_MIXED_LAYER))
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
      OpenAD_Symbol_106 = 0_w2f__i8
      DO L = 1, 6, 1
        R(INT(L)) = ((R(L) * U0) / SV)
        OpenAD_Symbol_106 = (INT(OpenAD_Symbol_106) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_106)
      OpenAD_Symbol_107 = 0_w2f__i8
      DO L = 1, 6, 1
        IF(L .LE. 3) THEN
          PROJ_T(INT(L)) = 1.0D00
          PROJ_S(INT(L)) = 0.0D00
          OpenAD_Symbol_108 = 1_w2f__i8
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_108)
        ELSE
          PROJ_T(INT(L)) = 0.0D00
          PROJ_S(INT(L)) = 1.0D00
          OpenAD_Symbol_109 = 0_w2f__i8
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_109)
        ENDIF
        OpenAD_Symbol_107 = (INT(OpenAD_Symbol_107) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_107)
      OpenAD_Symbol_110 = 0_w2f__i8
      DO L = 1, 6, 1
        R_T(INT(L)) = (PROJ_T(L) * R(L))
        R_S(INT(L)) = (PROJ_S(L) * R(L))
        OpenAD_Symbol_110 = (INT(OpenAD_Symbol_110) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_110)
      R1(1) = 1.0D00
      R1(2) = (-1.0D00)
      R1(3) = 0.0D00
      R1(4) = 0.0D00
      R1(5) = 0.0D00
      R1(6) = 0.0D00
      OpenAD_Symbol_111 = 0_w2f__i8
      DO J = 1, 6, 1
        OpenAD_Symbol_112 = 0_w2f__i8
        DO I = 1, 6, 1
          X(INT(I), INT(J)) = (R(I) * R(J))
          OpenAD_Symbol_112 = (INT(OpenAD_Symbol_112) + INT(1_w2f__i8))
        END DO
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_112)
        OpenAD_Symbol_111 = (INT(OpenAD_Symbol_111) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_111)
      EPSILON_REGULARIZE = 1.00000000000000002092D-08
      OpenAD_Symbol_113 = 0_w2f__i8
      DO J = 1, 6, 1
        OpenAD_Symbol_114 = 0_w2f__i8
        DO I = 1, 6, 1
          X(INT(I), INT(J)) = (X(I, J) + EPSILON_REGULARIZE)
          OpenAD_Symbol_114 = (INT(OpenAD_Symbol_114) + INT(1_w2f__i8))
        END DO
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_114)
        OpenAD_Symbol_113 = (INT(OpenAD_Symbol_113) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_113)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_91)
      OpenAD_Symbol_92 = 1
      DO WHILE(INT(OpenAD_Symbol_92) .LE. INT(OpenAD_Symbol_91))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_93)
        OpenAD_Symbol_94 = 1
        DO WHILE(INT(OpenAD_Symbol_94) .LE. INT(OpenAD_Symbol_93))
          OpenAD_Symbol_94 = INT(OpenAD_Symbol_94) + 1
        END DO
        OpenAD_Symbol_92 = INT(OpenAD_Symbol_92) + 1
      END DO
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_95)
      OpenAD_Symbol_96 = 1
      DO WHILE(INT(OpenAD_Symbol_96) .LE. INT(OpenAD_Symbol_95))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_97)
        OpenAD_Symbol_98 = 1
        DO WHILE(INT(OpenAD_Symbol_98) .LE. INT(OpenAD_Symbol_97))
          OpenAD_Symbol_98 = INT(OpenAD_Symbol_98) + 1
        END DO
        OpenAD_Symbol_96 = INT(OpenAD_Symbol_96) + 1
      END DO
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_99)
      OpenAD_Symbol_100 = 1
      DO WHILE(INT(OpenAD_Symbol_100) .LE. INT(OpenAD_Symbol_99))
        OpenAD_Symbol_100 = INT(OpenAD_Symbol_100) + 1
      END DO
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_101)
      OpenAD_Symbol_102 = 1
      DO WHILE(INT(OpenAD_Symbol_102) .LE. INT(OpenAD_Symbol_101))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_103)
        IF(OpenAD_Symbol_103 .ne. 0) THEN
        ENDIF
        OpenAD_Symbol_102 = INT(OpenAD_Symbol_102) + 1
      END DO
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_104)
      OpenAD_Symbol_105 = 1
      DO WHILE(INT(OpenAD_Symbol_105) .LE. INT(OpenAD_Symbol_104))
        OpenAD_Symbol_105 = INT(OpenAD_Symbol_105) + 1
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE

      SUBROUTINE box_model_body()
      use w2f__types
      use oad_intrinsics
      use all_globals_mod
      use oad_intrinsics
      use all_globals_mod
      use oad_intrinsics
      use all_globals_mod
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      INTEGER(w2f__i8) OpenAD_Symbol_115
      INTEGER(w2f__i8) OpenAD_Symbol_116
      INTEGER(w2f__i8) OpenAD_Symbol_117
      INTEGER(w2f__i8) OpenAD_Symbol_118
      INTEGER(w2f__i8) OpenAD_Symbol_119
      INTEGER(w2f__i8) OpenAD_Symbol_120
      INTEGER(w2f__i8) OpenAD_Symbol_121
      INTEGER(w2f__i8) OpenAD_Symbol_122
      INTEGER(w2f__i8) OpenAD_Symbol_123
      INTEGER(w2f__i8) OpenAD_Symbol_124
      INTEGER(w2f__i8) OpenAD_Symbol_125
      INTEGER(w2f__i8) OpenAD_Symbol_126
      INTEGER(w2f__i8) OpenAD_Symbol_127
      INTEGER(w2f__i8) OpenAD_Symbol_128
      INTEGER(w2f__i8) OpenAD_Symbol_129
      INTEGER(w2f__i8) OpenAD_Symbol_130
      INTEGER(w2f__i8) OpenAD_Symbol_131
      INTEGER(w2f__i8) OpenAD_Symbol_132
      INTEGER(w2f__i8) OpenAD_Symbol_133
      INTEGER(w2f__i8) OpenAD_Symbol_134
      INTEGER(w2f__i8) OpenAD_Symbol_135
      INTEGER(w2f__i8) OpenAD_Symbol_136
      INTEGER(w2f__i8) OpenAD_Symbol_137
      INTEGER(w2f__i8) OpenAD_Symbol_138
      INTEGER(w2f__i8) OpenAD_Symbol_139
      INTEGER(w2f__i8) OpenAD_Symbol_140
      INTEGER(w2f__i8) OpenAD_Symbol_141
      INTEGER(w2f__i8) OpenAD_Symbol_142
      INTEGER(w2f__i8) OpenAD_Symbol_143
      INTEGER(w2f__i8) OpenAD_Symbol_144
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
C     $OpenAD$ BEGIN REPLACEMENT 1
C$OPENAD XXX Template ad_template.f
      CALL box_ini_fields()
      MAXLEV2 = ((N_MAX / 73) + 1)
      IF(N_MAX .GT. 3650) THEN
        WRITE(*, *) 'NEED TO SET nlev1*nlev2 >= n_max '
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
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      CALL box_ini_fields()
      MAXLEV2 = ((N_MAX / 73) + 1)
      IF(N_MAX .GT. 3650) THEN
        WRITE(*, *) 'NEED TO SET nlev1*nlev2 >= n_max '
        OpenAD_Symbol_128 = 1_w2f__i8
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_128)
      ELSE
        OpenAD_Symbol_122 = 0_w2f__i8
        DO ILEV2 = 1, 50, 1
          IF(ILEV2 .LE. MAXLEV2) THEN
            OpenAD_Symbol_123 = 0_w2f__i8
            DO ILEV1 = 1, 73, 1
              ILOOP = (ILEV1 + ILEV2 * 73 +(-73))
              IF(ILOOP .LE. N_MAX) THEN
                CALL box_forward(ILEV1)
                OpenAD_Symbol_124 = 1_w2f__i8
C               $OpenAD$ INLINE push_i_s0(subst)
                CALL push_i_s0(OpenAD_Symbol_124)
              ELSE
                OpenAD_Symbol_125 = 0_w2f__i8
C               $OpenAD$ INLINE push_i_s0(subst)
                CALL push_i_s0(OpenAD_Symbol_125)
              ENDIF
              OpenAD_Symbol_123 = (INT(OpenAD_Symbol_123) + INT(
     > 1_w2f__i8))
            END DO
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_123)
            OpenAD_Symbol_127 = 1_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_127)
          ELSE
            OpenAD_Symbol_126 = 0_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_126)
          ENDIF
          OpenAD_Symbol_122 = (INT(OpenAD_Symbol_122) + INT(1_w2f__i8))
        END DO
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_122)
        CALL box_final_state()
        OpenAD_Symbol_129 = 0_w2f__i8
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_129)
      ENDIF
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_115)
      IF(OpenAD_Symbol_115 .ne. 0) THEN
      ELSE
        CALL box_final_state()
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_116)
        OpenAD_Symbol_117 = 1
        DO WHILE(INT(OpenAD_Symbol_117) .LE. INT(OpenAD_Symbol_116))
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_118)
          IF(OpenAD_Symbol_118 .ne. 0) THEN
C           $OpenAD$ INLINE pop_i_s0(subst)
            CALL pop_i_s0(OpenAD_Symbol_119)
            OpenAD_Symbol_120 = 1
            DO WHILE(INT(OpenAD_Symbol_120) .LE. INT(OpenAD_Symbol_119)
     > )
C             $OpenAD$ INLINE pop_i_s0(subst)
              CALL pop_i_s0(OpenAD_Symbol_121)
              IF(OpenAD_Symbol_121 .ne. 0) THEN
                CALL box_forward(ILEV1)
              ENDIF
              OpenAD_Symbol_120 = INT(OpenAD_Symbol_120) + 1
            END DO
          ENDIF
          OpenAD_Symbol_117 = INT(OpenAD_Symbol_117) + 1
        END DO
      ENDIF
      CALL box_ini_fields()
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 4
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(ALPHA)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(BETA)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(DELTA)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(DELTA_T)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(GAMMA_S)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(GAMMA_T)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(HUNDRED)
C     $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
      CALL cp_arg_store_integer_scalar(N_MAX)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(ROBERT_FILTER_COEFF)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(SV)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(U0)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(UBAR)
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(UVEL))
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(YEAR)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(AREA)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(FW)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(NULLFORCE)
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(RHO))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(S))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(SNEW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(SNOW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(SOLD))
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(SSTAR)
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(T))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(TNEW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(TNOW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(TOLD))
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(TSTAR)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(VOL)
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(XX))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 5
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 6
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(XX))
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(VOL)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(TSTAR)
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(TOLD))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(TNOW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(TNEW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(T))
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(SSTAR)
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(SOLD))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(SNOW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(SNEW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(S))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(RHO))
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(NULLFORCE)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(FW)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(AREA)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(YEAR)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(UVEL))
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(UBAR)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(U0)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(SV)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(ROBERT_FILTER_COEFF)
C     $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
      CALL cp_arg_restore_integer_scalar(N_MAX)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(HUNDRED)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(GAMMA_T)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(GAMMA_S)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(DELTA_T)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(DELTA)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(BETA)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(ALPHA)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 7
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 8
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(METRIC)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(METRIC1)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(METRIC2)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(THC_S)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(THC_T)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(THC_TOT)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(UBAR)
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(UVEL))
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(FW)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(NULLFORCE)
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(RHO))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(S))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(SNEW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(SNOW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(SOLD))
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(SSTAR)
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(T))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(TNEW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(TNOW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(TOLD))
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(TSTAR)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(TSVEC)
C     $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
      CALL cp_arg_store_integer_scalar(N_MAX)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 9
C     $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
      CALL cp_arg_restore_integer_scalar(N_MAX)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(TSVEC)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(TSTAR)
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(TOLD))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(TNOW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(TNEW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(T))
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(SSTAR)
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(SOLD))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(SNOW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(SNEW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(S))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(RHO))
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(NULLFORCE)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(FW)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(UVEL))
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(UBAR)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(THC_TOT)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(THC_T)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(THC_S)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(METRIC2)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(METRIC1)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(METRIC)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 10
C$OPENAD XXX Template ad_template.f
      CALL box_ini_fields()
      MAXLEV2 = ((N_MAX / 73) + 1)
      IF(N_MAX .GT. 3650) THEN
        WRITE(*, *) 'NEED TO SET nlev1*nlev2 >= n_max '
        OpenAD_Symbol_143 = 1_w2f__i8
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_143)
      ELSE
        OpenAD_Symbol_137 = 0_w2f__i8
        DO ILEV2 = 1, 50, 1
          IF(ILEV2 .LE. MAXLEV2) THEN
            OpenAD_Symbol_138 = 0_w2f__i8
            DO ILEV1 = 1, 73, 1
              ILOOP = (ILEV1 + ILEV2 * 73 +(-73))
              IF(ILOOP .LE. N_MAX) THEN
                CALL box_forward(ILEV1)
                OpenAD_Symbol_139 = 1_w2f__i8
C               $OpenAD$ INLINE push_i_s0(subst)
                CALL push_i_s0(OpenAD_Symbol_139)
              ELSE
                OpenAD_Symbol_140 = 0_w2f__i8
C               $OpenAD$ INLINE push_i_s0(subst)
                CALL push_i_s0(OpenAD_Symbol_140)
              ENDIF
              OpenAD_Symbol_138 = (INT(OpenAD_Symbol_138) + INT(
     > 1_w2f__i8))
            END DO
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_138)
            OpenAD_Symbol_142 = 1_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_142)
          ELSE
            OpenAD_Symbol_141 = 0_w2f__i8
C           $OpenAD$ INLINE push_i_s0(subst)
            CALL push_i_s0(OpenAD_Symbol_141)
          ENDIF
          OpenAD_Symbol_137 = (INT(OpenAD_Symbol_137) + INT(1_w2f__i8))
        END DO
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_137)
        CALL box_final_state()
        OpenAD_Symbol_144 = 0_w2f__i8
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_144)
      ENDIF
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_130)
      IF(OpenAD_Symbol_130 .ne. 0) THEN
      ELSE
        CALL box_final_state()
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_131)
        OpenAD_Symbol_132 = 1
        DO WHILE(INT(OpenAD_Symbol_132) .LE. INT(OpenAD_Symbol_131))
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_133)
          IF(OpenAD_Symbol_133 .ne. 0) THEN
C           $OpenAD$ INLINE pop_i_s0(subst)
            CALL pop_i_s0(OpenAD_Symbol_134)
            OpenAD_Symbol_135 = 1
            DO WHILE(INT(OpenAD_Symbol_135) .LE. INT(OpenAD_Symbol_134)
     > )
C             $OpenAD$ INLINE pop_i_s0(subst)
              CALL pop_i_s0(OpenAD_Symbol_136)
              IF(OpenAD_Symbol_136 .ne. 0) THEN
                CALL box_forward(ILEV1)
              ENDIF
              OpenAD_Symbol_135 = INT(OpenAD_Symbol_135) + 1
            END DO
          ENDIF
          OpenAD_Symbol_132 = INT(OpenAD_Symbol_132) + 1
        END DO
      ENDIF
      CALL box_ini_fields()
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a_d(subst)
      CALL cp_arg_store_real_scalar_a_d(__deriv__(UVEL))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
      CALL cp_arg_store_real_vector_a_d(__deriv__(RHO))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
      CALL cp_arg_store_real_vector_a_d(__deriv__(S))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
      CALL cp_arg_store_real_vector_a_d(__deriv__(SNEW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
      CALL cp_arg_store_real_vector_a_d(__deriv__(SNOW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
      CALL cp_arg_store_real_vector_a_d(__deriv__(SOLD))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
      CALL cp_arg_store_real_vector_a_d(__deriv__(T))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
      CALL cp_arg_store_real_vector_a_d(__deriv__(TNEW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
      CALL cp_arg_store_real_vector_a_d(__deriv__(TNOW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
      CALL cp_arg_store_real_vector_a_d(__deriv__(TOLD))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
      CALL cp_arg_restore_real_vector_a_d(__deriv__(TOLD))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
      CALL cp_arg_restore_real_vector_a_d(__deriv__(TNOW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
      CALL cp_arg_restore_real_vector_a_d(__deriv__(TNEW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
      CALL cp_arg_restore_real_vector_a_d(__deriv__(T))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
      CALL cp_arg_restore_real_vector_a_d(__deriv__(SOLD))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
      CALL cp_arg_restore_real_vector_a_d(__deriv__(SNOW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
      CALL cp_arg_restore_real_vector_a_d(__deriv__(SNEW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
      CALL cp_arg_restore_real_vector_a_d(__deriv__(S))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
      CALL cp_arg_restore_real_vector_a_d(__deriv__(RHO))
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a_d(subst)
      CALL cp_arg_restore_real_scalar_a_d(__deriv__(UVEL))
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE

      SUBROUTINE box_density(TLOC, SLOC, RHOLOC)
      use w2f__types
      use oad_intrinsics
      use all_globals_mod
      use oad_intrinsics
      use all_globals_mod
      use oad_intrinsics
      use all_globals_mod
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      INTEGER(w2f__i8) OpenAD_Symbol_12
      INTEGER(w2f__i8) OpenAD_Symbol_13
      INTEGER(w2f__i8) OpenAD_Symbol_14
      INTEGER(w2f__i8) OpenAD_Symbol_15
      INTEGER(w2f__i8) OpenAD_Symbol_16
      INTEGER(w2f__i8) OpenAD_Symbol_17
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) TLOC(1 : 3)
      TYPE (OpenADTy_active) SLOC(1 : 3)
      TYPE (OpenADTy_active) RHOLOC(1 : 3)
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
      INTEGER(w2f__i4) OpenAD_Symbol_152
      REAL(w2f__8) OpenAD_Symbol_153
      REAL(w2f__8) OpenAD_Symbol_154
      REAL(w2f__8) OpenAD_acc_0
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
C
C     **** Statements ****
C
C     $OpenAD$ BEGIN REPLACEMENT 1
C$OPENAD XXX Template ad_template.f
      DO L = 1, 3, 1
        __value__(RHOLOC(INT(L))) = (__value__(SLOC(L)) * BETA -
     >  __value__(TLOC(L)) * ALPHA)
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      OpenAD_Symbol_14 = 0_w2f__i8
      DO L = 1, 3, 1
        OpenAD_lin_0 = BETA
        OpenAD_lin_1 = ALPHA
        __value__(RHOLOC(INT(L))) = (__value__(SLOC(L)) * BETA -
     >  __value__(TLOC(L)) * ALPHA)
        OpenAD_acc_0 = (OpenAD_lin_1 * INT((-1_w2f__i8)))
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_lin_0)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_0)
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(L)
        OpenAD_Symbol_14 = (INT(OpenAD_Symbol_14) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_14)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_12)
      OpenAD_Symbol_13 = 1
      DO WHILE(INT(OpenAD_Symbol_13) .LE. INT(OpenAD_Symbol_12))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_152)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_153)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_154)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_153, __deriv__(RHOLOC(
     > OpenAD_Symbol_152)), __deriv__(TLOC(OpenAD_Symbol_152)))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_154, __deriv__(RHOLOC(
     > OpenAD_Symbol_152)), __deriv__(SLOC(OpenAD_Symbol_152)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(RHOLOC(OpenAD_Symbol_152)))
        OpenAD_Symbol_13 = INT(OpenAD_Symbol_13) + 1
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 4
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(ALPHA)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(BETA)
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(TLOC))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(SLOC))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 5
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 6
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(SLOC))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(TLOC))
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(BETA)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(ALPHA)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 7
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 8
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(RHOLOC))
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(ALPHA)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(BETA)
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(TLOC))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(SLOC))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 9
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(SLOC))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(TLOC))
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(BETA)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(ALPHA)
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(RHOLOC))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 10
C$OPENAD XXX Template ad_template.f
      OpenAD_Symbol_17 = 0_w2f__i8
      DO L = 1, 3, 1
        OpenAD_lin_0 = BETA
        OpenAD_lin_1 = ALPHA
        __value__(RHOLOC(INT(L))) = (__value__(SLOC(L)) * BETA -
     >  __value__(TLOC(L)) * ALPHA)
        OpenAD_acc_0 = (OpenAD_lin_1 * INT((-1_w2f__i8)))
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_lin_0)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_0)
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(L)
        OpenAD_Symbol_17 = (INT(OpenAD_Symbol_17) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_17)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_15)
      OpenAD_Symbol_16 = 1
      DO WHILE(INT(OpenAD_Symbol_16) .LE. INT(OpenAD_Symbol_15))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_152)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_153)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_154)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_153, __deriv__(RHOLOC(
     > OpenAD_Symbol_152)), __deriv__(TLOC(OpenAD_Symbol_152)))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_154, __deriv__(RHOLOC(
     > OpenAD_Symbol_152)), __deriv__(SLOC(OpenAD_Symbol_152)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(RHOLOC(OpenAD_Symbol_152)))
        OpenAD_Symbol_16 = INT(OpenAD_Symbol_16) + 1
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
      CALL cp_arg_store_real_vector_a_d(__deriv__(RHOLOC))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
      CALL cp_arg_restore_real_vector_a_d(__deriv__(RHOLOC))
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE

      SUBROUTINE box_transport(RHOLOC, UVELLOC)
      use w2f__types
      use oad_intrinsics
      use all_globals_mod
      use oad_intrinsics
      use all_globals_mod
      use oad_intrinsics
      use all_globals_mod
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) RHOLOC(1 : 3)
      TYPE (OpenADTy_active) UVELLOC
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) ISBYTE
      PARAMETER ( ISBYTE = 8)
      INTEGER(w2f__i4) NLEV1
      PARAMETER ( NLEV1 = 73)
      INTEGER(w2f__i4) NLEV2
      PARAMETER ( NLEV2 = 50)
      REAL(w2f__8) OpenAD_Symbol_155
      REAL(w2f__8) OpenAD_Symbol_156
      REAL(w2f__8) OpenAD_Symbol_157
      REAL(w2f__8) OpenAD_acc_1
      REAL(w2f__8) OpenAD_aux_0
      REAL(w2f__8) OpenAD_aux_1
      REAL(w2f__8) OpenAD_lin_2
      REAL(w2f__8) OpenAD_lin_3
      REAL(w2f__8) OpenAD_lin_4
      TYPE (OpenADTy_active) OpenAD_prp_0
C
C     **** Statements ****
C
C     $OpenAD$ BEGIN REPLACEMENT 1
C$OPENAD XXX Template ad_template.f
      __value__(UVELLOC) = (-(U0 *(__value__(RHOLOC(1)) * DELTA +
     >  __value__(RHOLOC(3)) *(1.0D00 - DELTA) - __value__(RHOLOC(2))))
     > )
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      OpenAD_aux_1 = (1.0D00 - DELTA)
      OpenAD_aux_0 = (__value__(RHOLOC(1)) * DELTA + __value__(RHOLOC(3
     > )) * OpenAD_aux_1 - __value__(RHOLOC(2)))
      OpenAD_lin_3 = DELTA
      OpenAD_lin_4 = OpenAD_aux_1
      OpenAD_lin_2 = U0
      __value__(UVELLOC) = (-(U0 * OpenAD_aux_0))
      OpenAD_acc_1 = (OpenAD_lin_2 * INT((-1_w2f__i8)))
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_lin_3)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_lin_4)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_acc_1)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_155)
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_156)
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_157)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_155, __deriv__(UVELLOC), __deriv__(
     > OpenAD_prp_0))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(UVELLOC))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_156, __deriv__(OpenAD_prp_0), __deriv__(
     > RHOLOC(3)))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_157, __deriv__(OpenAD_prp_0), __deriv__(
     > RHOLOC(1)))
C     $OpenAD$ INLINE DecDeriv(subst,subst)
      CALL DecDeriv(__deriv__(OpenAD_prp_0), __deriv__(RHOLOC(2)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 4
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(DELTA)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(U0)
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(RHOLOC))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 5
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 6
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(RHOLOC))
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(U0)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(DELTA)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 7
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 8
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(UVELLOC))
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(DELTA)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(U0)
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(RHOLOC))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 9
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(RHOLOC))
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(U0)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(DELTA)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(UVELLOC))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 10
C$OPENAD XXX Template ad_template.f
      OpenAD_aux_1 = (1.0D00 - DELTA)
      OpenAD_aux_0 = (__value__(RHOLOC(1)) * DELTA + __value__(RHOLOC(3
     > )) * OpenAD_aux_1 - __value__(RHOLOC(2)))
      OpenAD_lin_3 = DELTA
      OpenAD_lin_4 = OpenAD_aux_1
      OpenAD_lin_2 = U0
      __value__(UVELLOC) = (-(U0 * OpenAD_aux_0))
      OpenAD_acc_1 = (OpenAD_lin_2 * INT((-1_w2f__i8)))
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_lin_3)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_lin_4)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_acc_1)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_155)
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_156)
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_157)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_155, __deriv__(UVELLOC), __deriv__(
     > OpenAD_prp_0))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(UVELLOC))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_156, __deriv__(OpenAD_prp_0), __deriv__(
     > RHOLOC(3)))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_157, __deriv__(OpenAD_prp_0), __deriv__(
     > RHOLOC(1)))
C     $OpenAD$ INLINE DecDeriv(subst,subst)
      CALL DecDeriv(__deriv__(OpenAD_prp_0), __deriv__(RHOLOC(2)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a_d(subst)
      CALL cp_arg_store_real_scalar_a_d(__deriv__(UVELLOC))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a_d(subst)
      CALL cp_arg_restore_real_scalar_a_d(__deriv__(UVELLOC))
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE

      SUBROUTINE box_robert_filter(FLDNOW, FLDOLD, FLDNEW)
      use w2f__types
      use oad_intrinsics
      use all_globals_mod
      use oad_intrinsics
      use all_globals_mod
      use oad_intrinsics
      use all_globals_mod
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      INTEGER(w2f__i8) OpenAD_Symbol_24
      INTEGER(w2f__i8) OpenAD_Symbol_25
      INTEGER(w2f__i8) OpenAD_Symbol_26
      INTEGER(w2f__i8) OpenAD_Symbol_27
      INTEGER(w2f__i8) OpenAD_Symbol_28
      INTEGER(w2f__i8) OpenAD_Symbol_29
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) FLDNOW(1 : 3)
      TYPE (OpenADTy_active) FLDOLD(1 : 3)
      TYPE (OpenADTy_active) FLDNEW(1 : 3)
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
      INTEGER(w2f__i4) OpenAD_Symbol_174
      REAL(w2f__8) OpenAD_Symbol_175
      REAL(w2f__8) OpenAD_aux_22
      REAL(w2f__8) OpenAD_lin_31
      TYPE (OpenADTy_active) OpenAD_prp_7
      TYPE (OpenADTy_active) OpenAD_prp_8
      TYPE (OpenADTy_active) OpenAD_prp_9
C
C     **** Statements ****
C
C     $OpenAD$ BEGIN REPLACEMENT 1
C$OPENAD XXX Template ad_template.f
      DO L = 1, 3, 1
        __value__(FLDNOW(INT(L))) = (__value__(FLDNOW(L)) +
     >  ROBERT_FILTER_COEFF *(__value__(FLDOLD(L)) + __value__(FLDNEW(L
     > )) - __value__(FLDNOW(L)) * 2.0D00))
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      OpenAD_Symbol_26 = 0_w2f__i8
      DO L = 1, 3, 1
        OpenAD_aux_22 = (__value__(FLDOLD(L)) + __value__(FLDNEW(L)) -
     >  __value__(FLDNOW(L)) * 2.0D00)
        OpenAD_lin_31 = ROBERT_FILTER_COEFF
        __value__(FLDNOW(INT(L))) = (__value__(FLDNOW(L)) +
     >  ROBERT_FILTER_COEFF * OpenAD_aux_22)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_lin_31)
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(L)
        OpenAD_Symbol_26 = (INT(OpenAD_Symbol_26) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_26)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_24)
      OpenAD_Symbol_25 = 1
      DO WHILE(INT(OpenAD_Symbol_25) .LE. INT(OpenAD_Symbol_24))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_174)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_175)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_175, __deriv__(FLDNOW(
     > OpenAD_Symbol_174)), __deriv__(OpenAD_prp_9))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(FLDNOW(OpenAD_Symbol_174)), __deriv__(
     > OpenAD_prp_7))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(FLDNOW(OpenAD_Symbol_174)))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(-2.0D00, __deriv__(OpenAD_prp_9), __deriv__(
     > OpenAD_prp_8))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_9), __deriv__(FLDNEW(
     > OpenAD_Symbol_174)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_9), __deriv__(FLDOLD(
     > OpenAD_Symbol_174)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_9))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_8), __deriv__(FLDNOW(
     > OpenAD_Symbol_174)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_8))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_7), __deriv__(FLDNOW(
     > OpenAD_Symbol_174)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_7))
        OpenAD_Symbol_25 = INT(OpenAD_Symbol_25) + 1
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 4
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(ROBERT_FILTER_COEFF)
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(FLDNOW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(FLDOLD))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(FLDNEW))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 5
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 6
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(FLDNEW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(FLDOLD))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(FLDNOW))
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(ROBERT_FILTER_COEFF)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 7
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 8
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(FLDNOW))
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(ROBERT_FILTER_COEFF)
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(FLDNOW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(FLDOLD))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(FLDNEW))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 9
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(FLDNEW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(FLDOLD))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(FLDNOW))
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(ROBERT_FILTER_COEFF)
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(FLDNOW))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 10
C$OPENAD XXX Template ad_template.f
      OpenAD_Symbol_29 = 0_w2f__i8
      DO L = 1, 3, 1
        OpenAD_aux_22 = (__value__(FLDOLD(L)) + __value__(FLDNEW(L)) -
     >  __value__(FLDNOW(L)) * 2.0D00)
        OpenAD_lin_31 = ROBERT_FILTER_COEFF
        __value__(FLDNOW(INT(L))) = (__value__(FLDNOW(L)) +
     >  ROBERT_FILTER_COEFF * OpenAD_aux_22)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_lin_31)
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(L)
        OpenAD_Symbol_29 = (INT(OpenAD_Symbol_29) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_29)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_27)
      OpenAD_Symbol_28 = 1
      DO WHILE(INT(OpenAD_Symbol_28) .LE. INT(OpenAD_Symbol_27))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_174)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_175)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_175, __deriv__(FLDNOW(
     > OpenAD_Symbol_174)), __deriv__(OpenAD_prp_9))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(FLDNOW(OpenAD_Symbol_174)), __deriv__(
     > OpenAD_prp_7))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(FLDNOW(OpenAD_Symbol_174)))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(-2.0D00, __deriv__(OpenAD_prp_9), __deriv__(
     > OpenAD_prp_8))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_9), __deriv__(FLDNEW(
     > OpenAD_Symbol_174)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_9), __deriv__(FLDOLD(
     > OpenAD_Symbol_174)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_9))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_8), __deriv__(FLDNOW(
     > OpenAD_Symbol_174)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_8))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_7), __deriv__(FLDNOW(
     > OpenAD_Symbol_174)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_7))
        OpenAD_Symbol_28 = INT(OpenAD_Symbol_28) + 1
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
      CALL cp_arg_store_real_vector_a_d(__deriv__(FLDNOW))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
      CALL cp_arg_restore_real_vector_a_d(__deriv__(FLDNOW))
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE

      SUBROUTINE box_cycle_fields()
      use w2f__types
      use oad_intrinsics
      use all_globals_mod
      use oad_intrinsics
      use all_globals_mod
      use oad_intrinsics
      use all_globals_mod
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      INTEGER(w2f__i8) OpenAD_Symbol_30
      INTEGER(w2f__i8) OpenAD_Symbol_31
      INTEGER(w2f__i8) OpenAD_Symbol_32
      INTEGER(w2f__i8) OpenAD_Symbol_33
      INTEGER(w2f__i8) OpenAD_Symbol_34
      INTEGER(w2f__i8) OpenAD_Symbol_35
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
      INTEGER(w2f__i4) OpenAD_Symbol_176
      INTEGER(w2f__i4) OpenAD_Symbol_177
      TYPE (OpenADTy_active) OpenAD_prp_10
      TYPE (OpenADTy_active) OpenAD_prp_11
C
C     **** Statements ****
C
C     $OpenAD$ BEGIN REPLACEMENT 1
C$OPENAD XXX Template ad_template.f
      DO L = 1, 3, 1
        __value__(TOLD(INT(L))) = __value__(TNOW(L))
        __value__(TNOW(INT(L))) = __value__(TNEW(L))
        __value__(SOLD(INT(L))) = __value__(SNOW(L))
        __value__(SNOW(INT(L))) = __value__(SNEW(L))
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      OpenAD_Symbol_32 = 0_w2f__i8
      DO L = 1, 3, 1
        __value__(TOLD(INT(L))) = __value__(TNOW(L))
        __value__(TNOW(INT(L))) = __value__(TNEW(L))
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(L)
        __value__(SOLD(INT(L))) = __value__(SNOW(L))
        __value__(SNOW(INT(L))) = __value__(SNEW(L))
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(L)
        OpenAD_Symbol_32 = (INT(OpenAD_Symbol_32) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_32)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_30)
      OpenAD_Symbol_31 = 1
      DO WHILE(INT(OpenAD_Symbol_31) .LE. INT(OpenAD_Symbol_30))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_176)
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(SNOW(OpenAD_Symbol_176)), __deriv__(
     > SNEW(OpenAD_Symbol_176)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(SNOW(OpenAD_Symbol_176)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(SOLD(OpenAD_Symbol_176)), __deriv__(
     > OpenAD_prp_11))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(SOLD(OpenAD_Symbol_176)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_11), __deriv__(SNOW(
     > OpenAD_Symbol_176)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_11))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_177)
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(TNOW(OpenAD_Symbol_177)), __deriv__(
     > TNEW(OpenAD_Symbol_177)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(TNOW(OpenAD_Symbol_177)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(TOLD(OpenAD_Symbol_177)), __deriv__(
     > OpenAD_prp_10))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(TOLD(OpenAD_Symbol_177)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_10), __deriv__(TNOW(
     > OpenAD_Symbol_177)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_10))
        OpenAD_Symbol_31 = INT(OpenAD_Symbol_31) + 1
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 4
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(SNEW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(SNOW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(TNEW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(TNOW))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 5
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 6
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(TNOW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(TNEW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(SNOW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(SNEW))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 7
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 8
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(SNOW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(SOLD))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(TNOW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(TOLD))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(SNEW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(SNOW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(TNEW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(TNOW))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 9
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(TNOW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(TNEW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(SNOW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(SNEW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(TOLD))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(TNOW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(SOLD))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(SNOW))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 10
C$OPENAD XXX Template ad_template.f
      OpenAD_Symbol_35 = 0_w2f__i8
      DO L = 1, 3, 1
        __value__(TOLD(INT(L))) = __value__(TNOW(L))
        __value__(TNOW(INT(L))) = __value__(TNEW(L))
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(L)
        __value__(SOLD(INT(L))) = __value__(SNOW(L))
        __value__(SNOW(INT(L))) = __value__(SNEW(L))
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(L)
        OpenAD_Symbol_35 = (INT(OpenAD_Symbol_35) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_35)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_33)
      OpenAD_Symbol_34 = 1
      DO WHILE(INT(OpenAD_Symbol_34) .LE. INT(OpenAD_Symbol_33))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_176)
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(SNOW(OpenAD_Symbol_176)), __deriv__(
     > SNEW(OpenAD_Symbol_176)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(SNOW(OpenAD_Symbol_176)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(SOLD(OpenAD_Symbol_176)), __deriv__(
     > OpenAD_prp_11))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(SOLD(OpenAD_Symbol_176)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_11), __deriv__(SNOW(
     > OpenAD_Symbol_176)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_11))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_177)
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(TNOW(OpenAD_Symbol_177)), __deriv__(
     > TNEW(OpenAD_Symbol_177)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(TNOW(OpenAD_Symbol_177)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(TOLD(OpenAD_Symbol_177)), __deriv__(
     > OpenAD_prp_10))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(TOLD(OpenAD_Symbol_177)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_10), __deriv__(TNOW(
     > OpenAD_Symbol_177)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_10))
        OpenAD_Symbol_34 = INT(OpenAD_Symbol_34) + 1
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
      CALL cp_arg_store_real_vector_a_d(__deriv__(SNOW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
      CALL cp_arg_store_real_vector_a_d(__deriv__(SOLD))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
      CALL cp_arg_store_real_vector_a_d(__deriv__(TNOW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
      CALL cp_arg_store_real_vector_a_d(__deriv__(TOLD))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
      CALL cp_arg_restore_real_vector_a_d(__deriv__(TOLD))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
      CALL cp_arg_restore_real_vector_a_d(__deriv__(TNOW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
      CALL cp_arg_restore_real_vector_a_d(__deriv__(SOLD))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
      CALL cp_arg_restore_real_vector_a_d(__deriv__(SNOW))
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE

      SUBROUTINE box_update(FLDNEW, FLDOLD, DFLDDT)
      use w2f__types
      use oad_intrinsics
      use all_globals_mod
      use oad_intrinsics
      use all_globals_mod
      use oad_intrinsics
      use all_globals_mod
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      INTEGER(w2f__i8) OpenAD_Symbol_145
      INTEGER(w2f__i8) OpenAD_Symbol_146
      INTEGER(w2f__i8) OpenAD_Symbol_147
      INTEGER(w2f__i8) OpenAD_Symbol_148
      INTEGER(w2f__i8) OpenAD_Symbol_149
      INTEGER(w2f__i8) OpenAD_Symbol_150
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) FLDNEW(1 : 3)
      TYPE (OpenADTy_active) FLDOLD(1 : 3)
      TYPE (OpenADTy_active) DFLDDT(1 : 3)
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
      INTEGER(w2f__i4) OpenAD_Symbol_184
      REAL(w2f__8) OpenAD_Symbol_185
      REAL(w2f__8) OpenAD_aux_23
      REAL(w2f__8) OpenAD_lin_32
C
C     **** Statements ****
C
C     $OpenAD$ BEGIN REPLACEMENT 1
C$OPENAD XXX Template ad_template.f
      DO L = 1, 3, 1
        __value__(FLDNEW(INT(L))) = (__value__(FLDOLD(L)) + __value__(
     > DFLDDT(L)) * DELTA_T * 2.0D00)
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      OpenAD_Symbol_147 = 0_w2f__i8
      DO L = 1, 3, 1
        OpenAD_aux_23 = (DELTA_T * 2.0D00)
        OpenAD_lin_32 = OpenAD_aux_23
        __value__(FLDNEW(INT(L))) = (__value__(FLDOLD(L)) + __value__(
     > DFLDDT(L)) * OpenAD_aux_23)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_lin_32)
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(L)
        OpenAD_Symbol_147 = (INT(OpenAD_Symbol_147) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_147)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_145)
      OpenAD_Symbol_146 = 1
      DO WHILE(INT(OpenAD_Symbol_146) .LE. INT(OpenAD_Symbol_145))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_184)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_185)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_185, __deriv__(FLDNEW(
     > OpenAD_Symbol_184)), __deriv__(DFLDDT(OpenAD_Symbol_184)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(FLDNEW(OpenAD_Symbol_184)), __deriv__(
     > FLDOLD(OpenAD_Symbol_184)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(FLDNEW(OpenAD_Symbol_184)))
        OpenAD_Symbol_146 = INT(OpenAD_Symbol_146) + 1
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 4
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(DELTA_T)
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(FLDOLD))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(DFLDDT))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 5
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 6
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(DFLDDT))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(FLDOLD))
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(DELTA_T)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 7
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 8
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(FLDNEW))
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(DELTA_T)
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(FLDOLD))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(DFLDDT))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 9
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(DFLDDT))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(FLDOLD))
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(DELTA_T)
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(FLDNEW))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 10
C$OPENAD XXX Template ad_template.f
      OpenAD_Symbol_150 = 0_w2f__i8
      DO L = 1, 3, 1
        OpenAD_aux_23 = (DELTA_T * 2.0D00)
        OpenAD_lin_32 = OpenAD_aux_23
        __value__(FLDNEW(INT(L))) = (__value__(FLDOLD(L)) + __value__(
     > DFLDDT(L)) * OpenAD_aux_23)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_lin_32)
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(L)
        OpenAD_Symbol_150 = (INT(OpenAD_Symbol_150) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_150)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_148)
      OpenAD_Symbol_149 = 1
      DO WHILE(INT(OpenAD_Symbol_149) .LE. INT(OpenAD_Symbol_148))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_184)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_185)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_185, __deriv__(FLDNEW(
     > OpenAD_Symbol_184)), __deriv__(DFLDDT(OpenAD_Symbol_184)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(FLDNEW(OpenAD_Symbol_184)), __deriv__(
     > FLDOLD(OpenAD_Symbol_184)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(FLDNEW(OpenAD_Symbol_184)))
        OpenAD_Symbol_149 = INT(OpenAD_Symbol_149) + 1
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
      CALL cp_arg_store_real_vector_a_d(__deriv__(FLDNEW))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
      CALL cp_arg_restore_real_vector_a_d(__deriv__(FLDNEW))
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE

      SUBROUTINE box_timestep(GAMMALOC, FLDSTAR, EXTFORLOC, UVELLOC,
     >  FLDNOW, FLDOLD, FLDNEW)
      use w2f__types
      use oad_intrinsics
      use all_globals_mod
      use oad_intrinsics
      use all_globals_mod
      use oad_intrinsics
      use all_globals_mod
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
C
C     **** Parameters and Result ****
C
      REAL(w2f__8) GAMMALOC
      REAL(w2f__8) FLDSTAR(1 : 2)
      REAL(w2f__8) EXTFORLOC(1 : 2)
      TYPE (OpenADTy_active) UVELLOC
      TYPE (OpenADTy_active) FLDNOW(1 : 3)
      TYPE (OpenADTy_active) FLDOLD(1 : 3)
      TYPE (OpenADTy_active) FLDNEW(1 : 3)
C
C     **** Local Variables and Functions ****
C
      EXTERNAL box_update
      TYPE (OpenADTy_active) DFLDDT(1 : 3)
      INTEGER(w2f__i4) ISBYTE
      PARAMETER ( ISBYTE = 8)
      INTEGER(w2f__i4) NLEV1
      PARAMETER ( NLEV1 = 73)
      INTEGER(w2f__i4) NLEV2
      PARAMETER ( NLEV2 = 50)
      REAL(w2f__8) OpenAD_Symbol_158
      REAL(w2f__8) OpenAD_Symbol_159
      REAL(w2f__8) OpenAD_Symbol_160
      REAL(w2f__8) OpenAD_Symbol_161
      REAL(w2f__8) OpenAD_Symbol_162
      REAL(w2f__8) OpenAD_Symbol_163
      REAL(w2f__8) OpenAD_Symbol_164
      REAL(w2f__8) OpenAD_Symbol_165
      REAL(w2f__8) OpenAD_Symbol_166
      REAL(w2f__8) OpenAD_Symbol_167
      REAL(w2f__8) OpenAD_Symbol_168
      REAL(w2f__8) OpenAD_Symbol_169
      REAL(w2f__8) OpenAD_Symbol_170
      REAL(w2f__8) OpenAD_Symbol_171
      REAL(w2f__8) OpenAD_Symbol_172
      REAL(w2f__8) OpenAD_Symbol_173
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
      TYPE (OpenADTy_active) OpenAD_prp_1
      TYPE (OpenADTy_active) OpenAD_prp_2
      TYPE (OpenADTy_active) OpenAD_prp_3
      TYPE (OpenADTy_active) OpenAD_prp_4
      TYPE (OpenADTy_active) OpenAD_prp_5
      TYPE (OpenADTy_active) OpenAD_prp_6
C
C     **** Statements ****
C
C     $OpenAD$ BEGIN REPLACEMENT 1
C$OPENAD XXX Template ad_template.f
      IF(__value__(UVELLOC) .GE. 0.0D00) THEN
        __value__(DFLDDT(1)) = ((EXTFORLOC(1) + VOL(1) * GAMMALOC *(
     > FLDSTAR(1) - __value__(FLDNOW(1))) + __value__(UVELLOC) *(
     > __value__(FLDNOW(3)) - __value__(FLDNOW(1)))) / VOL(1))
        __value__(DFLDDT(2)) = ((EXTFORLOC(2) + VOL(2) * GAMMALOC *(
     > FLDSTAR(2) - __value__(FLDNOW(2))) + __value__(UVELLOC) *(
     > __value__(FLDNOW(1)) - __value__(FLDNOW(2)))) / VOL(2))
        __value__(DFLDDT(3)) = ((__value__(UVELLOC) *(__value__(FLDNOW(
     > 2)) - __value__(FLDNOW(3)))) / VOL(3))
      ELSE
        __value__(DFLDDT(1)) = ((EXTFORLOC(1) + VOL(1) * GAMMALOC *(
     > FLDSTAR(1) - __value__(FLDNOW(1))) - __value__(UVELLOC) *(
     > __value__(FLDNOW(2)) - __value__(FLDNOW(1)))) / VOL(1))
        __value__(DFLDDT(2)) = ((EXTFORLOC(2) + VOL(2) * GAMMALOC *(
     > FLDSTAR(2) - __value__(FLDNOW(2))) - __value__(UVELLOC) *(
     > __value__(FLDNOW(3)) - __value__(FLDNOW(2)))) / VOL(2))
        __value__(DFLDDT(3)) = (-((__value__(UVELLOC) *(__value__(
     > FLDNOW(1)) - __value__(FLDNOW(3)))) / VOL(3)))
      ENDIF
      CALL box_update(__deriv__(FLDNEW), __deriv__(FLDOLD), __deriv__(
     > DFLDDT))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      IF(__value__(UVELLOC) .GE. 0.0D00) THEN
        OpenAD_aux_4 = (FLDSTAR(1) - __value__(FLDNOW(1)))
        OpenAD_aux_3 = (GAMMALOC * OpenAD_aux_4)
        OpenAD_aux_5 = (__value__(FLDNOW(3)) - __value__(FLDNOW(1)))
        OpenAD_aux_2 = (EXTFORLOC(1) + VOL(1) * OpenAD_aux_3 +
     >  __value__(UVELLOC) * OpenAD_aux_5)
        OpenAD_lin_7 = GAMMALOC
        OpenAD_lin_6 = VOL(1)
        OpenAD_lin_8 = OpenAD_aux_5
        OpenAD_lin_9 = __value__(UVELLOC)
        OpenAD_lin_5 = (INT(1_w2f__i8) / VOL(1))
        __value__(DFLDDT(1)) = (OpenAD_aux_2 / VOL(1))
        OpenAD_acc_2 = (OpenAD_lin_8 * OpenAD_lin_5)
        OpenAD_acc_3 = (OpenAD_lin_9 * OpenAD_lin_5)
        OpenAD_acc_4 = (INT((-1_w2f__i8)) * OpenAD_lin_7 * OpenAD_lin_6
     >  * OpenAD_lin_5)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_2)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_3)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_4)
        OpenAD_aux_8 = (FLDSTAR(2) - __value__(FLDNOW(2)))
        OpenAD_aux_7 = (GAMMALOC * OpenAD_aux_8)
        OpenAD_aux_9 = (__value__(FLDNOW(1)) - __value__(FLDNOW(2)))
        OpenAD_aux_6 = (EXTFORLOC(2) + VOL(2) * OpenAD_aux_7 +
     >  __value__(UVELLOC) * OpenAD_aux_9)
        OpenAD_lin_12 = GAMMALOC
        OpenAD_lin_11 = VOL(2)
        OpenAD_lin_13 = OpenAD_aux_9
        OpenAD_lin_14 = __value__(UVELLOC)
        OpenAD_lin_10 = (INT(1_w2f__i8) / VOL(2))
        __value__(DFLDDT(2)) = (OpenAD_aux_6 / VOL(2))
        OpenAD_acc_5 = (OpenAD_lin_13 * OpenAD_lin_10)
        OpenAD_acc_6 = (OpenAD_lin_14 * OpenAD_lin_10)
        OpenAD_acc_7 = (INT((-1_w2f__i8)) * OpenAD_lin_12 *
     >  OpenAD_lin_11 * OpenAD_lin_10)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_5)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_6)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_7)
        OpenAD_aux_11 = (__value__(FLDNOW(2)) - __value__(FLDNOW(3)))
        OpenAD_aux_10 = (__value__(UVELLOC) * OpenAD_aux_11)
        OpenAD_lin_16 = OpenAD_aux_11
        OpenAD_lin_17 = __value__(UVELLOC)
        OpenAD_lin_15 = (INT(1_w2f__i8) / VOL(3))
        __value__(DFLDDT(3)) = (OpenAD_aux_10 / VOL(3))
        OpenAD_acc_8 = (OpenAD_lin_16 * OpenAD_lin_15)
        OpenAD_acc_9 = (OpenAD_lin_17 * OpenAD_lin_15)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_8)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_9)
        OpenAD_Symbol_19 = 1_w2f__i8
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_19)
      ELSE
        OpenAD_aux_14 = (FLDSTAR(1) - __value__(FLDNOW(1)))
        OpenAD_aux_13 = (GAMMALOC * OpenAD_aux_14)
        OpenAD_aux_15 = (__value__(FLDNOW(2)) - __value__(FLDNOW(1)))
        OpenAD_aux_12 = (EXTFORLOC(1) + VOL(1) * OpenAD_aux_13 -
     >  __value__(UVELLOC) * OpenAD_aux_15)
        OpenAD_lin_20 = GAMMALOC
        OpenAD_lin_19 = VOL(1)
        OpenAD_lin_21 = OpenAD_aux_15
        OpenAD_lin_22 = __value__(UVELLOC)
        OpenAD_lin_18 = (INT(1_w2f__i8) / VOL(1))
        __value__(DFLDDT(1)) = (OpenAD_aux_12 / VOL(1))
        OpenAD_acc_10 = (INT((-1_w2f__i8)) * OpenAD_lin_18)
        OpenAD_acc_11 = (OpenAD_lin_21 * OpenAD_acc_10)
        OpenAD_acc_12 = (OpenAD_lin_22 * OpenAD_acc_10)
        OpenAD_acc_13 = (INT((-1_w2f__i8)) * OpenAD_lin_20 *
     >  OpenAD_lin_19 * OpenAD_lin_18)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_11)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_12)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_13)
        OpenAD_aux_18 = (FLDSTAR(2) - __value__(FLDNOW(2)))
        OpenAD_aux_17 = (GAMMALOC * OpenAD_aux_18)
        OpenAD_aux_19 = (__value__(FLDNOW(3)) - __value__(FLDNOW(2)))
        OpenAD_aux_16 = (EXTFORLOC(2) + VOL(2) * OpenAD_aux_17 -
     >  __value__(UVELLOC) * OpenAD_aux_19)
        OpenAD_lin_25 = GAMMALOC
        OpenAD_lin_24 = VOL(2)
        OpenAD_lin_26 = OpenAD_aux_19
        OpenAD_lin_27 = __value__(UVELLOC)
        OpenAD_lin_23 = (INT(1_w2f__i8) / VOL(2))
        __value__(DFLDDT(2)) = (OpenAD_aux_16 / VOL(2))
        OpenAD_acc_14 = (INT((-1_w2f__i8)) * OpenAD_lin_23)
        OpenAD_acc_15 = (OpenAD_lin_26 * OpenAD_acc_14)
        OpenAD_acc_16 = (OpenAD_lin_27 * OpenAD_acc_14)
        OpenAD_acc_17 = (INT((-1_w2f__i8)) * OpenAD_lin_25 *
     >  OpenAD_lin_24 * OpenAD_lin_23)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_15)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_16)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_17)
        OpenAD_aux_21 = (__value__(FLDNOW(1)) - __value__(FLDNOW(3)))
        OpenAD_aux_20 = (__value__(UVELLOC) * OpenAD_aux_21)
        OpenAD_lin_29 = OpenAD_aux_21
        OpenAD_lin_30 = __value__(UVELLOC)
        OpenAD_lin_28 = (INT(1_w2f__i8) / VOL(3))
        __value__(DFLDDT(3)) = (-(OpenAD_aux_20 / VOL(3)))
        OpenAD_acc_18 = (OpenAD_lin_29 * OpenAD_lin_28 * INT((
     > -1_w2f__i8)))
        OpenAD_acc_19 = (OpenAD_lin_30 * OpenAD_lin_28 * INT((
     > -1_w2f__i8)))
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_18)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_19)
        OpenAD_Symbol_20 = 0_w2f__i8
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_20)
      ENDIF
      CALL box_update(__deriv__(FLDNEW), __deriv__(FLDOLD), __deriv__(
     > DFLDDT))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
      CALL box_update(__deriv__(FLDNEW), __deriv__(FLDOLD), __deriv__(
     > DFLDDT))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_18)
      IF(OpenAD_Symbol_18 .ne. 0) THEN
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_158)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_159)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_158, __deriv__(DFLDDT(3)), __deriv__(
     > OpenAD_prp_3))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_159, __deriv__(DFLDDT(3)), __deriv__(
     > UVELLOC))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(DFLDDT(3)))
C       $OpenAD$ INLINE DecDeriv(subst,subst)
        CALL DecDeriv(__deriv__(OpenAD_prp_3), __deriv__(FLDNOW(3)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_3), __deriv__(FLDNOW(2)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_3))
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_160)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_161)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_162)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_160, __deriv__(DFLDDT(2)), __deriv__(
     > FLDNOW(2)))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_161, __deriv__(DFLDDT(2)), __deriv__(
     > OpenAD_prp_2))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_162, __deriv__(DFLDDT(2)), __deriv__(
     > UVELLOC))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(DFLDDT(2)))
C       $OpenAD$ INLINE DecDeriv(subst,subst)
        CALL DecDeriv(__deriv__(OpenAD_prp_2), __deriv__(FLDNOW(2)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_2), __deriv__(FLDNOW(1)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_2))
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_163)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_164)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_165)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_163, __deriv__(DFLDDT(1)), __deriv__(
     > FLDNOW(1)))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_164, __deriv__(DFLDDT(1)), __deriv__(
     > OpenAD_prp_1))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_165, __deriv__(DFLDDT(1)), __deriv__(
     > UVELLOC))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(DFLDDT(1)))
C       $OpenAD$ INLINE DecDeriv(subst,subst)
        CALL DecDeriv(__deriv__(OpenAD_prp_1), __deriv__(FLDNOW(1)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_1), __deriv__(FLDNOW(3)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_1))
      ELSE
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_166)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_167)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_166, __deriv__(DFLDDT(3)), __deriv__(
     > OpenAD_prp_6))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_167, __deriv__(DFLDDT(3)), __deriv__(
     > UVELLOC))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(DFLDDT(3)))
C       $OpenAD$ INLINE DecDeriv(subst,subst)
        CALL DecDeriv(__deriv__(OpenAD_prp_6), __deriv__(FLDNOW(3)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_6), __deriv__(FLDNOW(1)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_6))
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_168)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_169)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_170)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_168, __deriv__(DFLDDT(2)), __deriv__(
     > FLDNOW(2)))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_169, __deriv__(DFLDDT(2)), __deriv__(
     > OpenAD_prp_5))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_170, __deriv__(DFLDDT(2)), __deriv__(
     > UVELLOC))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(DFLDDT(2)))
C       $OpenAD$ INLINE DecDeriv(subst,subst)
        CALL DecDeriv(__deriv__(OpenAD_prp_5), __deriv__(FLDNOW(2)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_5), __deriv__(FLDNOW(3)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_5))
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_171)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_172)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_173)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_171, __deriv__(DFLDDT(1)), __deriv__(
     > FLDNOW(1)))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_172, __deriv__(DFLDDT(1)), __deriv__(
     > OpenAD_prp_4))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_173, __deriv__(DFLDDT(1)), __deriv__(
     > UVELLOC))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(DFLDDT(1)))
C       $OpenAD$ INLINE DecDeriv(subst,subst)
        CALL DecDeriv(__deriv__(OpenAD_prp_4), __deriv__(FLDNOW(1)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_4), __deriv__(FLDNOW(2)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_4))
      ENDIF
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 4
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(FLDOLD))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(FLDNEW))
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(DELTA_T)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(GAMMALOC)
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(UVELLOC))
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(VOL)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(FLDSTAR)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(EXTFORLOC)
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(FLDNOW))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 5
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 6
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(FLDNOW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(EXTFORLOC)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(FLDSTAR)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(VOL)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(UVELLOC))
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(GAMMALOC)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(DELTA_T)
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(FLDNEW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(FLDOLD))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 7
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 8
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(FLDNEW))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(FLDOLD))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(FLDNEW))
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(GAMMALOC)
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(UVELLOC))
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(VOL)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(FLDSTAR)
C     $OpenAD$ INLINE cp_arg_store_real_vector(subst)
      CALL cp_arg_store_real_vector(EXTFORLOC)
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(FLDNOW))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 9
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(FLDNOW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(EXTFORLOC)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(FLDSTAR)
C     $OpenAD$ INLINE cp_arg_restore_real_vector(subst)
      CALL cp_arg_restore_real_vector(VOL)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(UVELLOC))
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(GAMMALOC)
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(FLDNEW))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(FLDOLD))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(FLDNEW))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 10
C$OPENAD XXX Template ad_template.f
      IF(__value__(UVELLOC) .GE. 0.0D00) THEN
        OpenAD_aux_4 = (FLDSTAR(1) - __value__(FLDNOW(1)))
        OpenAD_aux_3 = (GAMMALOC * OpenAD_aux_4)
        OpenAD_aux_5 = (__value__(FLDNOW(3)) - __value__(FLDNOW(1)))
        OpenAD_aux_2 = (EXTFORLOC(1) + VOL(1) * OpenAD_aux_3 +
     >  __value__(UVELLOC) * OpenAD_aux_5)
        OpenAD_lin_7 = GAMMALOC
        OpenAD_lin_6 = VOL(1)
        OpenAD_lin_8 = OpenAD_aux_5
        OpenAD_lin_9 = __value__(UVELLOC)
        OpenAD_lin_5 = (INT(1_w2f__i8) / VOL(1))
        __value__(DFLDDT(1)) = (OpenAD_aux_2 / VOL(1))
        OpenAD_acc_2 = (OpenAD_lin_8 * OpenAD_lin_5)
        OpenAD_acc_3 = (OpenAD_lin_9 * OpenAD_lin_5)
        OpenAD_acc_4 = (INT((-1_w2f__i8)) * OpenAD_lin_7 * OpenAD_lin_6
     >  * OpenAD_lin_5)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_2)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_3)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_4)
        OpenAD_aux_8 = (FLDSTAR(2) - __value__(FLDNOW(2)))
        OpenAD_aux_7 = (GAMMALOC * OpenAD_aux_8)
        OpenAD_aux_9 = (__value__(FLDNOW(1)) - __value__(FLDNOW(2)))
        OpenAD_aux_6 = (EXTFORLOC(2) + VOL(2) * OpenAD_aux_7 +
     >  __value__(UVELLOC) * OpenAD_aux_9)
        OpenAD_lin_12 = GAMMALOC
        OpenAD_lin_11 = VOL(2)
        OpenAD_lin_13 = OpenAD_aux_9
        OpenAD_lin_14 = __value__(UVELLOC)
        OpenAD_lin_10 = (INT(1_w2f__i8) / VOL(2))
        __value__(DFLDDT(2)) = (OpenAD_aux_6 / VOL(2))
        OpenAD_acc_5 = (OpenAD_lin_13 * OpenAD_lin_10)
        OpenAD_acc_6 = (OpenAD_lin_14 * OpenAD_lin_10)
        OpenAD_acc_7 = (INT((-1_w2f__i8)) * OpenAD_lin_12 *
     >  OpenAD_lin_11 * OpenAD_lin_10)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_5)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_6)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_7)
        OpenAD_aux_11 = (__value__(FLDNOW(2)) - __value__(FLDNOW(3)))
        OpenAD_aux_10 = (__value__(UVELLOC) * OpenAD_aux_11)
        OpenAD_lin_16 = OpenAD_aux_11
        OpenAD_lin_17 = __value__(UVELLOC)
        OpenAD_lin_15 = (INT(1_w2f__i8) / VOL(3))
        __value__(DFLDDT(3)) = (OpenAD_aux_10 / VOL(3))
        OpenAD_acc_8 = (OpenAD_lin_16 * OpenAD_lin_15)
        OpenAD_acc_9 = (OpenAD_lin_17 * OpenAD_lin_15)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_8)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_9)
        OpenAD_Symbol_22 = 1_w2f__i8
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_22)
      ELSE
        OpenAD_aux_14 = (FLDSTAR(1) - __value__(FLDNOW(1)))
        OpenAD_aux_13 = (GAMMALOC * OpenAD_aux_14)
        OpenAD_aux_15 = (__value__(FLDNOW(2)) - __value__(FLDNOW(1)))
        OpenAD_aux_12 = (EXTFORLOC(1) + VOL(1) * OpenAD_aux_13 -
     >  __value__(UVELLOC) * OpenAD_aux_15)
        OpenAD_lin_20 = GAMMALOC
        OpenAD_lin_19 = VOL(1)
        OpenAD_lin_21 = OpenAD_aux_15
        OpenAD_lin_22 = __value__(UVELLOC)
        OpenAD_lin_18 = (INT(1_w2f__i8) / VOL(1))
        __value__(DFLDDT(1)) = (OpenAD_aux_12 / VOL(1))
        OpenAD_acc_10 = (INT((-1_w2f__i8)) * OpenAD_lin_18)
        OpenAD_acc_11 = (OpenAD_lin_21 * OpenAD_acc_10)
        OpenAD_acc_12 = (OpenAD_lin_22 * OpenAD_acc_10)
        OpenAD_acc_13 = (INT((-1_w2f__i8)) * OpenAD_lin_20 *
     >  OpenAD_lin_19 * OpenAD_lin_18)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_11)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_12)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_13)
        OpenAD_aux_18 = (FLDSTAR(2) - __value__(FLDNOW(2)))
        OpenAD_aux_17 = (GAMMALOC * OpenAD_aux_18)
        OpenAD_aux_19 = (__value__(FLDNOW(3)) - __value__(FLDNOW(2)))
        OpenAD_aux_16 = (EXTFORLOC(2) + VOL(2) * OpenAD_aux_17 -
     >  __value__(UVELLOC) * OpenAD_aux_19)
        OpenAD_lin_25 = GAMMALOC
        OpenAD_lin_24 = VOL(2)
        OpenAD_lin_26 = OpenAD_aux_19
        OpenAD_lin_27 = __value__(UVELLOC)
        OpenAD_lin_23 = (INT(1_w2f__i8) / VOL(2))
        __value__(DFLDDT(2)) = (OpenAD_aux_16 / VOL(2))
        OpenAD_acc_14 = (INT((-1_w2f__i8)) * OpenAD_lin_23)
        OpenAD_acc_15 = (OpenAD_lin_26 * OpenAD_acc_14)
        OpenAD_acc_16 = (OpenAD_lin_27 * OpenAD_acc_14)
        OpenAD_acc_17 = (INT((-1_w2f__i8)) * OpenAD_lin_25 *
     >  OpenAD_lin_24 * OpenAD_lin_23)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_15)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_16)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_17)
        OpenAD_aux_21 = (__value__(FLDNOW(1)) - __value__(FLDNOW(3)))
        OpenAD_aux_20 = (__value__(UVELLOC) * OpenAD_aux_21)
        OpenAD_lin_29 = OpenAD_aux_21
        OpenAD_lin_30 = __value__(UVELLOC)
        OpenAD_lin_28 = (INT(1_w2f__i8) / VOL(3))
        __value__(DFLDDT(3)) = (-(OpenAD_aux_20 / VOL(3)))
        OpenAD_acc_18 = (OpenAD_lin_29 * OpenAD_lin_28 * INT((
     > -1_w2f__i8)))
        OpenAD_acc_19 = (OpenAD_lin_30 * OpenAD_lin_28 * INT((
     > -1_w2f__i8)))
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_18)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_19)
        OpenAD_Symbol_23 = 0_w2f__i8
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_23)
      ENDIF
      CALL box_update(__deriv__(FLDNEW), __deriv__(FLDOLD), __deriv__(
     > DFLDDT))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
      CALL box_update(__deriv__(FLDNEW), __deriv__(FLDOLD), __deriv__(
     > DFLDDT))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_21)
      IF(OpenAD_Symbol_21 .ne. 0) THEN
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_158)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_159)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_158, __deriv__(DFLDDT(3)), __deriv__(
     > OpenAD_prp_3))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_159, __deriv__(DFLDDT(3)), __deriv__(
     > UVELLOC))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(DFLDDT(3)))
C       $OpenAD$ INLINE DecDeriv(subst,subst)
        CALL DecDeriv(__deriv__(OpenAD_prp_3), __deriv__(FLDNOW(3)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_3), __deriv__(FLDNOW(2)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_3))
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_160)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_161)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_162)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_160, __deriv__(DFLDDT(2)), __deriv__(
     > FLDNOW(2)))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_161, __deriv__(DFLDDT(2)), __deriv__(
     > OpenAD_prp_2))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_162, __deriv__(DFLDDT(2)), __deriv__(
     > UVELLOC))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(DFLDDT(2)))
C       $OpenAD$ INLINE DecDeriv(subst,subst)
        CALL DecDeriv(__deriv__(OpenAD_prp_2), __deriv__(FLDNOW(2)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_2), __deriv__(FLDNOW(1)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_2))
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_163)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_164)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_165)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_163, __deriv__(DFLDDT(1)), __deriv__(
     > FLDNOW(1)))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_164, __deriv__(DFLDDT(1)), __deriv__(
     > OpenAD_prp_1))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_165, __deriv__(DFLDDT(1)), __deriv__(
     > UVELLOC))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(DFLDDT(1)))
C       $OpenAD$ INLINE DecDeriv(subst,subst)
        CALL DecDeriv(__deriv__(OpenAD_prp_1), __deriv__(FLDNOW(1)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_1), __deriv__(FLDNOW(3)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_1))
      ELSE
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_166)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_167)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_166, __deriv__(DFLDDT(3)), __deriv__(
     > OpenAD_prp_6))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_167, __deriv__(DFLDDT(3)), __deriv__(
     > UVELLOC))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(DFLDDT(3)))
C       $OpenAD$ INLINE DecDeriv(subst,subst)
        CALL DecDeriv(__deriv__(OpenAD_prp_6), __deriv__(FLDNOW(3)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_6), __deriv__(FLDNOW(1)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_6))
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_168)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_169)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_170)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_168, __deriv__(DFLDDT(2)), __deriv__(
     > FLDNOW(2)))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_169, __deriv__(DFLDDT(2)), __deriv__(
     > OpenAD_prp_5))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_170, __deriv__(DFLDDT(2)), __deriv__(
     > UVELLOC))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(DFLDDT(2)))
C       $OpenAD$ INLINE DecDeriv(subst,subst)
        CALL DecDeriv(__deriv__(OpenAD_prp_5), __deriv__(FLDNOW(2)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_5), __deriv__(FLDNOW(3)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_5))
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_171)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_172)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_173)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_171, __deriv__(DFLDDT(1)), __deriv__(
     > FLDNOW(1)))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_172, __deriv__(DFLDDT(1)), __deriv__(
     > OpenAD_prp_4))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_173, __deriv__(DFLDDT(1)), __deriv__(
     > UVELLOC))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(DFLDDT(1)))
C       $OpenAD$ INLINE DecDeriv(subst,subst)
        CALL DecDeriv(__deriv__(OpenAD_prp_4), __deriv__(FLDNOW(1)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_4), __deriv__(FLDNOW(2)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_4))
      ENDIF
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
      CALL cp_arg_store_real_vector_a_d(__deriv__(FLDNEW))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
      CALL cp_arg_restore_real_vector_a_d(__deriv__(FLDNEW))
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE
