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

  external head

  type(active) obj,g_obj0,g_obj1,g_obj2,g_obj3,g_obj4,g_obj5
  type(active) x1,x2,x3,x4,x5,x6
  type(active) h_obj0,h_obj1,h_obj2,h_obj3,h_obj4,h_obj5,h_obj6,&
       &h_obj7,h_obj8,h_obj9,h_obj10,h_obj11,h_obj12,h_obj13,h_obj14,&
       &h_obj15,h_obj16,h_obj17,h_obj18,h_obj19,h_obj20

  type(active) objph,g_obj0ph,g_obj1ph,g_obj2ph,g_obj3ph,&
       &g_obj4ph,g_obj5ph,x1ph,x2ph,x3ph,x4ph,x5ph,x6ph,&
       &h_obj0ph,h_obj1ph,h_obj2ph,h_obj3ph,h_obj4ph,h_obj5ph,&
       &h_obj6ph,h_obj7ph,h_obj8ph,&
       &h_obj9ph,h_obj10ph,h_obj11ph,h_obj12ph,h_obj13ph,h_obj14ph,&
       &h_obj15ph,h_obj16ph,&
       &h_obj17ph,h_obj18ph,h_obj19ph,h_obj20ph

  double precision, dimension(:), allocatable :: res_dd
  double precision, dimension(:,:), allocatable :: res_adj
  double precision h
  integer n,m
  integer i,k

  open(2,action='read',file='params.conf')
  read(2,'(I5,/,I5,/,F8.1)') n, m, h
  close(2)

  allocate(res_adj(m,n))
  allocate(res_dd(m))

  open(2,file='tmpOutput/dd.out')
  write(2,*) "DD"
  our_rev_mode%arg_store=.FALSE.
  our_rev_mode%arg_restore=.FALSE.
  our_rev_mode%res_store=.FALSE.
  our_rev_mode%res_restore=.FALSE.
  our_rev_mode%plain=.TRUE.
  our_rev_mode%tape=.FALSE.
  our_rev_mode%adjoint=.FALSE.
  ! equilateral triangle coordinates (x1,x2,x3,y1,y2,y3)
  x1%v=0.D0
  x2%v=.5D0
  x3%v=1.D0
  x4%v=0.D0
  x5%v=0.523598776D0
  x6%v=0.D0
  call head(obj,g_obj0,g_obj1,g_obj2,g_obj3,&
       &g_obj4,g_obj5,x1,x2,x3,x4,x5,x6,&
       &h_obj0,h_obj1,h_obj2,h_obj3,h_obj4,h_obj5,h_obj6,h_obj7,h_obj8,&
       &h_obj9,h_obj10,h_obj11,h_obj12,h_obj13,h_obj14,h_obj15,h_obj16,&
       &h_obj17,h_obj18,h_obj19,h_obj20)
  do i=1,n   
     x1ph%v=x1%v
     x2ph%v=x2%v
     x3ph%v=x3%v
     x4ph%v=x4%v
     x5ph%v=x5%v
     x6ph%v=x6%v
     if (i==1) then 
        x1ph%v=x1%v+h
     else if (i==2) then 
        x2ph%v=x2%v+h
     else if (i==3) then 
        x3ph%v=x3%v+h
     else if (i==4) then 
        x4ph%v=x4%v+h
     else if (i==5) then 
        x5ph%v=x5%v+h
     else if (i==6) then 
        x6ph%v=x6%v+h
     end if
     call head(objph,g_obj0ph,g_obj1ph,g_obj2ph,g_obj3ph,&
          &g_obj4ph,g_obj5ph,x1ph,x2ph,x3ph,x4ph,x5ph,x6ph,&
          &h_obj0ph,h_obj1ph,h_obj2ph,h_obj3ph,h_obj4ph,h_obj5ph,&
          &h_obj6ph,h_obj7ph,h_obj8ph,&
          &h_obj9ph,h_obj10ph,h_obj11ph,h_obj12ph,h_obj13ph,h_obj14ph,&
          &h_obj15ph,h_obj16ph,&
          &h_obj17ph,h_obj18ph,h_obj19ph,h_obj20ph)
     res_dd(1) =(objph%v-obj%v)/h
     res_dd(2) =(g_obj0ph%v-g_obj0%v)/h
     res_dd(3) =(g_obj1ph%v-g_obj1%v)/h
     res_dd(4) =(g_obj2ph%v-g_obj2%v)/h
     res_dd(5) =(g_obj3ph%v-g_obj3%v)/h
     res_dd(6) =(g_obj4ph%v-g_obj4%v)/h
     res_dd(7) =(g_obj5ph%v-g_obj5%v)/h
     res_dd(8) =(h_obj0ph%v-h_obj0%v)/h
     res_dd(9)=(h_obj1ph%v-h_obj1%v)/h
     res_dd(10)=(h_obj2ph%v-h_obj2%v)/h
     res_dd(11)=(h_obj3ph%v-h_obj3%v)/h
     res_dd(12)=(h_obj4ph%v-h_obj4%v)/h
     res_dd(13)=(h_obj5ph%v-h_obj5%v)/h
     res_dd(14)=(h_obj6ph%v-h_obj6%v)/h
     res_dd(15)=(h_obj7ph%v-h_obj7%v)/h
     res_dd(16)=(h_obj8ph%v-h_obj8%v)/h
     res_dd(17)=(h_obj9ph%v-h_obj9%v)/h
     res_dd(18)=(h_obj10ph%v-h_obj10%v)/h
     res_dd(19)=(h_obj11ph%v-h_obj11%v)/h
     res_dd(20)=(h_obj12ph%v-h_obj12%v)/h
     res_dd(21)=(h_obj13ph%v-h_obj13%v)/h
     res_dd(22)=(h_obj14ph%v-h_obj14%v)/h
     res_dd(23)=(h_obj15ph%v-h_obj15%v)/h
     res_dd(24)=(h_obj16ph%v-h_obj16%v)/h
     res_dd(25)=(h_obj17ph%v-h_obj17%v)/h
     res_dd(26)=(h_obj18ph%v-h_obj18%v)/h
     res_dd(27)=(h_obj19ph%v-h_obj19%v)/h
     res_dd(28)=(h_obj20ph%v-h_obj20%v)/h
     do k=1,m
        write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",k,",",i,")=",res_dd(k)
     end do
  end do
  close(2)

  call tape_init()
  open(2,file='tmpOutput/ad.out')
  write(2,*) "AD"
  do i=1,m   
     x1%v=0.
     x2%v=.5
     x3%v=1.
     x4%v=0.
     x5%v=0.523598776
     x6%v=0.
     x1%d=0.
     x2%d=0.
     x3%d=0.
     x4%d=0.
     x5%d=0.
     x6%d=0.
     obj%d=0.0
     g_obj0%d=0.0
     g_obj1%d=0.0
     g_obj2%d=0.0
     g_obj3%d=0.0
     g_obj4%d=0.0
     g_obj5%d=0.0
     h_obj0%d=0.0
     h_obj1%d=0.0
     h_obj2%d=0.0
     h_obj3%d=0.0
     h_obj4%d=0.0
     h_obj5%d=0.0
     h_obj6%d=0.0
     h_obj7%d=0.0
     h_obj8%d=0.0
     h_obj9%d=0.0
     h_obj10%d=0.0
     h_obj11%d=0.0
     h_obj12%d=0.0
     h_obj13%d=0.0
     h_obj14%d=0.0
     h_obj15%d=0.0
     h_obj16%d=0.0
     h_obj17%d=0.0
     h_obj18%d=0.0
     h_obj19%d=0.0
     h_obj20%d=0.0
     if (i==1) obj%d=1.0
     if (i==2) g_obj0%d=1.0
     if (i==3) g_obj1%d=1.0
     if (i==4) g_obj2%d=1.0
     if (i==5) g_obj3%d=1.0
     if (i==6) g_obj4%d=1.0
     if (i==7) g_obj5%d=1.0
     if (i==8) h_obj0%d=1.0
     if (i==9) h_obj1%d=1.0
     if (i==10) h_obj2%d=1.0
     if (i==11) h_obj3%d=1.0
     if (i==12) h_obj4%d=1.0
     if (i==13) h_obj5%d=1.0
     if (i==14) h_obj6%d=1.0
     if (i==15) h_obj7%d=1.0
     if (i==16) h_obj8%d=1.0
     if (i==17) h_obj9%d=1.0
     if (i==18) h_obj10%d=1.0
     if (i==19) h_obj11%d=1.0
     if (i==20) h_obj12%d=1.0
     if (i==21) h_obj13%d=1.0
     if (i==22) h_obj14%d=1.0
     if (i==23) h_obj15%d=1.0
     if (i==24) h_obj16%d=1.0
     if (i==25) h_obj17%d=1.0
     if (i==26) h_obj18%d=1.0
     if (i==27) h_obj19%d=1.0
     if (i==28) h_obj20%d=1.0
     our_rev_mode%arg_store=.FALSE.
     our_rev_mode%arg_restore=.FALSE.
     our_rev_mode%res_store=.FALSE.
     our_rev_mode%res_restore=.FALSE.
     our_rev_mode%plain=.FALSE.
     our_rev_mode%tape=.TRUE.
     our_rev_mode%adjoint=.FALSE.
     call head(obj,g_obj0,g_obj1,g_obj2,g_obj3,&
          &g_obj4,g_obj5,x1,x2,x3,x4,x5,x6,&
          &h_obj0,h_obj1,h_obj2,h_obj3,h_obj4,h_obj5,h_obj6,h_obj7,h_obj8,&
          &h_obj9,h_obj10,h_obj11,h_obj12,h_obj13,h_obj14,h_obj15,h_obj16,&
          &h_obj17,h_obj18,h_obj19,h_obj20)
     our_rev_mode%arg_store=.FALSE.
     our_rev_mode%arg_restore=.FALSE.
     our_rev_mode%res_store=.FALSE.
     our_rev_mode%res_restore=.FALSE.
     our_rev_mode%plain=.FALSE.
     our_rev_mode%tape=.FALSE.
     our_rev_mode%adjoint=.TRUE.
     call head(obj,g_obj0,g_obj1,g_obj2,g_obj3,&
          &g_obj4,g_obj5,x1,x2,x3,x4,x5,x6,&
          &h_obj0,h_obj1,h_obj2,h_obj3,h_obj4,h_obj5,h_obj6,h_obj7,h_obj8,&
          &h_obj9,h_obj10,h_obj11,h_obj12,h_obj13,h_obj14,h_obj15,h_obj16,&
          &h_obj17,h_obj18,h_obj19,h_obj20)
     do k=1,n
        if (k==1) res_adj(i,k)=x1%d
        if (k==2) res_adj(i,k)=x2%d
        if (k==3) res_adj(i,k)=x3%d
        if (k==4) res_adj(i,k)=x4%d
        if (k==5) res_adj(i,k)=x5%d
        if (k==6) res_adj(i,k)=x6%d
     end do
  end do
  do k=1,n
     do i=1,m   
        write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",i,",",k,")=",res_adj(i,k)
     end do
  end do

  close(2)

  deallocate(res_dd)
  deallocate(res_adj)

end program driver


