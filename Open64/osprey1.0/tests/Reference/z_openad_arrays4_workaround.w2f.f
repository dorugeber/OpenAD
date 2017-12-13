
      MODULE m1
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      INTEGER(w2f__i4) M1_I
C
C     **** Statements ****
C
      END MODULE

      MODULE m2
      use w2f__types
      use m1
      IMPLICIT NONE
      SAVE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i8) t__1
      INTEGER(w2f__i8) t__3
C
C     **** Statements ****
C
      CONTAINS

        SUBROUTINE BAR(BAR_ARG)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        INTEGER(w2f__i4) BAR_ARG(1 : M1_I)
C
C       **** Local Variables and Functions ****
C
        EXTERNAL FOO
C
C       **** Statements ****
C
        IF(BAR_ARG(2) .eq. 4) THEN
C         if (bar_arg(2)==4) print *, 'OK'
          WRITE(*, *) 'OK'
        ENDIF
        END SUBROUTINE

        SUBROUTINE FOO(FOO_ARG)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        INTEGER(w2f__i4) FOO_ARG(1 : M1_I)
C
C       **** Local Variables and Functions ****
C
        INTEGER(w2f__i4) FOO_LOCAL(1 : M1_I)
C
C       **** Statements ****
C
        FOO_LOCAL(1 : M1_I) = FOO_ARG(1 : M1_I)
        CALL BAR(FOO_LOCAL)
        END SUBROUTINE
      END

      PROGRAM arrays4
      use w2f__types
      use m1
      use m2
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) PROGRAM_LOCAL(1 : 2)
      SAVE PROGRAM_LOCAL
C
C     **** Temporary Variables ****
C
      INTEGER(w2f__i8) tmp0
C
C     **** Initializers ****
C
      DATA(PROGRAM_LOCAL(tmp0), tmp0 = 1, 2, 1) / 3, 4 /
C
C     **** Statements ****
C
      M1_I = 2
      CALL FOO(PROGRAM_LOCAL)
      
      END PROGRAM
