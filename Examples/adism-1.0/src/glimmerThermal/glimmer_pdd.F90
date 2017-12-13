
! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
! +                                                           +
! +  glimmer_pdd.f90 - part of the GLIMMER ice model          + 
! +                                                           +
! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
! 
! Copyright (C) 2004 GLIMMER contributors - see COPYRIGHT file 
! for list of contributors.
!
! This program is free software; you can redistribute it and/or 
! modify it under the terms of the GNU General Public License as 
! published by the Free Software Foundation; either version 2 of 
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
! GLIMMER is maintained by:
!
! Ian Rutt
! School of Geographical Sciences
! University of Bristol
! University Road
! Bristol
! BS8 1SS
! UK
!
! email: <i.c.rutt@bristol.ac.uk> or <ian.rutt@physics.org>
!
! GLIMMER is hosted on berliOS.de:
!
! https://developer.berlios.de/projects/glimmer-cism/
!
! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

module glimmer_pdd

  !*FD The GLIMMER annual positive degree day mass-balance scheme.
  !*FD Based on the original pdd mass-balance code from Tony's model.
  !*FD 
  !*FD {\bf N.B.} the module variables in this module are used for back-door 
  !*FD message passing, to make the integration of the PDD table look more 
  !*FD comprehensible, and avoid the need to have two customised copies of 
  !*FD the integration code.
  !*FD
  !*FD Note also that this code now deals in {\it unscaled} variables.
  !*FD 
  !*FD All precip and mass-balance amounts are as meters of water equivalent. PDD
  !*FD factors are no longer converted in the code.

  use glimmer_global, only : dp

  implicit none
  
  ! These parameters were formerly in the derived type
  integer  :: pdd_dx        = 1   !*FD Spacing of values in x-direction ($^{\circ}$C)
  integer  :: pdd_dy        = 1   !*FD Spacing of values in y-direction ($^{\circ}$C)
  integer  :: pdd_ix        = 0   !*FD Lower bound of $x$-axis ($^{\circ}$C)
  integer  :: pdd_iy        = -50 !*FD Lower bound of $y$-axis ($^{\circ}$C)
  integer,parameter  :: pdd_nx        = 31  !*FD Number of values in x-direction
  integer,parameter  :: pdd_ny        = 71  !*FD Number of values in y-direction
  real(dp) :: pdd_dailytemp = 0.d0 
  real(dp) :: pdd_tma       = 0.d0
  real(dp) :: pdd_tmj       = 0.d0
  real(dp) :: pdd_dtmj      = 0.d0
  real(dp) :: pdd_dd_sigma  = 5.d0 !*FD Standard deviation of daily temperature (K)
  real(dp) :: pdd_wmax        = 0.6d0   !*FD Fraction of melted snow that refreezes
  real(dp) :: pdd_pddfac_ice  = 0.014d0 !*FD PDD factor for ice (m water day$^{-1}$ $^{\circ}C$^{-1}$)
  real(dp) :: pdd_pddfac_snow = 0.003d0 !*FD PDD factor for snow (m water day$^{-1}$ $^{\circ}C$^{-1}$)

  ! Module parameters use for back-door message-passing
  real(dp) :: bd_dd_sigma            !*FD The value of $\sigma$ in the PDD integral
  real(dp) :: bd_t_a_prime           !*FD The value of $T'_{a}$ in the PDD integral
  real(dp) :: bd_mean_annual_temp    !*FD Mean annual temperature
  real(dp) :: bd_mean_july_temp      !*FD Mean july temperature

  !private :: dp, sp 	 
  !private
  public :: glimmer_pdd_init, glimmer_pdd_mbal,pdd_nx,pdd_ny

contains

!-------------------------------------------------------------------------------
! PUBLIC subroutines
!-------------------------------------------------------------------------------

  subroutine glimmer_pdd_init(pddtab)

    real(dp),dimension(pdd_nx,pdd_ny) :: pddtab

    ! Read the config file and output to log

!    call pdd_printconfig
    
    ! Allocate pdd table

    call pddtabgrn(pddtab)  

  end subroutine glimmer_pdd_init

