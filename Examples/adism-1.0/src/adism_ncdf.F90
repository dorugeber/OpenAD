! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
! +                                                               +
! + adism_ncdf.F90: part of the ADISM adjoint ice sheet model     +
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

module adism_ncdf

  ! Netcdf input and output for ADISM

  use netcdf
  use glimmer_global

  implicit none

  type adism_ncdata
     ! NetCDF
     character(50) :: outfile                     ! output file
     integer       :: ncid
     ! NetCDF: Dimension identifiers
     integer       :: xdimid, ydimid, tdimid      ! error, x dimension, y dim
     integer       :: xsdimid, ysdimid, zdimid    ! staggered grid and time
     ! NetCDF: Variable identifiers
     integer       :: usrfvarid                   ! surface elevation
     integer       :: topgvarid                   ! bedrock topography
     integer       :: latvarid                    ! latitude
     integer       :: lonvarid                    ! longitude
     integer       :: diffvarid                   ! diffusivity
     integer       :: stagdiffvarid               ! diffusivity
     integer       :: thckvarid                   ! thickness
     integer       :: nmsbvarid                   ! surface mass balance
     integer       :: prcpvarid                   ! precipitation
     integer       :: uvarid                      ! u velocity
     integer       :: vvarid                      ! v velocity
     integer       :: wvelvarid                   ! Vertical velocity
     integer       :: uvelvarid                   ! x velocity
     integer       :: vvelvarid                   ! y velocity
     integer       :: tempvarid                   ! temperature
     integer       :: dusrfdtmvarid               ! surface derivative w.r.t. time
     integer       :: dthckdtmvarid               ! thickness derivative w.r.t. time
     integer       :: gavarid                     ! Glen's A
     integer       :: sigmavarid                  ! Sigma levels
     integer       :: wgrdvarid                   ! Vertical velocity of grid
     integer       :: ubasvarid                   ! basal x velocity
     integer       :: vbasvarid                   ! basal y velocity
     integer       :: bwatvarid                   ! basal water depth
     integer       :: bmltvarid                   ! basal melt rate
     integer       :: artmvarid                   ! Air temperature
     integer       :: artm_presvarid              ! Present-day air temperature
     integer       :: maskvarid
  end type adism_ncdata

