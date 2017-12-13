
      MODULE m
      use OAD_active
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      type(active) :: M_X
C
C     **** Initializers ****
C
      DATA M_X%v /1.0D00/
C
C     **** Statements ****
C
      END MODULE

      PROGRAM p
      use OAD_active
      use w2f__types
      use m
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      type(active) :: X
      type(active) :: Y
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
      M_X%v = (X%v*M_X%v)
      Y%v = M_X%v
      
      END PROGRAM
