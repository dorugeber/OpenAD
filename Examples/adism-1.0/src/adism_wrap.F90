! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
! +                                                               +
! + adism_wrap.F90: part of the ADISM adjoint ice sheet model     +
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

module adism_wrap

  implicit none

contains

  subroutine main(n,m,nz,dx,dy,dt,nt,thck,topg,prcp,artm_pres,lat,lon,usrf,& 
       nmsb_ctrl,bslide_ctrl,G_ctrl,temp,vol,gthf,enFac,slide_flag,ncdf_out,calving_flag,&
       clim_flag,outfile,outfreq)

    use adism_ncdf
    use adism_loop
    use adism_core
    use glimmer_pdd

    implicit none

    ! Subroutine arguments
    integer,                intent(in)     :: n               ! number of grid points in x
    integer,                intent(in)     :: m               ! number of grid points in y
    integer,                intent(in)     :: nz              ! number grid points in vertical
    real(dp),               intent(in)     :: dx              ! grid spacing in x (m)
    real(dp),               intent(in)     :: dy              ! grid spacing in y (m)
    real(dp),               intent(in)     :: dt              ! time step (yrs)
    integer,                intent(in)     :: nt              ! number of time steps
    real(dp),dimension(n,m),intent(inout)  :: thck            ! thickness (m)
    real(dp),dimension(n,m),intent(in)     :: topg            ! bed topography (m)
    real(dp),dimension(n,m),intent(inout)  :: usrf            ! upper surface (m)
    real(dp),dimension(n,m)                :: nmsb_ctrl       ! Mass balance perturbation
    real(dp),dimension(n,m)                :: bslide_ctrl     ! Basal sliding perturbation
    real(dp),dimension(n,m)                :: G_ctrl          ! Geothermal heat flux perturbation
    real(dp),dimension(nz,0:n+1,0:m+1),intent(inout) :: temp  ! ice temperature
    real(dp),dimension(n,m)                :: prcp            ! Precipitation (annual, m)
    real(dp),dimension(n,m)                :: lat
    real(dp),dimension(n,m)                :: lon 
    real(dp),dimension(n,m)                :: artm_pres       ! present-day surface air temperature (degC)
    real(dp)                               :: gthf            ! Uniform Geothermal Heat flux
    real(dp)                               :: enFac           ! Flow enhancement factor
    logical,                intent(in)     :: slide_flag      ! set TRUE to run EISMINT2 H (unstable fast-flow spokes)
    logical,                intent(in)     :: ncdf_out        ! set TRUE if we want to have file output
    logical                                :: calving_flag    ! Set TRUE for calving parameterization
    integer                                :: clim_flag       ! Select climate (1: EISMINT2, 2: Greenland)
    character(200)                         :: outfile         ! Name of output file
    integer                                :: outfreq         ! Output to file every n timesteps

    ! Local variables
    real(dp),dimension(nz)            :: sigma        ! vertical layers
    real(dp),dimension(nz)            :: depth        ! for vertical integration of A
    real(dp),dimension(n,m)           :: G            ! geothermal heat flux
    integer                           :: up
    real(dp)                          :: vol          ! Volume               
    type(adism_ncdata)                :: ncdata       ! NetCDF files
    real(dp)                          :: tmp1
    real(dp),dimension(pdd_nx,pdd_ny) :: pddtab

    !$openad INDEPENDENT(nmsb_ctrl)

    ! Set uniform geothermal heat flux and add perturbation
    G = gthf
    G = G + G_ctrl

    ! Sigma levels
    do up=1,nz
       tmp1 = real(up-1,kind=dp)/real(nz-1,kind=dp)
       call calc_sigma(tmp1,2.d0,sigma(up))
    end do

    ! Calculate depth
    do up=1,nz-1
       depth(up) = ((sigma(up+1)+sigma(up))/2.0d0)**gn *(sigma(up+1)-sigma(up))
    end do
    depth(nz) = 0.0d0

    ! Find initial surface elevation  
    usrf = thck + topg

    ! Initialise mass-balance model
    call glimmer_pdd_init(pddtab)

    ! Open the netcdf file
    if (ncdf_out) call netcdf_create(ncdata,n,m,nz,outfile)

    call loop(n,m,nz,dx,dy,dt,nt,sigma,depth,thck,topg,prcp,artm_pres,lat,&
         lon,usrf,pddtab,temp,ncdf_out,G,vol,ncdata,nmsb_ctrl,bslide_ctrl,&
         enFac,slide_flag,calving_flag,clim_flag,outfreq)

    !$openad DEPENDENT(vol) 
    if (ncdf_out) call netcdf_close(ncdata)

  end subroutine main

end module adism_wrap
