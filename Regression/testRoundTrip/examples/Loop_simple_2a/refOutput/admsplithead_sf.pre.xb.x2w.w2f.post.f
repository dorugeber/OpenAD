
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
      SUBROUTINE head(X, Y)
          use OAD_tape
          use OAD_rev

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
      INTEGER(w2f__i8) OpenAD_Symbol_12
      INTEGER(w2f__i8) OpenAD_Symbol_13
      INTEGER(w2f__i8) OpenAD_Symbol_14
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
      type(active) :: X(1:3)
      type(active) :: Y(1:3)
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) J
      INTEGER(w2f__i4) K
      INTEGER(w2f__i4) OpenAD_Symbol_15
      INTEGER(w2f__i4) OpenAD_Symbol_16
      INTEGER(w2f__i4) OpenAD_Symbol_17
      REAL(w2f__8) OpenAD_Symbol_18
      REAL(w2f__8) OpenAD_Symbol_19
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      type(active) :: OpenAD_prp_0


          integer iaddr
          external iaddr
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C

         if (our_rev_mode%plain) then
C original function
C$OPENAD XXX Template ad_template.f
      DO I = 1, 3, 1
        Y(INT(I))%v = X(I)%v
      END DO
C$OPENAD XXX Simple loop
      DO I = 1,3,1
        DO J = 1,3,1
          DO K = 1,3,1
            Y(INT(I))%v = (X(J)%v*Y(I)%v)
          END DO
        END DO
      END DO
          end if
          if (our_rev_mode%tape) then
C taping
C$OPENAD XXX Template ad_template.f
      OpenAD_Symbol_2 = 0_w2f__i8
      DO I = 1,3,1
        Y(INT(I))%v = X(I)%v
        integer_tape(integer_tape_pointer) = I
        integer_tape_pointer = integer_tape_pointer+1
        OpenAD_Symbol_2 = (INT(OpenAD_Symbol_2)+INT(1_w2f__i8))
      END DO
      integer_tape(integer_tape_pointer) = OpenAD_Symbol_2
      integer_tape_pointer = integer_tape_pointer+1
C$OPENAD XXX Simple loop
      DO I = 1,3,1
        DO J = 1,3,1
          DO K = 1,3,1
            OpenAD_lin_0 = Y(I)%v
            OpenAD_lin_1 = X(J)%v
            Y(INT(I))%v = (X(J)%v*Y(I)%v)
            double_tape(double_tape_pointer) = OpenAD_lin_0
            double_tape_pointer = double_tape_pointer+1
            double_tape(double_tape_pointer) = OpenAD_lin_1
            double_tape_pointer = double_tape_pointer+1
          END DO
        END DO
      END DO
          end if
          if (our_rev_mode%adjoint) then
C adjoint
      I = 1+1*((3-1)/1)
      do while (I.GE.1)
        J = 1+1*((3-1)/1)
        do while (J.GE.1)
          K = 1+1*((3-1)/1)
          do while (K.GE.1)
            double_tape_pointer = double_tape_pointer-1
            OpenAD_Symbol_18 = double_tape(double_tape_pointer)
            double_tape_pointer = double_tape_pointer-1
            OpenAD_Symbol_19 = double_tape(double_tape_pointer)
            OpenAD_prp_0%d = OpenAD_prp_0%d+Y(I)%d*(OpenAD_Symbol_18)
            X(J)%d = X(J)%d+Y(I)%d*(OpenAD_Symbol_19)
            Y(I)%d = 0.0d0
            Y(I)%d = Y(I)%d+OpenAD_prp_0%d
            OpenAD_prp_0%d = 0.0d0
            K = K-1
          END DO
          J = J-1
        END DO
        I = I-1
      END DO
      integer_tape_pointer = integer_tape_pointer-1
      OpenAD_Symbol_0 = integer_tape(integer_tape_pointer)
      OpenAD_Symbol_1 = 1
      do while (INT(OpenAD_Symbol_1).LE.INT(OpenAD_Symbol_0))
        integer_tape_pointer = integer_tape_pointer-1
        OpenAD_Symbol_15 = integer_tape(integer_tape_pointer)
        X(OpenAD_Symbol_15)%d = X(OpenAD_Symbol_15)%d+Y(OpenAD_Symbol_15
     +)%d
        Y(OpenAD_Symbol_15)%d = 0.0d0
        OpenAD_Symbol_1 = INT(OpenAD_Symbol_1)+1
      END DO
          end if
        end subroutine head
