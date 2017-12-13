
module adism_ncdf

  type adism_ncdata
     integer dummy
  end type adism_ncdata
  ! Precision
  integer,parameter,private      ::     dp = kind(1.d0)

contains

  subroutine netcdf_create(ncdata,n,m,nz,outfile)

    type(adism_ncdata), intent(inout) :: ncdata  ! type holding netcdf data
    integer,           intent(in)    :: n       ! number of grid points in x
    integer,           intent(in)    :: m       ! number of grid points in y
    integer,           intent(in)    :: nz
    character(*),      intent(in)    :: outfile ! name of output file
  end subroutine netcdf_create

  subroutine netcdf_write(ncdata,sigma,lat,lon,thck,usrf,topg,nmsb,prcp,diff,stagdiff,bwat,bmlt, &
         artm,artm_pres,dusrfdtm,dthckdtm,ubas,vbas,temp,uvel,vvel,wvel,wgrd,ga,mask,outpos)

    type(adism_ncdata), intent(inout) :: ncdata  ! type holding netcdf data
    real(dp),dimension(:),intent(in) :: sigma
    real(dp),dimension(:,:),intent(in) :: thck,usrf,nmsb,diff,stagdiff,bwat,bmlt,artm,artm_pres
    real(dp),dimension(:,:),intent(in) :: dusrfdtm,dthckdtm,ubas,vbas
    real(dp),dimension(:,0:,0:),intent(in) :: temp
    real(dp),dimension(:,:,:),intent(in) :: uvel,vvel,wvel,wgrd,ga
    integer, dimension(:,:),intent(in) :: mask
    integer,intent(in) :: outpos
    real(dp),dimension(:,:),intent(inout)    :: prcp ! precipitation (annual, m)
    real(dp),dimension(:,:),intent(in)       :: lat  ! latitude (degN)
    real(dp),dimension(:,:),intent(in)       :: lon  ! latitude (degE)
    real(dp),dimension(:,:),intent(in)       :: topg
  end subroutine netcdf_write

  subroutine netcdf_close(ncdata)

    type(adism_ncdata), intent(in) :: ncdata

  end subroutine netcdf_close

end module adism_ncdf
