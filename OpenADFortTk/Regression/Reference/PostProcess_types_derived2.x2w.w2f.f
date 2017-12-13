
      MODULE coord2d
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
        CHARACTER(20) MSG
      END TYPE
      
      TYPE POLAR
        type(active) :: R
        type(active) :: THETA
        CHARACTER(20) MSG
      END TYPE

C
C     **** Statements ****
C
      END MODULE

      SUBROUTINE cart2polar(C, P)
      use OAD_active
      use w2f__types
      use coord2d
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(CART) :: C
      INTENT(IN) C
      type(POLAR) :: P
      INTENT(OUT) P
C
C     **** Statements ****
C
      P%R%v = ((C%X%v**2)+(C%Y%v**2))
      IF (P%R%v.eq.INT(0_w2f__i8)) THEN
        P%THETA%v = 0
      ELSE
        P%THETA%v = SIN(C%Y%v/P%R%v)
      ENDIF
      P%MSG = 'because'
      END SUBROUTINE

      PROGRAM dosomething
      use OAD_active
      use w2f__types
      use coord2d
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      type(CART) :: C
      EXTERNAL cart2polar
      REAL(w2f__8) CX
      SAVE CX
      type(POLAR) :: P
      type(CART) :: t__1
C
C     **** Initializers ****
C
      DATA CX / 0.0D00 /
C
C     **** Statements ****
C
      t__1%X%v = CX
      t__1%Y%v = 9.0D00
      t__1%MSG = 'why?'
      C = t__1
      CALL cart2polar(C,P)
      WRITE(*,'(A,2(F,A),2A)') 'cart c = (',C%X%v,',',C%Y%v,',',C%MSG,')
     +'
      WRITE(*,*) 'polar p = (',P%R%v,',',P%THETA%v,',',P%MSG,')'
      
      END PROGRAM
