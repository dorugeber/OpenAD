! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
! +                                                               +
! + adism_mbal.F90: part of the ADISM adjoint ice sheet model     +
! +                                                               +
! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
! 
! Copyright (C) 2013 ADISM contributors - see COPYRIGHT file 
! for list of contributors.
!
! This program is free software; you can redistribute it and/or 
! modify it under the terms of the GNU General Public License as 
! published by the Free Software Foundation; either version 3 of 
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
! ADISM is maintained by:
!
! Ian Rutt, Swansea University (i.c.rutt@swansea.ac.uk)
! Jonathan McGovern, LGGE, France
!
! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

module adism_mbal

  ! The EISMINT 2 mass balance specification

  use glimmer_global, only: dp
  use physcon, only: scyr

  implicit none

contains

  subroutine mass_balance_mm(nmsb,dx,dy,n,m)  

    implicit none

    ! Dummy arguments
    integer                 :: n,m                   ! Number of grids
    real(dp),dimension(n,m) :: nmsb                  ! Surface mass balance
    real(dp)                :: dx,dy                 ! Grid spacing (m)

    ! Mass balance parameters
    real(dp),parameter      :: xs=750000,ys=750000   ! Distance to centre of ice sheet (m)
    real(dp),parameter      :: Re=450000             ! Distance at which mass balance changes 
                                                     ! from positive to negative values (m)
    real(dp),parameter      :: s=1e-2/1000.0         ! Slope

    ! Other variables
    real(dp),dimension(n,m) :: d                     ! Distance (m)

    call distance(d,xs,ys,dx,dy,n,m)

    nmsb=min(0.5D0,s*(Re-d))/scyr              ! Surface mass balance. Accumulation is maximum at summit

  end subroutine mass_balance_mm

  !#################################################################################################

  subroutine distance(d,xs,ys,dx,dy,n,m)

    implicit none

    integer             :: n,m
    real(dp),dimension(n,m) :: d                     ! Distance (m)
    real(dp)                :: xs,ys                 ! Distance to centre of ice sheet (m)
    real(dp)                :: dx,dy                 ! Grid spacing

    integer             :: i,j                   ! Grid coordinates

    do i=1,n
       do j=1,m
          d(i,j)=sqrt(((i-1)*dx-xs)**2+((j-1)*dy-ys)**2)      ! Distance. It is a minimum at the summit
       end do
    end do

  end subroutine distance

end module adism_mbal
