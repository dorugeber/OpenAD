! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
! +                                                               +
! + adism_umf.F90: part of the ADISM adjoint ice sheet model      +
! +                                                               +
! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
! 
! Copyright (C) 2013 ADISM contributors - see COPYRIGHT file 
! for list of contributors.
!
! This program is free software; you can redistribute it and/or 
! modify it under the terms of the GNU General Public License as 
! published by the Free Software Foundation; either version 3 of 
! the License, or (at your option) any later version.
!
! This program is distributed in the hope that it will be useful, 
! but WITHOUT ANY WARRANTY; without even the implied warranty of 
! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
! GNU General Public License for more details.
!
! You should have received a copy of the GNU General Public License 
! along with this program; if not, write to the Free Software 
! Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 
! 02111-1307 USA
!
! ADISM is maintained by:
!
! Ian Rutt, Swansea University (i.c.rutt@swansea.ac.uk)
! Jonathan McGovern, LGGE, France
!
! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

! Subroutine to handle the connection to UMFPACK

subroutine adism_umf(totpts,matcount,matrow,matcol,matval,b,x)
  !$openad XXX Template umfpack_template.split.f

  implicit none

  ! variables/parameters for the UMFPACK routines
  ! UMD21I  Initialise user-controlled variables
  ! UMD2FA: Primary analysis+factorization routine
  ! UMD2RF: Numerical factorization routine
  ! UMD2SO: Routine for solving a linear system
  integer,parameter      ::     dp = kind(1.d0)
  integer, intent(in)              ::   totpts    ! order of matrix,n
  integer, intent(in)              ::   matcount
  integer, dimension(totpts*5),intent(in) ::   matrow
  integer, dimension(totpts*5),intent(in) ::   matcol
  real(dp),dimension(totpts*5),intent(in) ::   matval
  real(dp),dimension(totpts)            ::   b
  real(dp),dimension(totpts)            ::   x

  real(dp),dimension(20)           ::   RINFO
  integer,dimension(40)            ::   INFO

  integer                          :: lvalue,lindex
  integer,dimension(2000000)         :: index
  real(dp),dimension(2000000)        :: value
  integer                          :: ii

  integer,dimension(20)            ::   ICNTL
  integer,dimension(20)            ::   KEEP
  real(dp),dimension(10)           ::   CNTL
  real(dp),dimension(2*totpts)     ::   w

  lindex = 2000000
  lvalue = 2000000

  ! In triplet form, the non-zero entries are stored in terms of rows, columns and values
  ! Find a numerically-acceptable pivot order and computes LU factors, PAQ=LU for a 
  ! sparse matrix a. Pivoting done within each diagonal block.

  ! 1. Initialize default controls
  call UMD21I (KEEP,      & !
       CNTL,              &
       ICNTL)

  do ii=1,matcount
     value(ii)=matval(ii)
     index(ii)=matrow(ii)
     index(ii+matcount)=matcol(ii)
  end do

  ! 2. Factorize a
  call UMD2FA(totpts ,     &! n ... order of matrix
       matcount,           &! ne ... number non zeros
       1,                  &! job ... input matrix is overwritten
       .false.,            &! transa ... a is factorised 
       lvalue,             &! lvalue            
       lindex,             &! lindex
       value,              &! value
       index,              &! index
       KEEP,               &! keep
       CNTL,               &! cntl
       ICNTL,              &! icntl 
       INFO,               &! info
       RINFO )     ! valgrind error (use of unconditional value of size 4)

  if (INFO (1) .lt. 0) stop ! if info(1) is negative there is an error

  ! 3.  Solve Ax = b, the new ice surface elevation,without iterative refinement, given LU factors computed by UMD2FA
  call UMD2SO (totpts,     & !order of matrix (in)   segmentation fault
       0,                  & !input matrix is overwritten
       .false.,            & !A is factorised
       lvalue,             & !values of sparse matrix elements
       lindex,             & !row index of matrix format      
       value,              &
       index,              &
       KEEP,               & ! an integer value
       b,                  & ! right hand side vector x,    
       x,                  & !
       w,                  & ! array of size 4*n
       CNTL,               & ! array of size 10
       ICNTL,              & ! array of size 20
       INFO,               & ! array of size 40
       RINFO)                ! array size 20

  if (INFO (1).lt.0) stop    ! info(1) is the error

end subroutine adism_umf
