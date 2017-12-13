MODULE PMR4_CFDCV

!IMPLICIT NONE

PRIVATE
PUBLIC  :: PMR4_CFDCVInit


CONTAINS

!-------------------------------------------------------------------------------
!
!       PMR4_CFDCVInit - Perform CFD-CV Initialization
!
!-------------------------------------------------------------------------------
FUNCTION PMR4_CFDCVInit(inNCFDCV) RESULT(status)

        IMPLICIT NONE
        INTEGER(KIND=KIND(1)),                  INTENT(in)              :: inNCFDCV
        INTEGER(KIND=KIND(1))                                                   :: status
        status=1
        return;

END FUNCTION PMR4_CFDCVInit

end module

program p 
use PMR4_CFDCV
i=2
j=PMR4_CFDCVInit(i)
end 
