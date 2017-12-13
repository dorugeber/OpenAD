
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
      WHERE((C(0 : 5) .ne. 0.0))
      A(0 : 5) = (B(0 : 5) / C(0 : 5))
      END WHERE
      WHERE((.NOT.(C(0 : 5) .ne. 0.0)))
      A(0 : 5) = 0.0
      END WHERE
      IF(A(1) .eq. 0.0) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) 'failed'
      ENDIF
      
      END PROGRAM
