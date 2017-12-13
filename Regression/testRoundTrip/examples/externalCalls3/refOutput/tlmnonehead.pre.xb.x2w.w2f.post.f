
C$OPENAD XXX File_start [head.f]
      SUBROUTINE head(X, Y)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      REAL(w2f__8) X
      REAL(w2f__8) Y
C
C     **** Local Variables and Functions ****
C
      EXTERNAL barext
      REAL(w2f__8) T1
      REAL(w2f__8) T2
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      T1 = X
      CALL barext(X,T2)
      Y = T2
      END SUBROUTINE
