! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
! +                                                           +
! +  glide_tempFullSoln.f90 - part of Glimmer-CISM            + 
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

!> compute thermal evolution of ice sheet
!!
!! \author Ian Rutt

module glide_tempFullSoln

  use glimmer_global,     only: dp

  implicit none

  private
  public :: tstep_tempFullSoln

contains

  !> Calculates the ice temperature - full solution
  subroutine tstep_tempFullSoln(nx,ny,nz,dx,dy,sigma,temp,artm,thck,usrf, &
       uvel,vvel,ubas,vbas,wvel,wgrd,flwa,bheatflx,bwat,bmlt,dt,thklim,floatmask)
    use glimmer_utils
    use glimmer_deriv,    only: df_field_2d_staggered
    use glimmer_global,   only: dp
    use glimmer_pmpt,     only: corrpmpt
!    use glimmer_mask
    use physcon,          only: rhoi, shci, coni, grav, gn, lhci, scyr

    implicit none

    !------------------------------------------------------------------------------------
    ! Subroutine arguments
    !------------------------------------------------------------------------------------

    integer,                          intent(in)  :: nx       !< Number of nodes in x
    integer,                          intent(in)  :: ny       !< Number of nodes in y
    integer,                          intent(in)  :: nz       !< Number of nodes in z
    real(dp),                         intent(in)  :: dx       !< Grid spacing in x (m)
    real(dp),                         intent(in)  :: dy       !< Grid spacing in y (m)
    real(dp), dimension(nz),          intent(in)  :: sigma    !< Sigma levels (dimensionless)
    real(dp), dimension(nz,0:nx+1,0:ny+1),intent(inout) :: temp  !< Ice temperature (upn,0:ewn+1,0:nsn+1)
    real(dp), dimension(nx,ny),       intent(in)  :: artm     !< Surface air temperature (ewn,nsn)
    real(dp), dimension(nx,ny),       intent(in)  :: thck     !< Ice thickness (ewn,nsn)
    real(dp), dimension(nx,ny),       intent(in)  :: usrf     !< Upper surface elevation (ewn,nsn)
    real(dp), dimension(nz,nx-1,ny-1),intent(in)  :: uvel     !< x-velocity (ewn-1,nsn-1)
    real(dp), dimension(nz,nx-1,ny-1),intent(in)  :: vvel     !< y-velocity (ewn-1,nsn-1)
    real(dp), dimension(nx-1,ny-1),   intent(in)  :: ubas     !< basal x-velocity (ewn-1,nsn-1)
    real(dp), dimension(nx-1,ny-1),   intent(in)  :: vbas     !< basal y-velocity (ewn-1,nsn-1)
    real(dp), dimension(nz,nx,ny),    intent(in)  :: wvel     !< Vertical velocity (upn,ewn,nsn)
    real(dp), dimension(nz,nx,ny),    intent(in)  :: wgrd     !< Vertical grid velocity (upn,ewn,nsn)
    real(dp), dimension(nz,nx,ny),    intent(in)  :: flwa     !< Glen's A (upn,ewn,nsn)
    real(dp), dimension(nx,ny),       intent(in)  :: bheatflx !< Basal heat flux (ewn,nsn)
    real(dp), dimension(nx,ny),       intent(in)  :: bwat     !< Basal water depth
    real(dp), dimension(nx,ny),       intent(out) :: bmlt     !< Basal melt rate
    real(dp),                         intent(in)  :: dt       !< Timestep (years)
    real(dp),                         intent(in)  :: thklim   !< Lower thickness limit for calculation (m)
    logical, dimension(nx,ny),        intent(in)  :: floatmask  !< set true if floating
