! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
! +                                                           +
! +  glimmer_mask.f90 - part of the GLIMMER ice model         + 
! +                                                           +
! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
! 
! Copyright (C) 2004 GLIMMER contributors - see COPYRIGHT file 
! for list of contributors.
!
! This program is free software; you can redistribute it and/or 
! modify it under the terms of the GNU General Public License as 
! published by the Free Software Foundation; either version 2 of 
! the License, or (at your option) any later version.
!
! This program is distributed in the hope that it will be useful, 
! but WITHOUT ANY WARRANTY; without even the implied warranty of 
! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
! GNU General Public License for more details.
!
! You should have received a copy of the GNU General Public License 
! along with this program; if not, write to the Free Software 
! Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 
! 02111-1307 USA
!
! GLIMMER is maintained by:
!
! Ian Rutt
! School of Geographical Sciences
! University of Bristol
! University Road
! Bristol
! BS8 1SS
! UK
!
! email: <i.c.rutt@bristol.ac.uk> or <ian.rutt@physics.org>
!
! GLIMMER is hosted on berliOS.de:
!
! https://developer.berlios.de/projects/glimmer-cism/
!
! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++





module glimmer_mask
  !*FD masking ice thicknesses

  use glimmer_global, only: dp, sp

  implicit none

  integer, parameter :: glimmer_mask_ocean          = -2
  integer, parameter :: glimmer_mask_land           = -1
  integer, parameter :: glimmer_mask_boundary       = 0
  integer, parameter :: glimmer_mask_thin_ice       = 1
  integer, parameter :: glimmer_mask_interior       = 2
  integer, parameter :: glimmer_mask_shelf          = 4
  integer, parameter :: glimmer_mask_stream         = 8
  integer, parameter :: glimmer_mask_grounding_line = 16
  integer, parameter :: glimmer_mask_stream_margin  = 32
  integer, parameter :: glimmer_mask_land_margin    = 64
  integer, parameter :: glimmer_mask_shelf_front    = 128
  integer, parameter :: glimmer_mask_marine_edge    = 256

!contains
!!$
!!$  real(dp) function calc_ivol(mask,thck,dew,dns)
!!$    implicit none
!!$    
!!$    integer, dimension(:,:),intent(in) :: mask
!!$    real(dp),dimension(:,:),intent(in) :: thck
!!$    real(dp),               intent(in) :: dew
!!$    real(dp),               intent(in) :: dns
!!$
!!$    real(dp),dimension(size(thck,1),size(thck,2)) :: count
!!$
!!$    where (has_ice(mask))
!!$       count = 1.
!!$    elsewhere
!!$       count = 0.
!!$    end where
!!$
!!$    calc_ivol = sum(count*thck)*dew*dns
!!$
!!$  end function calc_ivol

!!$ 
!!$  real(dp) function calc_iarea(mask,thck,dew,dns)
!!$    implicit none
!!$    
!!$    integer, dimension(:,:),intent(in) :: mask
!!$    real(dp),dimension(:,:),intent(in) :: thck
!!$    real(dp),               intent(in) :: dew
!!$    real(dp),               intent(in) :: dns
!!$
!!$    real(dp),dimension(size(thck,1),size(thck,2)) :: count
!!$
!!$    where (has_ice(mask))
!!$       count = 1.
!!$    elsewhere
!!$       count = 0.
!!$    end where
!!$
!!$    calc_iarea = sum(count)*dew*dns
!!$
!!$  end function calc_iarea
 

