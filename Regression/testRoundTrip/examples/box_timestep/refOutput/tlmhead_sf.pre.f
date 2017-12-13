!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
C$openad XXX Template ad_template.f
C-----------------------------------------------------------------------
      subroutine box_timestep ( gammaLoc, fldStar, extForLoc, uVelLoc, f
     +ldNow, fldOld, fldNew )
      use OAD_intrinsics
C-----------------------------------------------------------------------

      implicit none

      integer l

C -- routine arguments:
      double precision uVelLoc
      double precision gammaLoc
      double precision fldStar(3 -1)
      double precision extForLoc(3 -1)
      double precision fldNow(3)
      double precision fldNew(3)
      double precision fldOld(3)

C-- local variables:
      double precision vol(3)
      double precision delta_t
      double precision dFldDt(3)
C$openad INDEPENDENT(fldnow)
C -- initialization
      vol(1)=10.D0
      vol(2)=12.D0
      vol(3)=14.D0
      delta_t=20.D0

C-- routine body

      if (uVelLoc.GE.0.) then
         dFldDt(1) = (extForLoc(1)+gammaLoc*(fldStar(1)-fldNow(1))*vol(1
     +)+uVelLoc*(fldNow(3)-fldNow(1)))/vol(1)
         dFldDt(2) = (extForLoc(2)+gammaLoc*(fldStar(2)-fldNow(2))*vol(2
     +)+uVelLoc*(fldNow(1)-fldNow(2)))/vol(2)
         dFldDt(3) = uVelLoc*(fldNow(2)-fldNow(3))/vol(3)
      else
         dFldDt(1) = (extForLoc(1)+gammaLoc*(fldStar(1)-fldNow(1))*vol(1
     +)-uVelLoc*(fldNow(2)-fldNow(1)))/vol(1)
         dFldDt(2) = (extForLoc(2)+gammaLoc*(fldStar(2)-fldNow(2))*vol(2
     +)-uVelLoc*(fldNow(3)-fldNow(2)))/vol(2)
         dFldDt(3) = -uVelLoc*(fldNow(1)-fldNow(3))/vol(3)
      end if

      do l = 1, 3
         fldNew(l) = fldOld(l)+2.D0*delta_t*dFldDt(l)
      enddo
C$openad DEPENDENT(fldnew)
      end
