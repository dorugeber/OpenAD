! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
! +                                                               +
! + adism_FD.F90: part of the ADISM adjoint ice sheet model       +
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

program adism_FD

  use adism_wrap
  use adism_mbal
  use glimmer_pdd
  use adism_data

  implicit none

  real(dp),dimension(:,:),allocatable :: nmsb       ! Mass balance (unperturbed)
  character(200)  :: config_fname,outf1,outf2

  ! Variables specific to FD sensitivity
  integer  :: which_var
  real(dp) :: vol_pert, h=1.0D-8, in_pert, fd, arg,arg_pert
  integer  :: x_pert, y_pert

  ! Get configuration filename from command line
  if (command_argument_count()/=1) then
     print*,'You must supply a configuration filename'
     stop
  end if
  call get_command_argument(1,config_fname)

  ! Read configuration file
  call readConfigData(config_fname)
  outf1 = trim(out_fname)//'.1.nc'
  outf2 = trim(out_fname)//'.2.nc'

  ! Allocate arrays
  call allocateArrays
  allocate(nmsb(n,m))

  ! Get information about perturbation from user
  write (*,fmt='(a)',advance='no') 'set perturbation factor:  '
  read (*,*) h
  write (*,fmt='(a)',advance='no') 'x position:  '
  read (*,*) x_pert
  write (*,fmt='(a)',advance='no') 'y position:  '
  read (*,*) y_pert
  write (*,fmt='(a)',advance='no') 'Which variable? (1: nmsb; 2: bslide; 3:geo)'
  read (*,*) which_var

  ! Check for valid variable number
  if (which_var<1.or.which_var>3) then
     print*,"Invalid value of which_var"
     stop
  end if

  ! Set perturbations to zero
  bslide_ctrl = 0.d0
  G_ctrl      = 0.d0
  nmsb_ctrl   = 0.d0

  ! Load data and initialise model
  call adsim_init

  ! Calculate mass balance
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

  ! ++++++++++++++++ RUN FIRST TIME +++++++++++++++++++++++++++++
  ! Main time loop
  call main(n,m,nz,dx,dy,dt,nt,thck,topg,prcp,artm_pres,lat,lon,usrf,&
       nmsb_ctrl,bslide_ctrl,G_ctrl,temp,vol,gthf,enFac,slide_flag,ncdf_flag,&
       calving_flag,clim_flag,outf1,out_freq)

  ! ++++++++++++++++ CALCULATE and APPLY PERTURBATION +++++++++++
  ! pick on nmsb element to perturb, the same as in the TLM driver
  select case (which_var)
  case(1)
     in_pert=nmsb(x_pert,y_pert)*h
     arg = nmsb(x_pert,y_pert)
     nmsb_ctrl(x_pert,y_pert)=in_pert
  case(2)
     in_pert = h * 1e-3/scyr 
     print*,'in_pert is ',in_pert
     bslide_ctrl(x_pert,y_pert)=in_pert
     arg_pert=bslide_ctrl(x_pert,y_pert)
  end select

  ! ++++++++++++++++ RE-INITIALISE MODEL ++++++++++++++++++++++++
  call adsim_init

  ! ++++++++++++++++ RUN SECOND TIME +++++++++++++++++++++++++++++
  ! Main time loop
  call main(n,m,nz,dx,dy,dt,nt,thck,topg,prcp,artm_pres,lat,lon,usrf,&
       nmsb_ctrl,bslide_ctrl,G_ctrl,temp,vol_pert,gthf,enFac,slide_flag,ncdf_flag,&
       calving_flag,clim_flag,outf2,out_freq)

  ! ++++++++++++++++ CALCULATE SENSITIVITY ++++++++++++++++++++++++
  if (in_pert/=0.0D0) then
     fd=(vol_pert-vol)/in_pert
  else 
     fd=0.0D0
  end if
  write(*,*) "arg=", arg, " pert=", in_pert, " vol=", vol, " vol_pert=", vol_pert, "d vol=",vol_pert-vol
  write(*,*) "finite difference (perturbation factor",h,") for d vol/ d nmsb(",x_pert,",",y_pert,")=",fd

end program adism_FD
