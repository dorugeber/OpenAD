      MODULE oad_intrinsics
      use OAD_active
      use w2f__types
      IMPLICIT NONE
      SAVE
      END MODULE
      MODULE all_globals_mod
      use OAD_active
      use w2f__types
      IMPLICIT NONE
      SAVE
      END MODULE
      subroutine common_cb_init()
      	use OAD_active
      	COMMON /cb/ G
      	type(active) :: G
      	G%d = 0
      end subroutine
