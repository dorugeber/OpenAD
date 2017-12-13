
      MODULE ad_types
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) AD_16
      PARAMETER ( AD_16 = 16)
      INTEGER(w2f__i4) AD_4
      PARAMETER ( AD_4 = 4)
      INTEGER(w2f__i4) AD_8
      PARAMETER ( AD_8 = 8)
      INTEGER(w2f__i4) AD_I1
      PARAMETER ( AD_I1 = 1)
      INTEGER(w2f__i4) AD_I2
      PARAMETER ( AD_I2 = 2)
      INTEGER(w2f__i4) AD_I4
      PARAMETER ( AD_I4 = 4)
      INTEGER(w2f__i4) AD_I8
      PARAMETER ( AD_I8 = 8)
C
C     **** Statements ****
C
      END MODULE

      SUBROUTINE compute(X, Y)
      use w2f__types
      use ad_types
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      REAL(w2f__8) X
      REAL(w2f__8) Y
C
C     **** Statements ****
C
      X = (X / Y)
      Y = (Y * 7.88999974727630615234D-01 +(-2.0D00))
      END SUBROUTINE

      PROGRAM simple3
      use w2f__types
      use ad_types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      EXTERNAL compute
      REAL(w2f__8) X
      REAL(w2f__8) Y
C
C     **** Statements ****
C
      X = 2.0D00
      Y = 8.0D00
      CALL compute(X, Y)
      
      END PROGRAM