contains

  subroutine netcdf_create(ncdata,n,m,nz,outfile)

    ! Create a netcdf file for output

    type(adism_ncdata), intent(inout) :: ncdata  ! type holding netcdf data
    integer,           intent(in)    :: n       ! number of grid points in x
    integer,           intent(in)    :: m       ! number of grid points in y
    integer,           intent(in)    :: nz      ! number of grid points in z
    character(*),      intent(in)    :: outfile ! name of output file

    integer :: ncerr

    ! Open and define a netcdf file
    ncdata%outfile = outfile

    ! Create file
    ncerr = nf90_create(ncdata%outfile,NF90_CLOBBER,ncdata%ncid)
    call ncerr_handle(ncerr,__LINE__)

    ! Define dimensions (z, x, y, and time)
    ncerr = nf90_def_dim(ncdata%ncid,'x',n,ncdata%xdimid)
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_def_dim(ncdata%ncid,'y',m,ncdata%ydimid)
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_def_dim(ncdata%ncid,'x_s',n-1,ncdata%xsdimid)
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_def_dim(ncdata%ncid,"y_s",m-1,ncdata%ysdimid)
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_def_dim(ncdata%ncid,"z",nz,ncdata%zdimid)
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_def_dim(ncdata%ncid,'time',NF90_UNLIMITED,ncdata%tdimid)
    call ncerr_handle(ncerr,__LINE__)

    ! Define variables
    ncerr = nf90_def_var(ncdata%ncid,'thck',NF90_DOUBLE, &
         (/ncdata%xdimid,ncdata%ydimid,ncdata%tdimid/),ncdata%thckvarid)   
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_def_var(ncdata%ncid,'usrf',NF90_DOUBLE, &
         (/ncdata%xdimid,ncdata%ydimid,ncdata%tdimid/),ncdata%usrfvarid)
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_def_var(ncdata%ncid,'topg',NF90_DOUBLE, &
         (/ncdata%xdimid,ncdata%ydimid,ncdata%tdimid/),ncdata%topgvarid)
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_def_var(ncdata%ncid,'lat',NF90_DOUBLE, &
         (/ncdata%xdimid,ncdata%ydimid,ncdata%tdimid/),ncdata%latvarid)
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_def_var(ncdata%ncid,'lon',NF90_DOUBLE, &
         (/ncdata%xdimid,ncdata%ydimid,ncdata%tdimid/),ncdata%lonvarid)
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_def_var(ncdata%ncid,'nmsb',NF90_DOUBLE, &
         (/ncdata%xdimid,ncdata%ydimid,ncdata%tdimid/),ncdata%nmsbvarid)
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_def_var(ncdata%ncid,'prcp',NF90_DOUBLE, &
         (/ncdata%xdimid,ncdata%ydimid,ncdata%tdimid/),ncdata%prcpvarid)
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_def_var(ncdata%ncid,'diff',NF90_DOUBLE, &
         (/ncdata%xdimid,ncdata%ydimid,ncdata%tdimid/),ncdata%diffvarid)
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_def_var(ncdata%ncid,'stagdiff',NF90_DOUBLE, &
         (/ncdata%xsdimid,ncdata%ysdimid,ncdata%tdimid/),ncdata%stagdiffvarid)
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_def_var(ncdata%ncid,'mask',NF90_INT, &
         (/ncdata%xdimid,ncdata%ydimid,ncdata%tdimid/),ncdata%maskvarid)
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_def_var(ncdata%ncid,'wvel',NF90_DOUBLE, &
         (/ncdata%zdimid,ncdata%xdimid,ncdata%ydimid,ncdata%tdimid/),ncdata%wvelvarid)
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_def_var(ncdata%ncid,'uvel',NF90_DOUBLE, &
         (/ncdata%zdimid,ncdata%xsdimid,ncdata%ysdimid,ncdata%tdimid/),ncdata%uvelvarid)
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_def_var(ncdata%ncid,'vvel',NF90_DOUBLE, &
         (/ncdata%zdimid,ncdata%xsdimid,ncdata%ysdimid,ncdata%tdimid/),ncdata%vvelvarid)
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_def_var(ncdata%ncid,'temp',NF90_DOUBLE, &
         (/ncdata%zdimid,ncdata%xdimid,ncdata%ydimid,ncdata%tdimid/),ncdata%tempvarid)
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_def_var(ncdata%ncid,'dusrfdtm',NF90_DOUBLE, &
         (/ncdata%xdimid,ncdata%ydimid,ncdata%tdimid/),ncdata%dusrfdtmvarid)
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_def_var(ncdata%ncid,'dthckdtm',NF90_DOUBLE, &
         (/ncdata%xdimid,ncdata%ydimid,ncdata%tdimid/),ncdata%dthckdtmvarid)
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_def_var(ncdata%ncid,'sigma',NF90_DOUBLE, &
         (/ncdata%zdimid/),ncdata%sigmavarid)
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_def_var(ncdata%ncid,'wgrid',NF90_DOUBLE, &
         (/ncdata%zdimid,ncdata%xdimid,ncdata%ydimid,ncdata%tdimid/),ncdata%wgrdvarid)  
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_def_var(ncdata%ncid,'ga',NF90_DOUBLE, &
         (/ncdata%zdimid,ncdata%xdimid,ncdata%ydimid,ncdata%tdimid/),ncdata%gavarid)
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_def_var(ncdata%ncid,'ubas',NF90_DOUBLE, &
         (/ncdata%xsdimid,ncdata%ysdimid,ncdata%tdimid/),ncdata%ubasvarid)
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_def_var(ncdata%ncid,'vbas',NF90_DOUBLE, &
         (/ncdata%xsdimid,ncdata%ysdimid,ncdata%tdimid/),ncdata%vbasvarid)
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_def_var(ncdata%ncid,'bwat',NF90_DOUBLE, &
         (/ncdata%xdimid,ncdata%ydimid,ncdata%tdimid/),ncdata%bwatvarid)
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_def_var(ncdata%ncid,'bmlt',NF90_DOUBLE, &
         (/ncdata%xdimid,ncdata%ydimid,ncdata%tdimid/),ncdata%bmltvarid)
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_def_var(ncdata%ncid,'artm',NF90_DOUBLE, &
         (/ncdata%xdimid,ncdata%ydimid,ncdata%tdimid/),ncdata%artmvarid)
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_def_var(ncdata%ncid,'artm_pres',NF90_DOUBLE, &
         (/ncdata%xdimid,ncdata%ydimid,ncdata%tdimid/),ncdata%artm_presvarid)
    call ncerr_handle(ncerr,__LINE__)

    ! End define mode
    ncerr = nf90_enddef(ncdata%ncid)
    call ncerr_handle(ncerr,__LINE__)

  end subroutine netcdf_create

  !================================================================================

  subroutine netcdf_write(ncdata,sigma,lat,lon,thck,usrf,topg,nmsb,prcp,diff,stagdiff, &
       bwat,bmlt,artm,artm_pres,dusrfdtm,dthckdtm,ubas,vbas,temp,u,v,wvel,wgrd,ga,mask,outpos)

    type(adism_ncdata), intent(inout) :: ncdata  ! type holding netcdf data
    real(dp),dimension(:),intent(in) :: sigma
    real(dp),dimension(:,:),intent(in) :: lat,lon,topg,thck,usrf,nmsb,prcp,diff,stagdiff,bwat,bmlt,artm,artm_pres
    real(dp),dimension(:,:),intent(in) :: dusrfdtm,dthckdtm,ubas,vbas
    real(dp),dimension(:,0:,0:),intent(in) :: temp
    real(dp),dimension(:,:,:),intent(in) :: u,v,wvel,wgrd,ga
    integer, dimension(:,:),intent(in) :: mask
    integer,intent(in) :: outpos
    integer :: ncerr,nz,up,n,m

    nz=size(temp,1)
    n= size(thck,1)
    m= size(thck,2)

    ! do loop for the vertical is needed, otherwise variables are missing
    do up=1,nz
       ncerr = nf90_put_var(ncdata%ncid,ncdata%tempvarid,temp(up,1:n,1:m),(/up,1,1,outpos/),(/1,n,m,1/))
       call ncerr_handle(ncerr,__LINE__)
       ncerr = nf90_put_var(ncdata%ncid,ncdata%uvelvarid,u(up,:,:),(/up,1,1,outpos/),(/1,n-1,m-1,1/))
       call ncerr_handle(ncerr,__LINE__)
       ncerr = nf90_put_var(ncdata%ncid,ncdata%vvelvarid,v(up,:,:),(/up,1,1,outpos/),(/1,n-1,m-1,1/))
       call ncerr_handle(ncerr,__LINE__)
       ncerr = nf90_put_var(ncdata%ncid,ncdata%wvelvarid,wvel(up,:,:),(/up,1,1,outpos/),(/1,n,m,1/))
       call ncerr_handle(ncerr,__LINE__)
       ncerr = nf90_put_var(ncdata%ncid,ncdata%wgrdvarid,wgrd(up,:,:),(/up,1,1,outpos/),(/1,n,m,1/))
       call ncerr_handle(ncerr,__LINE__)
       ncerr = nf90_put_var(ncdata%ncid,ncdata%gavarid,ga(up,:,:),(/up,1,1,outpos/),(/1,n,m,1/))
       call ncerr_handle(ncerr,__LINE__)           
    end do

    ncerr = nf90_put_var(ncdata%ncid,ncdata%thckvarid,thck,start=(/1,1,outpos/))
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_put_var(ncdata%ncid,ncdata%usrfvarid,usrf,start=(/1,1,outpos/))
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_put_var(ncdata%ncid,ncdata%topgvarid,topg,start=(/1,1,outpos/))
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_put_var(ncdata%ncid,ncdata%latvarid,lat,start=(/1,1,outpos/))
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_put_var(ncdata%ncid,ncdata%lonvarid,lon,start=(/1,1,outpos/))
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_put_var(ncdata%ncid,ncdata%nmsbvarid,nmsb,start=(/1,1,outpos/))
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_put_var(ncdata%ncid,ncdata%prcpvarid,prcp,start=(/1,1,outpos/))
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_put_var(ncdata%ncid,ncdata%sigmavarid,sigma)
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_put_var(ncdata%ncid,ncdata%dthckdtmvarid,dthckdtm,start=(/1,1,outpos/))
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_put_var(ncdata%ncid,ncdata%dusrfdtmvarid,dusrfdtm,start=(/1,1,outpos/))
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_put_var(ncdata%ncid,ncdata%diffvarid,diff,start=(/1,1,outpos/))
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_put_var(ncdata%ncid,ncdata%stagdiffvarid,stagdiff,start=(/1,1,outpos/))
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_put_var(ncdata%ncid,ncdata%ubasvarid,ubas,start=(/1,1,outpos/))
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_put_var(ncdata%ncid,ncdata%vbasvarid,vbas,start=(/1,1,outpos/))
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_put_var(ncdata%ncid,ncdata%bwatvarid,bwat,start=(/1,1,outpos/))
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_put_var(ncdata%ncid,ncdata%bmltvarid,bmlt,start=(/1,1,outpos/))
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_put_var(ncdata%ncid,ncdata%artmvarid,artm,start=(/1,1,outpos/))
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_put_var(ncdata%ncid,ncdata%artm_presvarid,artm_pres,start=(/1,1,outpos/))
    call ncerr_handle(ncerr,__LINE__)
    ncerr = nf90_put_var(ncdata%ncid,ncdata%maskvarid,mask,start=(/1,1,outpos/))
    call ncerr_handle(ncerr,__LINE__)

    ncerr = nf90_sync(ncdata%ncid)
    call ncerr_handle(ncerr,__LINE__)

  end subroutine netcdf_write

  !================================================================================

  subroutine netcdf_close(ncdata)

    type(adism_ncdata), intent(in) :: ncdata

    integer :: ncerr

    ! Close netcdf
    ncerr = nf90_close(ncdata%ncid)
    call ncerr_handle(ncerr,__LINE__)

  end subroutine netcdf_close

  !================================================================================
  ! subroutine to put thickness and topography as input for Greenland
  subroutine netcdf_read(fname,thck,topg,prcp,temp,lat,lon,slice)
    !$openad XXX Template ad_template.f

    character(*),              intent(in)    :: fname
    real(dp),dimension(:,:),   intent(inout) :: thck
    real(dp),dimension(:,:)                  :: topg,prcp
    real(dp),dimension(:,0:,0:),optional,intent(inout) :: temp
    real(dp),dimension(:,:),   intent(out)   :: lat,lon
    integer,                   intent(in)    :: slice

    integer :: nx,ny,nz
    integer :: ncid,thckinputvar,topginputvar
    integer :: latinputvar,loninputvar,prcpinputvar
    integer :: tempinputvar
    integer :: ncerr
    logical :: topg_flag,prcp_flag,temp_flag,lat_flag,lon_flag
    real(dp),dimension(:,:,:),allocatable :: temp0

    ! Set flags
    topg_flag = .true.
    prcp_flag = .true.
    temp_flag = .true.
    lat_flag  = .true.
    lon_flag  = .true.

    ! Find dimensions
    nx = size(thck,1)
    ny = size(thck,2)
    nz = size(temp,1)

    ! Allocate temp0
    allocate(temp0(nz,nx,ny))

    ! Open the netcdf file
    ncerr = nf90_open(fname,NF90_NOWRITE,ncid)
    call ncerr_handle(ncerr,__LINE__)

    ! Get variable IDs ----------------------------------------------
    ! Thickness may either be 'thk' or 'thck'
    ncerr = nf90_inq_varid(ncid,'thk',thckinputvar)
    if (ncerr==NF90_ENOTVAR) then
       ncerr = nf90_inq_varid(ncid,'thck',thckinputvar)
    endif
    call ncerr_handle(ncerr,__LINE__)

    ! Topography
    ncerr = nf90_inq_varid(ncid,'topg',topginputvar)
    if (ncerr==NF90_ENOTVAR) then
       topg_flag = .false.
    else
       call ncerr_handle(ncerr,__LINE__)
    end if

    ! Precipitation may be either 'presprcp' or 'prcp'
    ncerr = nf90_inq_varid(ncid,'presprcp',prcpinputvar)
    if (ncerr==NF90_ENOTVAR) then
       ncerr = nf90_inq_varid(ncid,'prcp',prcpinputvar)
    endif
    if (ncerr==NF90_ENOTVAR) then
       prcp_flag = .false.
    else
       call ncerr_handle(ncerr,__LINE__)
    end if

    ! Temperature
    ncerr = nf90_inq_varid(ncid,'temp',tempinputvar)
    if (ncerr==NF90_ENOTVAR) then
       temp_flag = .false.
    else
       call ncerr_handle(ncerr,__LINE__)
    end if

    ! Latitude
    ncerr = nf90_inq_varid(ncid,'lat',latinputvar)
    if (ncerr==NF90_ENOTVAR) then
       lat_flag = .false.
    else
       call ncerr_handle(ncerr,__LINE__)
    end if

    ! Longitude
    ncerr = nf90_inq_varid(ncid,'lon',loninputvar)
    if (ncerr==NF90_ENOTVAR) then
       lon_flag = .false.
    else
       call ncerr_handle(ncerr,__LINE__)
    end if

    ! Get values ----------------------------------------
    ! Thickness
    ncerr = nf90_get_var(ncid,thckinputvar,thck,start=(/1,1,slice/),count=(/nx,ny,1/))
    call ncerr_handle(ncerr,__LINE__)

    ! Topography
    if (topg_flag) then
       ncerr = nf90_get_var(ncid,topginputvar,topg,start=(/1,1,slice/),count=(/nx,ny,1/))
       call ncerr_handle(ncerr,__LINE__)
    else
       topg = 0.0
    end if

    ! Precip
    if (prcp_flag) then
       ncerr = nf90_get_var(ncid,prcpinputvar,prcp,start=(/1,1,slice/),count=(/nx,ny,1/))
       call ncerr_handle(ncerr,__LINE__)
    else
       prcp = 0.0
    end if

    ! Temperature
    if (temp_flag) then
       ncerr = nf90_get_var(ncid,tempinputvar,temp0,start=(/1,1,1,slice/),count=(/nz,nx,ny,1/))
       call ncerr_handle(ncerr,__LINE__)
       temp = 0.0
       temp(:,1:nx,1:ny) = temp0
    else
       temp = 0.0
    endif

    ! Latutude
    if (lat_flag) then
       ncerr = nf90_get_var(ncid,latinputvar,lat,start=(/1,1,slice/),count=(/nx,ny,1/))
       call ncerr_handle(ncerr,__LINE__)
    else
       lat = 0.0
    end if

    ! Longitude
    if (lon_flag) then
       ncerr = nf90_get_var(ncid,loninputvar,lon,start=(/1,1,slice/),count=(/nx,ny,1/))
       call ncerr_handle(ncerr,__LINE__)
    else
       lon = 0.0   
    endif

    ! Close the file
    ncerr = nf90_close(ncid)
    call ncerr_handle(ncerr,__LINE__)

    ! Deallocate temp0
    if (allocated(temp0)) deallocate(temp0)

  end subroutine netcdf_read

  !###############################################################################################################
  ! Subroutine to specify error associated with Netcdf output.
  subroutine ncerr_handle(ncerr,line)

    implicit none

    integer :: ncerr
    integer,optional :: line

    if (ncerr/=NF90_NOERR) then
       print*,'Netcdf error ',trim(nf90_strerror(ncerr))
       if (present(line)) print*,"Line: ",line
       stop
    end if

  end subroutine ncerr_handle

end module adism_ncdf
