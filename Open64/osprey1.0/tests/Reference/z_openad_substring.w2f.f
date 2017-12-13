
      PROGRAM chararrslice
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      CHARACTER(10) S
C
C     **** Statements ****
C
      S = 'blabla'
      IF((S(2_w2f__i8 : 1_w2f__i8 + 1) .EQ. 'l') .AND.(S(2_w2f__i8 :
     >  1_w2f__i8 + 3) .EQ. 'lab')) THEN
        WRITE(*, *) 'OK'
      ENDIF
      
      END PROGRAM
