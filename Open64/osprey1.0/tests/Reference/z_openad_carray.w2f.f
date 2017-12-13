
      MODULE carray
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Statements ****
C
      interface  COUNTARRAY
        module procedure  CN
        module procedure  C2
        module procedure  C1

      end interface 
      
      CONTAINS

        SUBROUTINE C1(F, CS)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        REAL(w2f__4) F(1 :)
        INTENT(IN)  F
        INTEGER(w2f__i4) CS
        INTENT(OUT)  CS
C
C       **** Statements ****
C
        CS = SIZE(F)
        END SUBROUTINE

        SUBROUTINE C2(F, CS)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        REAL(w2f__4) F(1 :, 1 :)
        INTENT(IN)  F
        INTEGER(w2f__i4) CS
        INTENT(OUT)  CS
C
C       **** Statements ****
C
        CS = SIZE(F)
        END SUBROUTINE

        SUBROUTINE CN(F, CS)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        REAL(w2f__4) F
        INTENT(IN)  F
        INTEGER(w2f__i4) CS
        INTENT(OUT)  CS
C
C       **** Statements ****
C
        CS = 0
        END SUBROUTINE
      END

      PROGRAM mc
      use w2f__types
      use carray
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__4) G0
      REAL(w2f__4) G1(1 : 3)
      REAL(w2f__4) G2(1 : 2, 1 : 2)
      INTEGER(w2f__i4) S1
      INTEGER(w2f__i4) S2
      INTEGER(w2f__i4) SN
      REAL(w2f__4) t__1(1 : 3)
      SAVE t__1
      REAL(w2f__4) t__2(1 : 2, 1 : 2)
      SAVE t__2
      INTEGER(w2f__i4) t__3(1 : 2)
      SAVE t__3
C
C     **** Temporary Variables ****
C
      INTEGER(w2f__i8) tmp0
      REAL(w2f__4) tmp1(1 : 4)
C
C     **** Initializers ****
C
      EQUIVALENCE(t__2, tmp1)
      DATA(t__1(tmp0), tmp0 = 1, 3, 1) / 1.0, 2.0, 3.0 /
      DATA(tmp1(tmp0), tmp0 = 1, 4, 1) / 1.0, 2.0, 3.0, 4.0 /
      DATA(t__3(tmp0), tmp0 = 1, 2, 1) / 2, 2 /
C
C     **** Statements ****
C
      G0 = 0.0
      G1(1 : 3) = t__1(1 : 3)
      G2(1 : 2, 1 : 2) = RESHAPE(t__2, t__3)
      CALL CN(G0, SN)
      CALL C1(G1, S1)
      CALL C2(G2, S2)
      IF((S2 .eq. 4) .AND.((S1 .eq. 3) .AND.(SN .eq. 0))) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) 'failed'
      ENDIF
      
      END PROGRAM
