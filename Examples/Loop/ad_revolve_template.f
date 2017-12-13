        subroutine template()
          use OAD_tape
          use OAD_rev
          use OAD_cp
          use revolve
!$TEMPLATE_PRAGMA_DECLARATIONS
! external C function used in inlined code
          integer iaddr
          external iaddr
          LOGICAL :: initialized=.FALSE.
          TYPE(rvAction) :: theAction
          CHARACTER(80) :: errorMsg
          integer :: cpCount
          integer :: currIter
          currIter=0
          write (*,fmt='(a)',advance='no') 'number of checkpoints = '
          read (*,*) cpCount 
          CALL rvVerbose(3)
          initialized=rvInit(n,cpCount,errorMsg)
          IF (.NOT.initialized) WRITE(*,'(A,A)') 'Error: ', errorMsg
          do while (theAction%actionFlag/=rvDone)
             theAction=rvNextAction()
             select case (theAction%actionFlag) 
             case (rvStore)
                call cp_write_open(theAction%cpNum)
!$PLACEHOLDER_PRAGMA$ id=4
                call cp_close
             case (rvRestore)
                call cp_read_open(theAction%cpNum)
!$PLACEHOLDER_PRAGMA$ id=6
                currIter=theAction%iteration
                call cp_close
             case (rvForward)
                call OAD_revPlain
                do currIter=currIter,theAction%iteration-1
                   call loopBody(x)
                end do
             case (rvFirstUTurn,rvUTurn)
                call OAD_revTape
                call loopBody(x)
                call OAD_revAdjoint
                call loopBody(x)
             end select   
          end do
        end subroutine template
