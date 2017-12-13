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

    call cp_store_real_vector(x,2,sd,cd,ad)
    call cp_store_real_vector(y,1,sd,cd,ad)

  end subroutine cp_store_globals_impl

  subroutine cp_restore_globals_impl(sd,cd,si,ci,ss,cs,sb,cb)
    implicit none
    integer :: cd,ad,ci,ai,cs,as,cb,ab
    double precision, dimension(*) :: sd
    integer, dimension(*) :: si
    character(80), dimension(*) :: ss
    logical, dimension(*) :: sb
    integer i,j

    do i=ubound(y,1),lbound(y,1),-1
       y(i)%v=sd(cd); cd=cd-1
    end do
    do i=ubound(x,1),lbound(x,1),-1
       x(i)%v=sd(cd); cd=cd-1
    end do

  end subroutine cp_restore_globals_impl

end module all_globals_cp_mod

