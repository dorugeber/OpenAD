
      MODULE assign_pointer_class
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      TYPE  MYTYPE
        INTEGER(w2f__i4) ,POINTER:: PR
      END TYPE
      
C
C     **** Statements ****
C
      interface  assignment (=)
        module procedure  ASSIGN_POINTER

      end interface 
      
      CONTAINS

        SUBROUTINE ASSIGN_POINTER(THIS, A)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        TYPE (MYTYPE) THIS
        INTENT(OUT)  THIS
        INTEGER(w2f__i4) A
        TARGET A
        INTENT(IN)  A
C
C       **** Statements ****
C
        THIS%PR => A
        END SUBROUTINE

        Function GET_VAL(THIS)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        TYPE (MYTYPE) THIS
        INTENT(OUT)  THIS
        INTEGER(w2f__i4) GET_VAL
C
C       **** Statements ****
C
        GET_VAL = INT(THIS%PR)
        RETURN
        END FUNCTION
      END

      PROGRAM main
      use w2f__types
      use assign_pointer_class
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) A
      TARGET A
      SAVE A
      TYPE (MYTYPE) X
C
C     **** Initializers ****
C
      DATA A / 1 /
C
C     **** Statements ****
C
      CALL ASSIGN_POINTER(X, A)
      A = A + GET_VAL(X)
      IF(A .eq. 2) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) 'failed'
      ENDIF
      
      END PROGRAM
