C ***********************************************************
C Fortran file translated from WHIRL Fri Oct 30 17:36:32 2009
C ***********************************************************
C ***********************************************************

      PROGRAM transp
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      
      EQUIVALENCE(A, t__1)
      
      INTEGER(w2f__i4) t__1(1 : 4)
      SAVE t__1
      INTEGER(w2f__i4) A(1 : 2, 1 : 2)
      SAVE A
      INTEGER(w2f__i4) B(1 : 2, 1 : 2)
C
C     **** Temporary Variables ****
C
      INTEGER(w2f__i8) tmp0
C
C     **** Initializers ****
C
      DATA(t__1(tmp0), tmp0 = 1, 4, 1) / 1, 2, 3, 4 /
C
C     **** Statements ****
C
      B(1 : 2, 1 : 2) = TRANSPOSE(A)
      IF(B(1, 2) .eq. 2) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) B, A
      ENDIF
      
      END PROGRAM
