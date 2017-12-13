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

  external head

  double precision, dimension(:), allocatable :: x0
  double precision, dimension(:), allocatable :: x, xph
  double precision, dimension(:), allocatable :: y, yph
  double precision h
  integer n,m
  integer i,j,k

  open(2,action='read',file='params.conf')
  read(2,'(I5,/,I5,/,F8.1)') n, m, h
  close(2)


  allocate(x0(n))
  allocate(x(n))
  allocate(xph(n))
  allocate(y(m))
  allocate(yph(m))

  x0(1) = 1.72
  x0(2) = 3.45
  x0(3) = 4.16
  x0(4) = 4.87
  x0(5) = 4.16
  x0(6) = 3.45
  x0(7) = 1.72
  x0(8) = 1.3
  x0(9) = 0.245828 

  open(2,file='tmpOutput/dd.out')
  write(2,*) "DD"
  do i=1,n   
     do j=1,n   
        x(j)=x0(j)
        if (i==j) then 
           xph(j)=x0(j)+h
        else
           xph(j)=x0(j)
        end if
     end do
     call head(xph,yph)
     call head(x,y)
     do k=1,m
        write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",k,",",i,")=",(yph(k)-y(k))/h
     end do
  end do
  close(2)

  deallocate(x0)
  deallocate(x)
  deallocate(xph)
  deallocate(y)
  deallocate(yph)

end program driver


