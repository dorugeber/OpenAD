
C$OPENAD XXX File_start [head.f]
      SUBROUTINE head(X, Y)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X(1:2)
      type(active) :: Y(1:2)
C
C     **** Local Variables and Functions ****
C
      type(active) :: T(1:INT(SIZE(X)))
      type(active) :: U(1:INT(SIZE(Y)))
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
      OpenAD_lin_0 = COS(X%v)
      T(1:INT(SIZE(X)))%v = SIN(X%v)
      CALL sax(OpenAD_lin_0,X,T(1:INT(SIZE(X))))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_1,T(1:INT(SIZE(X))))
      OpenAD_lin_1 = T(1:INT(SIZE(X)))%v
      OpenAD_lin_2 = X(1:2)%v
      U(1:INT(SIZE(Y)))%v = (X(1:2)%v*T(1:INT(SIZE(X)))%v+5.0D00)
      CALL sax(OpenAD_lin_1,X(1:2),U(1:INT(SIZE(Y))))
      CALL saxpy(OpenAD_lin_2,T(1:INT(SIZE(X))),U(1:INT(SIZE(Y))))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_aux_0,U)
      OpenAD_aux_0 = SQRT(U%v)
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_3,OpenAD_aux_0)
      OpenAD_lin_3 = (5.0D-01/OpenAD_aux_0)
      Y(1:2)%v = OpenAD_aux_0
      CALL sax(OpenAD_lin_3,U,Y(1:2))
      END SUBROUTINE
