        subroutine template()
          use RevStats
          use OAD_tape
          use OAD_rev
          use OAD_cp

!$TEMPLATE_PRAGMA_DECLARATIONS

          type(modeType) :: our_orig_mode

          if (our_rev_mode%arg_store) then 
             call cp_write_open
!$PLACEHOLDER_PRAGMA$ id=4
             call cp_close
          end if 
          if (our_rev_mode%arg_restore) then
             call cp_read_open
!$PLACEHOLDER_PRAGMA$ id=6
             call cp_close
          end if
          if (our_rev_mode%plain) then
            revStatsFCounters%__SRNAME__=revStatsFCounters%__SRNAME__+1
            our_orig_mode=our_rev_mode
            our_rev_mode%arg_store=.FALSE.
!$PLACEHOLDER_PRAGMA$ id=1
            our_rev_mode=our_orig_mode
          end if 
          if (our_rev_mode%tape) then
            revStatsFCounters%__SRNAME__=revStatsFCounters%__SRNAME__+1
            call OAD_revStorePlain
C            print*, " tape       ", our_rev_mode
!$PLACEHOLDER_PRAGMA$ id=2
            call OAD_revAdjoint
          end if 
          call revStatsUpdateTape(oad_dt_ptr-1)
          if (our_rev_mode%adjoint) then
             call OAD_revRestoreTape
C            print*, " adjoint    ", our_rev_mode
!$PLACEHOLDER_PRAGMA$ id=3
             call OAD_revRestoreTape
          end if 
          call revStatsUpdateCP(cp_fNumber())
        end subroutine template
