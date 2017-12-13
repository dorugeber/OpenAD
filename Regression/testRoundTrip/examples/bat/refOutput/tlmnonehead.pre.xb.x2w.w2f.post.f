
C$OPENAD XXX File_start [head.f]
      SUBROUTINE head(X, Y)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X(1:4)
      type(active) :: Y(1:4)
C
C     **** Local Variables and Functions ****
C
      type(active) :: T1
      type(active) :: T2
      type(active) :: T3
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_10
      REAL(w2f__8) OpenAD_lin_11
      REAL(w2f__8) OpenAD_lin_2
      REAL(w2f__8) OpenAD_lin_3
      REAL(w2f__8) OpenAD_lin_4
      REAL(w2f__8) OpenAD_lin_5
      REAL(w2f__8) OpenAD_lin_6
      REAL(w2f__8) OpenAD_lin_7
      REAL(w2f__8) OpenAD_lin_8
      REAL(w2f__8) OpenAD_lin_9
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      OpenAD_lin_0 = (INT(1_w2f__i8)/X(2)%v)
      OpenAD_lin_1 = (-(X(1)%v/(X(2)%v*X(2)%v)))
      T1%v = (X(1)%v/X(2)%v)
      OpenAD_lin_2 = X(4)%v
      OpenAD_lin_3 = X(3)%v
      T2%v = (X(3)%v*X(4)%v)
      OpenAD_lin_4 = T2%v
      OpenAD_lin_5 = T1%v
      T3%v = (T1%v*T2%v)
      OpenAD_lin_6 = T3%v
      OpenAD_lin_7 = T1%v
      Y(1)%v = (T1%v*T3%v)
      CALL sax(OpenAD_lin_0,X(1),T1)
      CALL saxpy(OpenAD_lin_1,X(2),T1)
      CALL sax(OpenAD_lin_2,X(3),T2)
      CALL saxpy(OpenAD_lin_3,X(4),T2)
      CALL sax(OpenAD_lin_4,T1,T3)
      CALL saxpy(OpenAD_lin_5,T2,T3)
      CALL sax(OpenAD_lin_6,T1,Y(1))
      CALL saxpy(OpenAD_lin_7,T3,Y(1))
      OpenAD_lin_8 = (-(1.0D00/(T3%v*T3%v)))
      Y(2)%v = (1.0D00/T3%v)
      CALL sax(OpenAD_lin_8,T3,Y(2))
      OpenAD_lin_9 = COS(T3%v)
      Y(3)%v = SIN(T3%v)
      CALL sax(OpenAD_lin_9,T3,Y(3))
      OpenAD_lin_10 = T3%v
      OpenAD_lin_11 = T2%v
      Y(4)%v = (T2%v*T3%v)
      CALL sax(OpenAD_lin_10,T2,Y(4))
      CALL saxpy(OpenAD_lin_11,T3,Y(4))
      END SUBROUTINE
