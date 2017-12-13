
      PROGRAM equiv
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      
      EQUIVALENCE(B, A)
      
      REAL(w2f__4) A
      SAVE A
      REAL(w2f__4) B
      SAVE B
      
      EQUIVALENCE(GJW, GJC)
      EQUIVALENCE(GJW, GJP)
      
      REAL(w2f__4) GJC(1 : 36)
      SAVE GJC
      REAL(w2f__4) GJP(1 : 6, 1 : 6)
      SAVE GJP
      REAL(w2f__4) GJW(1 : 6, 1 : 6)
      SAVE GJW
C
C     **** Temporary Variables ****
C
      INTEGER(w2f__i8) tmp0
C
C     **** Initializers ****
C
      DATA A / 1.0 /
      DATA(GJC(tmp0), tmp0 = 1, 36, 1) / 0.0, 5.5555558205E-01,
     >  8.739387989E-01, 9.5491147041E-01, 9.8046720028E-01,
     >  9.902908206E-01, 2.0, 0.0, 2.8606122732E-01, 6.5127015114E-01,
     >  8.26603055E-01, 9.0725797415E-01, 7.2783446312E-01,
     >  1.2721655369, 0.0, 1.6932809353E-01, 4.7704395652E-01,
     >  6.8412768841E-01, 2.7930793166E-01, 9.1696441174E-01,
     >  8.0372768641E-01, 0.0, 1.1094751209E-01, 3.5681754351E-01,
     >  1.2472388148E-01, 5.1939016581E-01, 8.1385827065E-01,
     >  5.4202765226E-01, 0.0, 7.8034900129E-02, 6.2991656363E-02,
     >  2.9563549161E-01, 5.8554792404E-01, 6.6869854927E-01,
     >  3.8712635636E-01, 0.0 /
C
C     **** Statements ****
C
      IF(GJP(3, 3) .eq. GJW(3, 3)) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) 'FAILED'
      ENDIF
      
      END PROGRAM
