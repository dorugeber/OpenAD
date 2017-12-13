C ***********************************************************
C Fortran file translated from WHIRL Wed Jul  1 11:14:29 2009
C ***********************************************************
C ***********************************************************

      PROGRAM writeformat
      use w2f__types
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
C     open(80,file='writeFormat.out')
      OPEN(UNIT = 80, FILE = 'writeFormat.out')
      WRITE(80, '(1x," ***Outer Convergence of Thermal Flux (MG/P' //
     >  'W) Was",                   t55,2es12.5)') X, Y
C     close(80)
      CLOSE(UNIT = 80)
      WRITE(*, *) 'OK'
      
      END PROGRAM
