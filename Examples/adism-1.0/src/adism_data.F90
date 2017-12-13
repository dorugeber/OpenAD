! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
! +                                                               +
! + adism_data.F90: part of the ADISM adjoint ice sheet model     +
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

module adism_data

  ! Contains core data for ADISM, and subroutines for
  ! configuring the model and its adjoint.

  use glimmer_global, only: dp
  use glimmer_config, only: ConfigSection
  use physcon,        only: scyr

  implicit none

  !############################################################################################
  ! Model parameters

  ! Grid
  integer         :: n                        ! number of grid points in x
  integer         :: m                        ! number of grid points in y
  integer         :: nz                       ! number of vertical levels
  real(dp)        :: dx, dy                   ! grid spacing (m)

  ! time 
  real(dp)        :: dt                       ! time step (s)
  integer         :: nt                       ! number of time steps

  ! Physical parameters
  real(dp)        :: enFac         = 1.0      ! Flow enhancement factor
  real(dp)        :: gthf          = -0.042   ! Uniform geothermal heat flux
  real(dp)        :: vol                      ! Ice volume at end of run
  real(dp)        :: init_temp_val = -10.0    ! Initial uniform ice temperature

  ! Configuration parameters
  integer         :: clim_flag     = 1        ! Select climate (1: EISMINT2, 2: Greenland)
  logical         :: calving_flag  = .false.  ! True if calving is turned on
  logical         :: slide_flag    = .false.  ! True if sliding is turned on
  integer         :: init_temp                ! Set to 1 if we are starting from an isothermal state

  ! Files
  character(100)  :: init_fname               ! Filename for inital conditions
  integer         :: start_slice              ! Slice to start from
  character(200)  :: out_fname                ! Output filename
  integer         :: out_freq      = 20       ! Output to file every n timesteps
  logical         :: ncdf_flag     = .true.   ! Set true if we are outputing to file

  !############################################################################################
  ! Model fields
  real(dp),dimension(:,:),  allocatable :: usrf            ! upper surface
  real(dp),dimension(:,:),  allocatable :: topg            ! topography
  real(dp),dimension(:,:),  allocatable :: thck            ! thickness
  real(dp),dimension(:,:),  allocatable :: nmsb_ctrl       ! Mass balance perturbation
  real(dp),dimension(:,:),  allocatable :: bslide_ctrl     ! Basal sliding parameter perturbation
  real(dp),dimension(:,:),  allocatable :: G_ctrl          ! Geothermal heat flux parameter perturbation
  real(dp),dimension(:,:,:),allocatable :: temp            ! Ice temperature
  real(dp),dimension(:,:),  allocatable :: prcp            ! Precipitation (annual, m)
  real(dp),dimension(:,:),  allocatable :: artm_pres       ! Present-day air temperature
  real(dp),dimension(:,:),  allocatable :: lat             ! Latitude
  real(dp),dimension(:,:),  allocatable :: lon             ! Longitude

  type(ConfigSection),pointer :: config => null()          ! Configuration data

