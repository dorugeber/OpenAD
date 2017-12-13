
!$OPENAD XXX File_start [OAD_intrinsics.f90]
MODULE oad_intrinsics
use OAD_active
use w2f__types
IMPLICIT NONE
SAVE
!
!     **** Statements ****
!
END MODULE

C$OPENAD XXX File_start [all_globals_mod.f]
      MODULE all_globals_mod
      use OAD_active
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Statements ****
C
      END MODULE
C#########################################################
C This file is part of OpenAD released under the LGPL.   #
C The full COPYRIGHT notice can be found in the top      #
C level directory of the OpenAD distribution             #
C#########################################################
C
C$OPENAD XXX File_start [head.f]
      SUBROUTINE box_timestep(GAMMALOC, FLDSTAR, EXTFORLOC, UVELLOC, FLD
     +NOW, FLDOLD, FLDNEW)
          use OAD_tape
          use OAD_rev
          use OAD_cp

C original arguments get inserted before version
C         ! and declared here together with all local variables
C         ! generated by xaifBooster

      use OAD_active
      use w2f__types
      use oad_intrinsics
      use oad_intrinsics
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      INTEGER(w2f__i8) OpenAD_Symbol_0
      INTEGER(w2f__i8) OpenAD_Symbol_1
      INTEGER(w2f__i8) OpenAD_Symbol_10
      INTEGER(w2f__i8) OpenAD_Symbol_11
      INTEGER(w2f__i8) OpenAD_Symbol_2
      INTEGER(w2f__i8) OpenAD_Symbol_3
      INTEGER(w2f__i8) OpenAD_Symbol_4
      INTEGER(w2f__i8) OpenAD_Symbol_5
      INTEGER(w2f__i8) OpenAD_Symbol_6
      INTEGER(w2f__i8) OpenAD_Symbol_7
      INTEGER(w2f__i8) OpenAD_Symbol_8
      INTEGER(w2f__i8) OpenAD_Symbol_9
