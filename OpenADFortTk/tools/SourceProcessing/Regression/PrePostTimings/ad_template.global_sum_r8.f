

      subroutine template()
      use OAD_rev

!$TEMPLATE_PRAGMA_DECLARATIONS

      type(modeType) :: our_orig_mode

c lovcal vars:

      real*8 sumPhip


      if (our_rev_mode%plain .OR. our_rev_mode%tape) then
         sumPhip = sumPhi%v
c set up for plain execution
         our_orig_mode=our_rev_mode
         our_rev_mode%arg_store=.FALSE.
         our_rev_mode%arg_restore=.FALSE.
         our_rev_mode%plain=.TRUE.
         our_rev_mode%tape=.FALSE.
         our_rev_mode%adjoint=.FALSE.
         call global_sum_r8( sumPhip,myThid )
c reset the mode
         our_rev_mode=our_orig_mode
c copy back
         sumPhi%v = sumPhip
      end if
      if (our_rev_mode%adjoint) then
         sumPhip = sumPhi%d
c set up for plain execution
         our_orig_mode=our_rev_mode
         our_rev_mode%arg_store=.FALSE.
         our_rev_mode%arg_restore=.FALSE.
         our_rev_mode%plain=.TRUE.
         our_rev_mode%tape=.FALSE.
         our_rev_mode%adjoint=.FALSE.
         call global_adsum_r8(sumPhip, myThid)
c reset the mode
         our_rev_mode=our_orig_mode
c copy back
         sumPhi%d = sumPhip
      end if


      end subroutine template
