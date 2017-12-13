
      MODULE moda
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Statements ****
C
      interface  FOO_I
        SUBROUTINE foo(SIZEA, A)
        use w2f__types
        INTEGER(w2f__i4) SIZEA
        INTEGER(w2f__i4) A(1 : 1)
        END SUBROUTINE

      end interface 
      
      END MODULE

      SUBROUTINE foo(SIZEADEF, ADEF)
      use w2f__types
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      INTEGER(w2f__i4) SIZEADEF
      INTEGER(w2f__i4) ADEF(1 : INT((SIZEADEF + 1)))
C
C     **** Statements ****
C
      ADEF(1) = (ADEF(1) + 1)
      END SUBROUTINE

      PROGRAM p
      use w2f__types
      use moda
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) A(1 : 2)
      INTEGER(w2f__i4) N
      PARAMETER ( N = 1)
C
C     **** Statements ****
C
      A(1) = 1
      A(2) = 2
      CALL foo((1), A)
      IF((A(1) .eq. 2) .AND.(A(2) .eq. 2)) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) 'failure'
      ENDIF
      
      END PROGRAM