C
C     **** Parameters and Result ****
C
      REAL(w2f__8) GAMMALOC
      REAL(w2f__8) FLDSTAR(1 : 2)
      REAL(w2f__8) EXTFORLOC(1 : 2)
      REAL(w2f__8) UVELLOC
      type(active) :: FLDNOW(1:3)
      REAL(w2f__8) FLDOLD(1 : 3)
      type(active) :: FLDNEW(1:3)
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) DELTA_T
      type(active) :: DFLDDT(1:3)
      INTEGER(w2f__i4) L
      REAL(w2f__8) VOL(1 : 3)
      REAL(w2f__8) OpenAD_Symbol_12
      REAL(w2f__8) OpenAD_Symbol_13
      REAL(w2f__8) OpenAD_Symbol_14
      REAL(w2f__8) OpenAD_Symbol_15
      REAL(w2f__8) OpenAD_Symbol_16
      INTEGER(w2f__i4) OpenAD_Symbol_17
      REAL(w2f__8) OpenAD_Symbol_18
      REAL(w2f__8) OpenAD_Symbol_19
      REAL(w2f__8) OpenAD_Symbol_20
      REAL(w2f__8) OpenAD_Symbol_21
      REAL(w2f__8) OpenAD_Symbol_22
      REAL(w2f__8) OpenAD_Symbol_23
      REAL(w2f__8) OpenAD_acc_0
      REAL(w2f__8) OpenAD_acc_1
      REAL(w2f__8) OpenAD_acc_2
      REAL(w2f__8) OpenAD_acc_3
      REAL(w2f__8) OpenAD_acc_4
      REAL(w2f__8) OpenAD_acc_5
      REAL(w2f__8) OpenAD_acc_6
      REAL(w2f__8) OpenAD_acc_7
      REAL(w2f__8) OpenAD_acc_8
      REAL(w2f__8) OpenAD_acc_9
      REAL(w2f__8) OpenAD_aux_0
      REAL(w2f__8) OpenAD_aux_1
      REAL(w2f__8) OpenAD_aux_10
      REAL(w2f__8) OpenAD_aux_11
      REAL(w2f__8) OpenAD_aux_12
      REAL(w2f__8) OpenAD_aux_13
      REAL(w2f__8) OpenAD_aux_14
      REAL(w2f__8) OpenAD_aux_15
      REAL(w2f__8) OpenAD_aux_16
      REAL(w2f__8) OpenAD_aux_17
      REAL(w2f__8) OpenAD_aux_18
      REAL(w2f__8) OpenAD_aux_19
      REAL(w2f__8) OpenAD_aux_2
      REAL(w2f__8) OpenAD_aux_20
      REAL(w2f__8) OpenAD_aux_3
      REAL(w2f__8) OpenAD_aux_4
      REAL(w2f__8) OpenAD_aux_5
      REAL(w2f__8) OpenAD_aux_6
      REAL(w2f__8) OpenAD_aux_7
      REAL(w2f__8) OpenAD_aux_8
      REAL(w2f__8) OpenAD_aux_9
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_10
      REAL(w2f__8) OpenAD_lin_11
      REAL(w2f__8) OpenAD_lin_12
      REAL(w2f__8) OpenAD_lin_13
      REAL(w2f__8) OpenAD_lin_14
      REAL(w2f__8) OpenAD_lin_15
      REAL(w2f__8) OpenAD_lin_16
      REAL(w2f__8) OpenAD_lin_17
      REAL(w2f__8) OpenAD_lin_18
      REAL(w2f__8) OpenAD_lin_19
      REAL(w2f__8) OpenAD_lin_2
      REAL(w2f__8) OpenAD_lin_20
      REAL(w2f__8) OpenAD_lin_3
      REAL(w2f__8) OpenAD_lin_4
      REAL(w2f__8) OpenAD_lin_5
      REAL(w2f__8) OpenAD_lin_6
      REAL(w2f__8) OpenAD_lin_7
      REAL(w2f__8) OpenAD_lin_8
      REAL(w2f__8) OpenAD_lin_9
      type(active) :: OpenAD_prp_0
      type(active) :: OpenAD_prp_1
      type(active) :: OpenAD_prp_2
      type(active) :: OpenAD_prp_3
      type(active) :: OpenAD_prp_4
      type(active) :: OpenAD_prp_5


C checkpointing stacks and offsets
          integer :: cp_loop_variable_1,cp_loop_variable_2,cp_loop_varia
     +ble_3,cp_loop_variable_4,cp_loop_variable_5,cp_loop_variable_6

C floats 'F'
          double precision, dimension(:), allocatable, save :: theArgFSt
     +ack
          integer, save :: theArgFStackoffset=0, theArgFStackSize=0
C integers 'I'
          integer, dimension(:), allocatable, save :: theArgIStack
          integer, save :: theArgIStackoffset=0, theArgIStackSize=0
C booleans 'B'
          logical, dimension(:), allocatable, save :: theArgBStack
          integer, save :: theArgBStackoffset=0, theArgBStackSize=0
C strings 'S'
          character*(80), dimension(:), allocatable, save :: theArgSStac
     +k
          integer, save :: theArgSStackoffset=0, theArgSStackSize=0

          type(modeType) :: our_orig_mode

C external C function used in inlined code
          integer iaddr
          external iaddr
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(FLDNOW)
C$OPENAD DEPENDENT(FLDNEW)
C
C     **** Statements ****
C

          if (our_rev_mode%arg_store) then
C store arguments
          end if
          if (our_rev_mode%arg_restore) then
C restore arguments
          end if
          if (our_rev_mode%plain) then
            our_orig_mode=our_rev_mode
            our_rev_mode%arg_store=.FALSE.
