module OAD_intrinsics
end module
      module theModule
      use OAD_intrinsics
!       module adapted from SCALE6: src/scalelib/ki3_I.f90
!       Revision: $Revision: 1.2 $
!       Author: $Author: LMPetrie $
!       Date: $Date: 2007/11/01 14:51:19 $
!       State: $State: Stab $
!       Locker: $Locker:  $

        implicit none
        
        real, parameter, public :: ad = 3.0D0
        private
        public :: f
        public :: oad_s_f
        
        contains
        function f(x)
!         third order bickley function by the rational expansion of
!         pomentale and gargantini
          double precision :: x,f
          if (x<=0) then
            f = 1.0D0
          elseif (x<=39) then
            f = 2.0D0
          else
            f = ad
          end if
        end function f
        subroutine oad_s_f(x,f)
!         third order bickley function by the rational expansion of
!         pomentale and gargantini
          double precision :: x
          double precision,intent(out) :: f
          if (x<=0) then
            f = 1.0D0
          elseif (x<=39) then
            f = 2.0D0
          else
            f = ad
          end if
        end subroutine oad_s_f
      end module theModule

      program moduleContainsPublicFunction
        use OAD_intrinsics
        use theModule
        implicit none
        double precision :: x,y
        double precision :: oad_ctmp0
        x=ad
        call oad_s_f(x,oad_ctmp0)
        y = oad_ctmp0
        print *,'y = ', y
      end program moduleContainsPublicFunction

