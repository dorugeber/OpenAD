
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
      TYPE (OpenADTy_active) Z(1 : INT(SIZE(__value__(X))))
      REAL(w2f__8) OpenAD_acc_0(1 : 2)
      REAL(w2f__8) OpenAD_acc_1(1 : 2)
      REAL(w2f__8) OpenAD_acc_2(1 : 2)
      REAL(w2f__8) OpenAD_aux_0(1 : 2)
      REAL(w2f__8) OpenAD_aux_1(1 : 2)
      REAL(w2f__8) OpenAD_lin_0(1 : 2)
      REAL(w2f__8) OpenAD_lin_1(:)
      ALLOCATABLE OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_2(1 : 2)
      REAL(w2f__8) OpenAD_lin_3(1 : 2)
      REAL(w2f__8) OpenAD_lin_4(1 : 2)
      REAL(w2f__8) OpenAD_lin_5(:)
      ALLOCATABLE OpenAD_lin_5
      REAL(w2f__8) OpenAD_lin_6(1 : 2)
      TYPE (OpenADTy_active) OpenAD_prp_0(1 : 2)
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
      OpenAD_aux_0 = COS(__value__(X))
      OpenAD_lin_0 = OpenAD_aux_0
      OpenAD_lin_2 = (- SIN(__value__(X)))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_1, __deriv__(Z(1 : INT(SIZE(
     > X)))))
      OpenAD_lin_1 = __value__(Z(1 : INT(SIZE(X))))
      __value__(Y(1 : 2)) = (__value__(Z(1 : INT(SIZE(X)))) *
     >  OpenAD_aux_0)
      OpenAD_acc_0 = (OpenAD_lin_2 * OpenAD_lin_1)
      CALL sax(OpenAD_lin_0, __deriv__(Z(1 : INT(SIZE(X)))), __deriv__(
     > Y(1 : 2)))
      CALL saxpy(OpenAD_acc_0, __deriv__(X), __deriv__(Y(1 : 2)))
      OpenAD_aux_1 = (__value__(X(1 : 2)) * __value__(Z(1 : INT(SIZE(X)
     > ))))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_5, __deriv__(Z(1 : INT(SIZE(
     > X)))))
      OpenAD_lin_5 = __value__(Z(1 : INT(SIZE(X))))
      OpenAD_lin_6 = __value__(X(1 : 2))
      OpenAD_lin_3 = (INT(1_w2f__i8) / __value__(Y(1 : 2)))
      OpenAD_lin_4 = (-(OpenAD_aux_1 /(__value__(Y(1 : 2)) * __value__(
     > Y(1 : 2)))))
      __value__(Y(1 : 2)) = (OpenAD_aux_1 / __value__(Y(1 : 2)))
      OpenAD_acc_1 = (OpenAD_lin_5 * OpenAD_lin_3)
      OpenAD_acc_2 = (OpenAD_lin_6 * OpenAD_lin_3)
      CALL setderiv(__deriv__(OpenAD_prp_0), __deriv__(Y(1 : 2)))
      CALL sax(OpenAD_lin_4, __deriv__(OpenAD_prp_0), __deriv__(Y(1 : 2
     > )))
      CALL saxpy(OpenAD_acc_1, __deriv__(X(1 : 2)), __deriv__(Y(1 : 2))
     > )
      CALL saxpy(OpenAD_acc_2, __deriv__(Z(1 : INT(SIZE(X)))),
     >  __deriv__(Y(1 : 2)))
      END SUBROUTINE
