
C$OPENAD XXX File_start p1a p1aend
      MODULE a
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      INTEGER(w2f__i4) AI
C
C     **** Statements ****
C
C$OPENAD XXX P1end
      END MODULE

C$OPENAD XXX FILE_start p2 p2end
      MODULE b
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      INTEGER(w2f__i4) BI
C
C     **** Statements ****
C
C$OPENAD XXX P2a p2aend
      END MODULE

C$OPENAD XXX File_start  p3a p3aend
      PROGRAM p
      use w2f__types
      use a
      use b
      IMPLICIT NONE
C
C     **** Statements ****
C
C$OPENAD XXX P3 p3end
      WRITE(*, *) 'OK'
      
      END PROGRAM
