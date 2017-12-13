

      subroutine template()
      use OAD_cp
      use OAD_tape
      use OAD_rev

!$TEMPLATE_PRAGMA_DECLARATIONS

      integer :: cp_loop_variable_1,cp_loop_variable_2,
     +     cp_loop_variable_3,cp_loop_variable_4

      integer iaddr
      external iaddr


      if (our_rev_mode%plain) then
!$PLACEHOLDER_PRAGMA$ id=1
      end if
      if (our_rev_mode%tape .and..not.our_rev_mode%strictAnonymous) then
!$PLACEHOLDER_PRAGMA$ id=2
      end if 
      if (our_rev_mode%adjoint 
     +.and. .not.our_rev_mode%strictAnonymous) then
!$PLACEHOLDER_PRAGMA$ id=3
      end if 
      if (our_rev_mode%tape .and. our_rev_mode%strictAnonymous) then
!$PLACEHOLDER_PRAGMA$ id=10
      end if 
      if (our_rev_mode%adjoint 
     +.and. our_rev_mode%strictAnonymous) then
!$PLACEHOLDER_PRAGMA$ id=11
      end if 


      end subroutine template
