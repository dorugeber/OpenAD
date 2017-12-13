
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
      SUBROUTINE foo(A)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) A(1 : 1, 1 : 1, 1 : 1, 1 : 1, 1 : 1)
C
C     **** Local Variables and Functions ****
C
      TYPE (OpenADTy_active) OpenAD_prp_0
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      __value__(A(1, 1, 1, 1, 1)) = (__value__(A(1, 1, 1, 1, 1)) *
     >  2.0D00)
      CALL setderiv(__deriv__(OpenAD_prp_0), __deriv__(A(1, 1, 1, 1, 1)
     > ))
      CALL sax(2.0D00, __deriv__(OpenAD_prp_0), __deriv__(A(1, 1, 1, 1,
     >  1)))
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
      EXTERNAL foo
      TYPE (OpenADTy_active) Z(1 : 1, 1 : 1, 1 : 1, 1 : 1, 1 : 1)
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      __value__(Z(1, 1, 1, 1, 1)) = __value__(X(1))
      CALL setderiv(__deriv__(Z(1, 1, 1, 1, 1)), __deriv__(X(1)))
      CALL foo(__deriv__(Z))
      __value__(Y(1)) = __value__(Z(1, 1, 1, 1, 1))
      CALL setderiv(__deriv__(Y(1)), __deriv__(Z(1, 1, 1, 1, 1)))
      END SUBROUTINE
