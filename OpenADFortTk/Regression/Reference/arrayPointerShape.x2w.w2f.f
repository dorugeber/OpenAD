
      PROGRAM arraypointershape
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__4) BTN(:, :)
      POINTER BTN
C
C     **** Statements ****
C
      ALLOCATE(BTN(1, 1))
      BTN(1, 1) = 5.5
      WRITE(*, *) BTN(1, 1)
      WRITE(*, *) 'OK'
      
      END PROGRAM