C original function
C$OPENAD XXX Template ad_template.f
      VOL(1) = 1.0D+01
      VOL(2) = 1.2D+01
      VOL(3) = 1.4D+01
      DELTA_T = 2.0D+01
      IF(UVELLOC .GE. 0.0D00) THEN
        DFLDDT(1)%v = ((EXTFORLOC(1)+VOL(1)*GAMMALOC*(FLDSTAR(1)-FLDNOW(
     +1)%v)+UVELLOC*(FLDNOW(3)%v-FLDNOW(1)%v))/VOL(1))
        DFLDDT(2)%v = ((EXTFORLOC(2)+VOL(2)*GAMMALOC*(FLDSTAR(2)-FLDNOW(
     +2)%v)+UVELLOC*(FLDNOW(1)%v-FLDNOW(2)%v))/VOL(2))
        DFLDDT(3)%v = ((UVELLOC*(FLDNOW(2)%v-FLDNOW(3)%v))/VOL(3))
      ELSE
        DFLDDT(1)%v = ((EXTFORLOC(1)+VOL(1)*GAMMALOC*(FLDSTAR(1)-FLDNOW(
     +1)%v)-UVELLOC*(FLDNOW(2)%v-FLDNOW(1)%v))/VOL(1))
        DFLDDT(2)%v = ((EXTFORLOC(2)+VOL(2)*GAMMALOC*(FLDSTAR(2)-FLDNOW(
     +2)%v)-UVELLOC*(FLDNOW(3)%v-FLDNOW(2)%v))/VOL(2))
        DFLDDT(3)%v = (-((UVELLOC*(FLDNOW(1)%v-FLDNOW(3)%v))/VOL(3)))
      ENDIF
      DO L = 1,3,1
        FLDNEW(INT(L))%v = (FLDOLD(L)+DFLDDT(L)%v*DELTA_T*2.0D00)
      END DO

C original function end
            our_rev_mode=our_orig_mode
          end if
          if (our_rev_mode%tape) then
C            print*, " tape       ", our_rev_mode
            our_rev_mode%arg_store=.TRUE.
            our_rev_mode%arg_restore=.FALSE.
            our_rev_mode%plain=.TRUE.
            our_rev_mode%tape=.FALSE.
            our_rev_mode%adjoint=.FALSE.
