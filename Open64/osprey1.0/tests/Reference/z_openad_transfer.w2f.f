
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
      INTEGER(w2f__i4) B(1 : 4)
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
      B(1 : 4) = TRANSFER(A, B)
      IF(B(3) .eq. 3) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) B, A
      ENDIF
      
      END PROGRAM
