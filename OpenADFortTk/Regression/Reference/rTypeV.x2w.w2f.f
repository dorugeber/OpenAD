
      PROGRAM rtype
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      
      EQUIVALENCE(A, t__1)
      
      REAL(w2f__8) t__1(1 : 4)
      SAVE t__1
      REAL(w2f__8) A(1 : 2, 1 : 2)
      SAVE A
C
C     **** Temporary Variables ****
C
      INTEGER(w2f__i8) tmp0
C
C     **** Initializers ****
C
      DATA(t__1(tmp0), tmp0 = 1, 4, 1) / 1.0D00, 2.0D00, 3.0D00, 4.0D00
     >  /
C
C     **** Statements ****
C
      A(1 : 2, 1 : 2) = TRANSPOSE(A)
      WRITE(*, *) A
      
      END PROGRAM
