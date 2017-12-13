
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
      MODULE m
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      TYPE (OpenADTy_active) T(:)
      ALLOCATABLE T
C
C     **** Statements ****
C
      END MODULE

      SUBROUTINE foo(T)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      REAL(w2f__8) T(1 :)
C
C     **** Local Variables and Functions ****
C
      EXTERNAL bar
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      WRITE(*, *) SHAPE(T)
      CALL bar()
      WRITE(*, *) SHAPE(T)
      END SUBROUTINE

      SUBROUTINE bar()
      use w2f__types
      use oad_intrinsics
      use m
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      DEALLOCATE(T)
      ALLOCATE(T(3))
      DO I = 1, 3, 1
        __value__(T(INT(I))) = (I * 5.0D-01 + 1.0D00)
        CALL zero_deriv(__deriv__(T(INT(I))))
      END DO
      END SUBROUTINE

      SUBROUTINE head(X, Y)
      use w2f__types
      use oad_intrinsics
      use m
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      REAL(w2f__8) OpenAD_Symbol_0(:)
      ALLOCATABLE OpenAD_Symbol_0
      REAL(w2f__8) OpenAD_Symbol_1
      REAL(w2f__8) OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_2
      TYPE (OpenADTy_active) OpenAD_prop_0
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X(1 : 1)
      TYPE (OpenADTy_active) Y(1 : 1)
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
      interface 
        SUBROUTINE foo(T)
        use w2f__types
        REAL(w2f__8) T(1 :)
        END SUBROUTINE

      end interface 
      
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      ALLOCATE(T(2))
      __value__(T(2)) = __value__(X(1))
      CALL setderiv(__deriv__(T(2)), __deriv__(X(1)))
      OpenAD_Symbol_1 = (__value__(T(2)) * 2.0D00)
      __value__(T(2)) = OpenAD_Symbol_1
      CALL setderiv(__deriv__(OpenAD_prop_0), __deriv__(T(2)))
      CALL sax(2.0D00, __deriv__(OpenAD_prop_0), __deriv__(T(2)))
      __value__(X(1)) = __value__(T(2))
      CALL setderiv(__deriv__(X(1)), __deriv__(T(2)))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_Symbol_0, __deriv__(T))
C     $OpenAD$ INLINE convert_a2p_vector(subst,subst)
      CALL convert_a2p_vector(OpenAD_Symbol_0, __deriv__(T))
      CALL foo(OpenAD_Symbol_0)
C     $OpenAD$ INLINE oad_ShapeTest(subst,subst)
      CALL oad_ShapeTest(OpenAD_Symbol_0, __deriv__(T))
C     $OpenAD$ INLINE convert_p2a_vector(subst,subst)
      CALL convert_p2a_vector(__deriv__(T), OpenAD_Symbol_0)
      __value__(Y(1)) = (__value__(X(1)) * __value__(T(3)))
      OpenAD_lin_1 = __value__(T(3))
      OpenAD_lin_2 = __value__(X(1))
      CALL sax(OpenAD_lin_1, __deriv__(X(1)), __deriv__(Y(1)))
      CALL saxpy(OpenAD_lin_2, __deriv__(T(3)), __deriv__(Y(1)))
      DEALLOCATE(T)
      END SUBROUTINE
