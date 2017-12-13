C ***********************************************************
C Fortran file translated from WHIRL Wed Nov 17 15:24:15 2010
C ***********************************************************
C ***********************************************************

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
        INTEGER(w2f__i8) t__2
        INTEGER(w2f__i8) t__4
        EXTERNAL FOO
C
C       **** Statements ****
C
        t__2 = M1_I
        t__4 = MAX(t__2, 0_w2f__i8)
        IF(BAR_ARG(2) .eq. 4) THEN
C         if (bar_arg(2)==4) print *, 'OK'
          WRITE(*, *) 'OK'
        ENDIF
        RETURN
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
        INTEGER(w2f__i8) t__1
        INTEGER(w2f__i8) t__3
        INTEGER(w2f__i4) FOO_LOCAL(1 : M1_I)
C
C       **** Statements ****
C
        t__1 = M1_I
        t__3 = MAX(t__1, 0_w2f__i8)
        FOO_LOCAL(1 : t__1) = FOO_ARG(1 : t__1)
        CALL BAR(FOO_LOCAL)
        RETURN
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
