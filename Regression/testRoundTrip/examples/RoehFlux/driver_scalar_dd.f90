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
  double precision nrm(3), priml(5), primr(5) 
  double precision gamma, gm1, gm1inv, nlefix
  double precision lefix, mcheps

  double precision  nrm_ph(3), priml_ph(5), primr_ph(5)
  double precision gamma_ph, gm1_ph, gm1inv_ph, nlefix_ph
  double precision lefix_ph, mcheps_ph

  ! outputs
  double precision flux(5)
  double precision flux_ph(5)

  double precision, dimension(19) :: x0
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
  nrm(1)=x0(1)
  nrm(2)=x0(2)
  nrm(3)=x0(3)
  priml(1)=x0(4)
  priml(2)=x0(5)
  priml(3)=x0(6)
  priml(4)=x0(7)
  priml(5)=x0(8)
  primr(1)=x0(9)
  primr(2)=x0(10)
  primr(3)=x0(11)
  primr(4)=x0(12)
  primr(5)=x0(13)
  gamma=x0(14) 
  gm1=x0(15) 
  gm1inv=x0(16) 
  nlefix=x0(17)
  lefix=x0(18)
  mcheps=x0(19)

  call ad_roehf5 (nrm, priml, primr, gamma, gm1, gm1inv, nlefix, lefix, mcheps, flux)

  do i=1,n   
     nrm_ph(1)=x0(1)
     nrm_ph(2)=x0(2)
     nrm_ph(3)=x0(3)
     priml_ph(1)=x0(4)
     priml_ph(2)=x0(5)
     priml_ph(3)=x0(6)
     priml_ph(4)=x0(7)
     priml_ph(5)=x0(8)
     primr_ph(1)=x0(9)
     primr_ph(2)=x0(10)
     primr_ph(3)=x0(11)
     primr_ph(4)=x0(12)
     primr_ph(5)=x0(13)
     gamma_ph=x0(14) 
     gm1_ph=x0(15) 
     gm1inv_ph=x0(16) 
     nlefix_ph=x0(17)
     lefix_ph=x0(18)
     mcheps_ph=x0(19)


     if (i==1) nrm_ph(1)=nrm_ph(1)+h
     if (i==2) nrm_ph(2)=nrm_ph(2)+h
     if (i==3) nrm_ph(3)=nrm_ph(3)+h
     if (i==4) priml_ph(1)=priml_ph(1)+h
     if (i==5) priml_ph(2)=priml_ph(2)+h
     if (i==6) priml_ph(3)=priml_ph(3)+h
     if (i==7) priml_ph(4)=priml_ph(4)+h
     if (i==8) priml_ph(5)=priml_ph(5)+h
     if (i==9) primr_ph(1)=primr_ph(1)+h
     if (i==10) primr_ph(2)=primr_ph(2)+h
     if (i==11) primr_ph(3)=primr_ph(3)+h
     if (i==12) primr_ph(4)=primr_ph(4)+h
     if (i==13) primr_ph(5)=primr_ph(5)+h
     if (i==14) gamma_ph=gamma_ph+h
     if (i==15) gm1_ph=gm1_ph+h
     if (i==16) gm1inv_ph=gm1inv_ph+h
     if (i==17) nlefix_ph=nlefix_ph+h
     if (i==18) lefix_ph=lefix_ph+h
     if (i==19) mcheps_ph=mcheps_ph+h
     call ad_roehf5 (nrm_ph, priml_ph, primr_ph, gamma_ph, gm1_ph, gm1inv_ph, nlefix_ph, lefix_ph, mcheps_ph, flux_ph)
     do k=1,m
        write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",k,",",i,")=",(flux_ph(k)-flux(k))/h
     end do
  end do
  close(2)

end program driver
