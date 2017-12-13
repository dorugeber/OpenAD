
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

      PROGRAM intrinsics
      use w2f__types
      use ad_types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      CHARACTER(3) A
      SAVE A
      CHARACTER(3) B
      SAVE B
      REAL(w2f__4) X
      SAVE X
      REAL(w2f__8) YY
      SAVE YY
      REAL(w2f__8) ZZZ
      SAVE ZZZ
C
C     **** Initializers ****
C
      DATA A / 'aaa' /
      DATA B / 'bbb' /
      DATA X / 2.0 /
      DATA YY / 3.0D00 /
      DATA ZZZ / 4.0D00 /
C
C     **** Statements ****
C
      IF(A .LT. B) THEN
        ZZZ = 5.0D00
      ENDIF
      X = SQRT(DBLE(X))
      YY = SQRT(YY)
      X = SIN(DBLE(X))
      YY = SIN(YY)
      ZZZ = SIN(ZZZ)
      X = COS(DBLE(X))
      YY = COS(YY)
      ZZZ = COS(ZZZ)
      X = EXP(DBLE(X))
      YY = EXP(YY)
      ZZZ = EXP(ZZZ)
      X = LOG(DBLE(X))
      YY = LOG(YY)
      ZZZ = LOG(ZZZ)
      X = (DBLE(X) ** DBLE(X))
      YY = (YY ** YY)
      ZZZ = (ZZZ ** ZZZ)
      WRITE(*, *) X
      WRITE(*, *) YY
      WRITE(*, *) ZZZ
      
      END PROGRAM
