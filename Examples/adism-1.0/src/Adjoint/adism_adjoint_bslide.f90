
program adism_adjoint_bslide

  use OAD_active
  use OAD_tape
  use OAD_rev

  use adism_data, usrf_X => usrf, thck_X => thck, &
       bslide_X => bslide_ctrl, temp_X => temp, vol_X => vol
  use adism_wrap_bslide

  implicit none

  !############################################################################################
  ! Active model fields
  type(active),dimension(:,:),  allocatable  :: usrf         ! upper surface
  type(active),dimension(:,:),  allocatable  :: thck         ! thickness
  type(active),dimension(:,:),  allocatable  :: bslide_ctrl  ! Basal sliding
  type(active),dimension(:,:,:),allocatable  :: temp         ! Ice temperature
  type(active) :: vol

  character(200)  :: config_fname
  integer::i,j

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
  allocate(usrf(n,m))
  allocate(thck(n,m))
  allocate(bslide_ctrl(n,m))
  allocate(temp(nz,0:n+1,0:m+1))

  ncdf_flag = .false.

  ! Load data and initialise model
  call adsim_init
  thck%v = thck_X
  usrf%v = usrf_X
  temp%v = temp_X

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
  open(20,file='adjoint_thermo_sens.txt')
  do i=1,n
     do j=1,m
        write(20,*) i,j,bslide_ctrl(i,j)%d
     end do
  end do
  close(20)


end program adism_adjoint_bslide
