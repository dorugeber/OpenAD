
      MODULE erro_i
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Statements ****
C
      END MODULE

      MODULE data_c
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      INTEGER(w2f__i4) A
      INTEGER(w2f__i4) B
      INTEGER(w2f__i4) C
C
C     **** Statements ****
C
      END MODULE

      PROGRAM useonly
      use w2f__types
      use erro_i
      use data_c ,only: B , C
      IMPLICIT NONE
C
C     **** Statements ****
C
      
      END PROGRAM
