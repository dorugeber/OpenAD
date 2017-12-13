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
  use OAD_tape
  use OAD_rev
  implicit none 

  external head

  double precision, dimension(:), allocatable :: x0
  double precision, dimension(:,:), allocatable :: res_dd, res_ad
  type(active), dimension(:), allocatable :: x, xph
  type(active), dimension(:), allocatable :: y, yph
  double precision h
  integer n,m,nx1,nx2
  integer i,j,k
  double precision  hx1,hx2,xx2,xx1
  double precision :: r
  double precision one
  parameter(one=1.0d0)

  nx1=5
  nx2=5
  !	  n should nx1*nx2 

  open(2,action='read',file='params.conf')
  read(2,'(I5,/,I5,/,F8.1)') n, m, h
  close(2)

  r=10.0D0

  allocate(x0(n))
  allocate(res_dd(m,n))
  allocate(res_ad(m,n))
  allocate(x(n))
  allocate(xph(n))
  allocate(y(m))
  allocate(yph(m))

  !         initial value 

  hx1 = one/dble(nx1 + 1)
  hx2 = one/dble(nx2 + 1)

  xx2 = hx2
  do i = 1, nx2
     xx1 = hx1
     do j = 1, nx1
        k = (i - 1)*nx1 + j
        x0(k) = - xx1*(one - xx1)*xx2*(one - xx2)
        xx1 = xx1 + hx1
     end do
     xx1 = hx1
     xx2 = xx2 + hx2
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
  do i=1,n   
     do j=1,n   
        x(j)%v=x0(j)
        if (i==j) then 
           xph(j)%v=x0(j)+h
        else
           xph(j)%v=x0(j)
        end if
     end do
     call head(nx1,nx2,xph,yph,r)
     call head(nx1,nx2,x,y,r)
     do k=1,m
        res_dd(k,i)=(yph(k)%v-y(k)%v)/h
     end do
  end do
  do k=1,n
     do i=1,m   
        write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",i,",",k,")=",res_dd(i,k)
     end do
  end do
  close(2)

  call tape_init()
  open(2,file='tmpOutput/ad.out')
  write(2,*) "AD"
  do i=1,m   
     do j=1,n   
        x(j)%v=x0(j)
        x(j)%d=0.0
     end do
     do j=1,m   
        if (i==j) then 
           y(j)%d=1.0
        else
           y(j)%d=0.0
        end if
     end do
     our_rev_mode%arg_store=.FALSE.
     our_rev_mode%arg_restore=.FALSE.
     our_rev_mode%res_store=.FALSE.
     our_rev_mode%res_restore=.FALSE.
     our_rev_mode%plain=.FALSE.
     our_rev_mode%tape=.TRUE.
     our_rev_mode%adjoint=.TRUE.
     call head(nx1,nx2,x,y,r)
     do k=1,n
        res_ad(i,k)=x(k)%d
     end do
  end do
  !  do i=1,m   
  !     print *, "y(",i,")=",y(i)%v
  !  end do
  do k=1,n
     do i=1,m   
        write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",i,",",k,")=",res_ad(i,k)
     end do
  end do
  close(2)

  deallocate(x0)
  deallocate(res_dd)
  deallocate(res_ad)
  deallocate(x)
  deallocate(xph)
  deallocate(y)
  deallocate(yph)

end program driver