!-------------------------------------------------------------------------------

  subroutine glimmer_pdd_mbal(pddtab,artm,arng,prcp,ablt,acab,landsea)

    !*FD Calculates mass-balance over the ice model domain, by the
    !*FD positive-degree-day method.

    implicit none 
 
    real(dp),dimension(pdd_nx,pdd_ny)       :: pddtab
    real(dp), dimension(:,:), intent(in)    :: artm    !*FD Annual mean air-temperature 
                                                       !*FD ($^{\circ}$C)
    real(dp), dimension(:,:), intent(in)    :: arng    !*FD Annual temperature half-range ($^{\circ}$C)
    real(dp), dimension(:,:), intent(in)    :: prcp    !*FD Annual accumulated precipitation 
                                                       !*FD (m water equivalent)
    real(dp), dimension(:,:), intent(out)   :: ablt    !*FD Annual ablation (m water equivalent)
    real(dp), dimension(:,:), intent(out)   :: acab    !*FD Annual mass-balance (m water equivalent)
    logical,  dimension(:,:), intent(in)    :: landsea !*FD Land-sea mask (land is TRUE)

    ! Internal variables

    real(dp) :: wfrac, pablt, tx, ty, pdd
    integer  :: ns,ew,nsn,ewn,kx,ky,jx,jy

    ! Get size of arrays. All arrays should be the same size as this.

    ewn=size(artm,1) ; nsn=size(artm,2)

    !-----------------------------------------------------------------------
    ! Main loop
    !-----------------------------------------------------------------------

    do ns = 1, nsn
      do ew = 1, ewn 

         if (landsea(ew,ns)) then
          ! Only calculate mass-balance over 'land'
          ! Find the no. of pdd from the mean annual temp and its range

          ky = int((artm(ew,ns)-real(pdd_iy,dp))/real(pdd_dy,dp))
          kx = int((arng(ew,ns)-real(pdd_ix,dp))/real(pdd_dx,dp)) 

          ! Check to see if indicies are in range

          if ( kx < 0 ) then 
            tx = 0.d0
            jx = 2
            kx = 1
          else if ( kx > pdd_nx-2 ) then
            tx = 1.d0
            jx = pdd_nx
            kx = pdd_nx-1
          else
            tx = arng(ew,ns) - real(kx * pdd_dx,dp) - real(pdd_ix,dp)
            jx = kx + 2
            kx = kx + 1
          end if

          if ( ky < 0 ) then 
            ty = 0.d0
            jy = 2
            ky = 1
          else if ( ky > pdd_ny-2 ) then
            ty = 1.d0
            jy = pdd_ny
            ky = pdd_ny-1
          else
            ty = artm(ew,ns) - real(ky * pdd_dy,dp) - real(pdd_iy,dp)
            jy = ky + 2
            ky = ky + 1
          end if
            
          ! this is done using a look-up table constructed earlier

          pdd = pddtab(kx,ky)*(1.d0-tx)*(1.d0-ty) + &
                pddtab(jx,ky) * tx * (1.d0 - ty) + &
                pddtab(jx,jy) * tx * ty +         &
                pddtab(kx,jy) * (1.d0 - tx) * ty

          ! this is the depth of superimposed ice that would need to be
          ! melted before runoff can occur

          wfrac = pdd_wmax * prcp(ew,ns)

          ! this is the total potential ablation of SNOW
    
          pablt = pdd * pdd_pddfac_snow

          ! if the total snow ablation is less than the depth of 
          ! superimposed ice - no runoff occurs

          ! else if the total snow ablation is more than the depth
          ! of superimposed ice BUT less than the total amount of
          ! prcpitation - runoff occurs (at a rate equal to the
          ! total potential snowmelt minus that which forms superimposed ice)

          ! else if the total snow ablation is more than the amount
          ! of prcpitation - all snow that is not superimposed ice is lost 
          ! and the potential ablation not used on snow is used on ice
          ! (including the superimposed ice)

          ! there is a change in the pddfi term, replaced wfrac with prcp
          ! error spotted by jonathan 18-04-00

          if ( pablt <= wfrac ) then
           ablt(ew,ns) = 0.d0
          else if(pablt > wfrac .and.pablt <= prcp(ew,ns)) then   
            ablt(ew,ns) = pablt - wfrac 
          else
            ablt(ew,ns) = prcp(ew,ns) - wfrac + pdd_pddfac_ice*(pdd-prcp(ew,ns)/pdd_pddfac_snow) 
          end if

          ! Finally, mass-balance is difference between accumulation and
          ! ablation.

          acab(ew,ns) = prcp(ew,ns) - ablt(ew,ns)
         else
            ablt(ew,ns)=prcp(ew,ns)
            acab(ew,ns)=0.d0
         endif
      end do
    end do

  end subroutine glimmer_pdd_mbal