!   integer, parameter                            :: glimmer_mask_thin_ice = 1
!   integer, parameter                            :: glimmer_mask_shelf = 4
!   logical,integer                               :: is_thin,is_float
    !------------------------------------------------------------------------------------
    ! Internal variables
    !------------------------------------------------------------------------------------

    real(dp),dimension(nz) :: subd, diag, supd, rhsd
    real(dp),dimension(nz) :: prevtemp, iteradvt, diagadvt
    real(dp) :: tempresid

    integer :: iter
    integer :: ew,ns,up

    real(dp),parameter :: tempthres = 0.001d0
    integer, parameter :: mxit = 100
    real(dp),parameter :: wmax = 5.0d0

    real(dp), dimension(nz) :: weff

    real(dp) :: advconst1,advconst2
    real(dp) :: cons1,cons2,cons3,cons4
    real(dp),dimension(nz) :: c1
    real(dp) :: f3
    real(dp) :: slidef1, slidef2

    real(dp),dimension(nz,nx,ny)  :: initadvt
    real(dp),dimension(nz,nx,ny)  :: inittemp
    real(dp),dimension(nz,nx,ny)  :: dissip
    real(dp),dimension(nz,nx,ny)  :: hadv_u
    real(dp),dimension(nz,nx,ny)  :: hadv_v
    real(dp),dimension(nx-1,ny-1) :: stagthck
    real(dp),dimension(nx-1,ny-1) :: dusrfdew
    real(dp),dimension(nx-1,ny-1) :: dusrfdns
    real(dp),dimension(nz) :: sumtemp

    ! Finite difference factors

    real(dp),dimension(nz)   :: dupa, dupb, dupc
    real(dp),dimension(nz,3) :: dups
    real(dp)                 :: dupn

    real(dp),dimension(nz,5) :: tempx,tempy
    real(dp)                 :: minartm
    integer :: i1, i2
    !------------------------------------------------------------------------------------
    ! Set up various parameters
    !------------------------------------------------------------------------------------

    advconst1 = dt / (16.0d0 * dx)
    advconst2 = dt / (16.0d0 * dy)

    cons1 = 2.0d0 * scyr * dt * coni / (2.0d0 * rhoi * shci)
    cons2 = dt / 2.0d0
    cons3 = 2.0d0 * scyr * dt / (rhoi * shci)
    cons4 = dt / coni

    c1 = (sigma * rhoi * grav )**(gn+1) * &
         2.0d0 * dt / (16.0d0 * rhoi * shci)

    f3 = rhoi * shci /  (dt * lhci * rhoi)

    ! sliding contribution to basal heat flux

    slidef1 = grav * dt / shci                      ! vert diffusion
    slidef2 = rhoi * grav * dt / (coni * scyr) ! vert advection

    ! FD parameters
    dupa(1) = 0.0d0
    dupa(2) = 0.0d0
    do up=3,nz
       dupa(up) = (sigma(up)   - sigma(up-1)) / ((sigma(up-2) - sigma(up-1)) * (sigma(up-2) - sigma(up)))
    end do
    !dupa = (/ 0.0d0, 0.0d0, &
    !     ((sigma(up)   - sigma(up-1)) / ((sigma(up-2) - sigma(up-1)) * (sigma(up-2) - sigma(up))), up=3,nz) /)

    dupb(1) = 0.0d0
    dupb(2) = 0.0d0
    do up=3,nz
       dupb(up) = (sigma(up)   - sigma(up-2)) / ((sigma(up-1) - sigma(up-2)) * (sigma(up-1) - sigma(up)))
    end do
    !dupb = (/ 0.0d0, 0.0d0, &
    !     ((sigma(up)   - sigma(up-2)) / ((sigma(up-1) - sigma(up-2)) * (sigma(up-1) - sigma(up))), up=3,nz) /)

    dupc(1) = (sigma(2) - sigma(1)) / 2.0d0
    do up=2,nz-1
       dupc(up) = (sigma(up+1) - sigma(up-1)) / 2.0d0
    end do
    dupc(nz) = (sigma(nz) - sigma(nz-1)) / 2.0d0
    !dupc = (/ (sigma(2) - sigma(1)) / 2.0d0, ((sigma(up+1) - sigma(up-1)) / 2.0d0, up=2,nz-1), &
    !     (sigma(nz) - sigma(nz-1)) / 2.0d0 /)

    dups = 0.0d0
    do up = 2, nz-1
       dups(up,1) = 1.d0/((sigma(up+1) - sigma(up-1)) * (sigma(up)   - sigma(up-1)))
       dups(up,2) = 1.d0/((sigma(up+1) - sigma(up-1)) * (sigma(up+1) - sigma(up)))
       dups(up,3) = 1.d0/( sigma(up+1) - sigma(up-1))
    end do

    dupn = sigma(nz) - sigma(nz-1)

    !------------------------------------------------------------------------------------
    ! ewbc/nsbc set the type of boundary condition applied at the end of
    ! the domain. a value of 0 implies zero gradient.
    !------------------------------------------------------------------------------------

    inittemp = 0.0d0
    initadvt = 0.0d0

    ! Calculate staggered variables and spatial derivatives -----------------------------

    call stagvarb(thck,stagthck,nx,ny)
    call df_field_2d_staggered(usrf,dx,dy,dusrfdew,dusrfdns)

    ! Calculate dissipative term --------------------------------------------------------

    call finddisp(dissip,              &
         thck,                         &
         stagthck,                     &
         dusrfdew,                     &
         dusrfdns,                     &
         flwa,                         &
         nx,                           &
         ny,                           &
         c1,                           &
         thklim)

    ! translate velo field --------------------------------------------------------------

    do ns = 2,ny-1
       do ew = 2,nx-1
          !call hsum4(uvel(:,ew-1:ew,ns-1:ns),sumtemp,2,2,nz)
          sumtemp = uvel(:,ew-1,ns-1) + uvel(:,ew,ns-1) + uvel(:,ew-1,ns) + uvel(:,ew,ns)
          hadv_u(:,ew,ns) = advconst1 * sumtemp
          !call hsum4(vvel(:,ew-1:ew,ns-1:ns),sumtemp,2,2,nz)
          sumtemp = vvel(:,ew-1,ns-1) + vvel(:,ew,ns-1) + vvel(:,ew-1,ns) + vvel(:,ew,ns)
          hadv_v(:,ew,ns) = advconst2 * sumtemp
       end do
    end do

    ! Calculate initial upwinding terms -------------------------------------------------

    call hadvall(initadvt,temp,thck,thklim,hadv_u,hadv_v,nx,ny,nz)

    ! Iterative temperature solution ----------------------------------------------------

    iter = 0
    tempresid = abs(tempthres*2.d0)  ! To make sure the loop is executed at least once
   
    do while (tempresid.gt.tempthres .and. iter.le.mxit)
       tempresid = 0.0d0

       do ns = 2,ny-1
          do ew = 2,nx-1
             if(thck(ew,ns) > thklim) then

                ! Calculate effective vertical velocity
                weff = wvel(:,ew,ns) - wgrd(:,ew,ns)

                ! Set effective vertical velocity to zero if it exceeds a threshold
                if (maxval(abs(weff)) > wmax) then
                   weff = 0.0d0
                end if
                do i1=1,5
                   do i2=1,nz
                      tempx(i2,i1)=temp(i2,ew-3+i1,ns)
                      tempy(i2,i1)=temp(i2,ew,ns-3+i1)
                   end do
                end do
                ! Calculate upwinded advection term
                call hadvpnt(iteradvt,                       &
                     diagadvt,                               &
                     tempx,                   &
                     tempy,                   &
                     hadv_u(:,ew,ns),                        &
                     hadv_v(:,ew,ns),                        &
                     nz)

                call findvtri(nz,                &
                     dups,                       &
                     dupn,                       &
                     thck(ew,ns),                &
                     subd,                       &
                     diag,                       &
                     supd,                       &
                     diagadvt,                   &
                     weff,                       &
                     floatmask(ew,ns),           &
                     cons1,                      &
                     cons2)

                if (iter==0) then
                   call findvtri_init(nz,           &
                        dupn,                       &
                        initadvt,                   &
                        dissip,                     &
                        inittemp,                   &
                        bheatflx,                   &
                        dusrfdew,                   &
                        dusrfdns,                   &
                        ew,                         &
                        ns,                         &
                        subd,                       &
                        diag,                       &
                        supd,                       &
                        weff,                       &
                        ubas,                       &
                        vbas,                       &
                        temp(:,ew,ns),              &
                        thck(ew,ns),                &
                        floatmask(ew,ns),           &
                        cons3,                      &
                        cons4,                      &
                        slidef1,                    &
                        slidef2)
                end if

                call findvtri_rhs(nz,               &
                     inittemp(:,ew,ns),             &
                     artm(ew,ns),                   &
                     iteradvt,                      &
                     rhsd,                          &
                     floatmask(ew,ns))

                prevtemp = temp(:,ew,ns)

                call tridiag(subd,                  &
                     diag,                    &
                     supd,                    &
                     temp(:,ew,ns),              &
                     rhsd,                          &
                     nz)

                call corrpmpt(temp(:,ew,ns),         &
                     thck(ew,ns),                    &
                     bwat(ew,ns),                    &
                     sigma,                          &
                     nz)

                tempresid = max(tempresid,maxval(abs(temp(:,ew,ns)-prevtemp(:))))
             endif
          end do
       end do

       iter = iter + 1
    end do

    ! set temperature of thin ice to the air temperature and set ice free nodes to zero

    do ns = 1,ny
       do ew = 1,nx
          if (thck(ew,ns)<thklim) then
             minartm=min(0.0d0,dble(artm(ew,ns)))
             do i2=1,nz
                temp(i2,ew,ns) = minartm
             end do
          end if
       end do
    end do

    ! Calculate basal melt rate --------------------------------------------------


    call calcbmlt(nx,                     &
         ny,                              &
         nz,                              &
         sigma,                           &
         dupa,                            &
         dupb,                            &
         dupc,                            &
         dissip,                          &
         temp,                            &
         thck,                            &
         stagthck,                        &
         dusrfdew,                        &
         dusrfdns,                        &
         ubas,                            &
         vbas,                            &
         bheatflx,                        &
         bmlt,                            &
         floatmask,                       &
         f3,                              &
         thklim)

  end subroutine tstep_tempFullSoln

  !-------------------------------------------------------------------

  subroutine hadvpnt(iteradvt,diagadvt,tempx,tempy,u,v,upn)
    use glimmer_global, only: dp

    implicit none

    integer,                    intent(in)  :: upn        !< Number of points in vertical
    real(dp), dimension(upn),   intent(out) :: iteradvt
    real(dp), dimension(upn),   intent(out) :: diagadvt
    real(dp), dimension(upn,5), intent(in)  :: tempx
    real(dp), dimension(upn,5), intent(in)  :: tempy
    real(dp), dimension(upn),   intent(in)  :: u
    real(dp), dimension(upn),   intent(in)  :: v

    integer :: up

    iteradvt = 0.0d0
    diagadvt = 0.0d0

    if (u(1) > 0.0d0) then
       do up = 1,upn
          iteradvt(up) = u(up) * (- 4.0d0*tempx(up,2) + tempx(up,1))
          diagadvt(up) = u(up) * 3.0d0
       end do
    else if (u(1) < 0.0d0) then
       do up = 1,upn
          iteradvt(up) = u(up) * (4.0d0*tempx(up,4) - tempx(up,5))
          diagadvt(up) = - u(up) * 3.0d0
       end do
    end if

    if (v(1) > 0.0d0) then
       do up = 1,upn
          iteradvt(up) = iteradvt(up) + v(up) * (- 4.0d0*tempy(up,2) + tempy(up,1))
          diagadvt(up) = diagadvt(up) + v(up) * 3.0d0
       end do
    else if (v(1) < 0.0d0) then
       do up = 1,upn
          iteradvt(up) = iteradvt(up) + v(up) * (4.0d0*tempy(up,4) - tempy(up,5))
          diagadvt(up) = diagadvt(up) - v(up) * 3.0d0
       end do
    end if

  end subroutine hadvpnt

  !-------------------------------------------------------------------------

