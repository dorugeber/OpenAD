

      subroutine template()
      use OAD_tape
      use OAD_rev

!$TEMPLATE_PRAGMA_DECLARATIONS

      type(modeType) :: our_orig_mode

c lovcal vars:

      INTEGER myThidHelper
      Real*8 phip(1-OLx:sNx+OLx,1-OLy:sNy+OLy,myNz,nSx,nSy)


      if (our_rev_mode%plain) then
         Phip = Phi%v
c set up for plain execution
         our_orig_mode=our_rev_mode
         our_rev_mode%arg_store=.FALSE.
         our_rev_mode%arg_restore=.FALSE.
         our_rev_mode%plain=.TRUE.
         our_rev_mode%tape=.FALSE.
         our_rev_mode%adjoint=.FALSE.
         call EXCH_Z_3D_RS( Phip,myNz,myThid )
c reset the mode
         our_rev_mode=our_orig_mode
c copy back
         Phi%v = Phip
      end if
      if (our_rev_mode%tape) then

         mythidHelper=mythid
         Phip = Phi%v
c set up for plain execution
         our_orig_mode=our_rev_mode
         our_rev_mode%arg_store=.FALSE.
         our_rev_mode%arg_restore=.FALSE.
         our_rev_mode%plain=.TRUE.
         our_rev_mode%tape=.FALSE.
         our_rev_mode%adjoint=.FALSE.
         call EXCH_Z_3D_RS( Phip,myNz,myThid )
c reset the mode
         our_rev_mode=our_orig_mode
c copy back
         Phi%v = Phip
         if(oad_it_sz.lt. oad_it_ptr) call oad_it_grow()
         oad_it(oad_it_ptr)=mythidHelper; oad_it_ptr=oad_it_ptr+1
      end if
      if (our_rev_mode%adjoint) then
      oad_it_ptr=oad_it_ptr-1
      mythidHelper=oad_it(oad_it_ptr)
         Phip = Phi%d
c set up for plain execution
         our_orig_mode=our_rev_mode
         our_rev_mode%arg_store=.FALSE.
         our_rev_mode%arg_restore=.FALSE.
         our_rev_mode%plain=.TRUE.
         our_rev_mode%tape=.FALSE.
         our_rev_mode%adjoint=.FALSE.
         call ADEXCH_Z_3D_RS(Phip,myNz, myThidHelper )
c reset the mode
         our_rev_mode=our_orig_mode
c copy back
         Phi%d = Phip
      end if


      end subroutine template
