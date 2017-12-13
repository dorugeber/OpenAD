

      subroutine template()
      use OAD_tape
      use OAD_rev

!$TEMPLATE_PRAGMA_DECLARATIONS

      type(modeType) :: our_orig_mode

c lovcal vars:

      LOGICAL withSignsHelper
      INTEGER myThidHelper
      Real*8 Uphip(1-OLx:sNx+OLx,1-OLy:sNy+OLy,Nr,nSx,nSy)
      Real*8 Vphip(1-OLx:sNx+OLx,1-OLy:sNy+OLy,Nr,nSx,nSy)



      if (our_rev_mode%plain) then
         Uphip = Uphi%v
         Vphip = Vphi%v
c set up for plain execution
         our_orig_mode=our_rev_mode
         our_rev_mode%arg_store=.FALSE.
         our_rev_mode%arg_restore=.FALSE.
         our_rev_mode%plain=.TRUE.
         our_rev_mode%tape=.FALSE.
         our_rev_mode%adjoint=.FALSE.
         call EXCH_UV_XYZ_RL( Uphip,Vphip, withSigns, myThid )
c reset the mode
         our_rev_mode=our_orig_mode
c copy back
         Uphi%v = Uphip
         Vphi%v = Vphip
      end if
      if (our_rev_mode%tape) then

         withSignsHelper=withSigns
         mythidHelper=mythid
         Uphip = Uphi%v
         Vphip = Vphi%v
c set up for plain execution
         our_orig_mode=our_rev_mode
         our_rev_mode%arg_store=.FALSE.
         our_rev_mode%arg_restore=.FALSE.
         our_rev_mode%plain=.TRUE.
         our_rev_mode%tape=.FALSE.
         our_rev_mode%adjoint=.FALSE.
         call EXCH_UV_XYZ_RL( Uphip,Vphip, withSigns, myThid )
c reset the mode
         our_rev_mode=our_orig_mode
c copy back
         Uphi%v = Uphip
         Vphi%v = Vphip
         if(oad_lt_sz.lt. oad_lt_ptr) call oad_lt_grow()
         oad_lt(oad_lt_ptr)=withSignsHelper; oad_lt_ptr=oad_lt_ptr+1
         if(oad_it_sz.lt. oad_it_ptr) call oad_it_grow()
         oad_it(oad_it_ptr)=mythidHelper; oad_it_ptr=oad_it_ptr+1
      end if
      if (our_rev_mode%adjoint) then
      oad_it_ptr=oad_it_ptr-1
      mythidHelper=oad_it(oad_it_ptr)
      oad_lt_ptr=oad_lt_ptr-1
      withSignsHelper=oad_lt(oad_lt_ptr)
         Uphip = Uphi%d
         Vphip = Vphi%d
c set up for plain execution
         our_orig_mode=our_rev_mode
         our_rev_mode%arg_store=.FALSE.
         our_rev_mode%arg_restore=.FALSE.
         our_rev_mode%plain=.TRUE.
         our_rev_mode%tape=.FALSE.
         our_rev_mode%adjoint=.FALSE.
         call ADEXCH_UV_XYZ_RL(Uphip,Vphip,withSignsHelper,myThidHelper)
c reset the mode
         our_rev_mode=our_orig_mode
c copy back
         Uphi%d = Uphip
         Vphi%d = Vphip
      end if


      end subroutine template
