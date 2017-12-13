
      MODULE active_module
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Top Level Pragmas ****
C
      interface  AMETHOD
        module procedure  AMETHODIMPL

      end interface 
      
      interface  ASECONDMETHOD
        module procedure  ASECONDMETHODIMPL

      end interface 
      
      interface  ATHIRDMETHOD
        module procedure  ATHIRDMETHODIMPL

      end interface 
      
C
C     **** Statements ****
C
C$OPENAD XXX Afirstpragma
C$OPENAD XXX Asecondpragma
      CONTAINS

        SUBROUTINE AMETHODIMPL(I)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        INTEGER(w2f__i4) I
C
C       **** Statements ****
C
        WRITE(*, *) 'aMethodImpl', I
        END SUBROUTINE

        SUBROUTINE ASECONDMETHODIMPL(I)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        INTEGER(w2f__i4) I
C
C       **** Statements ****
C
        WRITE(*, *) 'aSecondMethodImpl', I
        END SUBROUTINE

        SUBROUTINE ATHIRDMETHODIMPL(I)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        INTEGER(w2f__i4) I
C
C       **** Statements ****
C
C$OPENAD XXX Asthirdpragma
        WRITE(*, *) 'aThirdMethodImpl', I
        END SUBROUTINE
      END

      PROGRAM test
      use w2f__types
      use active_module
      IMPLICIT NONE
C
C     **** Statements ****
C
      CALL AMETHODIMPL(1)
      CALL ASECONDMETHODIMPL(2)
      CALL ATHIRDMETHODIMPL(3)
      
      END PROGRAM
