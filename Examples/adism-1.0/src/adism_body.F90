! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
! +                                                               +
! + adism_body.F90: part of the ADISM adjoint ice sheet model     +
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

module adism_body

  ! Main time loop of ADISM

  implicit none

contains

  subroutine body(n,m,nz,dx,dy,dt,sigma,depth,thck,topg,usrf,prcp,artm_pres,lat,lon,thckold,usrfold,dthckdtm,dusrfdtm,diff, &
       pddtab,bwat,bmlt,temp,ncdf_out,G,vol,ncdata,nmsb_ctrl,bslide_ctrl,enFac,slide_flag,calving_flag,clim_flag,outfreq,t)
    !$openad XXX Template body_template.split.f90

    use adism_core  
    use adism_ncdf
    use glide_velo
    use glide_tempfullsoln
    use glimmer_deriv, only: df_field_2d_staggered
    use adism_mbal
    use glimmer_pdd
    use physcon, only: scyr

    implicit none

    ! Subroutine arguments
    integer,                intent(in)       :: n            ! number of grid points in x
    integer,                intent(in)       :: m            ! number of grid points in y
    integer,                intent(in)       :: nz           ! number grid points in vertical ( for full temperature solution)
    real(dp),               intent(in)       :: dx           ! grid spacing in x (m)
    real(dp),               intent(in)       :: dy           ! grid spacing in y (m)
    real(dp),               intent(in)       :: dt           ! time step (yrs)
    real(dp),dimension(nz)                   :: sigma        ! vertical layers
    real(dp),dimension(nz)                   :: depth        ! for vertical integration of A
    real(dp),dimension(n,m),intent(inout)    :: thck         ! ice thickness (m)
    real(dp),dimension(n,m),intent(in)       :: topg         ! bed topography (m)
    real(dp),dimension(n,m),intent(inout)    :: usrf         ! upper surface elevation (m)
    real(dp),dimension(n,m)                  :: prcp         ! precipitation (m/yr)
    real(dp),dimension(n,m)                  :: artm_pres    ! present-day precipitation (m/yr)
    real(dp),dimension(n,m)                  :: lat          ! latitude (degN)
    real(dp),dimension(n,m)                  :: lon          ! latitude (degE)
    real(dp),dimension(n,m)                  :: thckold      ! old ice thickness (m)
    real(dp),dimension(n,m)                  :: usrfold      ! old ice surface (m)
    real(dp),dimension(n,m)                  :: dthckdtm     ! rate of change of thickness (staggered grid)
    real(dp),dimension(n,m)                  :: dusrfdtm     ! rate of change of surface (staggered grid)
    real(dp),dimension(n,m)                  :: diff         ! diffusivity
    real(dp),dimension(pdd_nx,pdd_ny)        :: pddtab       ! The pdd look-up table
    real(dp),dimension(n,m)                  :: bwat         ! basal water depth (m)
    real(dp),dimension(n,m)                  :: bmlt         ! basal melt rate (m/yr)
    real(dp),dimension(nz,0:n+1,0:m+1),intent(inout) :: temp   ! ice temperature
    logical,                intent(in)       :: ncdf_out     ! set TRUE if we want to have file output
    real(dp),dimension(n,m)                  :: G            ! geothermal heat flux
    real(dp)                                 :: vol          ! Volume               
    type(adism_ncdata)                       :: ncdata       ! NetCDF files
    real(dp),dimension(n,m)                  :: nmsb_ctrl    ! Mass balance perturbation (m/yr)
    real(dp),dimension(n,m)                  :: bslide_ctrl  ! Basal sliding perturbation
    real(dp)                                 :: enFac        ! Flow enhancement factor
    logical                                  :: slide_flag   ! set TRUE to run EISMINT2 H (unstable fast-flow spokes)
    logical                                  :: calving_flag ! Set TRUE for calving parameterization
    integer                                  :: clim_flag    ! Select climate (1: EISMINT2, 2: Greenland)
    integer                                  :: outfreq      ! Output to file every n timesteps
    integer                                  :: t            ! Current time (years)

    ! Local variables
    real(dp),dimension(n-1,m-1)    :: stagdiff     ! diffusivity (staggered grid)
    real(dp),dimension(n-1,m-1)    :: stagthck     ! thickness (staggered grid)
    real(dp),dimension(n,m)        :: ga2d         ! Vertically-integrated part of diffusivity calculation
    real(dp),dimension(nz,n,m)     :: ga3d         ! Glen's A (3d version)
    real(dp),dimension(nz,n-1,m-1) :: uvel         ! x velocity field
    real(dp),dimension(nz,n-1,m-1) :: vvel         ! y velocity field
    real(dp),dimension(n,m)        :: ubas         ! u horizontal basal velocity
    real(dp),dimension(n,m)        :: vbas         ! v horizontal basal velocity
    real(dp),dimension(n-1,m-1)    :: stagubas     ! u horizontal basal velocity (staggered grid)
    real(dp),dimension(n-1,m-1)    :: stagvbas     ! v horizontal basal velocity (staggered grid)
    real(dp),dimension(n,m)        :: bslide       ! sliding parameter
    logical, dimension(n,m)        :: slide_mask   ! Sliding mask (true where sliding is taking place)
    real(dp),dimension(nz,n,m)     :: wvel         ! verical ice velocity
    real(dp),dimension(nz,n,m)     :: wgrd         ! vertical grid
    real(dp),dimension(nz,0:n+1,0:m+1) :: tStar    ! ice temperature relative to PMP
    real(dp),dimension(n,m)        :: artm         ! surface air temperature (degC)
    real(dp),dimension(n,m)        :: nmsb         ! Surface mass balance
    integer, dimension(n,m)        :: mask         ! ice thickness mask
    logical, dimension(n,m)        :: floatmask    
    real(dp),dimension(n,m)        :: new_usrf
    integer                        :: outpos       ! current output slice
    integer                        :: count        ! mask count
    real(dp)                       :: sval
    real(dp)                       :: thklim       ! thickness depth limit to be considered
    real(dp), parameter            :: rho = 1000.0
    integer                        :: i,j

    ! Initialisation ---------------------------------------------
    ga3d     = 0.0
    ga2d     = 0.0
    uvel     = 0.0       ! x velocity
    vvel     = 0.0       ! y velocity
    ubas     = 0.0
    vbas     = 0.0
    bslide   = 0.0
    nmsb     = 0.0       ! Mass balance
    stagubas = 0.0       ! x basal velocity
    stagvbas = 0.0       ! y basal velocity
    stagdiff = 0.0       ! diffussivity
    wvel     = 0.0       ! vertical velocity
    wgrd     = 0.0       ! vertical grid
    thklim   = 200.0     ! minimum thickness to be considered when calculating grid velocity
    floatmask = .false.

    ! Copy initial conditions to output
    if (ncdf_out.and.t==1) call netcdf_write(ncdata,sigma,lat,lon,thck,usrf,topg,nmsb,prcp,diff,stagdiff,bwat,bmlt, &
         artm,artm_pres,dusrfdtm,dthckdtm,stagubas,stagvbas,temp,uvel,vvel,wvel,wgrd,ga3d,mask,1)

    ! Calculate mass balance
    select case(clim_flag)
    case(1)
       call mass_balance_mm(nmsb,dx,dy,n,m)
    case(2)
       call greenland_massbalance(n,m,usrf,lat,prcp,artm,artm_pres,nmsb,pddtab)
    case default
       print*,"Unknown climate"
       stop
    end select
    nmsb = nmsb + nmsb_ctrl

    ! Create a mask of the ice sheet to be used in matrix filling
    call masker(n,m,thck,nmsb,mask,count)

    ! Calculate temperature relative to PMP
    call calcTStar(n,m,nz,tStar,temp,thck,sigma)

    ! Set sliding parameter
    call greenland_basalsliding(n,m,nz,tStar,bslide,slide_mask,slide_flag) 
    bslide = bslide + bslide_ctrl

    ! Calculate Glen's A in 3D
    call calcGlenA(n,m,nz,ga3d,tStar,enFac)

    ! Stagger thickness and calculate gradients
    call stagvarb(thck,stagthck,n,m)

    ! Calculate vertical integral in diffusivity
    call calc_gaIntegral(n,m,nz,sigma,depth,ga2d,thck,ga3d)

    ! Calcuate ice-flow diffusivity   
    call calcdiff(n,m,diff,usrf,thck,ga2d,bslide,slide_mask,dx,dy)

    ! Stagger diffusivity
    call stagvarb(diff,stagdiff,n,m)

    ! Solve for new ice surface elevation
    call calcthck(n,m,diff,usrf,nmsb,mask,dx,dy,dt,count,new_usrf)  ! valgrind error(condition jump)
    thck = new_usrf-topg
    usrf = new_usrf

    if (clim_flag==1) then
       ! Calculate surface temperature (EISMINT 2)
       do i = 1,n
          do j = 1,m
             artm(i,j) = 238.15 + 1.67e-5 * sqrt(((i-1)*dx-xs)**2 + ((j-1)*dy-ys)**2) - 273.15
          end do
       end do
    end if

    ! Check we don't have negative thickness
    sval=0.0
    do i=1,n
       do j=1,m
          if (thck(i,j)<0.0) then 
             thck(i,j)=0.0
          end if
          sval=sval+thck(i,j)
       end do
    end do

    ! Do calving 
    ! Using flotation criterion of Veili and Payne 2005, 
    ! set ice thickness and surface elevation to zero where floating ice is less than 0m
    ! floating thck = (sea level - bedrock elevation)*(waterdensity/rhoi)
    ! ->   thck=0,usrf=0 for thck < floating thickness
    ! thck < (sea level - bedrock elevation)*(rho/rhoi)
    ! thck < (rho/rhoi)*(0-topg)
    ! thck < -(rho/rhoi)*(topg)
    if (calving_flag) then
       do i=1,n
          do j=1,m
             if (thck(i,j)< 200-(rho/rhoi)*topg(i,j)) then  
                thck(i,j)=0.0      
             end if
          end do
       end do
    end if

    ! Update upper surface
    usrf = topg + thck

    ! Calculate basal sliding velocities on main grid
    call greenland_basalvelocity(n,m,dx,dy,ubas,vbas,usrf,thck,bslide,slide_mask)

    ! Stagger onto velocity grid
    call stagvarb(ubas,stagubas,n,m)
    call stagvarb(vbas,stagvbas,n,m)

    ! Set surface to bedrock elevation outside ice sheet
    do i=1,n
       do j=1,m
          if (mask(i,j)==0) then 
             usrf(i,j) = topg(i,j)
          end if
          if (usrf(i,j)<topg(i,j)) then 
             usrf(i,j) = topg(i,j)
          end if
       end do
    end do

    call calc_velo(n, &
         m, &
         nz, &
         dx, &
         dy, &
         sigma, &
         ga3d, &
         stagdiff, & ! output
         usrf, &
         thck, &
         stagubas, &
         stagvbas, &
         uvel, &  ! output ( on staggered grid)
         vvel)    ! output ( on staggered grid)

    ! Calculate vertical velocity and grid velocity. In this subroutine, the staggered grids and spatial 
    ! derivatives are calculated. The verical velocity of the grid and the actual vertical velocity are calculated.

    call calcVerticalVelocity(n, &
         m, &
         nz, &
         dx, &
         dy, &
         sigma, &
         1, &         ! 
         thklim, &
         thck, &
         usrf, &
         dthckdtm, &
         dusrfdtm, &
         bmlt, &
         nmsb, &
         uvel, &
         vvel, &
         wvel, &
         wgrd)    ! the last two terms are the output 

    ! Do temperature calculation. In this, finite difference ppearameters are set up in terms of sima vertical 
    ! layers, the staggered variables and spatial derivatives are calculted. Terms of the temperature equation 
    ! is calculated: the dissipation term, trhe upwinded advection term. The temperature of thin ice is set to 
    ! the air temperature and ice free nodes to zero. Finally the basal melt rate, bmlt, is calculated.

    call tstep_tempFullSoln( &
         n,   &
         m,   &
         nz,   &
         dx,   &
         dy,   &
         sigma, &
         temp, &
         artm, &
         thck, &
         usrf, &
         uvel*scyr, &
         vvel*scyr, &
         stagubas*scyr, &
         stagvbas*scyr, &
         wvel*scyr, &
         wgrd*scyr, &
         ga3d*scyr, &
         G, &
         bwat, &
         bmlt, &
         dt/scyr,   &
         thklim,            &
         floatmask)

    ! Convert basal melt into m/s
    bmlt = bmlt/scyr

    ! unimplemented sum
    vol = sval*dx*dy

    print*, t, 'Ice volume',vol

    ! dusrfdtm and dthckdtm. usrf is calculated at every time step. 
    ! It is taken as the sum of the surface elevation at every time step, 
    ! divided by the number of time steps  
    dusrfdtm = (usrf-usrfold)/dt
    dthckdtm = (thck-thckold)/dt

    ! Update stored thickness and surface
    usrfold = usrf
    thckold = thck

    if (mod(t,outfreq) == 0) then
       outpos=t/outfreq+1
       if (ncdf_out) then
          print*,"Writing to file at time:",t
          call netcdf_write(ncdata,sigma,lat,lon,thck,usrf,topg,nmsb,prcp,diff,stagdiff,bwat,bmlt, &
               artm,artm_pres,dusrfdtm,dthckdtm,stagubas,stagvbas,temp,uvel,vvel,wvel,wgrd,ga3d,mask,outpos)
       end if
    endif

  end subroutine body

  !-----------------------------------------------------------------------------------------

  subroutine greenland_massbalance(n,m,usrf,lat,prcp,artm,artm_pres,nmsb,pddtab)
    !$openad XXX Template body_template.split.f90

    use glimmer_global,        only: dp
    use glimmer_pdd,           only: pdd_nx, pdd_ny, glimmer_pdd_mbal
    use adism_core, only: diagwrite2d
    use physcon, only: scyr

    integer,                intent(in)       :: n    ! number of grid points in x
    integer,                intent(in)       :: m    ! number of grid points in y
    real(dp),dimension(n,m),intent(inout)    :: usrf ! upper surface (m)
    real(dp),dimension(n,m)                  :: lat  ! latitude (degN)
    real(dp),dimension(n,m)                  :: prcp ! precipitation (annual, m)
    real(dp),dimension(n,m)                  :: artm ! surface air temperature (degC)
    real(dp),dimension(n,m)                  :: artm_pres
    real(dp),dimension(n,m)                  :: nmsb ! Mass balance
    real(dp),dimension(pdd_nx,pdd_ny)        :: pddtab

    ! Local variables
    real(dp), dimension(n,m)       :: arng    !*FD Annual temperature half-range ($^{\circ}$C)
    real(dp), dimension(n,m)       :: ablt    !*FD Annual ablation (m water equivalent)
    logical,  dimension(n,m)       :: landsea !*FD Land-sea mask (land is TRUE)
    real(dp), dimension(n,m)       :: prcp_scale   ! precipitation (annual, m)
    integer :: i,j

    ablt = 0.0
    arng = 0.0
    prcp_scale = 0.0

    ! Calculate surface temperature and half-range
    do i=1,n
       do j=1,m
          if(usrf(i,j)>0.0) then
             artm(i,j) = 49.13 - 0.007992 * max(usrf(i,j),20.d0*(lat(i,j)-65.d0)) -0.7576 * lat(i,j)
             arng(i,j) = 30.38 - 0.006277 * usrf(i,j) - 0.3262 * lat(i,j) - artm(i,j)
          else
             artm(i,j) = 49.13 - 0.007992 * max(0.d0,20.d0*(lat(i,j)-65.d0)) -0.7576 * lat(i,j)
             arng(i,j) = 30.38 - 0.006277 * 0.0 - 0.3262 * lat(i,j) - artm(i,j)
          end if
       enddo
    enddo

    do i=1,n
       do j=1,m 
          if (usrf(i,j)<0.0) then 
             landsea(i,j) = .false.
          else
             landsea(i,j) = .true.
          end if
       end do
    enddo

    ! Set precipitation scaling
    do i=1,n
       do j=1,m
          prcp_scale(i,j)=prcp(i,j)*1.0543d0**(artm(i,j)-artm_pres(i,j))
       enddo
    enddo

    ! Call the mass balance parameterisation, based on the degree day
    call glimmer_pdd_mbal(pddtab,artm,arng,prcp_scale,ablt,nmsb,landsea)

    ! Convert mass-balance to SI units
    nmsb = nmsb/scyr

  end subroutine greenland_massbalance

  !-----------------------------------------------------------------------------------------

  subroutine greenland_basalsliding(n,m,nz,tStar,bslide,slide_mask,slide_flag)
    !$openad XXX Template body_template.split.f90    

    use glimmer_global,        only: dp
    use physcon, only: scyr

    integer,                intent(in)       :: n          ! number of grid points in x
    integer,                intent(in)       :: m          ! number of grid points in y
    integer,                intent(in)       :: nz         ! number of grid points in z
    real(dp),dimension(nz,0:n+1,0:m+1)       :: tStar      ! ice temperature relative to PMP
    real(dp),dimension(n,m),intent(inout)    :: bslide     ! upper surface (m)
    logical, dimension(n,m),intent(inout)    :: slide_mask ! Mask, set true for sliding
    logical                                  :: slide_flag ! set TRUE to run EISMINT2 H (unstable fast-flow spokes)

    ! Local variables
    integer :: i,j

    ! Set uniform basal sliding parameter
    bslide = 1e-3/scyr 

    ! Reset sliding mask
    slide_mask = .false.

    ! Set sliding mask
    if (slide_flag) then
       do i = 1,n
          do j = 1,m
             if (tStar(nz,i,j) == 0.0) then
                slide_mask(i,j) = .true. 
             else
                slide_mask(i,j) = .false.
             end if
          end do
       end do
    end if

  end subroutine greenland_basalsliding

  !-----------------------------------------------------------------------------------------

  subroutine greenland_basalvelocity(n,m,dx,dy,ubas,vbas,usrf,thck,bslide,slide_mask)
    !$openad XXX Template body_template.split.f90    

    use glimmer_global,        only: dp
    use physcon,               only: rhoi, grav
    use adism_core

    integer                    :: n    ! number of grid points in x
    integer                    :: m    ! number of grid points in y
    real(dp)                   :: dx   ! grid spacing in x (m)
    real(dp)                   :: dy   ! grid spacing in y (m)
    real(dp),dimension(n,m)    :: ubas ! latitude (degN)
    real(dp),dimension(n,m)    :: vbas ! precipitation (annual, m)
    real(dp),dimension(n,m)    :: usrf
    real(dp),dimension(n,m)    :: thck
    real(dp),dimension(n,m),intent(inout) :: bslide ! upper surface (m)
    logical, dimension(n,m)    :: slide_mask
    integer :: i,j

    !Setup for bslide_ctrl pertubation to be 0 in areas where ice is non-temperate, for sensitivity.
    do i = 2,n-1
       do j = 2,m-1
          if (slide_mask(i,j)) then
             ubas(i,j) = -bslide(i,j) * rhoi * grav * thck(i,j) * (usrf(i+1,j)-usrf(i-1,j))/(2.0*dx)
             vbas(i,j) = -bslide(i,j) * rhoi * grav * thck(i,j) * (usrf(i,j+1)-usrf(i,j-1))/(2.0*dy)
          else 
             ubas(i,j) = 0.0
             vbas(i,j) = 0.0
          end if
       enddo
    enddo

  end subroutine greenland_basalvelocity

end module adism_body
