PROGRAM example
  USE revolve
  IMPLICIT NONE
  INTEGER :: checkpoints,steps, guessedCheckpoints,verbosity=0
  TYPE(rvAction) :: theAction
  LOGICAL :: initialized=.FALSE.
  CHARACTER(80) :: errorMsg
  DO WHILE (verbosity<1 .OR. verbosity >3)
     WRITE (*,FMT='(A)',ADVANCE='NO') 'verbosity level (1=summary, 2=checkpoints, 3=all actions)? :'
     READ (*,*) verbosity
     if (verbosity<1 .OR. verbosity >3) THEN 
        WRITE(*,*) 'invalid entry, try again'
     ELSE
        CALL rvVerbose(verbosity)
     ENDIF
  END DO
  DO WHILE (.NOT.initialized)
     WRITE (*,FMT='(A)',ADVANCE='NO') 'number of steps? :'
     READ (*,*) steps
     guessedCheckPoints=rvGuess(steps)
     IF (guessedCheckPoints==-1) THEN 
        WRITE(*,*) 'rvGuess had an error'
        CYCLE
     END IF 
     WRITE (*,'(A,I8,A,F8.4)') 'revolve suggests :', guessedCheckPoints, ' checkpoints for a factor of ', &
          rvFactor(steps,guessedCheckPoints)
     WRITE (*,FMT='(A)',ADVANCE='NO') 'how many checkpoints are allowed? :'
     READ (*,*) checkpoints
     initialized=rvInit(steps,checkpoints,errorMsg)
     IF (.NOT.initialized) WRITE(*,'(A,A)') 'Error: ', errorMsg
  END DO
  if (verbosity>1) WRITE(*,'(A)') 'actions:'
  DO WHILE (theAction%actionFlag/=rvDone)
     theAction=rvNextAction()
  END DO
END PROGRAM example
