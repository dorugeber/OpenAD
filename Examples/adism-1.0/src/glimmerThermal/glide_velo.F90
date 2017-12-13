! +  Glimmer_velo.f90 - part of the GLIMMER ice model         + 
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

module glide_velo

  use glimmer_global, only: dp
  use physcon, only: rhoi, grav, gn

  implicit none

  integer, private, parameter :: p2 = gn-1
  integer, private, parameter :: p3 = 2*gn+1
  integer, private, parameter :: p4 = gn+2
  real(dp),private, parameter :: c = -2.0d0*(rhoi*grav)**gn/8.0d0

contains

  subroutine calc_velo(nx,ny,nz,dx,dy,sigma,flwa,diffu,usrf,thck,ubas,vbas,uvel,vvel)
    !*FD calculate 3D horizontal velocity field and 2D flux field from diffusivity
    use glimmer_utils
    use glimmer_global, only: dp
    use glimmer_deriv, only: df_field_2d_staggered
    use glimmer_utils
    implicit none

    !------------------------------------------------------------------------------------
    ! Subroutine arguments
    !------------------------------------------------------------------------------------
    integer,                     intent(in)  :: nx
    integer,                     intent(in)  :: ny
    integer,                     intent(in)  :: nz
    real(dp),                    intent(in)  :: dx
    real(dp),                    intent(in)  :: dy
    real(dp),dimension(nz),      intent(in)  :: sigma
    real(dp),dimension(nz,nx,ny),intent(in)  :: flwa
    real(dp),dimension(nx-1,ny-1),intent(out) :: diffu
    real(dp),dimension(nx,ny),   intent(in)  :: usrf
    real(dp),dimension(nx,ny),   intent(in)  :: thck
    real(dp),dimension(nx-1,ny-1),intent(in)  :: ubas
    real(dp),dimension(nx-1,ny-1),intent(in)  :: vbas
    real(dp),dimension(nz,nx-1,ny-1),intent(out) :: uvel
    real(dp),dimension(nz,nx-1,ny-1),intent(out) :: vvel
    !------------------------------------------------------------------------------------
    ! Internal variables
    !------------------------------------------------------------------------------------
    real(dp),dimension(nz) :: hrzflwa
    real(dp),dimension(nz) :: depth
    real(dp) :: factor
    real(dp),dimension(3)  :: const
    integer :: ew,ns,up
    real(dp),dimension(nx-1,ny-1) :: dintflwa
    real(dp),dimension(nx-1,ny-1) :: stagthck
    real(dp),dimension(nx-1,ny-1) :: dusrfdew
    real(dp),dimension(nx-1,ny-1) :: dusrfdns
    real(dp),dimension(nz) :: sumtemp

    ! Calculate depth
    do up=1,nz-1
       depth(up) = ((sigma(up+1)+sigma(up))/2.0d0)**gn *(sigma(up+1)-sigma(up))
    end do
    depth(nz) = 0.0d0
    !depth = (/ (((sigma(up+1)+sigma(up))/2.0d0)**gn *(sigma(up+1)-sigma(up)),up=1,nz-1),0.0d0 /)

    ! Stagger thickness
    call stagvarb(thck,stagthck,nx,ny)

    ! Calculate spatial derivatives
    call df_field_2d_staggered(usrf,dx,dy,dusrfdew,dusrfdns)

    ! Integrate flow law vertically
    call integrate_flwa(nx,ny,nz,sigma,depth,dintflwa,stagthck,flwa)

    ! Calculate diffusivity
    call velo_calc_diffu(nx,ny,dintflwa,stagthck,dusrfdew,dusrfdns,diffu)

    do ns = 1,ny-1
       do ew = 1,nx-1
          if (stagthck(ew,ns) /= 0.0d0) then

             uvel(nz,ew,ns) = ubas(ew,ns)
             vvel(nz,ew,ns) = vbas(ew,ns)

             call hsum4(flwa(:,ew:ew+1,ns:ns+1),sumtemp,2,2,nz)  
             hrzflwa = sumtemp

             factor = dintflwa(ew,ns)*stagthck(ew,ns)
             if (factor**2 /= 0.0d0) then
                const(2) = c * diffu(ew,ns) / factor
                const(3) = const(2) * dusrfdns(ew,ns)  
                const(2) = const(2) * dusrfdew(ew,ns) 
             else
                const(2:3) = 0.0d0
             end if

             do up = nz-1, 1, -1
                const(1) = depth(up) * (hrzflwa(up)+hrzflwa(up+1))
                uvel(up,ew,ns) = uvel(up+1,ew,ns) + const(1) * const(2)
                vvel(up,ew,ns) = vvel(up+1,ew,ns) + const(1) * const(3) 
             end do

          else 

             uvel(:,ew,ns) = 0.0d0
             vvel(:,ew,ns) = 0.0d0

          end if
       end do
    end do
  end subroutine calc_velo

  !------------------------------------------------------------------------------

  subroutine velo_calc_diffu(nx,ny,dintflwa,stagthck,dusrfdew,dusrfdns,diffu)

    !*FD calculate diffusivities
    use glimmer_global, only: dp

    implicit none
    
    !------------------------------------------------------------------------------------
    ! Subroutine arguments
    !------------------------------------------------------------------------------------
    integer,                        intent(in)    :: nx
    integer,                        intent(in)    :: ny
    real(dp),dimension(nx-1,ny-1),  intent(in)    :: dintflwa
    real(dp),dimension(nx-1,ny-1),  intent(in)    :: stagthck
    real(dp),dimension(nx-1,ny-1),  intent(in)    :: dusrfdew
    real(dp),dimension(nx-1,ny-1),  intent(in)    :: dusrfdns
    real(dp),dimension(nx-1,ny-1),  intent(out)   :: diffu

    integer :: i,j


    do i = 1,nx-1
       do j = 1,ny-1
          if (stagthck(i,j).ne.0.d0 .and.dusrfdew(i,j)**2.ne.0.d0 &
               .and.dusrfdns(i,j)**2.ne.0.d0 .and. diffu(i,j)>0.d0) then
             diffu(i,j) = dintflwa(i,j) * stagthck(i,j)**p4 &
                  * sqrt(dusrfdew(i,j)**2 + dusrfdns(i,j)**2)**p2 
          else
             diffu(i,j) = 0.0d0
          end if
       end do
    end do

  end subroutine velo_calc_diffu

  !------------------------------------------------------------------------------

  subroutine integrate_flwa(nx,ny,nz,sigma,depth,dintflwa,stagthck,flwa)
    
    !*FD this routine calculates the part of the vertically averaged velocity 
    !*FD field which solely depends on the temperature

    use glimmer_utils
    use glimmer_global, only: dp
    implicit none

    !------------------------------------------------------------------------------------
    ! Subroutine arguments
    !------------------------------------------------------------------------------------
    integer,                      intent(in)  :: nx
    integer,                      intent(in)  :: ny
    integer,                      intent(in)  :: nz
    real(dp),dimension(nz),       intent(in)  :: sigma
    real(dp),dimension(nz),       intent(in)  :: depth
    real(dp),dimension(nx-1,ny-1),intent(out) :: dintflwa
    real(dp),dimension(nx-1,ny-1),intent(in)  :: stagthck       !*FD ice thickness on staggered grid
    real(dp),dimension(nz,nx,ny), intent(in)  :: flwa           !*FD ice flow factor

    !------------------------------------------------------------------------------------
    ! Internal variables
    !------------------------------------------------------------------------------------
    real(dp),dimension(nz) :: dups
    real(dp),dimension(nz) :: hrzflwa, intflwa 
    integer :: ew,ns,up

    do up=1,nz-1
       dups(up) = sigma(up+1) - sigma(up)
    end do
    dups(nz) = 0.0d0
    !dups  = (/ (sigma(up+1) - sigma(up), up=1,nz-1),0.0d0 /)

    do ns = 1,ny-1
       do ew = 1,nx-1
          if (stagthck(ew,ns) /= 0.0d0) then
             
             call hsum4(flwa(:,ew:ew+1,ns:ns+1),hrzflwa,2,2,nz)  
             intflwa(nz) = 0.0d0

             do up = nz-1, 1, -1
                intflwa(up) = intflwa(up+1) + depth(up) * (hrzflwa(up)+hrzflwa(up+1))
             end do

             dintflwa(ew,ns) = c * vertintg(dups,intflwa)

          else 

             dintflwa(ew,ns) = 0.0d0

          end if
       end do
    end do
  end subroutine integrate_flwa

  !--------------------------------------------------------------------------------------------

  !> Calculates the ice temperature - full solution
  subroutine calcVerticalVelocity(nx,ny,nz,dx,dy,sigma,whichwvel,thklim,thck,usrf, &
       dthckdtm,dusrfdtm,bmlt,acab,uvel,vvel,wvel,wgrd)
    use glimmer_deriv, only: df_field_2d_staggered
    use glimmer_utils, only: stagvarb

    implicit none

    !------------------------------------------------------------------------------------
    ! Subroutine arguments
    !------------------------------------------------------------------------------------

    integer,                  intent(in)  :: nx
    integer,                  intent(in)  :: ny
    integer,                  intent(in)  :: nz
    real(dp),                 intent(in)  :: dx
    real(dp),                 intent(in)  :: dy
    real(dp),dimension(nz),   intent(in)  :: sigma
    integer, intent(in) :: whichwvel                   !< set to 1 if the Vertical integration constrained so 
    !< that upper kinematic B.C. obeyed
    real(dp),                     intent(in)  :: thklim    !< Minimum thickness to be considered when calculating the grid velocity. 
    !< This is in m, divided by \texttt{thk0}.
    real(dp),dimension(nx,ny),    intent(in)  :: thck      !< Ice-sheet thickness (divided by <tt>thk0</tt>)
    real(dp),dimension(nx,ny),    intent(in)  :: usrf      !< The elevation of the upper ice surface (divided by <tt>thk0</tt>)
    real(dp),dimension(nx,ny),    intent(in)  :: dthckdtm  !< Rate of change of ice thickness (m/yr)
    real(dp),dimension(nx,ny),    intent(in)  :: dusrfdtm  !< Rate of change of surface elevation (m/yr)
    real(dp),dimension(nx,ny),    intent(in)  :: bmlt      !< Basal melt-rate (scaled?) This is required in the basal boundary condition. See <em>Payne and Dongelmans</em> equation 14.
    real(dp),dimension(nx,ny),    intent(in)  :: acab      !< surface Mass-balance (scaled)
    real(dp),dimension(nz,nx-1,ny-1),intent(inout) :: uvel !< $x$ velocity field at top model level (scaled, on staggered grid).
    real(dp),dimension(nz,nx-1,ny-1),intent(inout) :: vvel !< $y$ velocity field at top model level (scaled, on staggered grid).
    real(dp),dimension(nz,nx,ny),intent(inout) :: wvel !< Vertical velocity field, 
    real(dp),dimension(nz,nx,ny),intent(inout) :: wgrd !< The grid velocity at each point. This is the output.

    real(dp),dimension(nx-1,ny-1) :: stagthck  !< ice thickness averaged onto the staggered grid.
    real(dp),dimension(nx-1,ny-1) :: dthckdew  !< E-W derivative of ice thickness.
    real(dp),dimension(nx-1,ny-1) :: dthckdns  !< N-S derivative of ice thickness.
    real(dp),dimension(nx-1,ny-1) :: dusrfdew  !< E-W derivative of upper surface elevation.
    real(dp),dimension(nx-1,ny-1) :: dusrfdns  !< N-S derivative of upper surface elevation.

    ! Calculate staggerings, etc.
    call stagvarb(thck,stagthck,nx,ny)

    ! Calculate spatial derivatives
    call df_field_2d_staggered(usrf,dx,dy,dusrfdew,dusrfdns)
    call df_field_2d_staggered(thck,dx,dy,dthckdew,dthckdns)

    ! Calculate the vertical velocity of the grid ------------------------------------

    call gridwvel(nx,ny,nz,sigma,thklim,uvel,vvel, &
       dusrfdew,dusrfdns,dthckdew,dthckdns,dusrfdtm,dthckdtm,thck,wgrd)

    ! Calculate the actual vertical velocity ------------
    call wvelintg(nx,ny,nz,dx,dy,sigma,thklim,stagthck,dusrfdew,dusrfdns, &
         dthckdew,dthckdns,uvel,vvel,wgrd(nz,:,:),thck,bmlt,wvel)

    ! Vertical integration constrained so kinematic upper BC obeyed.

    if (whichwvel==1) then
       call chckwvel(nx,ny,nz,sigma,thklim,dusrfdew,dusrfdns, &
            dusrfdtm,uvel(1,:,:),vvel(1,:,:),wvel,thck,acab)
    end if

  end subroutine calcVerticalVelocity

  !*****************************************************************************
  ! old velo functions come here
  !*****************************************************************************

  !> Calculates the vertical velocity of the grid.
  !!
  !! This is necessary because the model uses a sigma coordinate system.
  !! The equation for grid velocity is:
  !! \f[
  !! \mathtt{wgrd}(x,y,\sigma)=\frac{\partial s}{\partial t}+\mathbf{U}\cdot\nabla s
  !! -\sigma\left(\frac{\partial H}{\partial t}+\mathbf{U}\cdot\nabla H\right)
  !! \f]
  !! Compare this with equation A1 in <em>Payne and Dongelmans</em>.
  subroutine gridwvel(nx,ny,nz,sigma,thklim,uvel,vvel, &
       dusrfdew,dusrfdns,dthckdew,dthckdns,dusrfdtm,dthckdtm,thck,wgrd)
    use glimmer_utils

    implicit none 

    !------------------------------------------------------------------------------------
    ! Subroutine arguments
    !------------------------------------------------------------------------------------

    integer,                  intent(in)  :: nx
    integer,                  intent(in)  :: ny
    integer,                  intent(in)  :: nz
    real(dp),dimension(nz),   intent(in)  :: sigma
    real(dp),                 intent(in)  :: thklim    !< Minimum thickness to be considered when calculating the grid velocity. This is in m, divided by \texttt{thk0}.
    real(dp),dimension(:,:,:),intent(in)  :: uvel      !< The $x$-velocity field (scaled). Velocity is on the staggered grid
    real(dp),dimension(:,:,:),intent(in)  :: vvel      !< The $y$-velocity field (scaled). Velocity is on the staggered grid
    real(dp),dimension(:,:),  intent(in)  :: dusrfdew  !< E-W derivative of upper surface elevation.
    real(dp),dimension(:,:),  intent(in)  :: dusrfdns  !< N-S derivative of upper surface elevation.
    real(dp),dimension(:,:),  intent(in)  :: dthckdew  !< E-W derivative of ice thickness.
    real(dp),dimension(:,:),  intent(in)  :: dthckdns  !< N-S derivative of ice thickness.
    real(dp),dimension(:,:),  intent(in)  :: dusrfdtm
    real(dp),dimension(:,:),  intent(in)  :: dthckdtm
    real(dp),dimension(:,:),  intent(in)  :: thck      !< Ice-sheet thickness (divided by <tt>thk0</tt>)
    real(dp),dimension(:,:,:),intent(out) :: wgrd      !< The grid velocity at each point. This is the output.

    !------------------------------------------------------------------------------------
    ! Internal variables
    !------------------------------------------------------------------------------------

    integer :: ns,ew,up
    real(dp),dimension(nz) :: st1,st2
    real(dp) :: sum1,sum2,sum3,sum4

    !------------------------------------------------------------------------------------

    do ns = 2,ny-1
      do ew = 2,nx-1
        if (thck(ew,ns) > thklim) then
           call hsum4(uvel(:,ew-1:ew,ns-1:ns),st1,2,2,nz)
           call hsum4(vvel(:,ew-1:ew,ns-1:ns),st2,2,2,nz)
           call mysum(dusrfdew(ew-1:ew,ns-1:ns),sum1,2,2)
           call mysum(dthckdew(ew-1:ew,ns-1:ns),sum2,2,2)
           call mysum(dusrfdns(ew-1:ew,ns-1:ns),sum3,2,2)
           call mysum(dthckdns(ew-1:ew,ns-1:ns),sum4,2,2)
           do up = 1,nz
              wgrd(up,ew,ns) = dusrfdtm(ew,ns) - sigma(up) * dthckdtm(ew,ns) + & 
                   ( st1(up) * &
                   (sum1 - sigma(up) * &
                   sum2) + &
                   st2(up) * &
                   (sum3 - sigma(up) * &
                   sum4)) / 16.0d0
           end do
        else
           do up = 1,nz
              wgrd(up,ew,ns) = 0.0d0
           end do
        end if
      end do
    end do

  end subroutine gridwvel

