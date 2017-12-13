MODULE globals
  IMPLICIT NONE
  SAVE
  
  TYPE (oadactive) GX
  TYPE (oadactive) GY
  
END MODULE globals

SUBROUTINE bar(BARX, BARY)
  use globals
  IMPLICIT NONE
  
  REAL BARX
  REAL BARY
  
  GX%v = BARX*2
  GY%v = BARY*2
END SUBROUTINE bar
