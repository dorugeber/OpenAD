
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
      type(active) :: A(1:INT(SIZE(X)))
      type(active) :: B(1:INT(SIZE(X)))
      type(active) :: C(1:INT(SIZE(X)))
      REAL(w2f__8) OpenAD_acc_0(1 : 2)
      REAL(w2f__8) OpenAD_acc_1(1 : 2)
      REAL(w2f__8) OpenAD_acc_10(:)
      ALLOCATABLE OpenAD_acc_10
      REAL(w2f__8) OpenAD_acc_2(1 : 2)
      REAL(w2f__8) OpenAD_acc_3(1 : 2)
      REAL(w2f__8) OpenAD_acc_4(1 : 2)
      REAL(w2f__8) OpenAD_acc_5(1 : 2)
      REAL(w2f__8) OpenAD_acc_6(1 : 2)
      REAL(w2f__8) OpenAD_acc_7(1 : 2)
      REAL(w2f__8) OpenAD_acc_8(1 : 2)
      REAL(w2f__8) OpenAD_acc_9(1 : 2)
      REAL(w2f__8) OpenAD_aux_0(1 : 2)
      REAL(w2f__8) OpenAD_aux_1(1 : 2)
      REAL(w2f__8) OpenAD_aux_2(1 : 2)
      REAL(w2f__8) OpenAD_aux_3(1 : 2)
      REAL(w2f__8) OpenAD_aux_4(1 : 2)
      REAL(w2f__8) OpenAD_aux_5(:)
      ALLOCATABLE OpenAD_aux_5
      REAL(w2f__8) OpenAD_aux_6(1 : 2)
      REAL(w2f__8) OpenAD_aux_7(:)
      ALLOCATABLE OpenAD_aux_7
      REAL(w2f__8) OpenAD_aux_8(:)
      ALLOCATABLE OpenAD_aux_8
      REAL(w2f__8) OpenAD_lin_0(1 : 2)
      REAL(w2f__8) OpenAD_lin_1(1 : 2)
      REAL(w2f__8) OpenAD_lin_10(:)
      ALLOCATABLE OpenAD_lin_10
      REAL(w2f__8) OpenAD_lin_11(1 : 2)
      REAL(w2f__8) OpenAD_lin_12(:)
      ALLOCATABLE OpenAD_lin_12
      REAL(w2f__8) OpenAD_lin_13(:)
      ALLOCATABLE OpenAD_lin_13
      REAL(w2f__8) OpenAD_lin_14(:)
      ALLOCATABLE OpenAD_lin_14
      REAL(w2f__8) OpenAD_lin_15(1 : 2)
      REAL(w2f__8) OpenAD_lin_16(:)
      ALLOCATABLE OpenAD_lin_16
      REAL(w2f__8) OpenAD_lin_17(1 : 2)
      REAL(w2f__8) OpenAD_lin_18(:)
      ALLOCATABLE OpenAD_lin_18
      REAL(w2f__8) OpenAD_lin_19(:)
      ALLOCATABLE OpenAD_lin_19
      REAL(w2f__8) OpenAD_lin_2(1 : 2)
      REAL(w2f__8) OpenAD_lin_20(:)
      ALLOCATABLE OpenAD_lin_20
      REAL(w2f__8) OpenAD_lin_21(:)
      ALLOCATABLE OpenAD_lin_21
      REAL(w2f__8) OpenAD_lin_3(1 : 2)
      REAL(w2f__8) OpenAD_lin_4(1 : 2)
      REAL(w2f__8) OpenAD_lin_5(:)
      ALLOCATABLE OpenAD_lin_5
      REAL(w2f__8) OpenAD_lin_6(1 : 2)
      REAL(w2f__8) OpenAD_lin_7(1 : 2)
      REAL(w2f__8) OpenAD_lin_8(:)
      ALLOCATABLE OpenAD_lin_8
      REAL(w2f__8) OpenAD_lin_9(1 : 2)
      type(active) :: OpenAD_prp_0(:)
      ALLOCATABLE OpenAD_prp_0
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      OpenAD_aux_0 = TAN(X%v)
      OpenAD_aux_1 = SIN(X%v)
      OpenAD_lin_2 = (OpenAD_aux_0*OpenAD_aux_0+INT(1_w2f__i8))
      OpenAD_lin_0 = (INT(1_w2f__i8)/OpenAD_aux_1)
      OpenAD_lin_3 = COS(X%v)
      OpenAD_lin_1 = (-(OpenAD_aux_0/(OpenAD_aux_1*OpenAD_aux_1)))
      A(1:INT(SIZE(X)))%v = (OpenAD_aux_0/OpenAD_aux_1)
      OpenAD_acc_0 = (OpenAD_lin_3*OpenAD_lin_1)
      OpenAD_acc_1 = (OpenAD_lin_2*OpenAD_lin_0)
      CALL sax(OpenAD_acc_0,X,A(1:INT(SIZE(X))))
      CALL saxpy(OpenAD_acc_1,X,A(1:INT(SIZE(X))))
      OpenAD_aux_2 = (X%v*8.0D00)
      OpenAD_lin_4 = (-SIN(OpenAD_aux_2))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_5,A(1:INT(SIZE(X))))
      OpenAD_lin_5 = A(1:INT(SIZE(X)))%v
      OpenAD_lin_6 = X(1:2)%v
      B(1:INT(SIZE(X)))%v = (COS(OpenAD_aux_2)+X(1:2)%v*A(1:INT(SIZE(X))
     +)%v)
      OpenAD_acc_2 = (8.0D00*OpenAD_lin_4)
      CALL sax(OpenAD_acc_2,X,B(1:INT(SIZE(X))))
      CALL saxpy(OpenAD_lin_5,X(1:2),B(1:INT(SIZE(X))))
      CALL saxpy(OpenAD_lin_6,A(1:INT(SIZE(X))),B(1:INT(SIZE(X))))
      OpenAD_aux_4 = TAN(X%v)
      OpenAD_aux_3 = (A(1:INT(SIZE(X)))%v*OpenAD_aux_4)
      OpenAD_lin_7 = OpenAD_aux_3
      OpenAD_lin_9 = OpenAD_aux_4
      OpenAD_lin_11 = (OpenAD_aux_4*OpenAD_aux_4+INT(1_w2f__i8))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_10,A(1:INT(SIZE(X))))
      OpenAD_lin_10 = A(1:INT(SIZE(X)))%v
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_8,B(1:INT(SIZE(X))))
      OpenAD_lin_8 = B(1:INT(SIZE(X)))%v
      B(1:INT(SIZE(X)))%v = (B(1:INT(SIZE(X)))%v*OpenAD_aux_3)
      OpenAD_acc_3 = (OpenAD_lin_9*OpenAD_lin_8)
      OpenAD_acc_4 = (OpenAD_lin_11*OpenAD_lin_10*OpenAD_lin_8)
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_prp_0,B(1:INT(SIZE(X))))
      CALL setderiv(OpenAD_prp_0,B(1:INT(SIZE(X))))
      CALL sax(OpenAD_lin_7,OpenAD_prp_0,B(1:INT(SIZE(X))))
      CALL saxpy(OpenAD_acc_3,A(1:INT(SIZE(X))),B(1:INT(SIZE(X))))
      CALL saxpy(OpenAD_acc_4,X,B(1:INT(SIZE(X))))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_aux_5,A(1:INT(SIZE(X))))
      OpenAD_aux_5 = (4.0D00/A(1:INT(SIZE(X)))%v)
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_aux_7,A)
      OpenAD_aux_7 = COS(A%v)
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_aux_8,B)
      OpenAD_aux_8 = TAN(B%v)
      OpenAD_aux_6 = SQRT(X%v*A%v+OpenAD_aux_7*OpenAD_aux_8)
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_12,OpenAD_aux_5)
      OpenAD_lin_12 = (OpenAD_aux_5*(A(1:INT(SIZE(X)))%v**(OpenAD_aux_5-
     +INT(1_w2f__i8))))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_14,A(1:INT(SIZE(X))))
      OpenAD_lin_14 = (-(4.0D00/(A(1:INT(SIZE(X)))%v*A(1:INT(SIZE(X)))%v
     +)))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_13,OpenAD_aux_5)
      OpenAD_lin_13 = (LOG(A(1:INT(SIZE(X)))%v)*(A(1:INT(SIZE(X)))%v**Op
     +enAD_aux_5))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_16,A)
      OpenAD_lin_16 = A%v
      OpenAD_lin_17 = X%v
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_20,A)
      OpenAD_lin_20 = (-SIN(A%v))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_18,OpenAD_aux_8)
      OpenAD_lin_18 = OpenAD_aux_8
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_21,OpenAD_aux_8)
      OpenAD_lin_21 = (OpenAD_aux_8*OpenAD_aux_8+INT(1_w2f__i8))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_19,OpenAD_aux_7)
      OpenAD_lin_19 = OpenAD_aux_7
      OpenAD_lin_15 = (5.0D-01/OpenAD_aux_6)
      C(1:INT(SIZE(X)))%v = ((A(1:INT(SIZE(X)))%v**OpenAD_aux_5)-OpenAD_
     +aux_6)
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_acc_10,OpenAD_lin_13)
      OpenAD_acc_5 = (OpenAD_lin_15*INT((-1_w2f__i8)))
      OpenAD_acc_6 = (OpenAD_lin_21*OpenAD_lin_19*OpenAD_acc_5)
      OpenAD_acc_7 = (OpenAD_lin_20*OpenAD_lin_18*OpenAD_acc_5)
      OpenAD_acc_8 = (OpenAD_lin_16*OpenAD_acc_5)
      OpenAD_acc_9 = (OpenAD_lin_17*OpenAD_acc_5)
      OpenAD_acc_10 = (OpenAD_lin_14*OpenAD_lin_13)
      CALL sax(OpenAD_acc_6,B,C(1:INT(SIZE(X))))
      CALL saxpy(OpenAD_acc_7,A,C(1:INT(SIZE(X))))
      CALL saxpy(OpenAD_acc_8,X,C(1:INT(SIZE(X))))
      CALL saxpy(OpenAD_acc_9,A,C(1:INT(SIZE(X))))
      CALL saxpy(OpenAD_lin_12,A(1:INT(SIZE(X))),C(1:INT(SIZE(X))))
      CALL saxpy(OpenAD_acc_10,A(1:INT(SIZE(X))),C(1:INT(SIZE(X))))
      Y(1:2)%v = (A(1:INT(SIZE(X)))%v+C(1:INT(SIZE(X)))%v*5.0D00)
      CALL setderiv(Y(1:2),A(1:INT(SIZE(X))))
      CALL saxpy(5.0D00,C(1:INT(SIZE(X))),Y(1:2))
      END SUBROUTINE
