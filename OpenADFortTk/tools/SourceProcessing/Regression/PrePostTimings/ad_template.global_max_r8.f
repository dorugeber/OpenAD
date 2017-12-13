

      subroutine template()
      use OAD_rev

!$TEMPLATE_PRAGMA_DECLARATIONS

      type(modeType) :: our_orig_mode

c lovcal vars:

      real*8 maxPhip


      if (our_rev_mode%plain .OR. our_rev_mode%tape) then
         maxPhip = maxPhi%v
c set up for plain execution
         our_orig_mode=our_rev_mode
         our_rev_mode%arg_store=.FALSE.
         our_rev_mode%arg_restore=.FALSE.
         our_rev_mode%plain=.TRUE.
         our_rev_mode%tape=.FALSE.
         our_rev_mode%adjoint=.FALSE.
         call global_max_r8( maxPhip,myThid )
c reset the mode
         our_rev_mode=our_orig_mode
c copy back
         maxPhi%v = maxPhip
      end if
      if (our_rev_mode%adjoint) then
         maxPhip = maxPhi%d
c set up for plain execution
         our_orig_mode=our_rev_mode
         our_rev_mode%arg_store=.FALSE.
         our_rev_mode%arg_restore=.FALSE.
         our_rev_mode%plain=.TRUE.
         our_rev_mode%tape=.FALSE.
         our_rev_mode%adjoint=.FALSE.
         call global_admax_r8( maxPhip,myThid )
c reset the mode
         our_rev_mode=our_orig_mode
c copy back
         maxPhi%d = maxPhip
      end if


      end subroutine template
