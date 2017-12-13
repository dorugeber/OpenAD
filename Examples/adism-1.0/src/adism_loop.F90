! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
! +                                                               +
! + adism_loop.F90: part of the ADISM adjoint ice sheet model  +
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

module adism_loop

  ! Module to contain the ADISM time loop only (required for REVOLVE)

  implicit none

contains

  subroutine loop(n,m,nz,dx,dy,dt,nt,sigma,depth,thck,topg,prcp,artm_pres,lat,lon,usrf,pddtab,temp,ncdf_out,G, &
       vol,ncdata,nmsb_ctrl,bslide_ctrl,enFac,slide_flag,calving_flag,clim_flag,outfreq)
    !$openad XXX Template loop_template.revolve.f90

    use adism_body
    use adism_core
    use adism_ncdf
    use glimmer_pdd

    integer,                intent(in)       :: n            ! number of grid points in x
    integer,                intent(in)       :: m            ! number of grid points in y
    integer,                intent(in)       :: nz           ! number of vertical levels
    real(dp),               intent(in)       :: dx           ! grid spacing in x (m)
    real(dp),               intent(in)       :: dy           ! grid spacing in y (m)
    real(dp),               intent(in)       :: dt           ! time step (yrs)
    integer,                intent(in)       :: nt           ! number of time steps
    real(dp),dimension(nz)                   :: sigma        ! vertical layers
    real(dp),dimension(nz)                   :: depth        ! for vertical integration of A
    real(dp),dimension(n,m),intent(inout)    :: thck         ! thickness (m)
    real(dp),dimension(n,m),intent(in)       :: topg         ! bed topography (m)
    real(dp),dimension(n,m)                  :: prcp         ! Precipitation (annual, m)
    real(dp),dimension(n,m)                  :: artm_pres
    real(dp),dimension(n,m)                  :: lat
    real(dp),dimension(n,m)                  :: lon 
    real(dp),dimension(n,m),intent(inout)    :: usrf         ! upper surface (m)
    real(dp),dimension(pdd_nx,pdd_ny)        :: pddtab
    real(dp),dimension(nz,0:n+1,0:m+1)       :: temp
    logical,                intent(in)       :: ncdf_out     ! set TRUE if we want to have file output
    real(dp),dimension(n,m)                  :: G            ! geothermal heat flux
    real(dp)                                 :: vol          ! Volume               
    type(adism_ncdata)                       :: ncdata       ! NetCDF files
    real(dp),dimension(n,m)                  :: nmsb_ctrl    ! Mass-balance perturbation
    real(dp),dimension(n,m)                  :: bslide_ctrl  ! Basal sliding perturbation
    real(dp)                                 :: enFac        ! Flow enhancement factor
    logical                                  :: slide_flag   ! set TRUE to run EISMINT2 H (unstable fast-flow spokes)
    logical                                  :: calving_flag ! Set TRUE for calving parameterization
    integer                                  :: clim_flag    ! Select climate (1: EISMINT2, 2: Greenland)
    integer                                  :: outfreq      ! Output to file every n timesteps

    ! Local variables
    integer :: t
    real(dp),dimension(n,m)                  :: thckold      ! old thickness
    real(dp),dimension(n,m)                  :: usrfold      ! old surface
    real(dp),dimension(n,m)                  :: dthckdtm     ! rate of change of thickness (staggered grid)
    real(dp),dimension(n,m)                  :: dusrfdtm     ! rate of change of surface (staggered grid)
    real(dp),dimension(n,m)                  :: diff         ! diffusivity
    real(dp),dimension(n,m)                  :: bwat         ! basal water depth
    real(dp),dimension(n,m)                  :: bmlt         ! basal melt rate

    usrfold  = usrf
    thckold  = thck
    dthckdtm = 0.0
    dusrfdtm = 0.0
    bwat     = 0.0
    bmlt     = 0.0
    diff     = 0.0

    do t=1,nt
       call body(n,m,nz,dx,dy,dt,sigma,depth,thck,topg,usrf,prcp,artm_pres,lat,lon, &
            thckold,usrfold,dthckdtm,dusrfdtm,diff,pddtab,bwat,bmlt,temp,ncdf_out,G,vol, &
            ncdata,nmsb_ctrl,bslide_ctrl,enFac,slide_flag,calving_flag,clim_flag,outfreq,t)
       diff=diff
    enddo

  end subroutine loop

end module adism_loop


