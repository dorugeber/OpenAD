C ***********************************************************
C Fortran file translated from WHIRL Tue Sep  9 13:38:12 2008
C ***********************************************************
C ***********************************************************

      MODULE private_complex_class
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      TYPE  PRIVATE_COMPLEX
        REAL(w2f__4) REAL
        REAL(w2f__4) IMAGINARY
      END TYPE
      
C
C     **** Statements ****
C
      interface  operator(*)
        module procedure  PC_MULT

      end interface 
      
      CONTAINS

        Function PC_MULT(A, B) result( C)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        TYPE (PRIVATE_COMPLEX) C
        TYPE (PRIVATE_COMPLEX) A
        INTENT(IN)  A
        TYPE (PRIVATE_COMPLEX) B
        INTENT(IN)  B
C
C       **** Statements ****
C
        C%REAL = (A%REAL * B%REAL - A%IMAGINARY * B%IMAGINARY)
        C%IMAGINARY = (A%REAL * B%IMAGINARY + A%IMAGINARY * B%REAL)
        RETURN
        END FUNCTION
      END

      MODULE monitor_complex_class
      use w2f__types
      use private_complex_class
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      TYPE  MONITOR_COMPLEX
        TYPE (PRIVATE_COMPLEX) PC
        CHARACTER(8) LAST_OP
      END TYPE
      
C
C     **** Statements ****
C
      interface  operator(*)
        module procedure  MC_MULT

      end interface 
      
      CONTAINS

        Function MC_MULT(A, B) result( C)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        TYPE (MONITOR_COMPLEX) C
        TYPE (MONITOR_COMPLEX) A
        INTENT(IN)  A
        TYPE (MONITOR_COMPLEX) B
        INTENT(IN)  B
C
C       **** Statements ****
C
        C%PC = PC_MULT(A%PC, B%PC)
        C%LAST_OP = 'MULTIPLY'
        RETURN
        END FUNCTION
      END

      PROGRAM main
      use w2f__types
      use monitor_complex_class
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      TYPE (PRIVATE_COMPLEX) A
      TYPE (PRIVATE_COMPLEX) B
      TYPE (PRIVATE_COMPLEX) C
      TYPE (MONITOR_COMPLEX) X
      TYPE (MONITOR_COMPLEX) Y
      TYPE (MONITOR_COMPLEX) Z
C
C     **** Statements ****
C
      A%REAL = 1.0
      A%IMAGINARY = 0.0
      B%REAL = 1.0
      B%IMAGINARY = 0.0
      X%PC%REAL = 1.0
      X%PC%IMAGINARY = 0.0
      Y%PC%REAL = 1.0
      Y%PC%IMAGINARY = 0.0
      C = PC_MULT(A, B)
      Z = MC_MULT(X, Y)
      IF((C%IMAGINARY .eq. 0.0) .AND.(Z%LAST_OP .EQ. 'MULTIPLY')) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) 'failed'
      ENDIF
      
      END PROGRAM