!-------------------------------------------------------------------------------
! PRIVATE subroutines and functions
!-------------------------------------------------------------------------------

!!$  subroutine pdd_printconfig
!!$
!!$    use glimmer_log
!!$
!!$    character(len=100) :: message
!!$
!!$    call write_log_div
!!$
!!$    call write_log('GLIMMER annual PDD Scheme parameters:')
!!$    call write_log('-----------------------------------')
!!$    write(message,*) 'x-spacing of pdd table',pdd_dx,' degC'
!!$    call write_log(message)
!!$    write(message,*) 'y-spacing of pdd table',pdd_dy,' degC'
!!$    call write_log(message)
!!$    write(message,*) 'Lower bound of x-axis',pdd_ix,' degC'
!!$    call write_log(message)
!!$    write(message,*) 'Lower bound of y-axis',pdd_iy,' degC'
!!$    call write_log(message)
!!$    write(message,*) 'Number of points in x',pdd_nx
!!$    call write_log(message)
!!$    write(message,*) 'Number of points in y',pdd_ny
!!$    call write_log(message)
!!$    write(message,*) 'Snow refreezing fraction',pdd_wmax
!!$    call write_log(message)
!!$    write(message,*) 'PDD factor for ice',pdd_pddfac_ice
!!$    call write_log(message)
!!$    write(message,*) 'PDD factor for snow',pdd_pddfac_snow
!!$    call write_log(message)
!!$    write(message,*) 'Standard deviation of temperature cycle',pdd_dd_sigma,' degC'
!!$    call write_log(message)
!!$    call write_log('')
!!$
!!$  end subroutine pdd_printconfig

!-------------------------------------------------------------------------------

 
  subroutine pddtabgrn(pddtab)

    !*FD Initialises the positive-degree-day-table.

    use glimmer_global, only: dp
    !use glimmer_log

    implicit none

    real(dp),dimension(pdd_nx,pdd_ny) :: pddtab

    ! Internal variables

    real(dp)           :: tma,dtmj
    real(dp),parameter :: twopi = 3.1416d0 * 2.d0 
    integer  :: kx,ky, i,j

    !--------------------------------------------------------------------
    ! Main loops:
    !  tma  -- the mean annual temperature (y-axis of pdd table)
    !  dtmj -- difference from mean july temperature (x-axis of table)
    !  tmj -- the actual july temperature
    !--------------------------------------------------------------------

    !call write_log('Calculating PDD table...',GM_DIAGNOSTIC)

    do j=0,pdd_ny-1
       tma=real(pdd_iy + j*pdd_dy,dp)

       ky = findgrid(tma,real(pdd_iy,dp),real(pdd_dy,dp))

       do i=0,pdd_nx-1
          dtmj = real(pdd_ix + i*pdd_dx,dp)

          bd_mean_july_temp = tma + dtmj   
          kx  = findgrid(dtmj,real(pdd_ix,dp),real(pdd_dx,dp)) 

          ! need these lines to take account of the backdoor message passing used here

          bd_mean_annual_temp=tma
          bd_dd_sigma=pdd_dd_sigma

          pddtab(kx,ky)=(1.d0/(bd_dd_sigma*sqrt(twopi)))*romberg_int_outer(0.0d0,twopi)

          ! convert to days     

          pddtab(kx,ky) = 365.d0 * pddtab(kx,ky) / twopi

       end do
    end do

    !call write_log('   ...done.',GM_DIAGNOSTIC)

  end subroutine pddtabgrn

