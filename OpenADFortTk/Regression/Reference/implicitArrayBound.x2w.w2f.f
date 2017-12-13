
      PROGRAM implicitarraybound
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) A(:)
      ALLOCATABLE A
      REAL(w2f__8) B(1 : 3)
C
C     **** Statements ****
C
      B(1) = 1.0D00
      B(2) = 2.0D00
      ALLOCATE(A(3))
      A( : 2) = B(1 : 2)
      WRITE(*, *) 'a(1) = ', A(1), 'a(2) = ', A(2)
      
      END PROGRAM