C taping
C$OPENAD XXX Template ad_template.f
      VOL(1) = 1.0D+01
      VOL(2) = 1.2D+01
      VOL(3) = 1.4D+01
      DELTA_T = 2.0D+01
      IF (UVELLOC.GE.0.0D00) THEN
        OpenAD_aux_2 = (FLDSTAR(1)-FLDNOW(1)%v)
        OpenAD_aux_1 = (GAMMALOC*OpenAD_aux_2)
        OpenAD_aux_3 = (FLDNOW(3)%v-FLDNOW(1)%v)
        OpenAD_aux_0 = (EXTFORLOC(1)+VOL(1)*OpenAD_aux_1+UVELLOC*OpenAD_
     +aux_3)
        OpenAD_lin_2 = GAMMALOC
        OpenAD_lin_1 = VOL(1)
        OpenAD_lin_3 = UVELLOC
        OpenAD_lin_0 = (INT(1_w2f__i8)/VOL(1))
        DFLDDT(1)%v = (OpenAD_aux_0/VOL(1))
        OpenAD_acc_0 = (OpenAD_lin_3*OpenAD_lin_0)
        OpenAD_acc_1 = (INT((-1_w2f__i8))*OpenAD_lin_2*OpenAD_lin_1*Open
     +AD_lin_0)
        double_tape(double_tape_pointer) = OpenAD_acc_0
        double_tape_pointer = double_tape_pointer+1
        double_tape(double_tape_pointer) = OpenAD_acc_1
        double_tape_pointer = double_tape_pointer+1
        OpenAD_aux_6 = (FLDSTAR(2)-FLDNOW(2)%v)
        OpenAD_aux_5 = (GAMMALOC*OpenAD_aux_6)
        OpenAD_aux_7 = (FLDNOW(1)%v-FLDNOW(2)%v)
        OpenAD_aux_4 = (EXTFORLOC(2)+VOL(2)*OpenAD_aux_5+UVELLOC*OpenAD_
     +aux_7)
        OpenAD_lin_6 = GAMMALOC
        OpenAD_lin_5 = VOL(2)
        OpenAD_lin_7 = UVELLOC
        OpenAD_lin_4 = (INT(1_w2f__i8)/VOL(2))
        DFLDDT(2)%v = (OpenAD_aux_4/VOL(2))
        OpenAD_acc_2 = (OpenAD_lin_7*OpenAD_lin_4)
        OpenAD_acc_3 = (INT((-1_w2f__i8))*OpenAD_lin_6*OpenAD_lin_5*Open
     +AD_lin_4)
        double_tape(double_tape_pointer) = OpenAD_acc_2
        double_tape_pointer = double_tape_pointer+1
        double_tape(double_tape_pointer) = OpenAD_acc_3
        double_tape_pointer = double_tape_pointer+1
        OpenAD_aux_9 = (FLDNOW(2)%v-FLDNOW(3)%v)
        OpenAD_aux_8 = (UVELLOC*OpenAD_aux_9)
        OpenAD_lin_9 = UVELLOC
        OpenAD_lin_8 = (INT(1_w2f__i8)/VOL(3))
        DFLDDT(3)%v = (OpenAD_aux_8/VOL(3))
        OpenAD_acc_4 = (OpenAD_lin_9*OpenAD_lin_8)
        double_tape(double_tape_pointer) = OpenAD_acc_4
        double_tape_pointer = double_tape_pointer+1
        OpenAD_Symbol_3 = 1_w2f__i8
        integer_tape(integer_tape_pointer) = OpenAD_Symbol_3
        integer_tape_pointer = integer_tape_pointer+1
      ELSE
        OpenAD_aux_13 = (FLDSTAR(1)-FLDNOW(1)%v)
        OpenAD_aux_12 = (GAMMALOC*OpenAD_aux_13)
        OpenAD_aux_14 = (FLDNOW(2)%v-FLDNOW(1)%v)
        OpenAD_aux_11 = (EXTFORLOC(1)+VOL(1)*OpenAD_aux_12-UVELLOC*OpenA
     +D_aux_14)
        OpenAD_lin_13 = GAMMALOC
        OpenAD_lin_12 = VOL(1)
        OpenAD_lin_14 = UVELLOC
        OpenAD_lin_11 = (INT(1_w2f__i8)/VOL(1))
        DFLDDT(1)%v = (OpenAD_aux_11/VOL(1))
        OpenAD_acc_5 = (OpenAD_lin_14*INT((-1_w2f__i8))*OpenAD_lin_11)
        OpenAD_acc_6 = (INT((-1_w2f__i8))*OpenAD_lin_13*OpenAD_lin_12*Op
     +enAD_lin_11)
        double_tape(double_tape_pointer) = OpenAD_acc_5
        double_tape_pointer = double_tape_pointer+1
        double_tape(double_tape_pointer) = OpenAD_acc_6
        double_tape_pointer = double_tape_pointer+1
        OpenAD_aux_17 = (FLDSTAR(2)-FLDNOW(2)%v)
        OpenAD_aux_16 = (GAMMALOC*OpenAD_aux_17)
        OpenAD_aux_18 = (FLDNOW(3)%v-FLDNOW(2)%v)
        OpenAD_aux_15 = (EXTFORLOC(2)+VOL(2)*OpenAD_aux_16-UVELLOC*OpenA
     +D_aux_18)
        OpenAD_lin_17 = GAMMALOC
        OpenAD_lin_16 = VOL(2)
        OpenAD_lin_18 = UVELLOC
        OpenAD_lin_15 = (INT(1_w2f__i8)/VOL(2))
        DFLDDT(2)%v = (OpenAD_aux_15/VOL(2))
        OpenAD_acc_7 = (OpenAD_lin_18*INT((-1_w2f__i8))*OpenAD_lin_15)
        OpenAD_acc_8 = (INT((-1_w2f__i8))*OpenAD_lin_17*OpenAD_lin_16*Op
     +enAD_lin_15)
        double_tape(double_tape_pointer) = OpenAD_acc_7
        double_tape_pointer = double_tape_pointer+1
        double_tape(double_tape_pointer) = OpenAD_acc_8
        double_tape_pointer = double_tape_pointer+1
        OpenAD_aux_20 = (FLDNOW(1)%v-FLDNOW(3)%v)
        OpenAD_aux_19 = (UVELLOC*OpenAD_aux_20)
        OpenAD_lin_20 = UVELLOC
        OpenAD_lin_19 = (INT(1_w2f__i8)/VOL(3))
        DFLDDT(3)%v = (-(OpenAD_aux_19/VOL(3)))
        OpenAD_acc_9 = (OpenAD_lin_20*OpenAD_lin_19*INT((-1_w2f__i8)))
        double_tape(double_tape_pointer) = OpenAD_acc_9
        double_tape_pointer = double_tape_pointer+1
        OpenAD_Symbol_4 = 0_w2f__i8
        integer_tape(integer_tape_pointer) = OpenAD_Symbol_4
        integer_tape_pointer = integer_tape_pointer+1
      ENDIF
      OpenAD_Symbol_5 = 0_w2f__i8
      DO L = 1,3,1
        OpenAD_aux_10 = (DELTA_T*2.0D00)
        OpenAD_lin_10 = OpenAD_aux_10
        FLDNEW(INT(L))%v = (FLDOLD(L)+DFLDDT(L)%v*OpenAD_aux_10)
        double_tape(double_tape_pointer) = OpenAD_lin_10
        double_tape_pointer = double_tape_pointer+1
        integer_tape(integer_tape_pointer) = L
        integer_tape_pointer = integer_tape_pointer+1
        OpenAD_Symbol_5 = (INT(OpenAD_Symbol_5)+INT(1_w2f__i8))
      END DO
      integer_tape(integer_tape_pointer) = OpenAD_Symbol_5
      integer_tape_pointer = integer_tape_pointer+1

