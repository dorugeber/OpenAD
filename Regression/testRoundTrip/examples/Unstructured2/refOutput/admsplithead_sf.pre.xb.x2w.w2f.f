
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
      INTEGER(w2f__i4) II
      INTEGER(w2f__i4) J
      INTEGER(w2f__i8) OpenAD_Symbol_15
      INTEGER(w2f__i8) OpenAD_Symbol_7
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
      GO TO 4
4     CONTINUE
      J = 1
      GO TO 18
5     CONTINUE
      J = J + 1
18    CONTINUE
      IF(J .LE. 5) THEN
        GO TO 6
      ELSE
        GO TO 16
      ENDIF
6     CONTINUE
      GO TO 7
7     CONTINUE
      II = 1
      GO TO 19
8     CONTINUE
      II = II + 1
19    CONTINUE
      IF(II .LE. 5) THEN
        GO TO 9
      ELSE
        GO TO 10
      ENDIF
9     CONTINUE
      GO TO 10
10    CONTINUE
      GO TO 11
11    CONTINUE
      II = 1
      GO TO 20
12    CONTINUE
      II = II + 1
20    CONTINUE
      IF(II .LE. 5) THEN
        GO TO 13
      ELSE
        GO TO 15
      ENDIF
13    CONTINUE
      __value__(Y(1)) = __value__(X(1))
      GO TO 14
14    CONTINUE
      GO TO 12
15    CONTINUE
      GO TO 5
16    CONTINUE
      GO TO 17
17    CONTINUE
      GO TO 1
1     CONTINUE
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
22    CONTINUE
      GO TO 23
23    CONTINUE
      OpenAD_Symbol_0 = 0_w2f__i8
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_0)
      GO TO 24
24    CONTINUE
      GO TO 25
25    CONTINUE
      OpenAD_Symbol_1 = 1_w2f__i8
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_1)
      GO TO 26
26    CONTINUE
      J = 1
      GO TO 45
27    CONTINUE
      J = J + 1
45    CONTINUE
      IF(J .LE. 5) THEN
        GO TO 28
      ELSE
        GO TO 42
      ENDIF
28    CONTINUE
      GO TO 29
29    CONTINUE
      OpenAD_Symbol_2 = 2_w2f__i8
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_2)
      GO TO 30
30    CONTINUE
      II = 1
      GO TO 46
31    CONTINUE
      II = II + 1
46    CONTINUE
      IF(II .LE. 5) THEN
        GO TO 32
      ELSE
        GO TO 34
      ENDIF
32    CONTINUE
      GO TO 33
33    CONTINUE
      OpenAD_Symbol_3 = 3_w2f__i8
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_3)
      GO TO 34
34    CONTINUE
      GO TO 35
35    CONTINUE
      OpenAD_Symbol_4 = 4_w2f__i8
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_4)
      GO TO 36
36    CONTINUE
      II = 1
      GO TO 47
37    CONTINUE
      II = II + 1
47    CONTINUE
      IF(II .LE. 5) THEN
        GO TO 38
      ELSE
        GO TO 41
      ENDIF
38    CONTINUE
      __value__(Y(1)) = __value__(X(1))
      GO TO 39
39    CONTINUE
      OpenAD_Symbol_5 = 5_w2f__i8
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_5)
      GO TO 40
40    CONTINUE
      GO TO 37
41    CONTINUE
      GO TO 27
42    CONTINUE
      GO TO 43
43    CONTINUE
      OpenAD_Symbol_6 = 6_w2f__i8
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_6)
      GO TO 44
44    CONTINUE
      GO TO 21
21    CONTINUE
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_7)
      DO WHILE(INT(OpenAD_Symbol_7) .ne. INT(0_w2f__i8))
        IF ( OpenAD_Symbol_7  .EQ.  1)  GO TO  70
        IF ( OpenAD_Symbol_7  .EQ.  2)  GO TO  71
        IF ( OpenAD_Symbol_7  .EQ.  3)  GO TO  72
        IF ( OpenAD_Symbol_7  .EQ.  4)  GO TO  73
        IF ( OpenAD_Symbol_7  .EQ.  5)  GO TO  74
        IF ( OpenAD_Symbol_7  .EQ.  6)  GO TO  75
70      CONTINUE
        GO TO 67
71      CONTINUE
        GO TO 67
72      CONTINUE
        GO TO 67
73      CONTINUE
        GO TO 67
74      CONTINUE
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(Y(1)), __deriv__(X(1)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(Y(1)))
        GO TO 67
75      CONTINUE
        GO TO 67
67      CONTINUE
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
179   CONTINUE
      GO TO 180
180   CONTINUE
      OpenAD_Symbol_8 = 0_w2f__i8
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_8)
      GO TO 181
181   CONTINUE
      GO TO 182
182   CONTINUE
      OpenAD_Symbol_9 = 1_w2f__i8
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_9)
      GO TO 183
183   CONTINUE
      J = 1
      GO TO 202
184   CONTINUE
      J = J + 1
202   CONTINUE
      IF(J .LE. 5) THEN
        GO TO 185
      ELSE
        GO TO 199
      ENDIF
185   CONTINUE
      GO TO 186
186   CONTINUE
      OpenAD_Symbol_10 = 2_w2f__i8
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_10)
      GO TO 187
187   CONTINUE
      II = 1
      GO TO 203
188   CONTINUE
      II = II + 1
203   CONTINUE
      IF(II .LE. 5) THEN
        GO TO 189
      ELSE
        GO TO 191
      ENDIF
189   CONTINUE
      GO TO 190
190   CONTINUE
      OpenAD_Symbol_11 = 3_w2f__i8
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_11)
      GO TO 191
191   CONTINUE
      GO TO 192
192   CONTINUE
      OpenAD_Symbol_12 = 4_w2f__i8
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_12)
      GO TO 193
193   CONTINUE
      II = 1
      GO TO 204
194   CONTINUE
      II = II + 1
204   CONTINUE
      IF(II .LE. 5) THEN
        GO TO 195
      ELSE
        GO TO 198
      ENDIF
195   CONTINUE
      __value__(Y(1)) = __value__(X(1))
      GO TO 196
196   CONTINUE
      OpenAD_Symbol_13 = 5_w2f__i8
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_13)
      GO TO 197
197   CONTINUE
      GO TO 194
198   CONTINUE
      GO TO 184
199   CONTINUE
      GO TO 200
200   CONTINUE
      OpenAD_Symbol_14 = 6_w2f__i8
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_14)
      GO TO 201
201   CONTINUE
      GO TO 178
178   CONTINUE
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_15)
      DO WHILE(INT(OpenAD_Symbol_15) .ne. INT(0_w2f__i8))
        IF ( OpenAD_Symbol_15  .EQ.  1)  GO TO  227
        IF ( OpenAD_Symbol_15  .EQ.  2)  GO TO  228
        IF ( OpenAD_Symbol_15  .EQ.  3)  GO TO  229
        IF ( OpenAD_Symbol_15  .EQ.  4)  GO TO  230
        IF ( OpenAD_Symbol_15  .EQ.  5)  GO TO  231
        IF ( OpenAD_Symbol_15  .EQ.  6)  GO TO  232
227     CONTINUE
        GO TO 224
228     CONTINUE
        GO TO 224
229     CONTINUE
        GO TO 224
230     CONTINUE
        GO TO 224
231     CONTINUE
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(Y(1)), __deriv__(X(1)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(Y(1)))
        GO TO 224
232     CONTINUE
        GO TO 224
224     CONTINUE
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_15)
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE
