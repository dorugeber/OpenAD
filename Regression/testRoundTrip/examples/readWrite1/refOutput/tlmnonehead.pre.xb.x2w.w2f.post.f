
C$OPENAD XXX File_start [head.f]
      SUBROUTINE head(X, Y)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X(1:1)
      type(active) :: Y(1:1)
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
      WRITE(3,'(EN26.16E3)') X(1)%v
C     close(3)
      close(unit=3)
C     open(3,file='data.tmp')
      open(unit=3,file='data.tmp')
      READ(3,'(EN26.16E3)') X(1)%v
C     close(3)
      close(unit=3)
      Y(1)%v = X(1)%v
      CALL setderiv(Y(1),X(1))
      END SUBROUTINE
