! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
! +                                                               +
! + adism_sens.F90: part of the ADISM adjoint ice sheet model     +
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

program adism_sens

  use adism_wrap
  use adism_mbal
  use glimmer_pdd
  use adism_data

  implicit none

  real(dp),dimension(:,:),allocatable :: nmsb  ! Mass balance (for perturbation calc only)
  logical, dimension(:,:),allocatable :: nosens_mask
  real(dp),dimension(:,:),allocatable :: sensitivity
  real(dp),dimension(:,:),allocatable :: perturb
  integer  :: which_var = 1
  real(dp) :: h = 1.0e-8
  real(dp) :: vol_plus
  real(dp) :: vol_minus
  integer  :: i,j
  character(200)  :: config_fname, sens_fname

  ! Get configuration filename from command line
  if (command_argument_count()/=1) then
     print*,'You must supply a configuration filename'
     stop
  end if
  call get_command_argument(1,config_fname)

  ! Read configuration file
  call readConfigData(config_fname)
  ncdf_flag = .false.

  ! Read sensitivity-specific config section
  call readSensConfig

  ! Allocate arrays
  call allocateArrays
  allocate(nmsb(n,m))
  allocate(nosens_mask(n,m))
  allocate(sensitivity(n,m))
  allocate(perturb(n,m))

  ! Read in starting state
  call adsim_init

  ! Calculate mask for zero sensitivity
  where (usrf.le.0.0)
     nosens_mask = .true.
     sensitivity = 0.0
  elsewhere
     nosens_mask = .false.
  end where

  ! Calculate unperturbed mass balance
  select case(clim_flag)
  case(1)
     call mass_balance_mm(nmsb,dx,dy,n,m) 
  case(2)
     ! Calculate present-day air temperature
     call calcPresArtm(usrf,lat,artm_pres,n,m)
  case default
     print*,'Unknown climate:',clim_flag
     stop
  end select
  
  ! Main loop 
  do i = 1, n
     do j = 1, m
        
        ! Check if we are at or below sea level
        if (nosens_mask(i,j)) cycle

        ! Read in starting state
        call adsim_init

        ! ++++++++++++++++ CALCULATE POSITIVE PERTURBATION +++++++++++
        select case (which_var)
        case(1)
           nmsb_ctrl = 0.0
           nmsb_ctrl(i,j)   = abs(h * nmsb(i,j))
           if (nmsb_ctrl(i,j)<1e-16) then
              nmsb_ctrl(i,j)   = h/scyr
           end if
           perturb(i,j) = nmsb_ctrl(i,j)
        case(2)
           bslide_ctrl = 0.0
           bslide_ctrl(i,j) = abs(h * 1e-3/scyr) 
           perturb(i,j) = bslide_ctrl(i,j)
        case default
           print*,'Unknown variable'
           stop
        end select
         
        ! ++++++++++++++++ RUN FIRST TIME +++++++++++++++++++++++++++++
        ! Main time loop
        call main(n,m,nz,dx,dy,dt,nt,thck,topg,prcp,artm_pres,lat,lon,usrf,&
             nmsb_ctrl,bslide_ctrl,G_ctrl,temp,vol_plus,gthf,enFac,slide_flag,ncdf_flag,&
             calving_flag,clim_flag,out_fname,out_freq)

        ! Read in starting state
        call adsim_init

        ! ++++++++++++++++ CALCULATE NEGATIVE PERTURBATION +++++++++++
        select case (which_var)
        case(1)
           nmsb_ctrl(i,j)   = - nmsb_ctrl(i,j)
        case(2)
           bslide_ctrl(i,j) = - bslide_ctrl(i,j)
        end select

        ! ++++++++++++++++ RUN SECOND TIME +++++++++++++++++++++++++++++
        ! Main time loop
        call main(n,m,nz,dx,dy,dt,nt,thck,topg,prcp,artm_pres,lat,lon,usrf,&
             nmsb_ctrl,bslide_ctrl,G_ctrl,temp,vol_minus,gthf,enFac,slide_flag,ncdf_flag,&
             calving_flag,clim_flag,out_fname,out_freq)

        ! Calculate sensitivity
        sensitivity(i,j) = (vol_plus - vol_minus)/(2.0*perturb(i,j))

     end do
  end do

  ! Open and write output file
  open(20,file=sens_fname)
  do i = 1, n
     do j = 1, m
        write(20,*) i,j,sensitivity(i,j),perturb(i,j)
     end do
  end do

contains

  subroutine readSensConfig

    use glimmer_config

    type(ConfigSection),pointer :: section
    section => null()

    call GetSection(config,section,'FD')
    call GetValue(section,'fname',sens_fname)
    call GetValue(section,'pfac',h)
    call GetValue(section,'var',which_var)

    print*,'Sensitivity output file: ',trim(sens_fname)
    print*,'Perturbation factor: ',h
    print*,'Variable ref (1: mass bal): ',which_var

  end subroutine readSensConfig

end program adism_sens
