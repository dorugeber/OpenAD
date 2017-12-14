c$openad XXX Template OADrts/ad_template.joint.f
      subroutine cost_depth( cf )

      use size
      use pfields
      use data
      use weights

      implicit none

c     interface
      real cf
c     end interface
cju #include "size.inc"
cju #include "pfields.inc"
cju #include "data.inc"
cju #include "weights.inc"

c     locals
      integer ix, iy, jx, jy
c$openad xxx simple loop
      do iy = 1, ny
         do jy = 1, ny
            do ix = 1, nx
               do jx = 1, nx
                  cf = cf 
     &                 + .5*(depth(ix,iy)-depth_data(ix,iy))
     &                 *(depth(jx,jy)-depth_data(jx,jy))
ctest     &                 *weight_depth(ix,jx,iy,jy)
               end do
            end do
         end do
      end do

      end ! subroutine cost_depth

