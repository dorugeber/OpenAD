! ========== begin copyright notice ==============
! This file is part of 
! ---------------
! xaifBooster
! ---------------
! Distributed under the BSD license as follows:
! Copyright (c) 2005, The University of Chicago
! All rights reserved.
!
! Redistribution and use in source and binary forms, 
! with or without modification, are permitted provided that the following conditions are met:
!
!    - Redistributions of source code must retain the above copyright notice, 
!      this list of conditions and the following disclaimer.
!    - Redistributions in binary form must reproduce the above copyright notice, 
!      this list of conditions and the following disclaimer in the documentation 
!      and/or other materials provided with the distribution.
!    - Neither the name of The University of Chicago nor the names of its contributors 
!      may be used to endorse or promote products derived from this software without 
!      specific prior written permission.
!
! THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
! EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
! OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
! SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
! INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
! PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
! INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
! LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
! OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
! 
! General Information:
! xaifBooster is intended for the transformation of 
! numerical programs represented as xml files according 
! to the XAIF schema. It is part of the OpenAD framework. 
! The main application is automatic 
! differentiation, i.e. the generation of code for 
! the computation of derivatives. 
! The following people are the principal authors of the 
! current version: 
! 	Uwe Naumann
!	Jean Utke
! Additional contributors are: 
!	Andrew Lyons
!	Peter Fine
!
! For more details about xaifBooster and its use in OpenAD please visit:
!   http://www.mcs.anl.gov/openad
!
! This work is partially supported by:
! 	NSF-ITR grant OCE-0205590
! ========== end copyright notice ==============
module all_globals_cp_mod

  use all_globals_mod
  use OAD_cp

  implicit none
  public :: cp_store_globals, cp_restore_globals

  interface cp_store_globals
     module procedure cp_store_globals_impl
  end interface

  interface cp_restore_globals
     module procedure cp_restore_globals_impl
  end interface

