! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
! +                                                           +
! +  glimmer_pmpt.f90 - part of the GLIMMER ice model         + 
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





module glimmer_pmpt

  use glimmer_global, only: dp

  implicit none

contains

  !-----------------------------------------------------------------------------------

!!$  function pmpt(thck)
!!$    !*FD Wrapper function for pressure-melting-point calculation
!!$
!!$    real(dp),intent(in) :: thck
!!$    real(dp) :: tmp
!!$    real(dp) :: pmpt
!!$
!!$    call calcpmptb(tmp,thck)
!!$    pmpt=tmp
!!$
!!$  end function pmpt

  !-------------------------------------------------------------------

  subroutine corrpmpt(temp,thck,bwat,sigma,nz)
    use glimmer_global, only : dp

    implicit none 
    integer :: nz
    real(dp), dimension(nz), intent(inout) :: temp
    real(dp), intent(in) :: thck, bwat
    real(dp),dimension(nz),intent(in) :: sigma

    real(dp), dimension(nz) :: pmptemp

    ! corrects a temperature column for melting point effects
    ! 1. if temperature at any point in column is above pressure melting point then 
    ! set temperature to pressure melting point 
    ! 2. if bed is wet set basal temperature to pressure melting point 

    call calcpmpt(pmptemp,thck,sigma)

    temp = min(temp,pmptemp)

    if (bwat > 0.0d0) temp(nz) = pmptemp(nz)

  end subroutine corrpmpt

  !-------------------------------------------------------------------

  subroutine calcpmpt(pmptemp,thck,sigma)
    !*FD Returns the pressure melting point of water (degC)

    use glimmer_global, only : dp !, upn
    use physcon, only : rhoi, grav, pmlt 

    implicit none 

    real(dp), dimension(:), intent(out) :: pmptemp
    real(dp), intent(in) :: thck
    real(dp),intent(in),dimension(:) :: sigma
    integer :: nz,i

    real(dp), parameter :: fact = - grav * rhoi * pmlt

    nz = size(sigma)
    do i = 1,nz
       pmptemp(i) = fact * thck * sigma(i)
    end do

  end subroutine calcpmpt
  
  !-----------------------------------------------------------------------------------

  subroutine calcbpmp(thck,bpmp)
    use glimmer_global, only: dp

    ! Calculate the pressure melting point at the base of the ice 

    real(dp), dimension(:,:), intent(in)  :: thck
    real(dp), dimension(:,:), intent(out) :: bpmp

    integer :: ew,ns,ewn,nsn

    ewn = size(thck,1)
    nsn = size(thck,2)

    bpmp = 0.d0

    do ns = 2, nsn-1
       do ew = 2, ewn-1
          call calcpmptb(bpmp(ew,ns),thck(ew,ns))
       end do
    end do

  end subroutine calcbpmp

  !-------------------------------------------------------------------

  subroutine calcpmptb(pmptemp,thck)
    use glimmer_global, only : dp
    use physcon, only : rhoi, grav, pmlt 

    implicit none 

    real(dp), intent(out) :: pmptemp
    real(dp), intent(in)  :: thck

    real(dp), parameter :: fact = - grav * rhoi * pmlt

    pmptemp = fact * thck 

  end subroutine calcpmptb


end module glimmer_pmpt
