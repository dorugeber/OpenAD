
      PROGRAM writeformat
      use w2f__types
      use OAD_active
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) X
      REAL(w2f__8) Y
C
C     **** Statements ****
C
      X = 0.0D00
      Y = 1.0D00
      WRITE(*,'(1x," ***Outer Convergence of Thermal Flux (MG/P'//'W) Wa
     +s",                   t55,2es12.5)') X,Y
      
      END PROGRAM
