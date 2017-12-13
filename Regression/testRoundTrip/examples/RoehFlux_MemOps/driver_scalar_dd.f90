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

  implicit none 

  external ad_roehf5

  ! inputs
  double precision nrm1, nrm2, nrm3, priml1, priml2, priml3, priml4, priml5, &
&primr1, primr2, primr3, primr4, primr5 
  double precision gamma, gm1, gm1inv, nlefix
  double precision lefix, mcheps

  double precision nrm_ph1, nrm_ph2, nrm_ph3, priml_ph1, priml_ph2, priml_ph3, &
&priml_ph4, priml_ph5, primr_ph1, primr_ph2, primr_ph3, primr_ph4, primr_ph5
  double precision gamma_ph, gm1_ph, gm1inv_ph, nlefix_ph
  double precision lefix_ph, mcheps_ph

  ! outputs
  double precision flux1, flux2, flux3, flux4, flux5
  double precision flux_ph1, flux_ph2, flux_ph3, flux_ph4, flux_ph5

  double precision, dimension(19) :: x0
  double precision :: h
  integer:: n,m
  integer i,j,k

  open(2,action='read',file='params.conf')
  read(2,'(I5,/,I5,/,F8.1)') n, m, h
  close(2)

  do i=1,n   
     x0(i)=sqrt(1.*i)
  end do

  open(2,file='tmpOutput/dd.out')
  write(2,*) "DD"
  nrm1=x0(1)
  nrm2=x0(2)
  nrm3=x0(3)
  priml1=x0(4)
  priml2=x0(5)
  priml3=x0(6)
  priml4=x0(7)
  priml5=x0(8)
  primr1=x0(9)
  primr2=x0(10)
  primr3=x0(11)
  primr4=x0(12)
  primr5=x0(13)
  gamma=x0(14) 
  gm1=x0(15) 
  gm1inv=x0(16) 
  nlefix=x0(17)
  lefix=x0(18)
  mcheps=x0(19)

  call ad_roehf5 (nrm1, nrm2, nrm3, priml1, priml2, priml3, priml4, priml5, &
&primr1, primr2, primr3, primr4, primr5, gamma, gm1, gm1inv, nlefix, lefix, mcheps, &
&flux1, flux2, flux3, flux4, flux5)

  do i=1,n   
     nrm_ph1=x0(1)
     nrm_ph2=x0(2)
     nrm_ph3=x0(3)
     priml_ph1=x0(4)
     priml_ph2=x0(5)
     priml_ph3=x0(6)
     priml_ph4=x0(7)
     priml_ph5=x0(8)
     primr_ph1=x0(9)
     primr_ph2=x0(10)
     primr_ph3=x0(11)
     primr_ph4=x0(12)
     primr_ph5=x0(13)
     gamma_ph=x0(14) 
     gm1_ph=x0(15) 
     gm1inv_ph=x0(16) 
     nlefix_ph=x0(17)
     lefix_ph=x0(18)
     mcheps_ph=x0(19)


     if (i==1) nrm_ph1=nrm_ph1+h
     if (i==2) nrm_ph2=nrm_ph2+h
     if (i==3) nrm_ph3=nrm_ph3+h
     if (i==4) priml_ph1=priml_ph1+h
     if (i==5) priml_ph2=priml_ph2+h
     if (i==6) priml_ph3=priml_ph3+h
     if (i==7) priml_ph4=priml_ph4+h
     if (i==8) priml_ph5=priml_ph5+h
     if (i==9) primr_ph1=primr_ph1+h
     if (i==10) primr_ph2=primr_ph2+h
     if (i==11) primr_ph3=primr_ph3+h
     if (i==12) primr_ph4=primr_ph4+h
     if (i==13) primr_ph5=primr_ph5+h
     if (i==14) gamma_ph=gamma_ph+h
     if (i==15) gm1_ph=gm1_ph+h
     if (i==16) gm1inv_ph=gm1inv_ph+h
     if (i==17) nlefix_ph=nlefix_ph+h
     if (i==18) lefix_ph=lefix_ph+h
     if (i==19) mcheps_ph=mcheps_ph+h
     call ad_roehf5 (nrm_ph1, nrm_ph2, nrm_ph3, priml_ph1, priml_ph2, priml_ph3, &
&priml_ph4, priml_ph5, primr_ph1, primr_ph2, primr_ph3, primr_ph4, primr_ph5, &
&gamma_ph, gm1_ph, gm1inv_ph, nlefix_ph, lefix_ph, mcheps_ph, &
&flux_ph1, flux_ph2, flux_ph3, flux_ph4, flux_ph5)
     write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",1,",",i,")=",(flux_ph1-flux1)/h
     write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",2,",",i,")=",(flux_ph2-flux2)/h
     write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",3,",",i,")=",(flux_ph3-flux3)/h
     write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",4,",",i,")=",(flux_ph4-flux4)/h
     write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",5,",",i,")=",(flux_ph5-flux5)/h
  end do
  close(2)

end program driver
