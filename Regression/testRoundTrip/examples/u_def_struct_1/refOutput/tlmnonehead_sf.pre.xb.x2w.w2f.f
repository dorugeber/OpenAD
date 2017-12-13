
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
      MODULE mytypemodule
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      TYPE  MYTYPE
      SEQUENCE
        TYPE (OpenADTy_active) FIELD1
        TYPE (OpenADTy_active) FIELD2
      END TYPE
      
C
C     **** Statements ****
C
      END MODULE

      SUBROUTINE head(X, Y)
      use w2f__types
      use oad_intrinsics
      use mytypemodule
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X(1 : 2)
      TYPE (OpenADTy_active) Y(1 : 1)
C
C     **** Local Variables and Functions ****
C
      TYPE (MYTYPE) TYPED_X
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      __value__(TYPED_X%FIELD1) = __value__(X(1))
      CALL setderiv(__deriv__(TYPED_X%FIELD1), __deriv__(X(1)))
      __value__(TYPED_X%FIELD2) = __value__(X(2))
      CALL setderiv(__deriv__(TYPED_X%FIELD2), __deriv__(X(2)))
      OpenAD_lin_0 = __value__(TYPED_X%FIELD2)
      OpenAD_lin_1 = __value__(TYPED_X%FIELD1)
      __value__(Y(1)) = (__value__(TYPED_X%FIELD1) * __value__(TYPED_X%
     > FIELD2))
      CALL sax(OpenAD_lin_0, __deriv__(TYPED_X%FIELD1), __deriv__(Y(1))
     > )
      CALL saxpy(OpenAD_lin_1, __deriv__(TYPED_X%FIELD2), __deriv__(Y(1
     > )))
      END SUBROUTINE
