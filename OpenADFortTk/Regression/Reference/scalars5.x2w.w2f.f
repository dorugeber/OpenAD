
      SUBROUTINE d_head(X, D_X, Y, D_Y)
      use w2f__types
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      REAL(w2f__8) X
      INTENT(IN)  X
      REAL(w2f__8) D_X
      INTENT(IN)  D_X
      REAL(w2f__8) Y
      REAL(w2f__8) D_Y
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      REAL(w2f__8) T1
      REAL(w2f__8) T2
      REAL(w2f__8) XB_AUX_1
      REAL(w2f__8) XB_AUX_2
      REAL(w2f__8) XB_AUX_3
      REAL(w2f__8) XB_AUX_4
      REAL(w2f__8) XB_AUX_5
C
C     **** Statements ****
C
      I = 1
      XB_AUX_1 = SIN(X)
      XB_AUX_2 = COS(X)
      T1 = (Y * XB_AUX_1)
      T2 = (I + X * T1)
      Y = (T1 + T2)
      XB_AUX_3 = (X + 1.0D00)
      XB_AUX_4 = (T1 + XB_AUX_3 * Y * XB_AUX_2)
      XB_AUX_5 = (XB_AUX_1 * XB_AUX_3)
      D_Y = (D_X * XB_AUX_4 + D_Y * XB_AUX_5)
      END SUBROUTINE

      PROGRAM deriv
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      EXTERNAL d_head
      REAL(w2f__8) D_X
      REAL(w2f__8) D_Y
      REAL(w2f__8) X
      REAL(w2f__8) Y
C
C     **** Statements ****
C
      X = 4.5D+01
      D_X = 9.0D00
      Y = 6.0D+01
      D_Y = 1.3D+01
      CALL d_head(X, D_X, Y, D_Y)
      WRITE(*, *) Y
      WRITE(*, *) D_Y
      
      END PROGRAM
