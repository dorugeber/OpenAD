
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
      TYPE (OpenADTy_active) T(1 : INT(SIZE(__value__(X))))
      TYPE (OpenADTy_active) U(1 : INT(SIZE(__value__(Y))))
      REAL(w2f__8) OpenAD_aux_0(:)
      ALLOCATABLE OpenAD_aux_0
      REAL(w2f__8) OpenAD_lin_0(1 : 2)
      REAL(w2f__8) OpenAD_lin_1(:)
      ALLOCATABLE OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_2(1 : 2)
      REAL(w2f__8) OpenAD_lin_3(:)
      ALLOCATABLE OpenAD_lin_3
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      OpenAD_lin_0 = COS(__value__(X))
      __value__(T(1 : INT(SIZE(X)))) = SIN(__value__(X))
      CALL sax(OpenAD_lin_0, __deriv__(X), __deriv__(T(1 : INT(SIZE(X))
     > )))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_1, __deriv__(T(1 : INT(SIZE(
     > X)))))
      OpenAD_lin_1 = __value__(T(1 : INT(SIZE(X))))
      OpenAD_lin_2 = __value__(X(1 : 2))
      __value__(U(1 : INT(SIZE(Y)))) = (__value__(X(1 : 2)) * __value__
     > (T(1 : INT(SIZE(X)))) + 5.0D00)
      CALL sax(OpenAD_lin_1, __deriv__(X(1 : 2)), __deriv__(U(1 : INT(
     > SIZE(Y)))))
      CALL saxpy(OpenAD_lin_2, __deriv__(T(1 : INT(SIZE(X)))),
     >  __deriv__(U(1 : INT(SIZE(Y)))))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_aux_0, __deriv__(U))
      OpenAD_aux_0 = SQRT(__value__(U))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_3, OpenAD_aux_0)
      OpenAD_lin_3 = (5.0D-01 / OpenAD_aux_0)
      __value__(Y(1 : 2)) = OpenAD_aux_0
      CALL sax(OpenAD_lin_3, __deriv__(U), __deriv__(Y(1 : 2)))
      END SUBROUTINE
