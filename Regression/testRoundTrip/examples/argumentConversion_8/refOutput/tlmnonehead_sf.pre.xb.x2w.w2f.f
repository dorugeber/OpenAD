
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
      SUBROUTINE bar(X, Y)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      REAL(w2f__8) X(1 : 4)
      TYPE (OpenADTy_active) Y(1 : 4)
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      TYPE (OpenADTy_active) OpenAD_prp_0
C
C     **** Statements ****
C
      DO I = 1, 4, 1
        IF(X(I) .GT. 1.20000004768371582031D00) THEN
          __value__(Y(INT(I))) = (__value__(Y(I)) * 2.0D00)
          CALL setderiv(__deriv__(OpenAD_prp_0), __deriv__(Y(I)))
          CALL sax(2.0D00, __deriv__(OpenAD_prp_0), __deriv__(Y(I)))
        ENDIF
      END DO
      END SUBROUTINE

      SUBROUTINE foo(X, Y)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X(1 :)
      TYPE (OpenADTy_active) Y(1 :)
C
C     **** Local Variables and Functions ****
C
      EXTERNAL bar
      REAL(w2f__8) OpenAD_tyc_0(:)
      ALLOCATABLE OpenAD_tyc_0
C
C     **** Statements ****
C
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_tyc_0, __deriv__(X))
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(OpenAD_tyc_0, __deriv__(X))
      CALL bar(OpenAD_tyc_0, __deriv__(Y))
C     $OpenAD$ INLINE oad_ShapeTest(subst,subst)
      CALL oad_ShapeTest(OpenAD_tyc_0, __deriv__(X))
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(__deriv__(X), OpenAD_tyc_0)
      END SUBROUTINE

      SUBROUTINE head(X, Y)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X(1 : 4)
      TYPE (OpenADTy_active) Y(1 : 4)
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
      interface 
        SUBROUTINE foo(X, Y)
        use w2f__types
        TYPE (OpenADTy_active) X(1 :)
        TYPE (OpenADTy_active) Y(1 :)
        END SUBROUTINE

      end interface 
      
C
C     **** Statements ****
C
      __value__(Y(1 : 4)) = __value__(X(1 : 4))
      CALL setderiv(__deriv__(Y(1 : 4)), __deriv__(X(1 : 4)))
      CALL foo(__deriv__(X), __deriv__(Y))
      END SUBROUTINE
