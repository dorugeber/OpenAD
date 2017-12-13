
!$OPENAD XXX File_start [OAD_intrinsics.f90]
MODULE oad_intrinsics
use w2f__types
use OAD_active
IMPLICIT NONE
SAVE
!
!     **** Statements ****
!
END MODULE

C$OPENAD XXX File_start [all_globals_mod.f]
      MODULE all_globals_mod
      use w2f__types
      use OAD_active
      IMPLICIT NONE
      SAVE
C
C     **** Statements ****
C
      END MODULE

C$OPENAD XXX File_start [head.f]
      MODULE m
      use w2f__types
      use OAD_active
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      type(active) :: T(:)
      ALLOCATABLE T
C
C     **** Statements ****
C
      END MODULE

      SUBROUTINE foo(T)
      use w2f__types
      use OAD_active
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
      WRITE(*,*) SHAPE(T)
      CALL bar()
      WRITE(*,*) SHAPE(T)
      END SUBROUTINE

      SUBROUTINE bar()
      use w2f__types
      use OAD_active
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
        T(INT(I))%v = (I*5.0D-01+1.0D00)
        CALL zero_deriv(T(INT(I)))
      END DO
      END SUBROUTINE

      SUBROUTINE head(X, Y)
      use w2f__types
      use OAD_active
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
      type(active) :: OpenAD_prop_0
C
C     **** Parameters and Result ****
C
      type(active) :: X(1:1)
      type(active) :: Y(1:1)
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
      interface
        SUBROUTINE foo(T)
        use w2f__types
      use OAD_active
        REAL(w2f__8) T(1 :)
        END SUBROUTINE

      end interface

C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      ALLOCATE(T(2))
      T(2)%v = X(1)%v
      CALL setderiv(T(2),X(1))
      OpenAD_Symbol_1 = (T(2)%v*2.0D00)
      T(2)%v = OpenAD_Symbol_1
      CALL setderiv(OpenAD_prop_0,T(2))
      CALL sax(2.0D00,OpenAD_prop_0,T(2))
      X(1)%v = T(2)%v
      CALL setderiv(X(1),T(2))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_Symbol_0,T)
C     $OpenAD$ INLINE convert_a2p_vector(subst,subst)
      CALL convert_a2p_vector(OpenAD_Symbol_0,T)
      CALL foo(OpenAD_Symbol_0)
C     $OpenAD$ INLINE oad_ShapeTest(subst,subst)
      CALL oad_ShapeTest(OpenAD_Symbol_0,T)
C     $OpenAD$ INLINE convert_p2a_vector(subst,subst)
      CALL convert_p2a_vector(T,OpenAD_Symbol_0)
      Y(1)%v = (X(1)%v*T(3)%v)
      OpenAD_lin_1 = T(3)%v
      OpenAD_lin_2 = X(1)%v
      CALL sax(OpenAD_lin_1,X(1),Y(1))
      CALL saxpy(OpenAD_lin_2,T(3),Y(1))
      deallocate(T)
      END SUBROUTINE
