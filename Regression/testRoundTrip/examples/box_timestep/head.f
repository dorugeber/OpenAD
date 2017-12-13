c$openad XXX Template ad_template.f
c-----------------------------------------------------------------------
      subroutine box_timestep (
     & gammaLoc, fldStar, extForLoc,
     & uVelLoc, fldNow, fldOld, fldNew )
c-----------------------------------------------------------------------

      implicit none

      integer l

c -- routine arguments:
      double precision uVelLoc
      double precision gammaLoc
      double precision fldStar(3 -1)
      double precision extForLoc(3 -1)
      double precision fldNow(3)
      double precision fldNew(3)
      double precision fldOld(3)

c-- local variables:
      double precision vol(3)
      double precision delta_t
      double precision dFldDt(3)
c$openad INDEPENDENT(fldnow)
c -- initialization
      vol(1)=10.D0
      vol(2)=12.D0
      vol(3)=14.D0
      delta_t=20.D0

c-- routine body

      if ( uVelLoc .GE. 0. ) then
         dFldDt(1) =
     & ( extForLoc(1)
     & + gammaLoc*( fldStar(1) - fldNow(1) )*vol(1)
     & + uVelLoc* ( fldNow(3) - fldNow(1) ) ) / vol(1)
         dFldDt(2) =
     & ( extForLoc(2)
     & + gammaLoc*( fldStar(2) - fldNow(2) )*vol(2)
     & + uVelLoc* ( fldNow(1) - fldNow(2) ) ) / vol(2)
         dFldDt(3) =
     & uVelLoc*( fldNow(2) - fldNow(3) ) / vol(3)
      else
         dFldDt(1) =
     & ( extForLoc(1)
     & + gammaLoc*( fldStar(1) - fldNow(1) )*vol(1)
     & - uVelLoc* ( fldNow(2) - fldNow(1) ) ) / vol(1)
         dFldDt(2) =
     & ( extForLoc(2)
     & + gammaLoc*( fldStar(2) - fldNow(2) )*vol(2)
     & - uVelLoc* ( fldNow(3) - fldNow(2) ) ) / vol(2)
         dFldDt(3) =
     & -uVelLoc*( fldNow(1) - fldNow(3) ) / vol(3)
      end if

      do l = 1, 3
         fldNew(l) = fldOld(l) + 2.D0*delta_t*dFldDt(l)
      enddo
c$openad DEPENDENT(fldnew)
      end
