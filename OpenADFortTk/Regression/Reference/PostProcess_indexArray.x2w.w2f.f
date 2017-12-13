
      PROGRAM indexarray
      use OAD_active
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I(1 : 2)
      type(active) :: L(1:2)
      type(active) :: R(1:3)
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(R)
C$OPENAD DEPENDENT(L)
C
C     **** Statements ****
C
      R(1)%v = 1.10000000000000008882D00
      R(2)%v = 1.19999999999999995559D00
      R(3)%v = 1.30000000000000004441D00
      I(1) = 1
      I(2) = 3
      L(1:2)%v = DBLE(R(I(1:2))%v)
      WRITE(*,*) L%v
      
      END PROGRAM
