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

  external ad_reroute

  ! inputs
  type(active) x1, x2
  type(active) x_ph1, x_ph2

  ! outputs
  type(active) y1, y2
  type(active) y_ph1, y_ph2

  double precision, dimension(2) :: x0
  double precision, dimension(2,2) :: res_adj
  double precision :: h
  integer :: n, m
  integer i,j,k

  open(2,action='read',file='params.conf')
  read(2,'(I5,/,I5,/,F8.1)') n, m, h
  close(2)

  do i=1,n
     x0(i)=sqrt(1.*(i+1))
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
  x1%v=x0(1)
  x2%v=x0(2)

  call ad_reroute (x1, x2, y1, y2)

  do i=1,n   
     x_ph1%v=x0(1)
     x_ph2%v=x0(2)

     if (i==1) x_ph1%v=x1%v+h
     if (i==2) x_ph2%v=x2%v+h
     call ad_reroute (x_ph1, x_ph2, y_ph1, y_ph2)
     write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",1,",",i,")=",(y_ph1%v-y1%v)/h
     write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",2,",",i,")=",(y_ph2%v-y2%v)/h
  end do
  close(2)

  open(2,file='tmpOutput/ad.out')
  write(2,*) "AD"
  call tape_init()
  do i=1,m
     y1%v=0.0
     if (i==1) y1%d=1.0
     y2%v=0.0
     if (i==2) y2%d=1.0

     x1%v=x0(1)
     x2%v=x0(2)

     x1%d=0.
     x2%d=0.
     our_rev_mode%arg_store=.FALSE.
     our_rev_mode%arg_restore=.FALSE.
     our_rev_mode%res_store=.FALSE.
     our_rev_mode%res_restore=.FALSE.
     our_rev_mode%plain=.FALSE.
     our_rev_mode%tape=.TRUE.
     our_rev_mode%adjoint=.TRUE.
     call ad_reroute (x1, x2, y1, y2)
     do k=1,n
        if (k==1)  res_adj(i,k)=x1%d
        if (k==2)  res_adj(i,k)=x2%d
     end do
  end do
  do k=1,n
     do i=1,m   
        write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",i,",",k,")=",res_adj(i,k)
     end do
  end do
  close(2)

end program driver
