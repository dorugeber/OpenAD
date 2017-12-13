module OAD_intrinsics
end module
MODULE PMR4_CFDCV
use OAD_intrinsics

!IMPLICIT NONE

PRIVATE
PUBLIC :: PMR4_CFDCVInit
public :: oad_s_pmr4_cfdcvinit


CONTAINS

!-------------------------------------------------------------------------------
!
!       PMR4_CFDCVInit - Perform CFD-CV Initialization
!
!-------------------------------------------------------------------------------
FUNCTION PMR4_CFDCVInit(inNCFDCV) RESULT(status)

        IMPLICIT NONE
        INTEGER(KIND=KIND(1)), INTENT(in) :: inNCFDCV
        INTEGER(KIND=KIND(1)) :: status
        status=1
        return

END FUNCTION PMR4_CFDCVInit

!-------------------------------------------------------------------------------
!
!       PMR4_CFDCVInit - Perform CFD-CV Initialization
!
!-------------------------------------------------------------------------------
subroutine oad_s_pmr4_cfdcvinit(inNCFDCV,status)

        IMPLICIT NONE
        INTEGER(KIND=KIND(1)), INTENT(in) :: inNCFDCV
        integer(kind = KIND(1)),intent(out) :: status
        status=1
        return
end subroutine oad_s_pmr4_cfdcvinit

end module

program p
use OAD_intrinsics
use PMR4_CFDCV
  integer :: oad_ctmp0
i=2
call oad_s_PMR4_CFDCVInit(i,oad_ctmp0)
j = oad_ctmp0
end
