
! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
! +                                                           +
! +  glimmer_utils.f90 - part of the GLIMMER ice model        + 
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





!> Module containing utility code for GLIMMER.
module glimmer_utils


  use glimmer_global

  implicit none

  interface array_bcs
    module procedure array_bcs1d,array_bcs2d
  end interface

!!$  interface check_conformal
!!$    module procedure check_conformal_2d_real
!!$  end interface

contains

  !> Returns the value of a 1D array location,checking first for the boundaries.
  !!
  !! the location is wrapped around the array boundaries until it falls within the array
  !! \author The value of the location in question.
  real(rk) function array_bcs1d(array,i)
    ! Arguments

    real(rk),dimension(:),intent(in) :: array !< The array to be indexed.
    integer,intent(in)               :: i     !< The location to be extracted.

    ! Internal variables

    integer :: n,ii

    n=size(array)
    ii=i

    if ((i<=n).and.(i>=1)) then
      array_bcs1d=array(i)
    endif

    do while (ii>n)
      ii=ii-n
    enddo

    do while (ii<1)
      ii=ii+n
    enddo

    array_bcs1d=array(ii)

  end function array_bcs1d

!+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  !> Returns the value of a 1D array location,checking first for the boundaries.
  !!
  !! the location is wrapped around the array boundaries until it falls within the array
  !! as array_bcs1d but for polar boundary conditions
  !! \author The value of the location in question.
  real(rk) function array_bcs_lats(array,i)

    ! Arguments

    real(rk),dimension(:),intent(in) :: array !< The array to be indexed.
    integer,intent(in) :: i !< The location to be extracted.

    ! Internal variables

    integer :: n,ii

    n=size(array)
    ii=i

    if ((i<=n).and.(i>=1)) then
      array_bcs_lats=array(i)
      return
    endif

    if (ii>n) then
      ii=2*n-ii
      array_bcs_lats=-180.d0+array(ii)
    endif

    if (ii<1) then
      ii=1-ii
      array_bcs_lats=180.d0-array(ii)
    endif

  end function array_bcs_lats

!+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  !> Returns the value of an array 
  !! location, checking first for the boundaries. 
  !! Over-the-pole boundary conditions are implemented here.
  !! \return The value of the location specified.
  real(rk) function array_bcs2d(array,i,j)
    ! Arguments

    real(rk),dimension(:,:),intent(in) :: array !< Array to be indexed
    integer,intent(in) :: i !< The location to be extracted    
    integer,intent(in) :: j !< The location to be extracted

    ! Internal variables

    integer :: nx,ny,ii,jj

    nx=size(array,1) ; ny=size(array,2)

    if ((i>=1).and.(i<=nx).and.(j>=1).and.(j<=ny)) then
      array_bcs2d=array(i,j)
      return
    endif

    ii=i ; jj=j

    if (jj>ny) then
      jj=2*ny-jj
      ii=ii+nx/2
    endif

    if (jj<1) then
      jj=1-jj
      ii=ii+nx/2
    endif

    do while (ii>nx) 
      ii=ii-nx
    enddo

    do while (ii<1)
      ii=ii+nx
    enddo  

    array_bcs2d=array(ii,jj)

  end function array_bcs2d

!++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  !> Adjusts array location indicies
  !! so that they fall within the domain.
  subroutine fix_bcs2d(i,j,nx,ny)

    integer,intent(inout) :: i !< The location of interest
    integer,intent(inout) :: j !< The location of interest
    integer,intent(in) :: nx  !< The size of the domain (number of points in each direction)
    integer,intent(in) :: ny  !< The size of the domain (number of points in each direction)

    if ((i>=1).and.(i<=nx).and.(j>=1).and.(j<=ny)) return

    if (j>ny) then
      j=2*ny-j
      i=i+nx/2
    endif

    if (j<1) then
      j=1-j
      i=i+nx/2
    endif

    do while (i>nx) 
      i=i-nx
    enddo

    do while (i<1)
      i=i+nx
    enddo  

  end subroutine fix_bcs2d

!+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

