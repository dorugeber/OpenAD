
      MODULE m
      use OAD_active
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Statements ****
C
      CONTAINS

        SUBROUTINE FOO(FOOIN, FOOOUT)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        type(active) :: FOOIN
        INTENT(IN) FOOIN
        type(active) :: FOOOUT
        INTENT(OUT) FOOOUT
C
C       **** Statements ****
C
        FOOOUT%v = FOOIN%v
        END SUBROUTINE
      END

      PROGRAM p
      use OAD_active
      use w2f__types
      use m
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      type(active) :: PIN
      type(active) :: POUT
      type(active) :: T(1:2)
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
      X%v = 1.0D00
      T(1)%v = X%v
      DO I = 1,2,1
        POUT%v = T(2)%v
        PIN%v = T(1)%v
        CALL FOO(PIN,POUT)
        T(2)%v = POUT%v
        T(1)%v = PIN%v
      END DO
      Y%v = T(2)%v
      WRITE(*,*) Y%v
      
      END PROGRAM