!------------------------------------------------------------------------------------------

  !> Calculates the vertical velocity field, which is returned in <tt>wvel</tt>.
  !!
  !! This is found by doing this integration:
  !! \f[
  !! w(\sigma)=\int_{1}^{\sigma}\left[\frac{\partial \mathbf{U}}{\partial \sigma}
  !! (\sigma) \cdot (\nabla s - \sigma \nabla H) +H\nabla \cdot \mathbf{U}(\sigma)\right]d\sigma
  !! + w(1)
  !! \f]
  !! (This is equation 13 in <em>Payne and Dongelmans</em>.) Note that this is only 
  !! done if the thickness is greater than the threshold given by <tt>thklim</tt>.

  subroutine wvelintg(nx,ny,nz,dx,dy,sigma,thklim,stagthck,dusrfdew,dusrfdns,dthckdew,dthckdns,uvel,vvel,wgrd,thck,bmlt,wvel)
    use glimmer_utils

    implicit none

    !------------------------------------------------------------------------------------
    ! Subroutine arguments
    !------------------------------------------------------------------------------------

    integer,                  intent(in)  :: nx
    integer,                  intent(in)  :: ny
    integer,                  intent(in)  :: nz
    real(dp),                 intent(in)  :: dx
    real(dp),                 intent(in)  :: dy
    real(dp),dimension(nz),   intent(in)  :: sigma
    real(dp),                 intent(in)   :: thklim   !< Minimum thickness to be considered when calculating the grid velocity. This is in m, divided by \texttt{thk0}.
    real(dp),dimension(:,:),  intent(in)   :: stagthck !< ice thickness averaged onto the staggered grid.
    real(dp),dimension(:,:),  intent(in)   :: dusrfdew !< E-W derivative of upper surface elevation.
    real(dp),dimension(:,:),  intent(in)   :: dusrfdns !< N-S derivative of upper surface elevation.
    real(dp),dimension(:,:),  intent(in)   :: dthckdew !< E-W derivative of ice thickness.
    real(dp),dimension(:,:),  intent(in)   :: dthckdns !< N-S derivative of ice thickness.
    real(dp),dimension(:,:,:),intent(in)   :: uvel     !< The $x$-velocity field (scaled). Velocity is on the staggered grid
    real(dp),dimension(:,:,:),intent(in)   :: vvel     !< The $y$-velocity field (scaled). Velocity is on the staggered grid
    real(dp),dimension(:,:),  intent(in)   :: thck     !< Ice-sheet thickness (divided by <tt>thk0</tt>)
    real(dp),dimension(:,:),intent(in)     :: wgrd     !< The grid velocity at each point. This is the output.
    real(dp),dimension(:,:),   intent(in)  :: bmlt     !< Basal melt-rate (scaled?) This is required in the basal boundary condition. See <em>Payne and Dongelmans</em> equation 14.
    real(dp),dimension(:,:,:), intent(out) :: wvel     !< The vertical velocity field.

    !------------------------------------------------------------------------------------
    ! Internal variables
    !------------------------------------------------------------------------------------

    real(dp) :: dew16, dns16        ! The grid-spacings multiplied by 16
    real(dp),dimension(6) :: cons   ! Holds temporary local values of derivatives
    integer :: ns,ew,up             ! Loop indicies
    real(dp),dimension(nz) :: dupsw
    real(dp),dimension(nz) :: depthw
    real(dp),dimension(nz) :: suvel
    real(dp),dimension(nz) :: svvel
    real(dp) :: sum1,sum2,sum3,sum4

    !------------------------------------------------------------------------------------

    do up=1,nz-1
       dupsw(up) = sigma(up+1)-sigma(up)
    end do
    dupsw(nz) = 0.0d0
    !dupsw  = (/ (sigma(up+1)-sigma(up), up=1,nz-1), 0.0d0 /) 

    ! Calculate the value of sigma for the levels between the standard ones -----------

    do up=1,nz-1
       depthw(up) = (sigma(up+1)+sigma(up)) / 2.0d0
    end do
    depthw(nz) = 0.0d0
    !depthw = (/ ((sigma(up+1)+sigma(up)) / 2.0d0, up=1,nz-1), 0.0d0 /)


    ! Multiply grid-spacings by 16 -----------------------------------------------------

    dew16 = 1d0/(16.0d0 * dx)
    dns16 = 1d0/(16.0d0 * dy)

    ! ----------------------------------------------------------------------------------
    ! Main loop over each grid-box
    ! ----------------------------------------------------------------------------------

    do ns = 2,ny-1
      do ew = 2,nx-1
        if (thck(ew,ns) > thklim) then
  
          ! Set the bottom boundary condition ------------------------------------------

          wvel(nz,ew,ns) = wgrd(ew,ns) - bmlt(ew,ns)

          ! Calculate temporary local values of thickness and surface ------------------
          ! elevation derivatives.

          call mysum(dusrfdew(ew-1:ew,ns-1:ns),cons(1),2,2)
          cons(1) = cons(1) / 16.0d0
          call mysum(dthckdew(ew-1:ew,ns-1:ns),cons(2),2,2) 
          cons(2) = cons(2) / 16.0d0
          call mysum(dusrfdns(ew-1:ew,ns-1:ns),cons(3),2,2) 
          cons(3) = cons(3) / 16.0d0
          call mysum(dthckdns(ew-1:ew,ns-1:ns),cons(4),2,2)
          cons(4) = cons(4) / 16.0d0
          call mysum(stagthck(ew-1:ew,ns-1:ns),cons(5),2,2)
          cons(6) = cons(5)*dns16
          cons(5) = cons(5)*dew16
          ! * better? (an alternative from TP's original code)
          !cons(5) = (thck(ew-1,ns)+2.0d0*thck(ew,ns)+thck(ew+1,ns)) * dew16
          !cons(6) = (thck(ew,ns-1)+2.0d0*thck(ew,ns)+thck(ew,ns+1)) * dns16

          call hsum4(uvel(:,ew-1:ew,ns-1:ns),suvel,2,2,nz)
          call hsum4(vvel(:,ew-1:ew,ns-1:ns),svvel,2,2,nz)

          ! Loop over each model level, starting from the bottom ----------------------

          do up = nz-1, 1, -1
             call mysum3d(uvel(up:up+1,ew,ns-1:ns),sum1,1,2,2)
             call mysum3d(uvel(up:up+1,ew-1,ns-1:ns),sum2,1,2,2)
             call mysum3d(vvel(up:up+1,ew-1:ew,ns),sum3,2,1,2)
             call mysum3d(vvel(up:up+1,ew-1:ew,ns-1),sum4,2,1,2)

            wvel(up,ew,ns) = wvel(up+1,ew,ns) &
                       - dupsw(up) * cons(5) * (sum1  - sum2) &
                       - dupsw(up) * cons(6) * (sum3  - sum4) &
                       - (suvel(up+1) - suvel(up)) * (cons(1) - depthw(up) * cons(2)) &
                       - (svvel(up+1) - svvel(up)) * (cons(3) - depthw(up) * cons(4)) 
          end do
        else 

          ! If there isn't enough ice, set velocities to zero ----------------------------

          wvel(:,ew,ns) = 0.0d0  

        end if
      end do
    end do

  end subroutine wvelintg

  !> set periodic EW boundary conditions
  subroutine wvel_ew(wgrd,wvel)
    implicit none
    real(dp),dimension(:,:,:) :: wvel 
    real(dp),dimension(:,:,:) :: wgrd

    integer :: ewn

    ewn = size(wgrd,2)

    wgrd(:,1,:)   = wgrd(:,ewn-1,:)
    wgrd(:,ewn,:) = wgrd(:,2,:)
    wvel(:,1,:)   = wvel(:,ewn-1,:)
    wvel(:,ewn,:) = wvel(:,2,:)
  end subroutine wvel_ew

!------------------------------------------------------------------------------------------

  !> Constrain the vertical velocity field to obey a kinematic upper boundary 
  !! condition.
  subroutine chckwvel(nx,ny,nz,sigma,thklim,dusrfdew,dusrfdns,dusrfdtm,uvel,vvel,wvel,thck,acab)
    use glimmer_utils

    implicit none

    !------------------------------------------------------------------------------------
    ! Subroutine arguments
    !------------------------------------------------------------------------------------

    integer,                 intent(in) :: nx
    integer,                 intent(in) :: ny
    integer,                 intent(in) :: nz
    real(dp),dimension(nz),  intent(in) :: sigma
    real(dp), intent(in) :: thklim                  !< the ice thickness below which no computations are done
    real(dp),dimension(nx-1,ny-1), intent(in) :: dusrfdew !< E-W derivative of upper surface elevation.
    real(dp),dimension(nx-1,ny-1), intent(in) :: dusrfdns !< N-S derivative of upper surface elevation.
    real(dp),dimension(nx,ny), intent(in) :: dusrfdtm
    real(dp),dimension(nx-1,ny-1),  intent(in)    :: uvel !< $x$ velocity field at top model level (scaled, on staggered grid).
    real(dp),dimension(nx-1,ny-1),  intent(in)    :: vvel !< $y$ velocity field at top model level (scaled, on staggered grid).
    real(dp),dimension(nz,nx,ny),intent(inout) :: wvel !< Vertical velocity field, 
    real(dp),dimension(nx,ny),  intent(in)    :: thck !< Ice thickness (scaled)
    real(dp),dimension(nx,ny),  intent(in)    :: acab !< Mass-balance (scaled)

    !------------------------------------------------------------------------------------
    ! Internal variables
    !------------------------------------------------------------------------------------

    real(dp) :: wchk
    real(dp) :: tempcoef
    integer  :: ns,ew,up
    real(dp) :: sum1,sum2,sum3,sum4

    ! Loop over all grid-boxes ----------------------------------------------------------

    do ns = 2,ny-1
      do ew = 2,nx-1
         if (thck(ew,ns) > thklim .and. wvel(1,ew,ns).ne.0.d0) then

            call mysum(uvel(ew-1:ew,ns-1:ns),sum1,2,2)
            call mysum(dusrfdew(ew-1:ew,ns-1:ns),sum2,2,2)
            call mysum(vvel(ew-1:ew,ns-1:ns),sum3,2,2)
            call mysum(dusrfdns(ew-1:ew,ns-1:ns),sum4,2,2)

            wchk = dusrfdtm(ew,ns) &
                 - acab(ew,ns) &
                 + (sum1 * sum2 &
                 +  sum3 * sum4) &
                 / 16.0d0
            
            tempcoef = wchk - wvel(1,ew,ns)
            do up = 1,nz
               wvel(up,ew,ns) = wvel(up,ew,ns) + tempcoef * (1.0d0 - sigma(up)) 
            end do
         end if
      end do
    end do

  end subroutine chckwvel

!
end module glide_velo
