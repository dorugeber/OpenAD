subroutine bar()
  use OAD_active
  implicit none
  save /cb/
  common /cb/ G
  type(active) :: G
end subroutine
subroutine common_cb_init()
	use OAD_active
	common /cb/ G
	type(active) :: G
	g%d = 0
end subroutine
subroutine OAD_globalVar_init()
	call common_cb_init()
end subroutine