!  subroutine fohadvpnt(nx,ny,nz,advconst1,advconst2,iteradvt,diagadvt,tempx,tempy,uvel,vvel)
!    use glimmer_global, only: dp
!    use glimmer_utils
!
!    implicit none
!
!    integer :: nx,ny,nz
!    real(dp),                   intent(in)  :: advconst1
!    real(dp),                   intent(in)  :: advconst2
!    real(dp), dimension(:),     intent(out) :: iteradvt
!    real(dp), dimension(:),     intent(out) :: diagadvt
!    real(dp), dimension(:,:),   intent(in)  :: tempx
!    real(dp), dimension(:,:),   intent(in)  :: tempy
!    real(dp), dimension(:,:,:), intent(in)  :: uvel
!    real(dp), dimension(:,:,:), intent(in)  :: vvel
!
!    real(dp), dimension(size(iteradvt)) :: u, v
!    real(dp), dimension(size(uvel,1)) :: sumtemp
!
!    iteradvt = 0.0d0
!    diagadvt = 0.0d0
!    print*,nx,ny,nz
!    call hsum(uvel(:,:,:),sumtemp,size(uvel,2),size(uvel,3),size(uvel,1))
!    u = advconst1 * sumtemp
!    call hsum(vvel(:,:,:),sumtemp,size(vvel,2),size(vvel,3),size(vvel,1))
!    v = advconst2 * sumtemp
!
!    if (u(1) > 0.0d0) then
!       iteradvt = - u * 2.0d0 * tempx(:,1)
!       diagadvt = 2.0d0 * u 
!    else if (u(1) < 0.0d0) then
!       iteradvt = u * 2.0d0 * tempx(:,3)
!       diagadvt = - 2.0d0 * u 
!    end if
!
!    if (v(1) > 0.0d0) then
!       iteradvt = iteradvt - v * 2.0d0 * tempy(:,1) 
!       diagadvt = diagadvt + 2.0d0 * v 
!    else if (v(1) < 0.0d0) then
!       iteradvt = iteradvt + v * 2.0d0 * tempy(:,3)
!       diagadvt = diagadvt - 2.0d0 * v 
!    end if
!
!  end subroutine fohadvpnt

  !-------------------------------------------------------------------------

  subroutine hadvall(initadvt,temp,thck,thklim,hadv_u,hadv_v,ewn,nsn,upn)
    use glimmer_global, only: dp 

    implicit none

    integer,                                 intent(in)  :: ewn
    integer,                                 intent(in)  :: nsn
    integer,                                 intent(in)  :: upn
    real(dp), dimension(upn,ewn,nsn),        intent(out) :: initadvt
    real(dp), dimension(upn,0:ewn+1,0:nsn+1),intent(in)  :: temp
    real(dp), dimension(ewn,nsn),            intent(in)  :: thck
    real(dp),                                intent(in)  :: thklim
    real(dp), dimension(upn,ewn,nsn),        intent(in)  :: hadv_u
    real(dp), dimension(upn,ewn,nsn),        intent(in)  :: hadv_v

    real(dp), dimension(upn) :: diagadvt
    real(dp), dimension(upn,5) :: tempx,tempy

    integer :: ew,ns, i1,i2

    initadvt = 0.0d0

    do ns = 2,nsn-1
       do ew = 2,ewn-1
          if (thck(ew,ns) > thklim) then
             do i1=1,5
                do i2=1,upn
                   tempx(i2,i1)=temp(i2,ew-3+i1,ns)
                   tempy(i2,i1)=temp(i2,ew,ns-3+i1)
                end do
             end do
             call hadvpnt(initadvt(:,ew,ns),      &
                  diagadvt,                       &
                  tempx,           &
                  tempy,           &
                  hadv_u(:,ew,ns),                &
                  hadv_v(:,ew,ns),                &
                  upn)
          end if
       end do
    end do

  end subroutine hadvall

  !-------------------------------------------------------------------------

  subroutine findvtri(nz,dups,dupn,thck,subd,diag,supd,diagadvt,weff,float,cons1,cons2)
    use glimmer_global,    only: dp

    implicit none

    integer,                intent(in)  :: nz
    real(dp), dimension(nz,3),intent(in) :: dups
    real(dp),               intent(in)  :: dupn
    real(dp),               intent(in)  :: thck
    real(dp), dimension(:), intent(in)  :: weff
    real(dp), dimension(:), intent(in)  :: diagadvt
    real(dp), dimension(:), intent(out) :: subd
    real(dp), dimension(:), intent(out) :: diag
    real(dp), dimension(:), intent(out) :: supd
    logical,                intent(in)  :: float
    real(dp),               intent(in)  :: cons1
    real(dp),               intent(in)  :: cons2

    real(dp) :: diff_factor,adv_factor
    integer :: i

    diff_factor = cons1 / thck**2
    adv_factor  = cons2 / thck
    
    do i = 2,nz-1
       subd(i) =   adv_factor  * weff(i) * dups(i,3)
       supd(i) = - subd(i) - diff_factor * dups(i,2)
       subd(i) =   subd(i) - diff_factor * dups(i,1)
    end do

    do i = 2,nz-1
       diag(i) = 1.0d0 - subd(i) - supd(i) + diagadvt(i)
    end do

    ! Upper surface: hold temperature constant

    supd(1) = 0.0d0
    subd(1) = 0.0d0
    diag(1) = 1.0d0

    ! now do the basal boundary
    ! for grounded ice, a heat flux is applied
    ! for floating ice, temperature held constant

    if (float) then
       supd(nz) = 0.0d0
       subd(nz) = 0.0d0
       diag(nz) = 1.0d0
    else 
       supd(nz) = 0.0d0 
       subd(nz) = -0.5d0*diff_factor/(dupn**2)
       diag(nz) = 1.0d0 - subd(nz) + diagadvt(nz)
    end if

  end subroutine findvtri

  !-------------------------------------------------------------------------

  !> called during first iteration to set inittemp
  subroutine findvtri_init(nz,dupn,initadvt,dissip,inittemp,bheatflx,dusrfdew,dusrfdns, &
       ew,ns,subd,diag,supd,weff,ubas,vbas,temp,thck,float, &
       cons3,cons4,slidef1,slidef2)
    use glimmer_global, only: dp
    use glimmer_pmpt

    implicit none

    integer,                  intent(in)  :: nz
    real(dp),                 intent(in)  :: dupn
    real(dp),dimension(:,:,:),intent(in)  :: initadvt
    real(dp),dimension(:,:,:),intent(in)  :: dissip
    real(dp),dimension(:,:,:),intent(inout) :: inittemp
    real(dp),dimension(:,:),  intent(in)  :: bheatflx
    real(dp),dimension(:,:),  intent(in)  :: dusrfdew
    real(dp),dimension(:,:),  intent(in)  :: dusrfdns
    integer,                  intent(in)  :: ew
    integer,                  intent(in)  :: ns
    real(dp),dimension(:),    intent(in)  :: temp
    real(dp),dimension(:),    intent(in)  :: diag
    real(dp),dimension(:),    intent(in)  :: subd
    real(dp),dimension(:),    intent(in)  :: supd
    real(dp),dimension(:),    intent(in)  :: weff
    real(dp),dimension(:,:),  intent(in)  :: ubas
    real(dp),dimension(:,:),  intent(in)  :: vbas
    real(dp),                 intent(in)  :: thck
    logical,                  intent(in)  :: float    
    real(dp),                 intent(in)  :: cons3
    real(dp),                 intent(in)  :: cons4
    real(dp),                 intent(in)  :: slidef1
    real(dp),                 intent(in)  :: slidef2

    ! local variables
    real(dp) :: slterm
    integer  :: ewp,nsp,up
    integer  :: slide_count

    ! Main body points
    do up = 2, nz-1
       inittemp(up,ew,ns) = temp(up) * (2.0d0 - diag(up)) &
            - temp(up-1) * subd(up)         &
            - temp(up+1)   * supd(up)         & 
            - initadvt(up,ew,ns)               &
            + dissip(up,ew,ns)
    end do
    
    ! Basal boundary points
    if (float) then
       call calcpmptb(inittemp(nz,ew,ns),thck)
    else 
       ! sliding contribution to basal heat flux
       slterm = 0.d0
       slide_count = 0
       ! only include sliding contrib if temperature node is surrounded by sliding velo nodes
       do nsp = ns-1,ns
          do ewp = ew-1,ew
             if (abs(ubas(ewp,nsp)).gt.0.000001d0 .or. abs(vbas(ewp,nsp)).gt.0.000001d0) then
                slide_count = slide_count + 1
                slterm = slterm + (dusrfdew(ewp,nsp)*ubas(ewp,nsp) + dusrfdns(ewp,nsp)*vbas(ewp,nsp))
             end if
          end do
       end do

       if (slide_count.ge.4) then
          slterm = 0.25d0*slterm
       else
          slterm = 0.d0
       end if

       inittemp(nz,ew,ns) = temp(nz) * (2.0d0 - diag(nz)) &
            - temp(nz-1) * subd(nz)                        &
            - 0.5d0 * cons3 * bheatflx(ew,ns) / (thck * dupn) &  ! geothermal heat flux (diff)
            - slidef1 * slterm / dupn                 &        ! sliding heat flux    (diff)
            - cons4 * bheatflx(ew,ns) * weff(nz)            &        ! geothermal heat flux (adv)
            - slidef2 * thck * slterm * weff(nz)            &        ! sliding heat flux    (adv)
            - initadvt(nz,ew,ns)                            &
            + dissip(nz,ew,ns)

    end if

  end subroutine findvtri_init

  !-------------------------------------------------------------------------

  !> RHS of temperature tri-diag system for a single column
  subroutine findvtri_rhs(nz,inittemp,artm,iteradvt,rhsd,float)
    use glimmer_global, only: dp

    implicit none

    integer,              intent(in)  :: nz
    real(dp),dimension(:),intent(in)  :: inittemp
    real(dp),             intent(in)  :: artm 
    real(dp),dimension(:),intent(in)  :: iteradvt
    real(dp),dimension(:),intent(out) :: rhsd
    logical,              intent(in)  :: float

    integer :: up

    ! upper boundary condition

    rhsd(1) = artm

    if (float) then
       rhsd(nz) = inittemp(nz)    
    else
       rhsd(nz) = inittemp(nz) - iteradvt(nz)
    end if

    do up = 2,nz-1
       rhsd(up) = inittemp(up) - iteradvt(up)
    end do

  end subroutine findvtri_rhs

  !-----------------------------------------------------------------------

  !> find dissipation term at H-pts by averaging quantities from u-pts
 subroutine finddisp(dissip,thck,stagthck,dusrfdew,dusrfdns,flwa,ewn,nsn,c1,thklim)
    use glimmer_global, only: dp
    use physcon,        only: gn
    use glimmer_utils

    implicit none

    real(dp), dimension(:,:,:), intent(out) :: dissip
    real(dp), dimension(:,:),   intent(in)  :: thck
    real(dp), dimension(:,:),   intent(in)  :: stagthck
    real(dp), dimension(:,:),   intent(in)  :: dusrfdew
    real(dp), dimension(:,:),   intent(in)  :: dusrfdns
    real(dp), dimension(:,:,:), intent(in)  :: flwa
    integer,                    intent(in)  :: ewn,nsn
    real(dp), dimension(:),     intent(in)  :: c1
    real(dp),                   intent(in)  :: thklim

    integer, parameter :: p1 = gn + 1  
    integer :: ew,ns,nz,up

    real(dp) :: c2
    real(dp) :: sum1,sum2,sum3

    dissip = 0.0d0
    nz = size(dissip,1)
    
    do ns = 2, nsn-1
       do ew = 2, ewn-1
          if (thck(ew,ns) > thklim) then
             
             call mysum(stagthck(ew-1:ew,ns-1:ns),sum1,2,2)
             call mysum(dusrfdew(ew-1:ew,ns-1:ns),sum2,2,2)
             call mysum(dusrfdns(ew-1:ew,ns-1:ns),sum3,2,2)

             if (sum2==0.d0.and.sum3==0.d0) then
                c2 = 0.d0
             else
                c2 = (0.25d0*sum1 * dsqrt((0.25d0*sum2)**2 &
                     + (0.25d0*sum3)**2))**p1
             end if
             do up = 1,nz
                dissip(up,ew,ns) = c2 * c1(up) * ( &
                     flwa(up,ew-1,ns-1) + flwa(up,ew-1,ns+1) + flwa(up,ew+1,ns+1) + flwa(up,ew+1,ns-1) + &
                     2.d0*(flwa(up,ew-1,ns)+flwa(up,ew+1,ns)+flwa(up,ew,ns-1)+flwa(up,ew,ns+1)) + &
                     4.d0*flwa(up,ew,ns))
             end do
          end if
       end do
    end do

  end subroutine finddisp

  !-----------------------------------------------------------------------------------

  subroutine calcbmlt(nx,ny,nz,sigma,dupa,dupb,dupc,dissip,temp,thck,stagthck, &
       dusrfdew,dusrfdns,ubas,vbas,bheatflx,bmlt,floater,f3,thklim)
    use glimmer_global,   only: dp
    use physcon,          only: coni, lhci, rhoi, grav, scyr
    use glimmer_pmpt

    implicit none 

    integer,                      intent(in)  :: nx
    integer,                      intent(in)  :: ny
    integer,                      intent(in)  :: nz
    real(dp), dimension(nz),      intent(in)  :: sigma
    real(dp), dimension(nz),      intent(in)  :: dupa
    real(dp), dimension(nz),      intent(in)  :: dupb
    real(dp), dimension(nz),      intent(in)  :: dupc
    real(dp), dimension(:,:,:),   intent(in)  :: dissip
    real(dp), dimension(:,0:,0:), intent(in)  :: temp
    real(dp), dimension(:,:),     intent(in)  :: thck
    real(dp), dimension(:,:),     intent(in)  :: stagthck
    real(dp), dimension(:,:),     intent(in)  :: dusrfdew
    real(dp), dimension(:,:),     intent(in)  :: dusrfdns
    real(dp), dimension(:,:),     intent(in)  :: ubas
    real(dp), dimension(:,:),     intent(in)  :: vbas
    real(dp), dimension(:,:),     intent(in)  :: bheatflx
    real(dp), dimension(:,:),     intent(out) :: bmlt
    logical,  dimension(:,:),     intent(in)  :: floater
    real(dp),                     intent(in)  :: f3
    real(dp),                     intent(in)  :: thklim
    

    real(dp), dimension(nz) :: pmptemp
    real(dp) :: slterm, newmlt

    integer :: ewp, nsp,up,ew,ns

    real(dp),parameter :: f1 = scyr * coni / (lhci * rhoi)
    real(dp),parameter :: f2 = scyr / (lhci * rhoi)
    real(dp),parameter :: f4 = grav * rhoi / (4.0d0 * rhoi * lhci)

    do ns = 2, ny-1
       do ew = 2, nx-1
          if (thck(ew,ns) > thklim .and. .not. floater(ew,ns)) then

             call calcpmpt(pmptemp,thck(ew,ns),sigma)

             if (abs(temp(nz,ew,ns)-pmptemp(nz)) .lt. 0.001d0) then

                slterm = 0.0d0

                do nsp = ns-1,ns
                   do ewp = ew-1,ew
                      slterm = slterm - stagthck(ewp,nsp) * &
                           (dusrfdew(ewp,nsp) * ubas(ewp,nsp) + dusrfdns(ewp,nsp) * vbas(ewp,nsp))
                   end do
                end do

                bmlt(ew,ns) = 0.0d0
                newmlt = f4 * slterm - f2 * bheatflx(ew,ns) &
                     + f3 * dupc(nz) * thck(ew,ns) * dissip(nz,ew,ns)

                up = nz - 1

                do while (abs(temp(up,ew,ns)-pmptemp(up)) .lt. 0.001d0 .and. up .ge. 3)
                   bmlt(ew,ns) = bmlt(ew,ns) + newmlt
                   newmlt = f3 * dupc(up) * thck(ew,ns) * dissip(up,ew,ns)
                   up = up - 1
                end do

                up = up + 1

                if (up == nz) then
                   bmlt(ew,ns) = newmlt - &
                        f1 * ( (temp(up-2,ew,ns) - pmptemp(up-2)) * dupa(up) &
                        + (temp(up-1,ew,ns) - pmptemp(up-1)) * dupb(up) ) / thck(ew,ns) 
                else
                   bmlt(ew,ns) = bmlt(ew,ns) + max(0.0d0, newmlt - &
                        f1 * ( (temp(up-2,ew,ns) - pmptemp(up-2)) * dupa(up) &
                        + (temp(up-1,ew,ns) - pmptemp(up-1)) * dupb(up) ) / thck(ew,ns)) 
                end if

             else

                bmlt(ew,ns) = 0.0d0

             end if

          else

             bmlt(ew,ns) = 0.0d0

          end if
       end do
    end do

  end subroutine calcbmlt

!real(dp) function is_thin(mask)
!    !*FD returns .true. if node is below dynamic limit
!    implicit none
!    integer, intent(in) :: mask

!    is_thin = (iand(mask,glimmer_mask_thin_ice) .gt. 0 .and. mask.gt.0)
!  end function is_thin

!real(dp) function is_float(mask)
!    !*FD returns .true. if node is floating
!    implicit none
!    integer, intent(in) :: mask

!    is_float = (iand(mask,glimmer_mask_shelf) .gt. 0 .and. mask.gt.0)
!  end function is_float

end module glide_tempFullSoln
