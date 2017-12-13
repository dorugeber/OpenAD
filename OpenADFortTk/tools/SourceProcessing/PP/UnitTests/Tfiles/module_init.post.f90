MODULE globals
  use OAD_active
  IMPLICIT NONE
  SAVE
  type(active) :: GX
  type(active) :: GY
contains
subroutine mod_globals_init()
	use OAD_active
	GX%d = 0
	GY%d = 0
end subroutine
  
END MODULE globals

SUBROUTINE bar(BARX, BARY)
  use OAD_active
  use globals
  IMPLICIT NONE
  REAL BARX
  REAL BARY
  GX%v = BARX*2
  GY%v = BARY*2
end subroutine
subroutine OAD_globalVar_init()
	use globals
	call mod_globals_init()
end subroutine
