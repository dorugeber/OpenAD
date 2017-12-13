
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
      SUBROUTINE foo(A, B, C, D)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) A
      TYPE (OpenADTy_active) B
      OPTIONAL  B
      TYPE (OpenADTy_active) C
      TYPE (OpenADTy_active) D
      OPTIONAL  D
C
C     **** Local Variables and Functions ****
C
      LOGICAL(w2f__i4) t__1
      TYPE (OpenADTy_active) OpenAD_prp_0
      TYPE (OpenADTy_active) OpenAD_prp_1
C
C     **** Statements ****
C
      __value__(C) = (__value__(A) * 2.0D00)
      CALL setderiv(__deriv__(OpenAD_prp_0), __deriv__(A))
      CALL sax(2.0D00, __deriv__(OpenAD_prp_0), __deriv__(C))
      t__1 = .TRUE.
      IF(.NOT. PRESENT(__value__(B))) THEN
        t__1 = .FALSE.
      ELSE
        IF(.NOT. PRESENT(__value__(D))) THEN
          t__1 = .FALSE.
        ENDIF
      ENDIF
      IF(t__1) THEN
        __value__(D) = (__value__(B) * 3.0D00)
        CALL setderiv(__deriv__(OpenAD_prp_1), __deriv__(B))
        CALL sax(3.0D00, __deriv__(OpenAD_prp_1), __deriv__(D))
      ENDIF
      END SUBROUTINE

      SUBROUTINE head(X, Y)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X(1 : 3)
      TYPE (OpenADTy_active) Y(1 : 3)
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
      interface 
        SUBROUTINE foo(A, B, C, D)
        use w2f__types
        TYPE (OpenADTy_active) A
        TYPE (OpenADTy_active) B
        OPTIONAL  B
        TYPE (OpenADTy_active) C
        TYPE (OpenADTy_active) D
        OPTIONAL  D
        END SUBROUTINE

      end interface 
      
C
C     **** Statements ****
C
      CALL foo(A = __deriv__(X(1)), C = __deriv__(Y(1)))
      CALL foo(__deriv__(X(2)), __deriv__(X(3)), __deriv__(Y(2)),
     >  __deriv__(Y(3)))
      END SUBROUTINE
