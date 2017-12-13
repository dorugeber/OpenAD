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
program driver

  use OAD_active
  use OAD_rev
  use OAD_tape

  implicit none 

  external ad_roehf5

  ! inputs
  type(active) nrm1, nrm2, nrm3, priml1, priml2, priml3, priml4, priml5, &
&primr1, primr2, primr3, primr4, primr5 
  type(active) gamma, gm1, gm1inv, nlefix
  type(active) lefix, mcheps

  type(active) nrm_ph1, nrm_ph2, nrm_ph3, priml_ph1, priml_ph2, priml_ph3, &
&priml_ph4, priml_ph5, primr_ph1, primr_ph2, primr_ph3, primr_ph4, primr_ph5
  type(active) gamma_ph, gm1_ph, gm1inv_ph, nlefix_ph
  type(active) lefix_ph, mcheps_ph

  ! outputs
  type(active) flux1, flux2, flux3, flux4, flux5
  type(active) flux_ph1, flux_ph2, flux_ph3, flux_ph4, flux_ph5

  double precision, dimension(19) :: x0
  double precision, dimension(5,19) :: res_adj
  double precision :: h
  integer :: n, m
  integer i,j,k

  open(2,action='read',file='params.conf')
  read(2,'(I5,/,I5,/,F8.1)') n, m, h
  close(2)

  do i=1,n   
     x0(i)=sqrt(1.*i)
  end do

  open(2,file='tmpOutput/dd.out')
  write(2,*) "DD"
  our_rev_mode%arg_store=.FALSE.
  our_rev_mode%arg_restore=.FALSE.
  our_rev_mode%res_store=.FALSE.
  our_rev_mode%res_restore=.FALSE.
  our_rev_mode%plain=.TRUE.
  our_rev_mode%tape=.FALSE.
  our_rev_mode%adjoint=.FALSE.
  nrm1%v=x0(1)
  nrm2%v=x0(2)
  nrm3%v=x0(3)
  priml1%v=x0(4)
  priml2%v=x0(5)
  priml3%v=x0(6)
  priml4%v=x0(7)
  priml5%v=x0(8)
  primr1%v=x0(9)
  primr2%v=x0(10)
  primr3%v=x0(11)
  primr4%v=x0(12)
  primr5%v=x0(13)
  gamma%v=x0(14) 
  gm1%v=x0(15) 
  gm1inv%v=x0(16) 
  nlefix%v=x0(17)
  lefix%v=x0(18)
  mcheps%v=x0(19)

  call ad_roehf5 (nrm1, nrm2, nrm3, priml1, priml2, priml3, priml4, priml5, &
&primr1, primr2, primr3, primr4, primr5, gamma, gm1, gm1inv, nlefix, lefix, mcheps, &
&flux1, flux2, flux3, flux4, flux5)

  do i=1,n   
     nrm_ph1%v=x0(1)
     nrm_ph2%v=x0(2)
     nrm_ph3%v=x0(3)
     priml_ph1%v=x0(4)
     priml_ph2%v=x0(5)
     priml_ph3%v=x0(6)
     priml_ph4%v=x0(7)
     priml_ph5%v=x0(8)
     primr_ph1%v=x0(9)
     primr_ph2%v=x0(10)
     primr_ph3%v=x0(11)
     primr_ph4%v=x0(12)
     primr_ph5%v=x0(13)
     gamma_ph%v=x0(14) 
     gm1_ph%v=x0(15) 
     gm1inv_ph%v=x0(16) 
     nlefix_ph%v=x0(17)
     lefix_ph%v=x0(18)
     mcheps_ph%v=x0(19)


     if (i==1) nrm_ph1%v=nrm_ph1%v+h
     if (i==2) nrm_ph2%v=nrm_ph2%v+h
     if (i==3) nrm_ph3%v=nrm_ph3%v+h
     if (i==4) priml_ph1%v=priml_ph1%v+h
     if (i==5) priml_ph2%v=priml_ph2%v+h
     if (i==6) priml_ph3%v=priml_ph3%v+h
     if (i==7) priml_ph4%v=priml_ph4%v+h
     if (i==8) priml_ph5%v=priml_ph5%v+h
     if (i==9) primr_ph1%v=primr_ph1%v+h
     if (i==10) primr_ph2%v=primr_ph2%v+h
     if (i==11) primr_ph3%v=primr_ph3%v+h
     if (i==12) primr_ph4%v=primr_ph4%v+h
     if (i==13) primr_ph5%v=primr_ph5%v+h
     if (i==14) gamma_ph%v=gamma_ph%v+h
     if (i==15) gm1_ph%v=gm1_ph%v+h
     if (i==16) gm1inv_ph%v=gm1inv_ph%v+h
     if (i==17) nlefix_ph%v=nlefix_ph%v+h
     if (i==18) lefix_ph%v=lefix_ph%v+h
     if (i==19) mcheps_ph%v=mcheps_ph%v+h
     call ad_roehf5 (nrm_ph1, nrm_ph2, nrm_ph3, priml_ph1, priml_ph2, priml_ph3, &
&priml_ph4, priml_ph5, primr_ph1, primr_ph2, primr_ph3, primr_ph4, primr_ph5, &
&gamma_ph, gm1_ph, gm1inv_ph, nlefix_ph, lefix_ph, mcheps_ph, &
&flux_ph1, flux_ph2, flux_ph3, flux_ph4, flux_ph5)
     write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",1,",",i,")=",(flux_ph1%v-flux1%v)/h
     write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",2,",",i,")=",(flux_ph2%v-flux2%v)/h
     write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",3,",",i,")=",(flux_ph3%v-flux3%v)/h
     write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",4,",",i,")=",(flux_ph4%v-flux4%v)/h
     write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",5,",",i,")=",(flux_ph5%v-flux5%v)/h
  end do
  close(2)

  open(2,file='tmpOutput/ad.out')
  write(2,*) "AD"
  call tape_init()
  do i=1,m
     flux1%v=0.0
     if (i==1) flux1%d=1.0
     flux2%v=0.0
     if (i==2) flux2%d=1.0
     flux3%v=0.0
     if (i==3) flux3%d=1.0
     flux4%v=0.0
     if (i==4) flux4%d=1.0
     flux5%v=0.0
     if (i==5) flux5%d=1.0

     nrm1%v=x0(1)
     nrm2%v=x0(2)
     nrm3%v=x0(3)
     priml1%v=x0(4)
     priml2%v=x0(5)
     priml3%v=x0(6)
     priml4%v=x0(7)
     priml5%v=x0(8)
     primr1%v=x0(9)
     primr2%v=x0(10)
     primr3%v=x0(11)
     primr4%v=x0(12)
     primr5%v=x0(13)
     gamma%v=x0(14) 
     gm1%v=x0(15) 
     gm1inv%v=x0(16) 
     nlefix%v=x0(17)
     lefix%v=x0(18)
     mcheps%v=x0(19)

     nrm1%d=0.
     nrm2%d=0.
     nrm3%d=0.
     priml1%d=0.
     priml2%d=0.
     priml3%d=0.
     priml4%d=0.
     priml5%d=0.
     primr1%d=0.
     primr2%d=0.
     primr3%d=0.
     primr4%d=0.
     primr5%d=0.
     gamma%d=0.
     gm1%d=0.
     gm1inv%d=0.
     nlefix%d=0.
     lefix%d=0.
     mcheps%d=0.
     our_rev_mode%arg_store=.FALSE.
     our_rev_mode%arg_restore=.FALSE.
     our_rev_mode%res_store=.FALSE.
     our_rev_mode%res_restore=.FALSE.
     our_rev_mode%plain=.FALSE.
     our_rev_mode%tape=.TRUE.
     our_rev_mode%adjoint=.FALSE.
     call ad_roehf5 (nrm1, nrm2, nrm3, priml1, priml2, priml3, priml4, &
&priml5, primr1, primr2, primr3, primr4, primr5, gamma, gm1, &
&gm1inv, nlefix, lefix, mcheps, flux1, flux2, flux3, flux4, flux5)
     our_rev_mode%arg_store=.FALSE.
     our_rev_mode%arg_restore=.FALSE.
     our_rev_mode%res_store=.FALSE.
     our_rev_mode%res_restore=.FALSE.
     our_rev_mode%plain=.FALSE.
     our_rev_mode%tape=.FALSE.
     our_rev_mode%adjoint=.TRUE.
     call ad_roehf5 (nrm1, nrm2, nrm3, priml1, priml2, priml3, priml4, &
&priml5, primr1, primr2, primr3, primr4, primr5, gamma, gm1, &
&gm1inv, nlefix, lefix, mcheps, flux1, flux2, flux3, flux4, flux5)
     do k=1,n
        if (k==1)  res_adj(i,k)=nrm1%d
        if (k==2)  res_adj(i,k)=nrm2%d
        if (k==3)  res_adj(i,k)=nrm3%d
        if (k==4)  res_adj(i,k)=priml1%d
        if (k==5)  res_adj(i,k)=priml2%d
        if (k==6)  res_adj(i,k)=priml3%d
        if (k==7)  res_adj(i,k)=priml4%d
        if (k==8)  res_adj(i,k)=priml5%d
        if (k==9)  res_adj(i,k)=primr1%d
        if (k==10) res_adj(i,k)=primr2%d
        if (k==11) res_adj(i,k)=primr3%d
        if (k==12) res_adj(i,k)=primr4%d
        if (k==13) res_adj(i,k)=primr5%d
        if (k==14) res_adj(i,k)=gamma%d
        if (k==15) res_adj(i,k)=gm1%d
        if (k==16) res_adj(i,k)=gm1inv%d
        if (k==17) res_adj(i,k)=nlefix%d
        if (k==18) res_adj(i,k)=lefix%d
        if (k==19) res_adj(i,k)=mcheps%d
     end do
  end do
  do k=1,n
     do i=1,m   
        write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",i,",",k,")=",res_adj(i,k)
     end do
  end do
  close(2)

end program driver
