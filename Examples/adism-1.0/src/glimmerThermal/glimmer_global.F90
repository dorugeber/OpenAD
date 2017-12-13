! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
! +                                                           +
! +  glimmer_global.f90 - part of the GLIMMER ice model       + 
! +                                                           +
! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
! 
! Copyright (C) 2004 GLIMMER contributors - see COPYRIGHT file 
! for list of contributors.
!
! This program is free software; you can redistribute it and/or 
! modify it under the terms of the GNU General Public License as 
! published by the Free Software Foundation; either version 2 of 
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
! GLIMMER is maintained by:
!
! Ian Rutt
! School of Geographical Sciences
! University of Bristol
! University Road
! Bristol
! BS8 1SS
! UK
!
! email: <i.c.rutt@bristol.ac.uk> or <ian.rutt@physics.org>
!
! GLIMMER is hosted on berliOS.de:
!
! https://developer.berlios.de/projects/glimmer-cism/
!
! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

!> Module holding global variables for Glimmer. Holds real-type
!! kind values, and other global code parameters.
module glimmer_global


  integer,parameter :: sp = kind(1.0)  !< Single precision --- Fortran single-precision real-type kind value.
  
  ! Note that if the code is being compiled with forced typing (e.g. with 
  ! the -r8 flag), then this parameter may need to be set in agreement with 
  ! that.

  integer,parameter :: dp = kind(1.0d0) !< Double precision --- Fortran double-precision real-type kind value
  
  ! Note that if the code is being compiled with forced typing (e.g. with
  ! the -r8 flag), then this parameter may need to be set in agreement
  ! with that

  integer,parameter :: rk=dp !< Precision of glimmer module --- the general Fortran real-type kind value for the Glimmer module and its interfaces.

  real, private, dimension(2) :: dummyarray !< dummy array used for figuring out integer kind used for size
  integer, parameter :: sizek = kind(size(dummyarray)) !< integer kind used for size, len, etc.  

  integer,parameter :: fname_length=200 !< Specifies the length of character string variables used to hold filenames.
  integer,parameter :: msg_length=500  !< lenght of message buffers

  character, parameter :: dirsep = '/' !< directory separator

  character, parameter :: linefeed = achar(10)          !< ASCII linefeed
  character, parameter :: char_ret = achar(13)          !< ASCII carriage-return
  character(2), parameter :: cr_lf = char_ret//linefeed !< default newline appropriate for UNIX-type systems
  character, parameter :: endline = linefeed

end module glimmer_global