!-------------------------------------------------------------------------------

 real(dp) function romberg_int_outer(lgr,rgr)

    !*FD Function to perform Romberg Integration on function \texttt{fct}, between
    !*FD limits \texttt{lgr} and \texttt{rgr}. The precision of the routine is 
    !*FD determined by the value of \texttt{ord}, an internal variable. 
    !*FD
    !*FD This routine is an implementation of ACM algorithm 60, by F. L. Bauer.
    !*FD (Comm. ACM, vol. 4, issue 6, June 1961).

    implicit none
    
    real(dp),intent(in) :: lgr    !*FD Lower bound
    real(dp),intent(in) :: rgr    !*FD Upper bound
    integer,parameter :: ord = 6

    real(dp),dimension(ord+1) :: t
    real(dp) :: l,u,m
    integer :: f,h,j,n

    l=rgr-lgr
    t(1)=(inner_integral(lgr)+inner_integral(rgr))/2.d0
    n=1

    do h=1,ord
       u=0.d0
       m=l/(2.d0*real(n,dp))

       do j=1,2*n-1,2
          u=u+inner_integral(lgr+real(j,dp)*m)
       end do

       t(h+1)=((u/real(n,dp))+t(h))/2.d0
       f=1
       
       do j=h,1,-1
          f=f*4
          t(j)=t(j+1)+(t(j+1)-t(j))/real(f-1,dp)
       end do

       n=2*n

    end do

    romberg_int_outer=t(1)*l

  end function romberg_int_outer

  real(dp) function inner_integral(day)

    !*FD Calculates the value of the inner integral, i.e.
    !*FD \begin{equation}
    !*FD \int^{T_{a}'+2.5\sigma}_{0}T_{a}\times
    !*FD \exp\left(\frac{-(T_a-T_{a}')^2}{2\sigma^2}\right)\,dT
    !*FD \end{equation}

    implicit none

    real(dp), intent(in) :: day !*FD The `day', in radians, so that a year is $2\pi$ long.

    real(dp) :: upper_limit

    bd_t_a_prime=bd_mean_annual_temp+(bd_mean_july_temp-bd_mean_annual_temp)*cos(day)

    upper_limit=bd_t_a_prime+2.5d0*bd_dd_sigma

    if (upper_limit<=0.d0) then
      inner_integral=0.d0
    else
      inner_integral=romberg_int_inner(0.0d0,upper_limit)
    endif

  end function inner_integral

  real(dp) function romberg_int_inner(lgr,rgr)

    !*FD Function to perform Romberg Integration on function \texttt{fct}, between
    !*FD limits \texttt{lgr} and \texttt{rgr}. The precision of the routine is 
    !*FD determined by the value of \texttt{ord}, an internal variable. 
    !*FD
    !*FD This routine is an implementation of ACM algorithm 60, by F. L. Bauer.
    !*FD (Comm. ACM, vol. 4, issue 6, June 1961).

    implicit none
   
    real(dp),intent(in) :: lgr    !*FD Lower bound
    real(dp),intent(in) :: rgr    !*FD Upper bound
    integer,parameter :: ord = 6

    real(dp),dimension(ord+1) :: t
    real(dp) :: l,u,m
    integer :: f,h,j,n

    l=rgr-lgr
    !! pdd_integrand = artm *  exp(- (artm - bd_t_a_prime)**2 / (2.0 * bd_dd_sigma**2))
    t(1)=(lgr *  exp(- (lgr - bd_t_a_prime)**2 / (2.d0 * bd_dd_sigma**2)) &
         + rgr *  exp(- (rgr - bd_t_a_prime)**2 / (2.d0 * bd_dd_sigma**2)))/2.d0
    n=1

    do h=1,ord
       u=0.d0
       m=l/real(2*n,dp)

       do j=1,2*n-1,2
          u=u + (lgr+real(j,dp)*m) *  exp(- ((lgr+real(j,dp)*m) - bd_t_a_prime)**2 / (2.d0 * bd_dd_sigma**2))
       end do

       t(h+1)=((u/real(n,dp))+t(h))/2.d0
       f=1
       
       do j=h,1,-1
          f=f*4
          t(j)=t(j+1)+(t(j+1)-t(j))/real(f-1,dp)
       end do

       n=2*n

    end do

    romberg_int_inner=t(1)*l

  end function romberg_int_inner

!-------------------------------------------------------------------------------

  integer function findgrid(rin,init,step)

    !*FD Calculates which row or column of the pdd table corresponds
    !*FD to a given value on the appropriate axis, so that:
    !*FD \[
    !*FD \mathtt{findgrid}=\frac{\mathtt{rin}-\mathtt{init}}{\mathtt{step}+1}
    !*FD \] 
    !*RV The relevant array index.

    use glimmer_global, only : dp
    
    implicit none
    
    real(dp), intent(in) :: rin  !*FD Value of axis variable at current point.
    real(dp), intent(in) :: init !*FD Value of axis variable at first point.
    real(dp), intent(in) :: step !*FD Grid spacing.
    
    findgrid = int((rin - init) / step) + 1

  end function findgrid
 
end module glimmer_pdd
