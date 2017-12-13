
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
C     **** Top Level Pragmas ****
C
      interface assignment (=)
        module procedure CART_ASSIGN_CART

      end interface

C
C     **** Statements ****
C
      CONTAINS

        SUBROUTINE CART_ASSIGN_CART(A, B)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        type(CART) :: A
        INTENT(OUT) A
        type(CART) :: B
        INTENT(IN) B
C
C       **** Statements ****
C
        A%X%v = B%X%v
        A%Y%v = B%Y%v
        END SUBROUTINE
      END

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
      type(CART) :: C1
      type(CART) :: C2
      REAL(w2f__4) D
      EXTERNAL distancefromorigin
      REAL(w2f__4) N
      SAVE N
      type(CART) :: t__1
C
C     **** Initializers ****
C
      DATA N / 7.0 /
C
C     **** Statements ****
C
      t__1%X%v = N
      t__1%Y%v = N
      CALL CART_ASSIGN_CART(C2,t__1)
      CALL CART_ASSIGN_CART(C1,C2)
      CALL distancefromorigin(C1,D)
      WRITE(*,*) D
      
      END PROGRAM
