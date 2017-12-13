
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
      TYPE (OpenADTy_active) X(1 : 2)
      TYPE (OpenADTy_active) Y(1 : 2)
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) N
      SAVE N
      TYPE (OpenADTy_active) Z(1 : INT(SIZE(__value__(X))))
C
C     **** Initializers ****
C
      DATA N / 2 /
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      __value__(Z(1 : INT(SIZE(X)))) = __value__(X(1 : 2))
      CALL setderiv(__deriv__(Z(1 : INT(SIZE(X)))), __deriv__(X(1 : 2))
     > )
      __value__(Y(1 : 2)) = __value__(Z(1 : INT(SIZE(X))))
      CALL setderiv(__deriv__(Y(1 : 2)), __deriv__(Z(1 : INT(SIZE(X))))
     > )
      END SUBROUTINE
