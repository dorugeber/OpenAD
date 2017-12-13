
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
      SUBROUTINE foo(A, X)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      CHARACTER(*) A
      TYPE (OpenADTy_active) X
C
C     **** Local Variables and Functions ****
C
      TYPE (OpenADTy_active) OpenAD_prp_0
      TYPE (OpenADTy_active) OpenAD_prp_1
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      IF(A(1 : LEN(A)) .EQ. 'two') THEN
        __value__(X) = (__value__(X) * 2.0D00)
        CALL setderiv(__deriv__(OpenAD_prp_0), __deriv__(X))
        CALL sax(2.0D00, __deriv__(OpenAD_prp_0), __deriv__(X))
      ENDIF
      IF(A(1 : LEN(A)) .EQ. 'three') THEN
        __value__(X) = (__value__(X) * 3.0D00)
        CALL setderiv(__deriv__(OpenAD_prp_1), __deriv__(X))
        CALL sax(3.0D00, __deriv__(OpenAD_prp_1), __deriv__(X))
      ENDIF
      END SUBROUTINE

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
C     **** Local Variables and Functions ****
C
      CHARACTER(10) A
      EXTERNAL foo
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      A = 'two'
      CALL foo(A, __deriv__(X))
      A = 'three'
      CALL foo(A, __deriv__(X))
      A = 'four'
      __value__(Y(1)) = __value__(X(1))
      CALL setderiv(__deriv__(Y(1)), __deriv__(X(1)))
      END SUBROUTINE
