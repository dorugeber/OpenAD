! ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
! +                                                            +
! + adism.F90: part of the ADISM adjoint ice sheet model       +
! +                                                            +
! ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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

program adism

  use adism_wrap
  use adism_data

  implicit none

  character(200)  :: config_fname

  ! Get configuration filename from command line
  if (command_argument_count()/=1) then
     print*,'You must supply a configuration filename'
     stop
  end if
  call get_command_argument(1,config_fname)

  ! Read configuration file
  call readConfigData(config_fname)
  
  ! Allocate arrays
  call allocateArrays

  ! Set perturbations to zero
  bslide_ctrl = 0.d0
  G_ctrl      = 0.d0
  nmsb_ctrl   = 0.d0

  ! Load data and initialise model
  call adsim_init

  ! Calculate present-day air temperature
  call calcPresArtm(usrf,lat,artm_pres,n,m)

  ! Main time loop
  call main(n,m,nz,dx,dy,dt,nt,thck,topg,prcp,artm_pres,lat,lon,usrf,&
       nmsb_ctrl,bslide_ctrl,G_ctrl,temp,vol,gthf,enFac,slide_flag,.true.,&
       calving_flag,clim_flag,out_fname,out_freq)

end program adism