!!$  !> Checks that two arrays are of the same size.
!!$  subroutine check_conformal_2d_real(array1,array2,label)
!!$    use glimmer_log
!!$
!!$
!!$    real(rk),dimension(:,:),intent(in) :: array1 !< The array 1 to be checked
!!$    real(rk),dimension(:,:),intent(in) :: array2 !< The array 2 to be checked
!!$    character(*),intent(in),optional :: label    !< Optional label, to facilitate bug tracking if the check fails.
!!$
!!$    if ((size(array1,1)/=size(array2,1)).or.(size(array1,2)/=size(array2,2))) then
!!$      if (present(label)) then
!!$        call write_log('Non-conformal arrays. Label: '//label,GM_FATAL)
!!$      else
!!$        call write_log('ERROR: Non-conformal arrays. No label',GM_FATAL)
!!$      endif
!!$    endif
!!$
!!$  end subroutine check_conformal_2d_real

!+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  !> compute horizontal sum for each vertical level
  !!
  !! Calculates the sum of a given three-dimensional field at each
  !! level. The vertical coordinate of the input is the first index of
  !! the array.
  !! \return
  !! A one-dimensional array of the same size as the first dimension of
  !! inp is returned, containing the sum of inp for 
  !! each level.
  subroutine hsum(inp,output,nx,ny,nz)

    implicit none

    integer :: nx,ny,nz
    real(dp),dimension(nz,nx,ny),intent(in) :: inp
    !< The input array. The first index is the vertical, the othe two horizontal.
    real(dp),dimension(nz)  :: output
    real(dp) :: tempvar
  
    integer up,i,j

    do up=1,size(inp,dim=1)
       tempvar = 0.d0
       do i=1,nx
          do j=1,ny
             tempvar = tempvar + inp(up,i,j)
          end do
       end do
       output(up) = tempvar
    end do

  end subroutine hsum

!+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  !> compute horizontal sum of a 2x2 horizontal mesh for each vertical level
  !!
  !! Calculates the sum of a given three-dimensional field at each
  !! level. The vertical coordinate of the input is the first index of
  !! the array.
  !! \return
  !! A one-dimensional array of the same size as the first dimension of
  !! inp is returned, containing the sum of inp for 
  !! each level.  
  subroutine hsum4(inp,output,nx,ny,nz)
    implicit none

    integer :: nx,ny,nz
    real(dp),dimension(nz,nx,ny),intent(in) :: inp 
    !< The input array. The first index is the vertical, the other two horizontal.
    real(dp),dimension(nz)  :: output
  
    output(:) = inp(:,1,1) + inp(:,2,1) + inp(:,1,2) + inp(:,2,2)

  end subroutine hsum4

!+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  !> Calculates the sum of a given two-dimensional field along one axis.
  !! Within GLIMMER, this function calculates the mean vertical profile
  !! in a 2D vertical slice. 
  !! \return
  !! A one-dimensional array of the same size as the first dimension of
  !! inp is returned, containing the sum of inp for 
  !! each row.
!!$  function lsum(inp)
!!$
!!$    implicit none
!!$
!!$    real(dp),dimension(:,:), intent(in) :: inp !< Input array
!!$    real(dp),dimension(size(inp,dim=1)) :: lsum
!!$    
!!$    lsum = sum(inp(:,:),dim=2)
!!$
!!$  end function lsum

!+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  !> Interpolates a two-dimensional field from the main grid to the velocity grid
  subroutine stagvarb(ipvr,opvr,ewn,nsn)
    use glimmer_global, only : dp 
 
    implicit none 

    real(dp), dimension(:,:), intent(out) :: opvr !< Output field (velocity grid)
    real(dp), dimension(:,:), intent(in)  :: ipvr !< Input field (ice grid)
    integer,                  intent(in)  :: ewn  !< Number of nodes in x (ice grid)
    integer,                  intent(in)  :: nsn  !< Number of nodes in y (ice grid)

    opvr(1:ewn-1,1:nsn-1) = (ipvr(2:ewn,1:nsn-1) + ipvr(1:ewn-1,2:nsn) + &
                             ipvr(2:ewn,2:nsn)   + ipvr(1:ewn-1,1:nsn-1)) / 4.0d0

  end subroutine stagvarb

  !+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  !> Tridiagonal solver. All input/output arrays should have the 
  !! same number of elements.
  subroutine tridiag(a,b,c,x,y,n)
    integer :: n
    real(dp),dimension(n),intent(in)  :: a !< Lower diagonal; a(1) is ignored.
    real(dp),dimension(n),intent(in)  :: b !< Centre diagonal
    real(dp),dimension(n),intent(in)  :: c !< Upper diagonal; c(n) is ignored.
    real(dp),dimension(n),intent(out) :: x !< Unknown vector
    real(dp),dimension(n),intent(in)  :: y !< Right-hand side

    real(dp),dimension(n) :: aa
    real(dp),dimension(n) :: bb

    integer :: i

    aa(1) = c(1)/b(1)
    bb(1) = y(1)/b(1)

    do i=2,n
       aa(i) = c(i)/(b(i)-a(i)*aa(i-1))
       bb(i) = (y(i)-a(i)*bb(i-1))/(b(i)-a(i)*aa(i-1))
    end do
    
    x(n) = bb(n)

    do i=n-1,1,-1
       x(i) = bb(i)-aa(i)*x(i+1)
    end do

  end subroutine tridiag

  !+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  function vertintg(dups,in)
    !*FD Performs a depth integral using the trapezium rule.
    !*RV The value of in integrated over depth.

    implicit none

    !------------------------------------------------------------------------------------
    ! Subroutine arguments
    !------------------------------------------------------------------------------------

    real(dp),dimension(:),         intent(in) :: dups !*FD Intervals between sigma layers
    real(dp),dimension(size(dups)),intent(in) :: in   !*FD Input array of vertical velocities (size = upn)
    real(dp) :: vertintg

    !------------------------------------------------------------------------------------
    ! Internal variables
    !------------------------------------------------------------------------------------

    integer :: up, upn

    ! Set up array of sigma intervals, if not done already ------------------------------

    upn=size(in)

    ! Do integration --------------------------------------------------------------------

    vertintg = 0.0d0

    do up = upn-1, 1, -1
      vertintg = vertintg + (in(up)+in(up+1)) * dups(up)                   
    end do

    vertintg = 0.5d0*vertintg

  end function vertintg

!----------------------------------------------------------------------

!!$  !> Simple filter for 2D fields.
!!$  subroutine filterthck(thck,ewn,nsn)
!!$    use glimmer_global, only : dp 
!!$
!!$    implicit none
!!$
!!$    real(dp), dimension(:,:), intent(inout) :: thck
!!$    real(dp), dimension(:,:), allocatable :: smth
!!$    integer :: ewn,nsn
!!$
!!$    real(dp), parameter :: f = 0.1d0 / 16.0d0
!!$    integer :: count
!!$    integer :: ns,ew
!!$
!!$    allocate(smth(ewn,nsn))
!!$    count = 1
!!$
!!$    do ns = 3,nsn-2
!!$      do ew = 3,ewn-2
!!$
!!$        if (all((thck(ew-2:ew+2,ns) > 0.0d0)) .and. all((thck(ew,ns-2:ns+2) > 0.0d0))) then
!!$          smth(ew,ns) =  thck(ew,ns) + f * &
!!$                        (thck(ew-2,ns) - 4.0d0 * thck(ew-1,ns) + 12.0d0 * thck(ew,ns) - &
!!$                         4.0d0 * thck(ew+1,ns) + thck(ew+2,ns) + &
!!$                         thck(ew,ns-2) - 4.0d0 * thck(ew,ns-1) - &
!!$                         4.0d0 * thck(ew,ns+1) + thck(ew,ns+2))
!!$          count = count + 1
!!$        else
!!$          smth(ew,ns) = thck(ew,ns)
!!$        end if
!!$
!!$      end do
!!$    end do
!!$
!!$    thck(3:ewn-2,3:nsn-2) = smth(3:ewn-2,3:nsn-2)
!!$    print *, count
!!$
!!$    deallocate(smth)            
!!$
!!$  end subroutine filterthck

  subroutine mysum(input,output,nx,ny)

    implicit none

    integer :: nx,ny
    real(dp),dimension(nx,ny) :: input
    real(dp) :: output
    integer :: i,j

    output = 0.d0
    do i=1,nx
       do j=1,ny
          output = output + input(i,j)
       end do
    end do

  end subroutine mysum

  subroutine mysum3d(input,output,nx,ny,nz)

    implicit none

    integer :: nx,ny,nz
    real(dp),dimension(nz,nx,ny) :: input
    real(dp) :: output
    integer :: i,j,k

    output = 0.d0
    do i=1,nx
       do j=1,ny
          do k=1,nz
             output = output + input(k,i,j)
          end do
       end do
    end do

  end subroutine mysum3d
  
end module glimmer_utils
