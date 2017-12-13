! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
! +                                                               +
! + adism_adjoint.F90: part of the ADISM adjoint ice sheet model  +
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

program adism_adjoint_nmsb

  use OAD_active
  use OAD_tape
  use OAD_rev

  use adism_data, usrf_X => usrf, thck_X => thck, &
       nmsb_X => nmsb_ctrl, temp_X => temp, vol_X => vol
  use adism_wrap

  implicit none

  !############################################################################################
  ! Active model fields
  type(active),dimension(:,:),  allocatable  :: usrf         ! upper surface
  type(active),dimension(:,:),  allocatable  :: thck         ! thickness
  type(active),dimension(:,:),  allocatable  :: nmsb_ctrl    ! Mass balance control
  type(active),dimension(:,:,:),allocatable  :: temp         ! Ice temperature
  type(active) :: vol

  character(200)  :: config_fname, sens_fname
  integer::i,j

  ! Get configuration filename from command line
  if (command_argument_count()/=1) then
     print*,'You must supply a configuration filename'
     stop
  end if
  call get_command_argument(1,config_fname)

  ! Read configuration file
  call readConfigData(config_fname)

  ! Read adjoint-specific config section
  call readAdjointConfig

  ! Allocate arrays
  call allocateArrays
  allocate(usrf(n,m))
  allocate(thck(n,m))
  allocate(nmsb_ctrl(n,m))
  allocate(temp(nz,0:n+1,0:m+1))

  ncdf_flag = .false.

  ! Load data and initialise model
  call adsim_init
  thck%v = thck_X
  usrf%v = usrf_X
  temp%v = temp_X
  nmsb_ctrl%v=0.d0

  ! Calculate present-day air temperature
  call calcPresArtm(usrf%v,lat,artm_pres,n,m)

  ! global split mode - start with taping
  call oad_tape_init()
  call OAD_revTape()

  call main(n,m,nz,dx,dy,dt,nt,thck,topg,prcp,artm_pres,lat,lon,&
  usrf,nmsb_ctrl,bslide_ctrl,G_ctrl,temp,vol,gthf,enFac,slide_flag,ncdf_flag,calving_flag,&
  clim_flag,out_fname,out_freq)
  
  ! global split mode - follow with reverse
  vol%d=1.0D0
  call OAD_revAdjoint()

  call main(n,m,nz,dx,dy,dt,nt,thck,topg,prcp,artm_pres,lat,lon,&
  usrf,nmsb_ctrl,bslide_ctrl,G_ctrl,temp,vol,gthf,enFac,slide_flag,ncdf_flag,calving_flag,&
  clim_flag,out_fname,out_freq)

  ! Write to file
  open(20,file=sens_fname)
  do i=1,n
     do j=1,m
        write(20,*) i,j,nmsb_ctrl(i,j)%d
     end do
  end do
  close(20)

contains

  subroutine readAdjointConfig

    use glimmer_config

    type(ConfigSection),pointer :: section
    section => null()

    call GetSection(config,section,'adjoint')
    call GetValue(section,'fname',sens_fname)

    print*,'Sensitivity output file: ',trim(sens_fname)

  end subroutine readAdjointConfig

end program adism_adjoint_nmsb
