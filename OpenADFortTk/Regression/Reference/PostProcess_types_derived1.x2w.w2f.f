
      MODULE coordrec
      use OAD_active
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      TYPE CART
        type(active) :: X
        type(active) :: Y
      END TYPE

C
C     **** Statements ****
C
      END MODULE

      SUBROUTINE distancefromorigin(C, R)
      use OAD_active
      use w2f__types
      use coordrec
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(CART) :: C
      INTENT(IN) C
      REAL(w2f__4) R
C
C     **** Statements ****
C
      R = SQRT((DBLE(C%X%v)**2)+(DBLE(C%Y%v)**2))
      END SUBROUTINE

      PROGRAM dosomething
      use OAD_active
      use w2f__types
      use coordrec
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      type(CART) :: C
      REAL(w2f__4) D
      EXTERNAL distancefromorigin
      INTEGER(w2f__i4) N
      SAVE N
      type(CART) :: t__1
C
C     **** Initializers ****
C
      DATA N / 7 /
C
C     **** Statements ****
C
      t__1%X%v = N
      t__1%Y%v = N
      C = t__1
      CALL distancefromorigin(C,D)
      WRITE(*,*) D
      
      END PROGRAM