!!$  subroutine glimmer_set_mask(mask,thck,topg,eus,thklim)
!!$    use physcon, only : rhoi, rhoo
!!$
!!$    implicit none
!!$
!!$    integer,  dimension(:,:), intent(out) :: mask
!!$    real(dp), dimension(:,:), intent(in)  :: thck
!!$    real(dp), dimension(:,:), intent(in)  :: topg
!!$    real(sp),                 intent(in)  :: eus
!!$    real(dp),                 intent(in)  :: thklim
!!$
!!$    ! local variables
!!$    integer ew,ns,ewn,nsn
!!$    real(dp), parameter :: con = - rhoi / rhoo
!!$
!!$    ewn = size(thck,1)
!!$    nsn = size(thck,2)
!!$
!!$    mask = 0
!!$
!!$    do ns=1,nsn
!!$       do ew = 1,ewn
!!$          
!!$          if (thck(ew,ns) .eq. 0.) then                 ! no ice
!!$             if (topg(ew,ns) .lt. eus) then             ! below SL
!!$                mask(ew,ns) = glimmer_mask_ocean
!!$             else                                       ! above SL
!!$                mask(ew,ns) = glimmer_mask_land
!!$             end if
!!$          else
!!$             if (topg(ew,ns) - eus &                    ! ice
!!$                  < con * thck(ew,ns)) then             ! floating ice
!!$                mask(ew,ns) = glimmer_mask_shelf
!!$             else                                       ! grounded ice
!!$                mask(ew,ns) = glimmer_mask_interior
!!$             end if
!!$             if (thck(ew,ns) .le. thklim) then          ! ice below dynamic limit
!!$                mask(ew,ns) = ior(mask(ew,ns),glimmer_mask_thin_ice)
!!$             end if
!!$          end if
!!$
!!$       end do
!!$    end do
!!$
!!$    ! finding boundaries
!!$    do ns=2,nsn-1
!!$       do ew = 2,ewn-1
!!$          if (is_float(mask(ew,ns))) then
!!$             ! shelf front
!!$             if (is_ocean(mask(ew-1,ns)) .or. is_ocean(mask(ew+1,ns)) .or. &
!!$                  is_ocean(mask(ew,ns-1)) .or. is_ocean(mask(ew,ns+1))) then
!!$                mask(ew,ns) = ior(mask(ew,ns),glimmer_mask_shelf_front)
!!$             end if
!!$          else if (is_ground(mask(ew,ns))) then
!!$             ! land margin
!!$             if (is_land(mask(ew-1,ns)) .or. is_land(mask(ew+1,ns)) .or. &
!!$                  is_land(mask(ew,ns-1)) .or. is_land(mask(ew,ns+1))) then
!!$                mask(ew,ns) = ior(mask(ew,ns),glimmer_mask_land_margin)
!!$             end if
!!$             ! grounding line
!!$             if (is_float(mask(ew-1,ns)) .or. is_float(mask(ew+1,ns)) .or. &
!!$                  is_float(mask(ew,ns-1)) .or. is_float(mask(ew,ns+1))) then
!!$                mask(ew,ns) = ior(mask(ew,ns),glimmer_mask_grounding_line)
!!$             end if
!!$          end if
!!$          ! Edge of marine ice, whether floating or not
!!$          if ((topg(ew,ns) .lt. eus.and.&
!!$               thck(ew,ns)>0.0).and. &
!!$               (is_ocean(mask(ew-1,ns)) .or. is_ocean(mask(ew+1,ns)) .or. &
!!$               is_ocean(mask(ew,ns-1)) .or. is_ocean(mask(ew,ns+1)))) then
!!$             mask(ew,ns) = ior(mask(ew,ns),glimmer_mask_marine_edge)
!!$          end if
!!$       end do
!!$    end do
!!$  end subroutine glimmer_set_mask

!!$  logical elemental function is_ocean(mask)
!!$    !*FD returns .true. if node is ocean
!!$    implicit none
!!$    integer, intent(in) :: mask 
!!$
!!$    is_ocean = mask.eq.glimmer_mask_ocean
!!$  end function is_ocean

!!$  logical elemental function is_land(mask)
!!$    !*FD returns .true. if node is land
!!$    implicit none
!!$    integer, intent(in) :: mask 
!!$
!!$    is_land = mask.eq.glimmer_mask_land
!!$  end function is_land

!!$  logical elemental function has_ice(mask)
!!$    !*FD returns .true. if node contains ice
!!$    implicit none
!!$    integer, intent(in) :: mask 
!!$
!!$    has_ice = mask .gt. 0
!!$  end function has_ice

!!$  logical elemental function is_thin(mask)
!!$    !*FD returns .true. if node is below dynamic limit
!!$    implicit none
!!$    integer, intent(in) :: mask
!!$
!!$    is_thin = (iand(mask,glimmer_mask_thin_ice) .gt. 0 .and. mask.gt.0)
!!$  end function is_thin

!!$  logical elemental function is_float(mask)
!!$    !*FD returns .true. if node is floating
!!$    implicit none
!!$    integer, intent(in) :: mask
!!$
!!$    is_float = (iand(mask,glimmer_mask_shelf) .gt. 0 .and. mask.gt.0)
!!$  end function is_float

!!$  logical elemental function is_ground(mask)
!!$    !*FD returns .true. if node is grounded ice
!!$    implicit none
!!$    integer, intent(in) :: mask
!!$
!!$    is_ground = (iand(mask,glimmer_mask_interior) .gt. 0 .and. mask.gt.0)
!!$  end function is_ground

!!$  logical elemental function is_calving(mask)
!!$    !*FD return .true. if node is at the shelf front
!!$    implicit none
!!$    integer, intent(in) :: mask
!!$
!!$    is_calving = (iand(mask,glimmer_mask_shelf_front) .gt. 0 .and. mask.gt.0)
!!$  end function is_calving

!!$  logical elemental function is_marine_ice_edge(mask)
!!$    !*FD return .true. if node is at edge of ice and topgraphy is
!!$    !*FD below sea-level
!!$    implicit none
!!$    integer, intent(in) :: mask
!!$
!!$    is_marine_ice_edge = (iand(mask,glimmer_mask_marine_edge) .gt. 0 .and. mask.gt.0)
!!$  end function is_marine_ice_edge

end module glimmer_mask
