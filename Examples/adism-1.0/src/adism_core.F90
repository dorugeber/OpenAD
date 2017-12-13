! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
! +                                                               +
! + adism_core.F90: part of the ADISM adjoint ice sheet model     +
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

module adism_core

  ! adism core:  contains all the subroutines that are called in the
  ! main program

  use glimmer_utils
  use glimmer_deriv
  use physcon, only: rhoi, grav, pmlt,gn

  implicit none

  ! Physical constants
  real(dp),parameter     ::     const = 2.0*(rhoi*grav)**gn/(gn + 2)
  real(dp),parameter     ::     xs=750000,ys=750000   ! Distance to centre of ice sheet (m)
  real(dp),parameter     ::     St=0.000167  

contains

  subroutine calcGlenA(n,m,nz,ga,tStar,enFac)

    ! Calculate Glen's flow law parmeter based on temperature relative to the pressure melting point

    use physcon, only: actenh, actenl, gascon, arrmlh, arrmll
    ! actenh: Activation energy for T*>263K (Pa^-3 s^-1)
    ! actenl: Activation energy for T*<263K (Pa^-3 s^-1)
    ! arrmlh: Arrhenius constant for T*>263K (Pa^-3 s^-1)
    ! arrmll: Arrhenius constant for T*<263K (Pa^-3 s^-1)

    implicit none

    integer,intent(in) :: n
    integer,intent(in) :: m
    integer,intent(in) :: nz
    real(dp),dimension(nz,n,m),        intent(out) :: ga    ! Glen's A
    real(dp),dimension(nz,0:n+1,0:m+1),intent(in)  :: tStar ! temperature corrected for PMP
    real(dp) :: enFac   ! Enhancement factor
    integer :: i,j,up

    ga=0.0
    do i=1,n
       do j=1,m
          do up=1,nz
             if (tStar(up,i,j).ge.-10.0) then
                ga(up,i,j) = enFac*arrmlh*exp(-(actenh/(gascon*(tStar(up,i,j)+273.15))))
             else
                ga(up,i,j) = enFac*arrmll*exp(-(actenl/(gascon*(tStar(up,i,j)+273.15))))
             end if
          enddo
       enddo
    enddo

  end subroutine calcGlenA

  !###############################################################################################################

  subroutine calcTStar(n,m,nz,tStar,temp,thck,sigma)

    ! Calculate the temperature corrected for pressure melting point

    use physcon, only: rhoi, grav, pmlt
    implicit none

    integer,intent(in) :: n
    integer,intent(in) :: m
    integer,intent(in) :: nz
    real(dp),dimension(nz,0:n+1,0:m+1),intent(out)  :: tStar ! temperature corrected for PMP
    real(dp),dimension(nz,0:n+1,0:m+1),intent(in)   :: temp  ! temperature
    real(dp),dimension(n,m),           intent(in)   :: thck  ! ice thickness
    real(dp),dimension(nz),            intent(in)   :: sigma ! sigma levels

    integer :: up,ew,ns

    tStar(:,0,:) = 0.0
    tStar(:,n+1,:) = 0.0
    tStar(:,:,0) = 0.0
    tStar(:,:,m+1) = 0.0

    do up = 1, nz
       do ew = 1,n
          do ns = 1,m
             tStar(up,ew,ns) = temp(up,ew,ns) + rhoi * grav * pmlt * thck(ew,ns) * sigma(up)
             if (tStar(up,ew,ns)>0.0) tStar(up,ew,ns) = 0.0
          end do
       end do
    end do

  end subroutine calcTStar

  !###############################################################################################################

  subroutine calcdiff(n,m,diff,usrf,thck,gaIntegral,bslide,slide_mask,dx,dy)

    ! Calculation of diffusivity on regular grid
    ! This version takes the whole of the vertical integral as input
    ! See Rutt et al. (2009) for details

    use physcon, only: rhoi, grav
    implicit none

    ! The vertically-integrated ice flux (m^2/yr) is the diffusivity D multiplied by 
    ! the appropriate ice-surface slope ds/dx

    ! Arguments -------------------------------------------------

    integer,            intent(in)      :: n,m             ! number of nodes in x and y
    real(dp),dimension(n,m),intent(inout) :: diff          ! Diffusivity
    real(dp),dimension(n,m),intent(in)  :: usrf            ! Upper surface (m)
    real(dp),dimension(n,m),intent(in)  :: thck            ! thickness (m)
    real(dp),dimension(n,m),intent(in)  :: gaIntegral      ! vertical integral including ga
    real(dp),dimension(n,m),intent(in)  :: bslide          ! sliding parameter
    logical, dimension(n,m),intent(in)  :: slide_mask      ! Mask where sliding is happening
    real(dp),               intent(in)  :: dx,dy           ! Grid spacing (m)

    ! Internal variables ----------------------------------------------------
    integer                     ::  i,j                     
    real(dp)                    ::  dsdx,dsdy                 ! Rate of change of surface / surface slope 

    diff(1,:)=0.0D0
    diff(n,:)=0.0D0
    diff(:,1)=0.0D0
    diff(:,m)=0.0D0

    do i=2,n-1
       do j=2,m-1
          if (thck(i,j) > 0.0) then
             dsdx = (usrf(i+1,j) - usrf(i-1,j)) / (2.0 * dx)
             dsdy = (usrf(i,j+1) - usrf(i,j-1)) / (2.0 * dy)
             if (slide_mask(i,j)) then
                diff(i,j) = gaIntegral(i,j) * (dsdx**2 + dsdy**2)**((gn-1)/2.0) * thck(i,j)**(gn+2) &
                     + bslide(i,j) * rhoi * grav * thck(i,j)**2.0
             else
                diff(i,j) = gaIntegral(i,j) * (dsdx**2 + dsdy**2)**((gn-1)/2.0) * thck(i,j)**(gn+2) 
             endif
          else
             diff(i,j) = 0.0
          end if
       end do
    end do

  end subroutine calcdiff

  !###############################################################################################################

  subroutine calc_gaIntegral(nx,ny,nz,sigma,depth,dintflwa,thck,flwa)
    !*FD this routine calculates the part of the vertically averaged velocity 
    !*FD field which solely depends on the temperature

    use glimmer_utils, only : vertintg
    use glimmer_global, only: dp
    use physcon, only: rhoi, grav
    implicit none

    !------------------------------------------------------------------------------------
    ! Subroutine arguments
    !------------------------------------------------------------------------------------
    integer,                      intent(in)  :: nx
    integer,                      intent(in)  :: ny
    integer,                      intent(in)  :: nz
    real(dp),dimension(nz),       intent(in)  :: sigma
    real(dp),dimension(nz),       intent(in)  :: depth
    real(dp),dimension(nx,ny),    intent(out) :: dintflwa
    real(dp),dimension(nx,ny),    intent(in)  :: thck           !*FD ice thickness
    real(dp),dimension(nz,nx,ny), intent(in)  :: flwa           !*FD ice flow factor

    !------------------------------------------------------------------------------------
    ! Internal variables
    !------------------------------------------------------------------------------------
    real(dp),dimension(nz) :: dups
    real(dp),dimension(nz) :: intflwa 
    integer :: ew,ns,up
    real(dp),parameter :: c = 2.0d0*(rhoi*grav)**gn/2.0d0

    do up=1,nz-1
       dups(up) = sigma(up+1) - sigma(up)
    end do
    dups(nz) = 0.0d0
    !dups  = (/ (sigma(up+1) - sigma(up), up=1,nz-1),0.0d0 /)

    do ns = 1,ny
       do ew = 1,nx
          if (thck(ew,ns) /= 0.0d0) then

             intflwa(nz) = 0.0d0

             do up = nz-1, 1, -1
                intflwa(up) = intflwa(up+1) + depth(up) * (flwa(up,ew,ns)+flwa(up+1,ew,ns))
             end do

             dintflwa(ew,ns) = c * vertintg(dups,intflwa)

          else 

             dintflwa(ew,ns) = 0.0d0

          end if
       end do
    end do
  end subroutine calc_gaIntegral

  !###############################################################################################################
  ! Calculation of ice thickness
