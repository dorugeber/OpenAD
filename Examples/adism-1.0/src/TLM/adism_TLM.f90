
program adism_TLM

  use OAD_active

  use adism_data, usrf_X => usrf, thck_X => thck, &
       nmsb_X => nmsb_ctrl, temp_X => temp, vol_X => vol
  use adism_wrap

  implicit none

  !############################################################################################
  ! Active model fields
  type(active),dimension(:,:),  allocatable :: usrf       ! upper surface
  type(active),dimension(:,:),  allocatable :: thck       ! thickness
  type(active),dimension(:,:),  allocatable :: nmsb_ctrl  ! Mass balance perturbation
  type(active),dimension(:,:,:),allocatable :: temp       ! Ice temperature
  type(active) :: vol

  character(200)  :: config_fname
  integer :: i,j
  integer :: x_pert=27, y_pert=43

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
  allocate(nmsb_ctrl(n,m))
  allocate(temp(nz,0:n+1,0:m+1))

  ! Load data and initialise model
  call adsim_init
  thck%v = thck_X
  usrf%v = usrf_X
  temp%v = temp_X

  ncdf_flag = .false.

  ! Calculate present-day air temperature
  call calcPresArtm(usrf%v,lat,artm_pres,n,m)

  ! pick one component that makes sense to look at: 
  nmsb_ctrl%d=0.0d0
  nmsb_ctrl(x_pert,y_pert)%d=1.0d0

  ! Main time loop
  call main(n,m,nz,dx,dy,dt,nt,thck,topg,prcp,artm_pres,lat,lon,usrf, &
       nmsb_ctrl,bslide_ctrl,G_ctrl,temp,vol,gthf,enFac,slide_flag,ncdf_flag,calving_flag,&
       clim_flag,out_fname,out_freq)

  write(*,*) "gradient component for d vol/ d nmsb(",x_pert,",",y_pert,")=",vol%d

end program adism_TLM