contains

  subroutine diagnostics(tag)

    character(*) :: tag

    print*,'======================',trim(tag),'======================'

    print*,'**DIAG01',n,m,nz,dx,dy
    print*,'**DIAG02',dt,nt
    print*,'**DIAG03',enFac,gthf,vol,init_temp_val
    print*,'**DIAG04',clim_flag,calving_flag,slide_flag,init_temp
    print*,'**DIAG05',trim(init_fname),start_slice,trim(out_fname),out_freq,ncdf_flag
    print*,'**DIAG06',sum(usrf),sum(topg),sum(thck)
    print*,'**DIAG07',sum(nmsb_ctrl),sum(bslide_ctrl),sum(G_ctrl)
    print*,'**DIAG07',sum(temp),sum(prcp),sum(artm_pres)
    print*,'**DIAG08',sum(lat),sum(lon)
    print*,'======================================================='

  end subroutine diagnostics

  subroutine readConfigData(config_fname)

    use glimmer_config

    character(*),intent(in) :: config_fname ! Name of config file

    type(ConfigSection),pointer :: section
    integer :: calv,slid

    section => null()

    call ConfigRead(config_fname,config)
    
    call GetSection(config,section,'grid')
    call GetValue(section,'nx',n)
    call GetValue(section,'ny',m)
    call GetValue(section,'nz',nz)
    call GetValue(section,'dx',dx)
    call GetValue(section,'dy',dy)

    call GetSection(config,section,'timesteps')
    call GetValue(section,'dt',dt)
    call GetValue(section,'nt',nt)
    dt = dt * scyr  ! Convert timestep to seconds

    call GetSection(config,section,'params')
    call GetValue(section,'enFac',enFac)
    call GetValue(section,'climate',clim_flag)
    call GetValue(section,'calving',calv)
    call GetValue(section,'sliding',slid)
    call GetValue(section,'gthf',gthf)
    if (calv==1) then
       calving_flag = .true.
    else
       calving_flag = .false.
    end if
    if (slid==1) then
       slide_flag = .true.
    else
       slide_flag = .false.
    end if

    init_fname = ""
    start_slice = 0
    call GetSection(config,section,'init')
    if (associated(section)) then
       call GetValue(section,'fname',init_fname)
       call GetValue(section,'init_temp',init_temp)
       call GetValue(section,'init_temp_val',init_temp_val)
       call GetValue(section,'slice',start_slice)
    else
       init_fname = ""
       init_temp = 1
    end if

    call GetSection(config,section,'output')
    call GetValue(section,'fname',out_fname)
    call GetValue(section,'freq',out_freq)

    print*,'NX:',n
    print*,'NY:',m
    print*,'NZ:',nz
    print*,'DX:',dx
    print*,'DY:',dy
    print*,'DT:',dt
    print*,'NT:',nt
    print*,'Enhancement factor:',enFac
    print*,'Geothermal heat flux:',gthf
    print*,'Climate flag:',clim_flag
    print*,'Calving flag:',calv
    print*,'Filename:',init_fname
    print*,'Slice:',start_slice
    print*,'Temperature initialisation (1=isothermal):',init_temp
    if (init_temp==1) print*,'Isothermal temperature:',init_temp_val
    print*,'Output filename:',out_fname
    print*,'Output period (timesteps):',out_freq

  end subroutine readConfigData

  !==========================================================================

  subroutine allocateArrays

    allocate(usrf(n,m))
    allocate(topg(n,m))
    allocate(thck(n,m))
    allocate(nmsb_ctrl(n,m))
    allocate(bslide_ctrl(n,m))
    allocate(G_ctrl(n,m))
    allocate(temp(nz,0:n+1,0:m+1))
    allocate(prcp(n,m))
    allocate(artm_pres(n,m))
    allocate(lat(n,m))
    allocate(lon(n,m))

  end subroutine allocateArrays

  !==========================================================================

  subroutine calcPresArtm(usrf,lat,artm_pres,n,m)

    integer,intent(in) :: n,m
    real(dp),dimension(n,m),intent(in)  :: usrf,lat
    real(dp),dimension(n,m),intent(out) :: artm_pres

    integer :: i,j

    do i=1,n
       do j=1,m
          if(usrf(i,j)>0.d0)then
             artm_pres(i,j) = 49.13d0 - 0.007992d0 * max(usrf(i,j),20.d0*(lat(i,j)-65.d0)) -0.7576d0 * lat(i,j)
          else
             artm_pres(i,j) = 49.13d0 - 0.007992d0 * max(0.d0,20.d0*(lat(i,j)-65.d0)) -0.7576d0 * lat(i,j)
          end if
       enddo
    enddo

  end subroutine calcPresArtm

  !==========================================================================

  subroutine adsim_init

    use adism_ncdf

    if (trim(init_fname)/="") then
       call netcdf_read(init_fname,thck,topg,prcp,temp,lat,lon,start_slice)
    else
       thck = 0.0
       topg = 0.0
    end if
    if (init_temp==1) temp = init_temp_val

    ! Calculate upper surface elevation
    usrf = topg + thck

  end subroutine adsim_init

end module adism_data