!!$
!!$  Ice thickness is found by solving matrix equation Ax=b
!!$  A is known matrix containing diffusivity elements 
!!$  b is vector of unknowns of lenght n (ice at old time step and mass balance)
!!$  x is vector of lenght n of surface elevations at new time step of lenght n
!!$  
!!$  Diffusivity solved on regular grid
!!$  Surface elevation solved on staggered 

  subroutine calcthck (n,m,diff,usrf,nmsb,mask,dx,dy,dt,totpts,new_usrf)

    implicit none

    ! Specify the mass balance           
    ! Calculate sparse matrix elements and fill sparse matrix
    ! Calculate row of sparse matrix equation 
    ! Solve the system using SLAP
    ! Move new surface elevation from vector to appropriate place in 2d planform grid. 

    ! Subroutine arguments ----------------------------------------------
    integer,                   intent(in)  :: n,m
    real(dp),   dimension(n,m),intent(in)  :: diff         ! Diffusivity
    real(dp),   dimension(n,m),intent(in)  :: usrf         ! Upper surface  
    real(dp),   dimension(n,m),intent(in)  :: nmsb         ! Mass balance
    integer,    dimension(n,m),intent(in)  :: mask         ! Index mask
    real(dp),                  intent(in)  :: dx,dy,dt     ! Grid spacing and timestep
    integer,                   intent(in)  :: totpts       ! Number of non-zero points in mask
    real(dp),   dimension(n,m),intent(out) :: new_usrf     ! new ice thicknesses

    ! Internal variables -------------------------------------------------
    ! For matrix solution
    real(dp),dimension(totpts)   :: b           ! RHS vector
    real(dp),dimension(totpts)   :: x           ! Unknown vector
    integer, dimension(totpts*5) :: matrow      ! Row index of sparse matrix elements
    integer, dimension(totpts*5) :: matcol      ! Column index of sparse matrix elements
    real(dp),dimension(totpts*5) :: matval      ! Values of sparse matrix elements
    integer                      :: matcount    ! Index of current final element

    ! Initialise sparse matrix to zero elements
    matcount = 0
    matrow = 0
    matcol = 0
    matval = 0.0
    x = 0.0

    ! calculate sparse matrix elements, fill sparse matrix
    call findsums(n,m,totpts,diff,mask,nmsb,usrf,b,matval,matcol,matrow,matcount,dx,dy,dt)

    ! Solve the system using UMFPACK
    call adism_umf(totpts,matcount,matrow,matcol,matval,b,x)

    ! Move new survector to surface elevation from vector to appropriate place in 
    ! 2d planform grid. If mask of vector b is greater than zero then each element xp,xm,yp,ym and 
    ! diagonal element 1-xp-xm-yp-ym are assigned to matrix A.
    call fillarray(totpts,usrf,x,new_usrf,mask,n,m)

  end subroutine calcthck

  !########################################################################################

  subroutine findsums(n,m,totpts,diff,mask,nmsb,usrf,b,matval,matcol,matrow,matcount,dx,dy,dt)

    ! subroutine to calculate the elements of the matrix

    implicit none
    integer,                intent(in)  :: n,m
    integer,                intent(in)  :: totpts       ! Number of non-zero points in mask
    real(dp),dimension(n,m),intent(in)  :: diff
    integer, dimension(n,m),intent(in)  :: mask
    real(dp),dimension(n,m),intent(in)  :: nmsb
    real(dp),dimension(n,m),intent(in)  :: usrf
    real(dp),dimension(totpts)          :: b           ! RHS vector
    integer, dimension(totpts*5)        :: matrow      ! Row index of sparse matrix elements
    integer, dimension(totpts*5)        :: matcol      ! Column index of sparse matrix elements
    real(dp),dimension(totpts*5)        :: matval      ! Values of sparse matrix elements
    integer                             :: matcount    ! Index of current final element
    real(dp),               intent(in)  :: dx,dy,dt    ! Grid spacing and timestep

    real(dp) :: xp,xm,yp,ym
    integer  :: i,j     
    real(dp) :: few, fns  

    ! calculate sparse matrix elements in terms of diffusivity
    few = -0.5D0 * dt / (dx * dx)        
    fns = -0.5D0 * dt / (dy * dy)

    do i = 2,n-1             ! from 2 otherwise diff(i-1,j) would be zero
       do j = 2,m-1

          if (mask(i,j)>0) then

             xp =(diff(i+1,j)+diff(i,j))*few            ! xp=(dt/2dx**2)(d(i+1,j)+d(i,j))     
             xm =(diff(i-1,j)+diff(i,j))*few            ! xm=(dt/2dx**2)(d(i-1,j)+d(i,j))
             yp =(diff(i,j+1)+diff(i,j))*fns            ! yp=(dt/2dx**2)(d(i,j+1)+d(i,j))
             ym =(diff(i,j-1)+diff(i,j))*fns            ! ym=(dt/2dx**2)(d(i,j-1)+d(i,j))

             call generate_row(n,m,totpts,i,j,mask,xp,xm,yp,ym,matval,matcol,matrow,matcount)    
             b(mask(i,j))  = dt * nmsb(i,j) + usrf(i,j)

          end if
       end do
    end do

  end subroutine findsums

  !##################################################

  subroutine generate_row(n,m,totpts,i,j,mask,xp,xm,yp,ym,matval,matcol,matrow,matcount)

    ! calculate row of sparse matrix equation 

    implicit none

    integer,                   intent(in)  :: n,m
    integer,                   intent(in)  :: totpts      ! Number of non-zero points in mask
    integer,                   intent(in)  :: i,j
    integer, dimension(n,m),   intent(in)  :: mask 
    real(dp),                  intent(in)  :: xp,xm,yp,ym ! Sums
    integer, dimension(totpts*5)           :: matrow      ! Row index of sparse matrix elements
    integer, dimension(totpts*5)           :: matcol      ! Column index of sparse matrix elements
    real(dp),dimension(totpts*5)           :: matval      ! Values of sparse matrix elements
    integer                                :: matcount    ! Index of current final element
    real(dp) :: tmp

    ! fill sparse matrix
    call putpcgc(totpts,xp,mask(i+1,j),mask(i,j),matval,matcol,matrow,matcount)
    call putpcgc(totpts,xm,mask(i-1,j),mask(i,j),matval,matcol,matrow,matcount)      
    call putpcgc(totpts,yp,mask(i,j+1),mask(i,j),matval,matcol,matrow,matcount)
    call putpcgc(totpts,ym,mask(i,j-1),mask(i,j),matval,matcol,matrow,matcount)
    tmp=xp+xm
    tmp=tmp+yp
    tmp=tmp+ym
    tmp=1.0D0-tmp
    call putpcgc(totpts,tmp,mask(i,j),mask(i,j),matval,matcol,matrow,matcount)

  end subroutine generate_row

  !##################################################

  subroutine putpcgc(totpts,value,col,row,matval,matcol,matrow,matcount)

    ! Add element to sparse matrix

    implicit none

    integer,                   intent(in)  :: totpts       ! Number of non-zero points in mask
    real(dp)              :: value
    integer               :: row, col
    integer, dimension(totpts*5) :: matrow      ! Row index of sparse matrix elements
    integer, dimension(totpts*5) :: matcol      ! Column index of sparse matrix elements
    real(dp),dimension(totpts*5) :: matval      ! Values of sparse matrix elements
    integer                      :: matcount    ! Index of current final element

    if (row==0.or.col==0) return

    matcount = matcount + 1
    matval(matcount) = value
    matcol(matcount) = col
    matrow(matcount) = row

  end subroutine putpcgc

  !###############################################################################################  

  subroutine fillarray (totpts,usrf,in,out,mask,n,m)

    ! subroutine to move new surface elevation from vector to appropriate place in 2d planform grid

    implicit none

    integer,                   intent(in)  :: totpts    ! Number of non-zero points in mask
    integer                                :: n,m       ! number of grids, coordinates
    real(dp),dimension(n,m),   intent(in)  :: usrf
    real(dp),dimension(n,m),   intent(out) :: out
    integer, dimension(n,m)                :: mask 
    real(dp),dimension(totpts),intent(in)  :: in  

    ! Internal variables --------------------------------------------------
    integer                   ::  i,j 

    ! End of declarations --------------------------------------------------

    do i = 1,n  
       do j = 1,m
          if (mask(i,j)> 0) then 
             out(i,j) = in(mask(i,j))
          else
             out(i,j) = usrf(i,j)
          end if
       end do
    end do

  end subroutine fillarray

  !#################################################################################################

  subroutine masker(n,m,thck,nmsb,mask,count)

    ! subroutine to create the mask  

    implicit none

    ! Each point has a unique identifier. 
    ! Points are included in the calcuation if they have ice or a neighbour has ice or they have a positive mass balance
    ! Points not in the calculation have a mask of zero

    integer,                   intent(in)  ::  n,m        ! number of grids, coordinates
    real(dp),   dimension(n,m),intent(in)  ::  thck       ! thickness
    real(dp),   dimension(n,m),intent(in)  ::  nmsb       ! Mass balance
    integer,    dimension(n,m),intent(out) ::  mask       ! Thickness mask
    integer,                   intent(out) ::  count      ! Number of non-zero points in mask

    ! Internal variables ---------------------------------------------------

    integer     ::  i,j

    ! End of declarations --------------------------------------------------

    count = 1
    mask  = 0

    do i=2,n-1    
       do j=2,m-1 
          if(any(thck(i-1:i+1,j-1:j+1) /= 0.0D0).or.nmsb(i,j)>0.0D0)then
             mask(i,j) = count
             count = count + 1   
          end if

       end do

    end do
    count = count - 1

  end subroutine masker

  !#################################################################################################

  subroutine calc_sigma(x,n,cs)

    ! calculate sigma levels

    implicit none

    real(dp), intent(in) :: x
    real(dp), intent(in) :: n

    real(dp), intent(out) :: cs

    cs = (1-(x+1)**(-n))/(1-2**(-n))

  end subroutine calc_sigma

  subroutine diagwrite2d(var)
    
    real(dp),dimension(:,:) :: var

    integer :: n1,n2,ii,jj

    n1 = size(var,1)
    n2 = size(var,2)

    open(30,file='diagout.txt')

    do ii = 1,n1
       do jj = 1,n2
          write(30,*) ii,jj,var(ii,jj)
       end do
    end do

    close(30)
    stop

  end subroutine diagwrite2d

end module adism_core
