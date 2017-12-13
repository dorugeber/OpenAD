        subroutine template()
          use OAD_tape
          use OAD_rev
          use OAD_cp
          use revolve
          use RevStats
!$TEMPLATE_PRAGMA_DECLARATIONS
          LOGICAL :: initialized=.FALSE.
          TYPE(rvAction) :: theAction
          CHARACTER(80) :: errorMsg
          integer :: jointCPCount
          integer :: currIter
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
            DO ILOOP = 1, N_MAX, 1
              CALL box_forward(ILOOP)
            END DO
            our_rev_mode=our_orig_mode
          end if 
          if (our_rev_mode%tape) then
             currIter=0
             jointCPcount=cp_fNumber()
!             CALL rvVerbose(3)
             initialized=rvInit(n_max,revStatsRevolveCPcount,errorMsg)
             IF (.NOT.initialized) WRITE(*,'(A,A)') 'Error: ', errorMsg
             do while (theAction%actionFlag/=rvDone)
                theAction=rvNextAction()
                select case (theAction%actionFlag) 
                case (rvStore)
                  call cp_write_open(theAction%cpNum+jointCPCount)
                  call revStatsUpdateCP(cp_fNumber()+1)
!$PLACEHOLDER_PRAGMA$ id=4
                  call cp_close
                case (rvRestore)
                  call cp_read_open(theAction%cpNum+jointCPCount)
!$PLACEHOLDER_PRAGMA$ id=6
                  currIter=theAction%iteration
                  call cp_close
                case (rvForward)
                  call OAD_revPlain
                  do currIter=currIter,theAction%iteration-1
                    call box_forward(currIter+1)
                  end do
                case (rvFirstUTurn,rvUTurn)
                  call OAD_revTape
                  call box_forward(currIter+1)
                  call OAD_revAdjoint
                  call box_forward(currIter+1)
                end select   
              end do
              call OAD_revRestoreTape
            end if 
       end subroutine template
