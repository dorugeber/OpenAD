
      SUBROUTINE test()
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      CHARACTER(1) CHAR_TEST
      SAVE CHAR_TEST
      REAL(w2f__4) NUM1_TEST
      SAVE NUM1_TEST
      INTEGER(w2f__i4) NUM2_TEST
      SAVE NUM2_TEST
C
C     **** Initializers ****
C
      DATA CHAR_TEST / 'T' /
      DATA NUM1_TEST / 6.0 /
      DATA NUM2_TEST / 11 /
C
C     **** Statements ****
C
      END SUBROUTINE

      PROGRAM myprog
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      CHARACTER(1) CHAR
      REAL(w2f__4) NUM1
      INTEGER(w2f__i4) NUM2
      EXTERNAL test
C
C     **** Statements ****
C
      CHAR = 'S'
      NUM1 = 5.0
      NUM2 = 10
      CALL test()
      
      END PROGRAM
