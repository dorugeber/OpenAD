
      MODULE bla
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Statements ****
C
      END MODULE

      SUBROUTINE head(X, Y)
      use w2f__types
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      INTEGER(w2f__i4) X(1 :, 1 :)
      INTEGER(w2f__i4) Y(1 : 10)
C
C     **** Statements ****
C
      Y(1 : 10) = RESHAPE(X, SHAPE(Y))
      END SUBROUTINE

      PROGRAM main
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) X(1 : 2, 1 : 5)
      INTEGER(w2f__i4) Y(1 : 10)
C
C     **** Statements ****
C
      interface 
        SUBROUTINE head(X, Y)
        use w2f__types
        INTEGER(w2f__i4) X(1 :, 1 :)
        INTEGER(w2f__i4) Y(1 : 10)
        END SUBROUTINE

      end interface 
      
      X(2, 3) = 6
      CALL head(X, Y)
      IF(Y(6) .eq. 6) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) 'failed; yields', Y(6), ' expected 6'
      ENDIF
      
      END PROGRAM