C taping end
            our_rev_mode%arg_store=.FALSE.
            our_rev_mode%arg_restore=.FALSE.
            our_rev_mode%plain=.FALSE.
            our_rev_mode%tape=.FALSE.
            our_rev_mode%adjoint=.TRUE.
          end if
          if (our_rev_mode%adjoint) then
C            print*, " adjoint    ", our_rev_mode
            our_rev_mode%arg_store=.FALSE.
            our_rev_mode%arg_restore=.TRUE.
            our_rev_mode%plain=.FALSE.
            our_rev_mode%tape=.TRUE.
            our_rev_mode%adjoint=.FALSE.
C adjoint
      integer_tape_pointer = integer_tape_pointer-1
      OpenAD_Symbol_0 = integer_tape(integer_tape_pointer)
      OpenAD_Symbol_1 = 1
      do while (INT(OpenAD_Symbol_1).LE.INT(OpenAD_Symbol_0))
        integer_tape_pointer = integer_tape_pointer-1
        OpenAD_Symbol_17 = integer_tape(integer_tape_pointer)
        double_tape_pointer = double_tape_pointer-1
        OpenAD_Symbol_18 = double_tape(double_tape_pointer)
        DFLDDT(OpenAD_Symbol_17)%d = DFLDDT(OpenAD_Symbol_17)%d+FLDNEW(O
     +penAD_Symbol_17)%d*(OpenAD_Symbol_18)
        FLDNEW(OpenAD_Symbol_17)%d = 0.0d0
        OpenAD_Symbol_1 = INT(OpenAD_Symbol_1)+1
      END DO
      integer_tape_pointer = integer_tape_pointer-1
      OpenAD_Symbol_2 = integer_tape(integer_tape_pointer)
      IF (OpenAD_Symbol_2.ne.0) THEN
        double_tape_pointer = double_tape_pointer-1
        OpenAD_Symbol_12 = double_tape(double_tape_pointer)
        OpenAD_prp_2%d = OpenAD_prp_2%d+DFLDDT(3)%d*(OpenAD_Symbol_12)
        DFLDDT(3)%d = 0.0d0
        FLDNOW(3)%d = FLDNOW(3)%d-OpenAD_prp_2%d
        FLDNOW(2)%d = FLDNOW(2)%d+OpenAD_prp_2%d
        OpenAD_prp_2%d = 0.0d0
        double_tape_pointer = double_tape_pointer-1
        OpenAD_Symbol_13 = double_tape(double_tape_pointer)
        double_tape_pointer = double_tape_pointer-1
        OpenAD_Symbol_14 = double_tape(double_tape_pointer)
        FLDNOW(2)%d = FLDNOW(2)%d+DFLDDT(2)%d*(OpenAD_Symbol_13)
        OpenAD_prp_1%d = OpenAD_prp_1%d+DFLDDT(2)%d*(OpenAD_Symbol_14)
        DFLDDT(2)%d = 0.0d0
        FLDNOW(2)%d = FLDNOW(2)%d-OpenAD_prp_1%d
        FLDNOW(1)%d = FLDNOW(1)%d+OpenAD_prp_1%d
        OpenAD_prp_1%d = 0.0d0
        double_tape_pointer = double_tape_pointer-1
        OpenAD_Symbol_15 = double_tape(double_tape_pointer)
        double_tape_pointer = double_tape_pointer-1
        OpenAD_Symbol_16 = double_tape(double_tape_pointer)
        FLDNOW(1)%d = FLDNOW(1)%d+DFLDDT(1)%d*(OpenAD_Symbol_15)
        OpenAD_prp_0%d = OpenAD_prp_0%d+DFLDDT(1)%d*(OpenAD_Symbol_16)
        DFLDDT(1)%d = 0.0d0
        FLDNOW(1)%d = FLDNOW(1)%d-OpenAD_prp_0%d
        FLDNOW(3)%d = FLDNOW(3)%d+OpenAD_prp_0%d
        OpenAD_prp_0%d = 0.0d0
      ELSE
        double_tape_pointer = double_tape_pointer-1
        OpenAD_Symbol_19 = double_tape(double_tape_pointer)
        OpenAD_prp_5%d = OpenAD_prp_5%d+DFLDDT(3)%d*(OpenAD_Symbol_19)
        DFLDDT(3)%d = 0.0d0
        FLDNOW(3)%d = FLDNOW(3)%d-OpenAD_prp_5%d
        FLDNOW(1)%d = FLDNOW(1)%d+OpenAD_prp_5%d
        OpenAD_prp_5%d = 0.0d0
        double_tape_pointer = double_tape_pointer-1
        OpenAD_Symbol_20 = double_tape(double_tape_pointer)
        double_tape_pointer = double_tape_pointer-1
        OpenAD_Symbol_21 = double_tape(double_tape_pointer)
        FLDNOW(2)%d = FLDNOW(2)%d+DFLDDT(2)%d*(OpenAD_Symbol_20)
        OpenAD_prp_4%d = OpenAD_prp_4%d+DFLDDT(2)%d*(OpenAD_Symbol_21)
        DFLDDT(2)%d = 0.0d0
        FLDNOW(2)%d = FLDNOW(2)%d-OpenAD_prp_4%d
        FLDNOW(3)%d = FLDNOW(3)%d+OpenAD_prp_4%d
        OpenAD_prp_4%d = 0.0d0
        double_tape_pointer = double_tape_pointer-1
        OpenAD_Symbol_22 = double_tape(double_tape_pointer)
        double_tape_pointer = double_tape_pointer-1
        OpenAD_Symbol_23 = double_tape(double_tape_pointer)
        FLDNOW(1)%d = FLDNOW(1)%d+DFLDDT(1)%d*(OpenAD_Symbol_22)
        OpenAD_prp_3%d = OpenAD_prp_3%d+DFLDDT(1)%d*(OpenAD_Symbol_23)
        DFLDDT(1)%d = 0.0d0
        FLDNOW(1)%d = FLDNOW(1)%d-OpenAD_prp_3%d
        FLDNOW(2)%d = FLDNOW(2)%d+OpenAD_prp_3%d
        OpenAD_prp_3%d = 0.0d0
      ENDIF

C adjoint end
            our_rev_mode%arg_store=.FALSE.
            our_rev_mode%arg_restore=.TRUE.
            our_rev_mode%plain=.FALSE.
            our_rev_mode%tape=.TRUE.
            our_rev_mode%adjoint=.FALSE.
          end if
        end subroutine box_timestep
