
      MODULE m
      use OAD_active
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      INTEGER(w2f__i4) NC
      PRIVATE NC
      INTEGER(w2f__i4) NT
      PRIVATE NT
      REAL(w2f__4) X
      PRIVATE X
      REAL(w2f__4) Y
      PRIVATE Y
C
C     **** Initializers ****
C
      DATA NC / 20 /
      DATA NT / 12 /
C
C     **** Top Level Pragmas ****
C
      interface FOO
        module procedure FOO_I

      end interface

C
C     **** Statements ****
C
      CONTAINS

        SUBROUTINE FOO_I
        use w2f__types
        IMPLICIT NONE
C
C       **** Statements ****
C
        X = 1.0
        Y = 1.0
        IF(((NC * NT) .eq. 240) .AND.((DBLE(X) * DBLE(Y)) .eq. DBLE(1.0 
     +))) THEN
          WRITE(*,*) 'OK'
        ELSE
          WRITE(*,*) (NC*NT),(X*Y)
        ENDIF
        END SUBROUTINE
      END

      PROGRAM privateq
      use OAD_active
      use w2f__types
      use m
      IMPLICIT NONE
C
C     **** Statements ****
C
      CALL FOO_I()
      
      END PROGRAM
