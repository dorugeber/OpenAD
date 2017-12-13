
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
      INTEGER(w2f__i8) OpenAD_Symbol_12
      INTEGER(w2f__i8) OpenAD_Symbol_13
      INTEGER(w2f__i8) OpenAD_Symbol_14
      INTEGER(w2f__i8) OpenAD_Symbol_2
      INTEGER(w2f__i8) OpenAD_Symbol_3
      INTEGER(w2f__i8) OpenAD_Symbol_4
      INTEGER(w2f__i8) OpenAD_Symbol_5
      INTEGER(w2f__i8) OpenAD_Symbol_6
      INTEGER(w2f__i8) OpenAD_Symbol_8
      INTEGER(w2f__i8) OpenAD_Symbol_9
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X(1 : 1)
      TYPE (OpenADTy_active) Y(1 : 1)
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      INTEGER(w2f__i8) OpenAD_Symbol_15
      INTEGER(w2f__i4) OpenAD_Symbol_16
      INTEGER(w2f__i8) OpenAD_Symbol_7
      INTEGER(w2f__i4) OpenAD_lin_0
      TYPE (OpenADTy_active) OpenAD_prp_0
      TYPE (OpenADTy_active) OpenAD_prp_1
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C     $OpenAD$ BEGIN REPLACEMENT 1
2     CONTINUE
      GO TO 3
3     CONTINUE
C$OPENAD XXX Template ad_template.f
      I = 2
      GO TO 4
4     CONTINUE
      IF(__value__(X(1)) .LT. 1.0D00) THEN
        GO TO 8
      ELSE
        GO TO 5
      ENDIF
5     CONTINUE
      GO TO 6
6     CONTINUE
      GO TO 7
7     CONTINUE
      __value__(Y(1)) = (__value__(Y(1)) * 2.0D00)
      GO TO 10
8     CONTINUE
      __value__(Y(1)) = (I * __value__(X(1)))
      GO TO 9
9     CONTINUE
      __value__(Y(1)) = (__value__(Y(1)) * 3.0D00)
      GO TO 10
10    CONTINUE
      GO TO 11
11    CONTINUE
      GO TO 1
1     CONTINUE
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
13    CONTINUE
      GO TO 14
14    CONTINUE
      OpenAD_Symbol_0 = 0_w2f__i8
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_0)
      GO TO 15
15    CONTINUE
C$OPENAD XXX Template ad_template.f
      I = 2
      GO TO 16
16    CONTINUE
      OpenAD_Symbol_1 = 1_w2f__i8
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_1)
      GO TO 17
17    CONTINUE
      IF(__value__(X(1)) .LT. 1.0D00) THEN
        GO TO 23
      ELSE
        GO TO 18
      ENDIF
18    CONTINUE
      GO TO 19
19    CONTINUE
      OpenAD_Symbol_5 = 5_w2f__i8
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_5)
      GO TO 20
20    CONTINUE
      GO TO 21
21    CONTINUE
      __value__(Y(1)) = (__value__(Y(1)) * 2.0D00)
      GO TO 22
22    CONTINUE
      OpenAD_Symbol_6 = 6_w2f__i8
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_6)
      GO TO 27
23    CONTINUE
      OpenAD_lin_0 = I
      __value__(Y(1)) = (I * __value__(X(1)))
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_lin_0)
      GO TO 24
24    CONTINUE
      OpenAD_Symbol_2 = 2_w2f__i8
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_2)
      GO TO 25
25    CONTINUE
      __value__(Y(1)) = (__value__(Y(1)) * 3.0D00)
      GO TO 26
26    CONTINUE
      OpenAD_Symbol_3 = 3_w2f__i8
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_3)
      GO TO 27
27    CONTINUE
      GO TO 28
28    CONTINUE
      OpenAD_Symbol_4 = 4_w2f__i8
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_4)
      GO TO 29
29    CONTINUE
      GO TO 12
