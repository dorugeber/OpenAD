
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
      SUBROUTINE head(X, Y)
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
      TYPE (OpenADTy_active) X(1 : 3)
      TYPE (OpenADTy_active) Y(1 : 3)
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) OpenAD_Symbol_12
      INTEGER(w2f__i4) OpenAD_Symbol_13
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
      IF(__value__(X(1)) .LT. 4.0D00) THEN
C$OPENAD XXX Simple loop
        DO I = 1, 3, 1
          __value__(Y(INT(I))) = __value__(X(I))
        END DO
      ELSE
C$OPENAD XXX Simple loop
        DO I = 1, 3, 1
          __value__(Y(INT(I))) = (__value__(X(I)) * 2.0D00)
        END DO
      ENDIF
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      IF(__value__(X(1)) .LT. 4.0D00) THEN
C$OPENAD XXX Simple loop
        DO I = 1, 3, 1
          __value__(Y(INT(I))) = __value__(X(I))
        END DO
        OpenAD_Symbol_1 = 1_w2f__i8
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_1)
      ELSE
C$OPENAD XXX Simple loop
        DO I = 1, 3, 1
          __value__(Y(INT(I))) = (__value__(X(I)) * 2.0D00)
        END DO
        OpenAD_Symbol_2 = 0_w2f__i8
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_2)
      ENDIF
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_0)
      IF(OpenAD_Symbol_0 .ne. 0) THEN
        I = 1 + 1 *((3 - 1) / 1)
        DO WHILE(I .GE. 1)
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(Y(I)), __deriv__(X(I)))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(Y(I)))
          I = I - 1
        END DO
      ELSE
        I = 1 + 1 *((3 - 1) / 1)
        DO WHILE(I .GE. 1)
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(2.0D00, __deriv__(Y(I)), __deriv__(X(I)))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(Y(I)))
          I = I - 1
        END DO
      ENDIF
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
      IF(__value__(X(1)) .LT. 4.0D00) THEN
C$OPENAD XXX Simple loop
        OpenAD_Symbol_8 = 0_w2f__i8
        DO I = 1, 3, 1
          __value__(Y(INT(I))) = __value__(X(I))
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(I)
          OpenAD_Symbol_8 = (INT(OpenAD_Symbol_8) + INT(1_w2f__i8))
        END DO
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_8)
        OpenAD_Symbol_10 = 1_w2f__i8
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_10)
      ELSE
C$OPENAD XXX Simple loop
        OpenAD_Symbol_9 = 0_w2f__i8
        DO I = 1, 3, 1
          __value__(Y(INT(I))) = (__value__(X(I)) * 2.0D00)
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(I)
          OpenAD_Symbol_9 = (INT(OpenAD_Symbol_9) + INT(1_w2f__i8))
        END DO
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_9)
        OpenAD_Symbol_11 = 0_w2f__i8
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_11)
      ENDIF
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_3)
      IF(OpenAD_Symbol_3 .ne. 0) THEN
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_6)
        OpenAD_Symbol_7 = 1
        DO WHILE(INT(OpenAD_Symbol_7) .LE. INT(OpenAD_Symbol_6))
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_12)
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(Y(OpenAD_Symbol_12)), __deriv__(X(
     > OpenAD_Symbol_12)))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(Y(OpenAD_Symbol_12)))
          OpenAD_Symbol_7 = INT(OpenAD_Symbol_7) + 1
        END DO
      ELSE
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_4)
        OpenAD_Symbol_5 = 1
        DO WHILE(INT(OpenAD_Symbol_5) .LE. INT(OpenAD_Symbol_4))
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_13)
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(2.0D00, __deriv__(Y(OpenAD_Symbol_13)), __deriv__(
     > X(OpenAD_Symbol_13)))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(Y(OpenAD_Symbol_13)))
          OpenAD_Symbol_5 = INT(OpenAD_Symbol_5) + 1
        END DO
      ENDIF
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE
