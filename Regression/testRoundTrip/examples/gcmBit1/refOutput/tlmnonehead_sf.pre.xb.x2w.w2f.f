
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
      SUBROUTINE find_bulkmod(LOCPRES, TFLD, SFLD, BULKMOD)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) LOCPRES(1 : 2, 1 : 2)
      TYPE (OpenADTy_active) TFLD(1 : 2, 1 : 2, 1 : 2, 1 : 2, 1 : 2)
      TYPE (OpenADTy_active) SFLD(1 : 2, 1 : 2, 1 : 2, 1 : 2, 1 : 2)
      TYPE (OpenADTy_active) BULKMOD(1 : 2, 1 : 2)
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) BI
      INTEGER(w2f__i4) BJ
      TYPE (OpenADTy_active) BMFRESH
      TYPE (OpenADTy_active) BMPRES
      TYPE (OpenADTy_active) BMSALT
      REAL(w2f__8) EOSJMDCKFW(1 : 5)
      REAL(w2f__8) EOSJMDCKP(1 : 14)
      REAL(w2f__8) EOSJMDCKSW(1 : 7)
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) J
      INTEGER(w2f__i4) K
      TYPE (OpenADTy_active) P
      TYPE (OpenADTy_active) P2
      TYPE (OpenADTy_active) S
      TYPE (OpenADTy_active) S3O2
      REAL(w2f__8) SITOBAR
      TYPE (OpenADTy_active) T
      TYPE (OpenADTy_active) T2
      TYPE (OpenADTy_active) T3
      TYPE (OpenADTy_active) T4
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
      REAL(w2f__8) OpenAD_acc_3
      REAL(w2f__8) OpenAD_acc_4
      REAL(w2f__8) OpenAD_acc_5
      REAL(w2f__8) OpenAD_acc_6
      REAL(w2f__8) OpenAD_acc_7
      REAL(w2f__8) OpenAD_acc_8
      REAL(w2f__8) OpenAD_acc_9
      REAL(w2f__8) OpenAD_aux_0
      REAL(w2f__8) OpenAD_aux_1
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
      REAL(w2f__8) OpenAD_lin_5
      REAL(w2f__8) OpenAD_lin_6
      REAL(w2f__8) OpenAD_lin_7
      REAL(w2f__8) OpenAD_lin_8
      REAL(w2f__8) OpenAD_lin_9
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      EOSJMDCKFW(1) = 1.9659330078E+04
      EOSJMDCKFW(2) = 1.4443040466E+02
      EOSJMDCKFW(3) = (-1.7061029673)
      EOSJMDCKFW(4) = 9.64870397E-03
      EOSJMDCKFW(5) = (-4.1902530938E-05)
      EOSJMDCKSW(1) = 5.2848548889E+01
      EOSJMDCKSW(2) = (-3.1010890007E-01)
      EOSJMDCKSW(3) = 6.2832632102E-03
      EOSJMDCKSW(4) = (-5.0841881603E-05)
      EOSJMDCKSW(5) = 3.8866400719E-01
      EOSJMDCKSW(6) = 9.085830301E-03
      EOSJMDCKSW(7) = (-4.6199240023E-04)
      EOSJMDCKP(1) = 3.1865189075
      EOSJMDCKP(2) = 2.2122759372E-02
      EOSJMDCKP(3) = (-2.9846420512E-04)
      EOSJMDCKP(4) = 1.9564149625E-06
      EOSJMDCKP(5) = 6.7043881863E-03
      EOSJMDCKP(6) = (-1.8473180535E-04)
      EOSJMDCKP(7) = 2.0593310524E-07
      EOSJMDCKP(8) = 1.480265928E-04
      EOSJMDCKP(9) = 2.1028979972E-04
      EOSJMDCKP(10) = (-1.2020160284E-05)
      EOSJMDCKP(11) = 1.3946799982E-07
      EOSJMDCKP(12) = (-2.0402369501E-06)
      EOSJMDCKP(13) = 6.128772867E-08
      EOSJMDCKP(14) = 6.2073229889E-10
      K = 1
      BI = 1
      BJ = 1
      SITOBAR = 1.0D00
      DO J = 1, 2, 1
        DO I = 1, 2, 1
          __value__(T) = __value__(TFLD(I, J, K, BI, BJ))
          OpenAD_lin_0 = __value__(T)
          OpenAD_lin_1 = __value__(T)
          __value__(T2) = (__value__(T) * __value__(T))
          OpenAD_lin_2 = __value__(T2)
          OpenAD_lin_3 = __value__(T)
          __value__(T3) = (__value__(T) * __value__(T2))
          OpenAD_lin_4 = __value__(T3)
          OpenAD_lin_5 = __value__(T)
          __value__(T4) = (__value__(T) * __value__(T3))
          __value__(S) = __value__(SFLD(I, J, K, BI, BJ))
          OpenAD_acc_0 = (OpenAD_lin_0 + OpenAD_lin_1)
          OpenAD_acc_1 = (OpenAD_lin_2 + OpenAD_acc_0 * OpenAD_lin_3)
          OpenAD_acc_2 = (OpenAD_lin_4 + OpenAD_acc_1 * OpenAD_lin_5)
          CALL setderiv(__deriv__(T), __deriv__(TFLD(I, J, K, BI, BJ)))
          CALL sax(OpenAD_acc_0, __deriv__(T), __deriv__(T2))
          CALL sax(OpenAD_acc_1, __deriv__(T), __deriv__(T3))
          CALL sax(OpenAD_acc_2, __deriv__(T), __deriv__(T4))
          CALL setderiv(__deriv__(S), __deriv__(SFLD(I, J, K, BI, BJ)))
          IF(__value__(S) .GT. 0.0D00) THEN
            OpenAD_aux_0 = SQRT(__value__(S))
            OpenAD_lin_6 = OpenAD_aux_0
            OpenAD_lin_8 = (5.0D-01 / OpenAD_aux_0)
            OpenAD_lin_7 = __value__(S)
            __value__(S3O2) = (__value__(S) * OpenAD_aux_0)
            OpenAD_acc_3 = (OpenAD_lin_8 * OpenAD_lin_7)
            CALL sax(OpenAD_lin_6, __deriv__(S), __deriv__(S3O2))
            CALL saxpy(OpenAD_acc_3, __deriv__(S), __deriv__(S3O2))
          ELSE
            __value__(S) = 0.0D00
            __value__(S3O2) = 0.0D00
            CALL zero_deriv(__deriv__(S))
            CALL zero_deriv(__deriv__(S3O2))
          ENDIF
          OpenAD_lin_9 = SITOBAR
          __value__(P) = (__value__(LOCPRES(I, J)) * SITOBAR)
          OpenAD_lin_10 = __value__(P)
          OpenAD_lin_11 = __value__(P)
          __value__(P2) = (__value__(P) * __value__(P))
          OpenAD_lin_12 = EOSJMDCKFW(2)
          OpenAD_lin_13 = EOSJMDCKFW(3)
          OpenAD_lin_14 = EOSJMDCKFW(4)
          OpenAD_lin_15 = EOSJMDCKFW(5)
          __value__(BMFRESH) = (EOSJMDCKFW(1) + EOSJMDCKFW(2) *
     >  __value__(T) + EOSJMDCKFW(3) * __value__(T2) + EOSJMDCKFW(4) *
     >  __value__(T3) + EOSJMDCKFW(5) * __value__(T4))
          OpenAD_aux_1 = (EOSJMDCKSW(1) + EOSJMDCKSW(2) * __value__(T)
     >  + EOSJMDCKSW(3) * __value__(T2) + EOSJMDCKSW(4) * __value__(T3)
     > )
          OpenAD_aux_2 = (EOSJMDCKSW(5) + EOSJMDCKSW(6) * __value__(T)
     >  + EOSJMDCKSW(7) * __value__(T2))
          OpenAD_lin_16 = OpenAD_aux_1
          OpenAD_lin_18 = EOSJMDCKSW(2)
          OpenAD_lin_19 = EOSJMDCKSW(3)
          OpenAD_lin_20 = EOSJMDCKSW(4)
          OpenAD_lin_17 = __value__(S)
          OpenAD_lin_21 = OpenAD_aux_2
          OpenAD_lin_23 = EOSJMDCKSW(6)
          OpenAD_lin_24 = EOSJMDCKSW(7)
          OpenAD_lin_22 = __value__(S3O2)
          __value__(BMSALT) = (__value__(S) * OpenAD_aux_1 + __value__(
     > S3O2) * OpenAD_aux_2)
          OpenAD_aux_3 = (EOSJMDCKP(1) + EOSJMDCKP(2) * __value__(T) +
     >  EOSJMDCKP(3) * __value__(T2) + EOSJMDCKP(4) * __value__(T3))
          OpenAD_aux_4 = (__value__(P) * __value__(S))
          OpenAD_aux_5 = (EOSJMDCKP(5) + EOSJMDCKP(6) * __value__(T) +
     >  EOSJMDCKP(7) * __value__(T2))
          OpenAD_aux_6 = (__value__(P) * __value__(S3O2))
          OpenAD_aux_7 = (EOSJMDCKP(9) + EOSJMDCKP(10) * __value__(T) +
     >  EOSJMDCKP(11) * __value__(T2))
          OpenAD_aux_8 = (__value__(P2) * __value__(S))
          OpenAD_aux_9 = (EOSJMDCKP(12) + EOSJMDCKP(13) * __value__(T)
     >  + EOSJMDCKP(14) * __value__(T2))
          OpenAD_lin_25 = OpenAD_aux_3
          OpenAD_lin_27 = EOSJMDCKP(2)
          OpenAD_lin_28 = EOSJMDCKP(3)
          OpenAD_lin_29 = EOSJMDCKP(4)
          OpenAD_lin_26 = __value__(P)
          OpenAD_lin_32 = __value__(S)
          OpenAD_lin_33 = __value__(P)
          OpenAD_lin_30 = OpenAD_aux_5
          OpenAD_lin_34 = EOSJMDCKP(6)
          OpenAD_lin_35 = EOSJMDCKP(7)
          OpenAD_lin_31 = OpenAD_aux_4
          OpenAD_lin_37 = __value__(S3O2)
          OpenAD_lin_38 = __value__(P)
          OpenAD_lin_36 = EOSJMDCKP(8)
          OpenAD_lin_39 = OpenAD_aux_7
          OpenAD_lin_41 = EOSJMDCKP(10)
          OpenAD_lin_42 = EOSJMDCKP(11)
          OpenAD_lin_40 = __value__(P2)
          OpenAD_lin_45 = __value__(S)
          OpenAD_lin_46 = __value__(P2)
          OpenAD_lin_43 = OpenAD_aux_9
          OpenAD_lin_47 = EOSJMDCKP(13)
          OpenAD_lin_48 = EOSJMDCKP(14)
          OpenAD_lin_44 = OpenAD_aux_8
          __value__(BMPRES) = (__value__(P) * OpenAD_aux_3 +
     >  OpenAD_aux_4 * OpenAD_aux_5 + EOSJMDCKP(8) * OpenAD_aux_6 +
     >  __value__(P2) * OpenAD_aux_7 + OpenAD_aux_8 * OpenAD_aux_9)
          __value__(BULKMOD(INT(I), INT(J))) = (__value__(BMPRES) +
     >  __value__(BMFRESH) + __value__(BMSALT))
          OpenAD_acc_4 = (OpenAD_lin_46 * OpenAD_lin_43)
          OpenAD_acc_5 = (OpenAD_lin_33 * OpenAD_lin_30)
          OpenAD_acc_6 = (OpenAD_lin_29 * OpenAD_lin_26)
          OpenAD_acc_7 = (OpenAD_lin_20 * OpenAD_lin_17)
          OpenAD_acc_8 = (OpenAD_lin_48 * OpenAD_lin_44)
          OpenAD_acc_9 = (OpenAD_lin_42 * OpenAD_lin_40)
          OpenAD_acc_10 = (OpenAD_lin_35 * OpenAD_lin_31)
          OpenAD_acc_11 = (OpenAD_lin_28 * OpenAD_lin_26)
          OpenAD_acc_12 = (OpenAD_lin_24 * OpenAD_lin_22)
          OpenAD_acc_13 = (OpenAD_lin_19 * OpenAD_lin_17)
          OpenAD_acc_14 = (OpenAD_lin_47 * OpenAD_lin_44)
          OpenAD_acc_15 = (OpenAD_lin_41 * OpenAD_lin_40)
          OpenAD_acc_16 = (OpenAD_lin_38 * OpenAD_lin_36)
          OpenAD_acc_17 = (OpenAD_lin_34 * OpenAD_lin_31)
          OpenAD_acc_18 = (OpenAD_lin_27 * OpenAD_lin_26)
          OpenAD_acc_19 = (OpenAD_lin_23 * OpenAD_lin_22)
          OpenAD_acc_20 = (OpenAD_lin_18 * OpenAD_lin_17)
          OpenAD_acc_21 = (OpenAD_lin_10 + OpenAD_lin_11)
          OpenAD_acc_22 = (OpenAD_lin_9 * OpenAD_lin_25 + OpenAD_lin_9
     >  * OpenAD_acc_21 * OpenAD_lin_45 * OpenAD_lin_43 + OpenAD_lin_9
     >  * OpenAD_acc_21 * OpenAD_lin_39 + OpenAD_lin_9 * OpenAD_lin_32
     >  * OpenAD_lin_30 + OpenAD_lin_9 * OpenAD_lin_37 * OpenAD_lin_36)
          CALL sax(OpenAD_acc_22, __deriv__(LOCPRES(I, J)), __deriv__(
     > BULKMOD(I, J)))
          CALL saxpy(OpenAD_lin_16, __deriv__(S), __deriv__(BULKMOD(I,
     >  J)))
          CALL saxpy(OpenAD_acc_4, __deriv__(S), __deriv__(BULKMOD(I, J
     > )))
          CALL saxpy(OpenAD_acc_5, __deriv__(S), __deriv__(BULKMOD(I, J
     > )))
          CALL saxpy(OpenAD_acc_6, __deriv__(T3), __deriv__(BULKMOD(I,
     >  J)))
          CALL saxpy(OpenAD_lin_21, __deriv__(S3O2), __deriv__(BULKMOD(
     > I, J)))
          CALL saxpy(OpenAD_acc_7, __deriv__(T3), __deriv__(BULKMOD(I,
     >  J)))
          CALL saxpy(OpenAD_acc_8, __deriv__(T2), __deriv__(BULKMOD(I,
     >  J)))
          CALL saxpy(OpenAD_acc_9, __deriv__(T2), __deriv__(BULKMOD(I,
     >  J)))
          CALL saxpy(OpenAD_acc_10, __deriv__(T2), __deriv__(BULKMOD(I,
     >  J)))
          CALL saxpy(OpenAD_acc_11, __deriv__(T2), __deriv__(BULKMOD(I,
     >  J)))
          CALL saxpy(OpenAD_acc_12, __deriv__(T2), __deriv__(BULKMOD(I,
     >  J)))
          CALL saxpy(OpenAD_acc_13, __deriv__(T2), __deriv__(BULKMOD(I,
     >  J)))
          CALL saxpy(OpenAD_acc_14, __deriv__(T), __deriv__(BULKMOD(I,
     >  J)))
          CALL saxpy(OpenAD_acc_15, __deriv__(T), __deriv__(BULKMOD(I,
     >  J)))
          CALL saxpy(OpenAD_acc_16, __deriv__(S3O2), __deriv__(BULKMOD(
     > I, J)))
          CALL saxpy(OpenAD_acc_17, __deriv__(T), __deriv__(BULKMOD(I,
     >  J)))
          CALL saxpy(OpenAD_acc_18, __deriv__(T), __deriv__(BULKMOD(I,
     >  J)))
          CALL saxpy(OpenAD_acc_19, __deriv__(T), __deriv__(BULKMOD(I,
     >  J)))
          CALL saxpy(OpenAD_acc_20, __deriv__(T), __deriv__(BULKMOD(I,
     >  J)))
          CALL saxpy(OpenAD_lin_13, __deriv__(T2), __deriv__(BULKMOD(I,
     >  J)))
          CALL saxpy(OpenAD_lin_14, __deriv__(T3), __deriv__(BULKMOD(I,
     >  J)))
          CALL saxpy(OpenAD_lin_15, __deriv__(T4), __deriv__(BULKMOD(I,
     >  J)))
          CALL saxpy(OpenAD_lin_12, __deriv__(T), __deriv__(BULKMOD(I,
     >  J)))
        END DO
      END DO
      END SUBROUTINE

      SUBROUTINE head(X, Y)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X(1 : 32)
      TYPE (OpenADTy_active) Y(1 : 4)
