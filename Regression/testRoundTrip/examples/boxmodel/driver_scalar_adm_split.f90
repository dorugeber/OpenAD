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
program boxmodel_adm_driver

  use OAD_active
  use OAD_rev
  use OAD_tape
  use all_globals_mod
  implicit none

  integer, parameter :: kdim=3
  integer i,j,n,m
  double precision :: h
  double precision tnew_o(kdim), snew_o(kdim)
  double precision jac(2*kdim,2*kdim)


  external box_model_body

  open(2,action='read',file='params.conf')
  read(2,'(I5,/,I5,/,F8.1)') n, m, h
  close(2)

  !
  ! DD code
  !
  open(2,file='tmpOutput/dd.out')
  write(2,*) "DD"
  our_rev_mode%arg_store=.FALSE.
  our_rev_mode%arg_restore=.FALSE.
  our_rev_mode%res_store=.FALSE.
  our_rev_mode%res_restore=.FALSE.
  our_rev_mode%plain=.TRUE.
  our_rev_mode%tape=.FALSE.
  our_rev_mode%adjoint=.FALSE.
  do i=1,2*kdim
     call box_ini_params
     call box_ini_fields
     call box_model_body
     do j=1,kdim
        tnew_o(j)=tnew(j)%v
     end do
     do j=1,kdim
        snew_o(j)=snew(j)%v
     end do
     call box_ini_params
     call box_ini_fields
     xx(i)%v=xx(i)%v+h
     call box_model_body
     do j=1,kdim
        jac(j,i)=(tnew(j)%v-tnew_o(j))/h
        jac(j+kdim,i)=(snew(j)%v-snew_o(j))/h
     end do
  end do
  do i=1,2*kdim
     do j=1,2*kdim
        write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",i,",",j,")=",jac(i,j)
     end do
  end do
  close(2)

  !
  ! ACTS code
  !
  open(2,file='tmpOutput/ad.out')
  write(2,*) "AD"
  call tape_init()
  do i=1,2*kdim
     do j=1,2*kdim
        call zero_deriv(xx(j))
     end do
     our_rev_mode%arg_store=.FALSE.
     our_rev_mode%arg_restore=.FALSE.
     our_rev_mode%res_store=.FALSE.
     our_rev_mode%res_restore=.FALSE.
     our_rev_mode%plain=.TRUE.
     our_rev_mode%tape=.FALSE.
     our_rev_mode%adjoint=.FALSE.
     call box_ini_params
     call box_ini_fields
     do j=1,kdim
        call zero_deriv(tnew(j))
        call zero_deriv(snew(j))
     enddo
     if (i>kdim) then
        do j=1,kdim
           if (j==i-kdim) then
              snew(j)%d=1
           end if
        end do
     else
        do j=1,kdim
           if (j==i) then
              tnew(j)%d=1
           end if
        end do
     end if
     our_rev_mode%arg_store=.FALSE.
     our_rev_mode%arg_restore=.FALSE.
     our_rev_mode%res_store=.FALSE.
     our_rev_mode%res_restore=.FALSE.
     our_rev_mode%plain=.FALSE.
     our_rev_mode%tape=.TRUE.
     our_rev_mode%adjoint=.FALSE.
     call box_model_body
     our_rev_mode%arg_store=.FALSE.
     our_rev_mode%arg_restore=.FALSE.
     our_rev_mode%res_store=.FALSE.
     our_rev_mode%res_restore=.FALSE.
     our_rev_mode%plain=.FALSE.
     our_rev_mode%tape=.FALSE.
     our_rev_mode%adjoint=.TRUE.
     call box_model_body
     do j=1,2*kdim
        write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",i,",",j,")=",xx(j)%d
     end do
  end do
  close(2)

end program boxmodel_adm_driver
