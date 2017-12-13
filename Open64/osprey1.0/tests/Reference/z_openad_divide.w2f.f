
      SUBROUTINE divide(MYB, MYC, MYA)
      use w2f__types
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      REAL(w2f__4) MYB(1 : 6)
      REAL(w2f__4) MYC(1 : 6)
      REAL(w2f__4) MYA(1 : 6)
C
C     **** Statements ****
C
      WHERE((MYC(1 : 6) .ne. 0.0))
      MYA(1 : 6) = (MYB(1 : 6) / MYC(1 : 6))
      END WHERE
      WHERE((.NOT.(MYC(1 : 6) .ne. 0.0)))
      MYA(1 : 6) = 0.0
      END WHERE
      END SUBROUTINE

      PROGRAM main
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__4) A(0 : 5)
      REAL(w2f__4) B(0 : 5)
      SAVE B
      REAL(w2f__4) C(0 : 5)
      SAVE C
      EXTERNAL divide
C
C     **** Temporary Variables ****
C
      INTEGER(w2f__i8) tmp0
C
C     **** Initializers ****
C
      DATA(B(tmp0), tmp0 = 0, 5, 1) / 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 /
      DATA(C(tmp0), tmp0 = 0, 5, 1) / 3.0, 0.0, 4.0, 0.0, 5.0, 6.0 /
C
C     **** Statements ****
C
      CALL divide(B, C, A)
      IF(A(1) .eq. 0.0) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) 'failed'
      ENDIF
      
      END PROGRAM