C
C     **** Local Variables and Functions ****
C
      TYPE (OpenADTy_active) BULKMOD(1 : 2, 1 : 2)
      EXTERNAL find_bulkmod
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) J
      INTEGER(w2f__i4) K
      INTEGER(w2f__i4) L
      TYPE (OpenADTy_active) LOCPRES(1 : 2, 1 : 2)
      INTEGER(w2f__i4) M
      TYPE (OpenADTy_active) SFLD(1 : 2, 1 : 2, 1 : 2, 1 : 2, 1 : 2)
      TYPE (OpenADTy_active) TFLD(1 : 2, 1 : 2, 1 : 2, 1 : 2, 1 : 2)
      INTEGER(w2f__i4) XCOUNT
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      XCOUNT = 1
      DO I = 1, 2, 1
        DO J = 1, 2, 1
          __value__(LOCPRES(INT(I), INT(J))) = (__value__(X(XCOUNT)) *
     >  2.0D00)
          CALL sax(2.0D00, __deriv__(X(XCOUNT)), __deriv__(LOCPRES(I, J
     > )))
          DO K = 1, 2, 1
            DO L = 1, 2, 1
              DO M = 1, 2, 1
                __value__(TFLD(INT(I), INT(J), INT(K), INT(L), INT(M)))
     >  = (__value__(X(XCOUNT)) * 3.0D00)
                __value__(SFLD(INT(I), INT(J), INT(K), INT(L), INT(M)))
     >  = (__value__(X(XCOUNT)) * 4.0D00)
                CALL sax(3.0D00, __deriv__(X(XCOUNT)), __deriv__(TFLD(I
     > , J, K, L, M)))
                CALL sax(4.0D00, __deriv__(X(XCOUNT)), __deriv__(SFLD(I
     > , J, K, L, M)))
                XCOUNT = (XCOUNT + 1)
              END DO
            END DO
          END DO
        END DO
      END DO
      CALL find_bulkmod(__deriv__(LOCPRES), __deriv__(TFLD), __deriv__(
     > SFLD), __deriv__(BULKMOD))
      DO I = 1, 2, 1
        DO J = 1, 2, 1
          __value__(Y(J + I * 2 + (-2))) = __value__(BULKMOD(I, J))
          CALL setderiv(__deriv__(Y(J + I * 2 + (-2))), __deriv__(
     > BULKMOD(I, J)))
        END DO
      END DO
      END SUBROUTINE