12    CONTINUE
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_7)
      DO WHILE(INT(OpenAD_Symbol_7) .ne. INT(0_w2f__i8))
        IF ( OpenAD_Symbol_7  .EQ.  1)  GO TO  52
        IF ( OpenAD_Symbol_7  .EQ.  2)  GO TO  53
        IF ( OpenAD_Symbol_7  .EQ.  3)  GO TO  54
        IF ( OpenAD_Symbol_7  .EQ.  4)  GO TO  55
        IF ( OpenAD_Symbol_7  .EQ.  5)  GO TO  56
        IF ( OpenAD_Symbol_7  .EQ.  6)  GO TO  57
52      CONTINUE
        GO TO 49
53      CONTINUE
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_16)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_16, __deriv__(Y(1)), __deriv__(X(1)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(Y(1)))
        GO TO 49
54      CONTINUE
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(3.0D00, __deriv__(Y(1)), __deriv__(OpenAD_prp_0))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(Y(1)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(Y(1)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
        GO TO 49
55      CONTINUE
        GO TO 49
56      CONTINUE
        GO TO 49
57      CONTINUE
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(2.0D00, __deriv__(Y(1)), __deriv__(OpenAD_prp_1))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(Y(1)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_1), __deriv__(Y(1)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_1))
        GO TO 49
49      CONTINUE
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_7)
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
161   CONTINUE
      GO TO 162
162   CONTINUE
      OpenAD_Symbol_8 = 0_w2f__i8
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_8)
      GO TO 163
163   CONTINUE
C$OPENAD XXX Template ad_template.f
      I = 2
      GO TO 164
164   CONTINUE
      OpenAD_Symbol_9 = 1_w2f__i8
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_9)
      GO TO 165
165   CONTINUE
      IF(__value__(X(1)) .LT. 1.0D00) THEN
        GO TO 171
      ELSE
        GO TO 166
      ENDIF
166   CONTINUE
      GO TO 167
167   CONTINUE
      OpenAD_Symbol_13 = 5_w2f__i8
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_13)
      GO TO 168
168   CONTINUE
      GO TO 169
169   CONTINUE
      __value__(Y(1)) = (__value__(Y(1)) * 2.0D00)
      GO TO 170
170   CONTINUE
      OpenAD_Symbol_14 = 6_w2f__i8
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_14)
      GO TO 175
171   CONTINUE
      OpenAD_lin_0 = I
      __value__(Y(1)) = (I * __value__(X(1)))
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_lin_0)
      GO TO 172
172   CONTINUE
      OpenAD_Symbol_10 = 2_w2f__i8
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_10)
      GO TO 173
173   CONTINUE
      __value__(Y(1)) = (__value__(Y(1)) * 3.0D00)
      GO TO 174
174   CONTINUE
      OpenAD_Symbol_11 = 3_w2f__i8
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_11)
      GO TO 175
175   CONTINUE
      GO TO 176
176   CONTINUE
      OpenAD_Symbol_12 = 4_w2f__i8
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_12)
      GO TO 177
177   CONTINUE
      GO TO 160
160   CONTINUE
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_15)
      DO WHILE(INT(OpenAD_Symbol_15) .ne. INT(0_w2f__i8))
        IF ( OpenAD_Symbol_15  .EQ.  1)  GO TO  200
        IF ( OpenAD_Symbol_15  .EQ.  2)  GO TO  201
        IF ( OpenAD_Symbol_15  .EQ.  3)  GO TO  202
        IF ( OpenAD_Symbol_15  .EQ.  4)  GO TO  203
        IF ( OpenAD_Symbol_15  .EQ.  5)  GO TO  204
        IF ( OpenAD_Symbol_15  .EQ.  6)  GO TO  205
200     CONTINUE
        GO TO 197
201     CONTINUE
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_16)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_16, __deriv__(Y(1)), __deriv__(X(1)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(Y(1)))
        GO TO 197
202     CONTINUE
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(3.0D00, __deriv__(Y(1)), __deriv__(OpenAD_prp_0))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(Y(1)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(Y(1)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
        GO TO 197
203     CONTINUE
        GO TO 197
204     CONTINUE
        GO TO 197
205     CONTINUE
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(2.0D00, __deriv__(Y(1)), __deriv__(OpenAD_prp_1))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(Y(1)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_1), __deriv__(Y(1)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_1))
        GO TO 197
197     CONTINUE
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_15)
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE
