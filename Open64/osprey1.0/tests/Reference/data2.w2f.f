C ***********************************************************
C Fortran file translated from WHIRL Tue Nov 10 13:14:08 2009
C ***********************************************************
C ***********************************************************

      PROGRAM mc
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__4) G2(1 : 2, 1 : 2)
      REAL(w2f__4) t__1(1 : 2, 1 : 2)
      SAVE t__1
      INTEGER(w2f__i4) t__2(1 : 2)
      SAVE t__2
C
C     **** Temporary Variables ****
C
      INTEGER(w2f__i8) tmp0
      REAL(w2f__4) tmp1(1 : 4)
C
C     **** Initializers ****
C
      EQUIVALENCE(t__1, tmp1)
      DATA(tmp1(tmp0), tmp0 = 1, 4, 1) / 1.0, 2.0, 3.0, 4.0 /
      DATA(t__2(tmp0), tmp0 = 1, 2, 1) / 2, 2 /
C
C     **** Statements ****
C
      G2(1 : 2, 1 : 2) = RESHAPE(t__1, t__2)
      IF(G2(2, 1) .eq. 2.0) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) 'failed'
      ENDIF
      
      END PROGRAM
