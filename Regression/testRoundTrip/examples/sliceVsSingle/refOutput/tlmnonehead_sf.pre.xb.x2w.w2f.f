
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
      SUBROUTINE foo(X)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X
C
C     **** Local Variables and Functions ****
C
      TYPE (OpenADTy_active) OpenAD_prp_0
C
C     **** Statements ****
C
      __value__(X) = (__value__(X) + 1.0D00)
      CALL setderiv(__deriv__(OpenAD_prp_0), __deriv__(X))
      CALL setderiv(__deriv__(X), __deriv__(OpenAD_prp_0))
      END SUBROUTINE

      SUBROUTINE bar(X)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X(1 : 1)
C
C     **** Local Variables and Functions ****
C
      TYPE (OpenADTy_active) OpenAD_prp_1
C
C     **** Statements ****
C
      __value__(X(1)) = (__value__(X(1)) + 1.0D00)
      CALL setderiv(__deriv__(OpenAD_prp_1), __deriv__(X(1)))
      CALL setderiv(__deriv__(X(1)), __deriv__(OpenAD_prp_1))
      END SUBROUTINE

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
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
      interface 
        SUBROUTINE foo(X)
        use w2f__types
        TYPE (OpenADTy_active) X
        END SUBROUTINE

      end interface 
      
      interface 
        SUBROUTINE bar(X)
        use w2f__types
        TYPE (OpenADTy_active) X(1 : 1)
        END SUBROUTINE

      end interface 
      
C
C     **** Statements ****
C
      CALL foo(__deriv__(X(1)))
      __value__(Y(1 : 2)) = __value__(X(1 : 2))
      CALL setderiv(__deriv__(Y(1 : 2)), __deriv__(X(1 : 2)))
      CALL bar(__deriv__(Y(2 : 2)))
      END SUBROUTINE
