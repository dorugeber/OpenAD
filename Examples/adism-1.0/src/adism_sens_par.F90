! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
! +                                                               +
! + adism_sens_par.F90: part of the ADISM adjoint ice sheet model +
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

  use mpi

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

  ! For mpi
  integer :: ierr
  integer :: nproc, iproc, nextproc
  real(dp) :: tmpsend,tmprecv
  integer,dimension(MPI_STATUS_SIZE) :: status

  ! Initialise MPI
  call MPI_Init(ierr)
  
  ! Discover configuration of MPI
  call MPI_Comm_size(MPI_COMM_WORLD,nproc,ierr)
  call MPI_Comm_rank(MPI_COMM_WORLD,iproc,ierr)
  
  ! Check we have enough processors
  if (nproc<2) then
     print*,'You must run this with more than one processor'
     call adism_exit
  end if

  ! INITIALISATION DONE BY ALL PROCESSORS =============================
  ! Get configuration filename from command line
  if (command_argument_count()/=1) then
     if (iproc==0) print*,'You must supply a configuration filename'
     call adism_exit
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
  nosens_mask = .false.
  sensitivity = 0.0
  perturb = 0.0

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
  ! END OF COMMON PART ==================================================
  
  if (iproc==0) then
     ! For controlling processor
     ! Send each point in turn
     do i = 1, n
        do j = 1, m
           ! Receive the idenitity of the next free processor in the queue
           call MPI_RECV(nextproc,1,MPI_INTEGER,MPI_ANY_SOURCE,10,MPI_COMM_WORLD,status,ierr)
           print*,'@@@PROC:',nextproc,'  I=',i,'  J=',j
           call MPI_SEND(i,1,MPI_INTEGER,nextproc,20,MPI_COMM_WORLD,ierr) 
           call MPI_SEND(j,1,MPI_INTEGER,nextproc,30,MPI_COMM_WORLD,ierr) 
        end do
     end do
     ! Tell processors we're out of points
     do i = 1,nproc-1
        call MPI_SEND(-1,1,MPI_INTEGER,i,20,MPI_COMM_WORLD,ierr) 
     end do
     ! For each point, reduce by addition into sensitivity array
     do i = 1, n
        do j = 1, m
           tmpsend = 0.0
           call MPI_ALLREDUCE (tmpsend,tmprecv,1,MPI_REAL8,MPI_SUM,MPI_COMM_WORLD,ierr) 
           sensitivity(i,j) = tmprecv
        end do
     end do
     ! For each point, reduce by addition into perturbation array
     do i = 1, n
        do j = 1, m
           tmpsend = 0.0
           call MPI_ALLREDUCE (tmpsend,tmprecv,1,MPI_REAL8,MPI_SUM,MPI_COMM_WORLD,ierr) 
           perturb(i,j) = tmprecv
        end do
     end do
  else
     ! For other processors
     do
        call MPI_SEND(iproc,1,MPI_INTEGER,0,10,MPI_COMM_WORLD,ierr) 
        call MPI_RECV(i,1,MPI_INTEGER,0,20,MPI_COMM_WORLD,status,ierr)
        if (i==-1) exit
        call MPI_RECV(j,1,MPI_INTEGER,0,30,MPI_COMM_WORLD,status,ierr)

        !==========================
        ! Do the calculation here
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
             nmsb_ctrl,bslide_ctrl,G_ctrl,temp,vol_plus,gthf,enFac,slide_flag,.false.,&
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
             nmsb_ctrl,bslide_ctrl,G_ctrl,temp,vol_minus,gthf,enFac,slide_flag,.false.,&
             calving_flag,clim_flag,out_fname,out_freq)

        ! Calculate sensitivity
        sensitivity(i,j) = (vol_plus - vol_minus)/(2.0*perturb(i,j))

     end do

     ! Reduce operation for sensitivities
     do i = 1, n
        do j = 1, m
           tmpsend = sensitivity(i,j)
           tmprecv = 0.0
           call MPI_ALLREDUCE (tmpsend,tmprecv,1,MPI_REAL8,MPI_SUM,MPI_COMM_WORLD,ierr) 
        end do
     end do

     ! Reduce operation for perturbations
     do i = 1, n
        do j = 1, m
           tmpsend = perturb(i,j)
           tmprecv = 0.0
           call MPI_ALLREDUCE (tmpsend,tmprecv,1,MPI_REAL8,MPI_SUM,MPI_COMM_WORLD,ierr) 
        end do
     end do
  end if

  ! Open and write output file
  if (iproc==0) then
     open(20,file=sens_fname)
     do i = 1, n
        do j = 1, m
           write(20,*) i,j,sensitivity(i,j),perturb(i,j)
        end do
     end do
  end if

  call adism_exit

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

  subroutine adism_exit
    
    integer :: ierr

    call MPI_Finalize(ierr)
    stop

  end subroutine adism_exit

end program adism_sens
