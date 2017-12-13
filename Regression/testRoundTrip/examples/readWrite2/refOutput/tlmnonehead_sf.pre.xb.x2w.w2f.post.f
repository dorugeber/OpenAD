
      MODULE all_globals_mod
      use w2f__types
      use OAD_active
      IMPLICIT NONE
      SAVE
C
C     **** Statements ****
C
      END MODULE

      SUBROUTINE head(X, Y)
      use w2f__types
      use OAD_active
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      REAL(w2f__8) OpenAD_Symbol_0
      REAL(w2f__8) OpenAD_Symbol_1
      REAL(w2f__8) OpenAD_Symbol_2
      REAL(w2f__8) OpenAD_Symbol_4
C
C     **** Parameters and Result ****
C
      type(active) :: X(1 : 2)
      type(active) :: Y(1 : 1)
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
C     open(3,file='data.tmp')
      OPEN(UNIT = 3, FILE = 'data.tmp')
      WRITE(3,'(EN26.16E3,EN26.16E3)') X%v
C     close(3)
      CLOSE(UNIT = 3)
C     open(3,file='data.tmp')
      OPEN(UNIT = 3, FILE = 'data.tmp')
      READ(3, '(EN26.16E3,EN26.16E3)') X
C     close(3)
      CLOSE(UNIT = 3)
      Y(1)%v = (X(1)%v*2.0D00+X(2)%v*3.0D00)
      OpenAD_Symbol_2 = 2.0D00
      OpenAD_Symbol_0 = 1_w2f__i8
      OpenAD_Symbol_4 = 3.0D00
      OpenAD_Symbol_1 = 1_w2f__i8
      CALL sax(2.0D00,X(1),Y(1))
      CALL saxpy(3.0D00,X(2),Y(1))
      END SUBROUTINE