contains 

  subroutine cp_store_globals_impl(sd,cd,ad,si,ci,ai,ss,cs,as,sb,cb,ab)
    implicit none
    integer :: cd,ad,ci,ai,cs,as,cb,ab
    double precision, dimension(:), allocatable :: sd
    integer, dimension(:), allocatable :: si
    character(80), dimension(:), allocatable :: ss
    logical, dimension(:), allocatable :: sb
    integer i

    call cp_store_int_scalar(n_max,si,ci,ai)

    call cp_store_p_real_vector(blength,ndim,sd,cd,ad)
    call cp_store_p_real_vector(bheight,ndim,sd,cd,ad)
    call cp_store_real_scalar(bwidth,sd,cd,ad)
    call cp_store_p_real_vector(area,ndim,sd,cd,ad)
    call cp_store_p_real_vector(vol,ndim,sd,cd,ad)
    call cp_store_p_real_vector(y,2*ndim,sd,cd,ad)
    call cp_store_p_real_vector(r,2*ndim,sd,cd,ad)
    call cp_store_p_real_vector(r1,2*ndim,sd,cd,ad)
    call cp_store_p_real_vector(r_t,2*ndim,sd,cd,ad)
    call cp_store_p_real_vector(r_s,2*ndim,sd,cd,ad)
    call cp_store_p_real_vector(proj_t,2*ndim,sd,cd,ad)
    call cp_store_p_real_vector(proj_s,2*ndim,sd,cd,ad)

    do i=lbound(x,1),ubound(x,1)
       call cp_store_p_real_vector(x(i,:),size(x(i,:)),sd,cd,ad)
    end do

    call cp_store_real_scalar(alpha,sd,cd,ad)
    call cp_store_real_scalar(beta,sd,cd,ad)

    call cp_store_real_scalar(u0,sd,cd,ad)
    call cp_store_real_scalar(delta,sd,cd,ad)

    call cp_store_real_scalar(robert_filter_coeff,sd,cd,ad)

    call cp_store_real_scalar(delta_t,sd,cd,ad)

    call cp_store_real_scalar(hundred,sd,cd,ad)
    call cp_store_real_scalar(thousand,sd,cd,ad)
    call cp_store_real_scalar(day,sd,cd,ad)
    call cp_store_real_scalar(year,sd,cd,ad)
    call cp_store_real_scalar(Sv,sd,cd,ad)
    call cp_store_real_scalar(days_per_50m_mixed_layer,sd,cd,ad)
    call cp_store_real_scalar(gamma_T,sd,cd,ad)
    call cp_store_real_scalar(gamma_S,sd,cd,ad)
    call cp_store_real_scalar(epsilon_ic,sd,cd,ad)
    call cp_store_real_scalar(noise_correlation_time,sd,cd,ad)
    call cp_store_real_scalar(integration_time,sd,cd,ad)
    call cp_store_real_scalar(epsilon_regularize,sd,cd,ad)
    call cp_store_real_scalar(fdeps,sd,cd,ad)

    call cp_store_bool_scalar(verbmode,sb,cb,ab)

    call cp_store_real_scalar(thc_tot,sd,cd,ad)
    call cp_store_real_scalar(thc_t,sd,cd,ad)
    call cp_store_real_scalar(thc_s,sd,cd,ad)

    call cp_store_real_vector(told,ndim,sd,cd,ad)
    call cp_store_real_vector(tnow,ndim,sd,cd,ad)
    call cp_store_real_vector(tnew,ndim,sd,cd,ad)
    call cp_store_real_vector(sold,ndim,sd,cd,ad)
    call cp_store_real_vector(snow,ndim,sd,cd,ad)
    call cp_store_real_vector(snew,ndim,sd,cd,ad)

    call cp_store_real_scalar(uvel%v,sd,cd,ad)

    call cp_store_real_vector(rho,ndim,sd,cd,ad)

    call cp_store_p_real_vector(nullForce,ndim-1,sd,cd,ad)
    call cp_store_p_real_vector(fw,ndim-1,sd,cd,ad)
    call cp_store_p_real_vector(tStar,ndim-1,sd,cd,ad)
    call cp_store_p_real_vector(sStar,ndim-1,sd,cd,ad)

    call cp_store_real_scalar(ubar,sd,cd,ad)

    call cp_store_real_vector(t,ndim,sd,cd,ad)
    call cp_store_real_vector(s,ndim,sd,cd,ad)

    call cp_store_real_scalar(metric1,sd,cd,ad)
    call cp_store_real_scalar(metric2,sd,cd,ad)
    call cp_store_real_scalar(metric,sd,cd,ad)

    call cp_store_real_vector(xx,2*ndim,sd,cd,ad)
    call cp_store_p_real_vector(tsvec,2*ndim,sd,cd,ad)

  end subroutine cp_store_globals_impl

  subroutine cp_restore_globals_impl(sd,cd,si,ci,ss,cs,sb,cb)
    implicit none
    integer :: cd,ad,ci,ai,cs,as,cb,ab
    double precision, dimension(*) :: sd
    integer, dimension(*) :: si
    character(80), dimension(*) :: ss
    logical, dimension(*) :: sb
    integer i,j

    do i=ubound(tsvec,1),lbound(tsvec,1),-1
       tsvec(i)=sd(cd); cd=cd-1
    end do
    do i=ubound(xx,1),lbound(xx,1),-1
       xx(i)%v=sd(cd); cd=cd-1
    end do

    metric=sd(cd); cd=cd-1
    metric2=sd(cd); cd=cd-1
    metric1=sd(cd); cd=cd-1

    do i=ubound(s,1),lbound(s,1),-1
       s(i)%v=sd(cd); cd=cd-1
    end do
    do i=ubound(t,1),lbound(t,1),-1
       t(i)%v=sd(cd); cd=cd-1
    end do

    ubar=sd(cd); cd=cd-1

    do i=ubound(sStar,1),lbound(sStar,1),-1
       sStar(i)=sd(cd); cd=cd-1
    end do
    do i=ubound(tStar,1),lbound(tStar,1),-1
       tStar(i)=sd(cd); cd=cd-1
    end do
    do i=ubound(fw,1),lbound(fw,1),-1
       fw(i)=sd(cd); cd=cd-1
    end do
    do i=ubound(nullForce,1),lbound(nullForce,1),-1
       nullForce(i)=sd(cd); cd=cd-1
    end do

    do i=ubound(rho,1),lbound(rho,1),-1
       rho(i)%v=sd(cd); cd=cd-1
    end do

    uvel%v=sd(cd); cd=cd-1

    do i=ubound(snew,1),lbound(snew,1),-1
       snew(i)%v=sd(cd); cd=cd-1
    end do
    do i=ubound(snow,1),lbound(snow,1),-1
       snow(i)%v=sd(cd); cd=cd-1
    end do
    do i=ubound(sold,1),lbound(sold,1),-1
       sold(i)%v=sd(cd); cd=cd-1
    end do
    do i=ubound(tnew,1),lbound(tnew,1),-1
       tnew(i)%v=sd(cd); cd=cd-1
    end do
    do i=ubound(tnow,1),lbound(tnow,1),-1
       tnow(i)%v=sd(cd); cd=cd-1
    end do
    do i=ubound(told,1),lbound(told,1),-1
       told(i)%v=sd(cd); cd=cd-1
    end do

    thc_s=sd(cd); cd=cd-1
    thc_t=sd(cd); cd=cd-1
    thc_tot=sd(cd); cd=cd-1

    verbmode=sb(cb); cb=cb-1

    fdeps=sd(cd); cd=cd-1
    epsilon_regularize=sd(cd); cd=cd-1
    integration_time=sd(cd); cd=cd-1
    noise_correlation_time=sd(cd); cd=cd-1
    epsilon_ic=sd(cd); cd=cd-1
    gamma_S=sd(cd); cd=cd-1
    gamma_T=sd(cd); cd=cd-1
    days_per_50m_mixed_layer=sd(cd); cd=cd-1
    Sv=sd(cd); cd=cd-1
    year=sd(cd); cd=cd-1
    day=sd(cd); cd=cd-1
    thousand=sd(cd); cd=cd-1
    hundred=sd(cd); cd=cd-1

    delta_t=sd(cd); cd=cd-1

    robert_filter_coeff=sd(cd); cd=cd-1

    delta=sd(cd); cd=cd-1
    u0=sd(cd); cd=cd-1

    beta=sd(cd); cd=cd-1
    alpha=sd(cd); cd=cd-1

    do i=ubound(x,1),lbound(x,1),-1
       do j=ubound(x,2),lbound(x,2),-1
          x(i,j)=sd(cd); cd=cd-1
       end do
    end do

    do i=ubound(proj_s,1),lbound(proj_s,1),-1
       proj_s(i)=sd(cd); cd=cd-1
    end do
    do i=ubound(proj_t,1),lbound(proj_t,1),-1
       proj_t(i)=sd(cd); cd=cd-1
    end do
    do i=ubound(r_s,1),lbound(r_s,1),-1
       r_s(i)=sd(cd); cd=cd-1
    end do
    do i=ubound(r_t,1),lbound(r_t,1),-1
       r_t(i)=sd(cd); cd=cd-1
    end do
    do i=ubound(r1,1),lbound(r1,1),-1
       r1(i)=sd(cd); cd=cd-1
    end do
    do i=ubound(r,1),lbound(r,1),-1
       r(i)=sd(cd); cd=cd-1
    end do
    do i=ubound(y,1),lbound(y,1),-1
       y(i)=sd(cd); cd=cd-1
    end do

    do i=ubound(vol,1),lbound(vol,1),-1
       vol(i)=sd(cd); cd=cd-1
    end do

    do i=ubound(area,1),lbound(area,1),-1
       area(i)=sd(cd); cd=cd-1
    end do

    bwidth=sd(cd); cd=cd-1

    do i=ubound(bheight,1),lbound(bheight,1),-1
       bheight(i)=sd(cd); cd=cd-1
    end do
    do i=ubound(blength,1),lbound(blength,1),-1
       blength(i)=sd(cd); cd=cd-1
    end do

    n_max=si(ci); ci=ci-1

  end subroutine cp_restore_globals_impl

end module all_globals_cp_mod

