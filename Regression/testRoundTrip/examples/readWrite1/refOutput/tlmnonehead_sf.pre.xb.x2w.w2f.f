
C$OPENAD XXX File_start [OAD_intrinsics.f90]
      MODULE oad_intrinsics
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Statements ****
C
      END MODULE

C$OPENAD XXX File_start [all_globals_mod.f]
      MODULE all_globals_mod
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Statements ****
C
      END MODULE

C$OPENAD XXX File_start [head.f]
      SUBROUTINE head(X, Y)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X(1 : 1)
      TYPE (OpenADTy_active) Y(1 : 1)
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
      WRITE(3, '(EN26.16E3)') __value__(X(1))
C     close(3)
      CLOSE(UNIT = 3)
C     open(3,file='data.tmp')
      OPEN(UNIT = 3, FILE = 'data.tmp')
      READ(3, '(EN26.16E3)') __value__(X(1))
C     close(3)
      CLOSE(UNIT = 3)
      __value__(Y(1)) = __value__(X(1))
      CALL setderiv(__deriv__(Y(1)), __deriv__(X(1)))
      END SUBROUTINE
