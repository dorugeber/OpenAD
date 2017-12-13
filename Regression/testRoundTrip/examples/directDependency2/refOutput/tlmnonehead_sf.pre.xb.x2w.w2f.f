
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
      SUBROUTINE foo()
      use w2f__types
      IMPLICIT NONE
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      END SUBROUTINE

      SUBROUTINE head(X, Y)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X(1 : 2)
      TYPE (OpenADTy_active) Y
C
C     **** Local Variables and Functions ****
C
      EXTERNAL foo
      TYPE (OpenADTy_active) T
      TYPE (OpenADTy_active) T1
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      __value__(T) = (__value__(X(1)) + __value__(X(2)))
      __value__(T1) = (__value__(T) * 2.0D00)
      CALL setderiv(__deriv__(T), __deriv__(X(1)))
      CALL inc_deriv(__deriv__(T), __deriv__(X(2)))
      CALL sax(2.0D00, __deriv__(T), __deriv__(T1))
      CALL foo()
      __value__(Y) = (__value__(T) * 2.0D00 + __value__(T1) * 3.0D00)
      CALL sax(2.0D00, __deriv__(T), __deriv__(Y))
      CALL saxpy(3.0D00, __deriv__(T1), __deriv__(Y))
      END SUBROUTINE
