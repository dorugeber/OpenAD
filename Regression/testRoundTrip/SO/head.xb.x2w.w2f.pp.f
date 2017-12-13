C ***********************************************************
C Fortran file translated from WHIRL Thu Sep  8 15:05:50 2005
C ***********************************************************
C ***********************************************************

      SUBROUTINE head(X, Y)
      use w2f__types
      use active_module
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      REAL(w2f__8) OpenAD1_Symbol_0
      REAL(w2f__8) OpenAD1_Symbol_1
      REAL(w2f__8) OpenAD1_Symbol_10
      REAL(w2f__8) OpenAD1_Symbol_11
      REAL(w2f__8) OpenAD1_Symbol_12
      type(active) :: OpenAD1_Symbol_13
      REAL(w2f__8) OpenAD1_Symbol_14
      type(active) :: OpenAD1_Symbol_15
      REAL(w2f__8) OpenAD1_Symbol_16
      type(active) :: OpenAD1_Symbol_17
      REAL(w2f__8) OpenAD1_Symbol_18
      type(active) :: OpenAD1_Symbol_19
      REAL(w2f__8) OpenAD1_Symbol_2
      REAL(w2f__8) OpenAD1_Symbol_3
      REAL(w2f__8) OpenAD1_Symbol_4
      REAL(w2f__8) OpenAD1_Symbol_5
      REAL(w2f__8) OpenAD1_Symbol_6
      REAL(w2f__8) OpenAD1_Symbol_7
      REAL(w2f__8) OpenAD1_Symbol_8
      REAL(w2f__8) OpenAD1_Symbol_9
C
C     **** Parameters and Result ****
C
      type(active) :: X(1 : 2)
      INTENT(IN)  X
      type(active) :: Y
      INTENT(OUT)  Y
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      OpenAD1_Symbol_5 = SQRT(X(2)%v)
      OpenAD1_Symbol_9 = (SIN(X(1)%v)+OpenAD1_Symbol_5+X(1)%v*X(2)%v)
      OpenAD1_Symbol_4 = COS(X(1)%v)
      OpenAD1_Symbol_2 = 1_w2f__i8
      OpenAD1_Symbol_6 = (5.0D-01 / OpenAD1_Symbol_5)
      OpenAD1_Symbol_3 = 1_w2f__i8
      OpenAD1_Symbol_0 = 1_w2f__i8
      OpenAD1_Symbol_7 = X(2)%v
      OpenAD1_Symbol_8 = X(1)%v
      OpenAD1_Symbol_1 = 1_w2f__i8
      Y%v = OpenAD1_Symbol_9
      OpenAD1_Symbol_10 = (OpenAD1_Symbol_4 * OpenAD1_Symbol_2)
      OpenAD1_Symbol_11 = (OpenAD1_Symbol_6 * OpenAD1_Symbol_3)
      OpenAD1_Symbol_12 = (OpenAD1_Symbol_7 * OpenAD1_Symbol_1)
      OpenAD1_Symbol_14 = (OpenAD1_Symbol_8 * OpenAD1_Symbol_1)
      OpenAD1_Symbol_16 = (OpenAD1_Symbol_10 * OpenAD1_Symbol_0)
      OpenAD1_Symbol_18 = (OpenAD1_Symbol_11 * OpenAD1_Symbol_0)
      CALL setderiv(OpenAD1_Symbol_19,X(2))
      CALL setderiv(OpenAD1_Symbol_17,X(1))
      CALL setderiv(OpenAD1_Symbol_15,X(2))
      CALL setderiv(OpenAD1_Symbol_13,X(1))
      CALL sax(OpenAD1_Symbol_12,OpenAD1_Symbol_13,Y)
      CALL saxpy(OpenAD1_Symbol_14,OpenAD1_Symbol_15,Y)
      CALL saxpy(OpenAD1_Symbol_16,OpenAD1_Symbol_17,Y)
      CALL saxpy(OpenAD1_Symbol_18,OpenAD1_Symbol_19,Y)
      RETURN
      END SUBROUTINE
