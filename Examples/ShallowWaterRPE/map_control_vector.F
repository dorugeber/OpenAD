c$openad XXX Template OADrts/ad_template.joint.f
      subroutine map_from_control_vector( n, xc ) 

      use size
      use pfields

      implicit none

!un #include "size.inc"
!un #include "pfields.inc"

c     interface
      integer n
      real xc(n)
c     end interface

c     locals
      integer k, ix, iy
c     makes sure, that we use the correct precision when adding one
      real one
      parameter ( one = 1. )

      k = 0
      do iy = 1, ny
         do ix = 1, nx
            if ( etamask(ix,iy) .ne. 0 ) then
               k = k+1
               depth(ix,iy) = scaledepth(ix,iy)*( one + xc(k) )
c               print *, ix, iy, xc(k), depth(ix,iy)
c               depth(ix,iy) = scaledepth(ix,iy) + xc(k)
c               depth(ix,iy) = xc(k)
            end if
         end do
      end do
      if ( k .ne. n ) then
         print *, 'map_from_control_vector: ',
     &        'dimensions of control vector are wrong'
         print *, k, ' should be ', n
cju         stop
      end if
      
      end ! subroutine map_from_control_vector

c$openad XXX Template OADrts/ad_template.joint.f
      subroutine map_to_control_vector( n, xc ) 

      use size
      use pfields

      implicit none

!un #include "size.inc"
!un #include "pfields.inc"

c     interface
      integer n
      real xc(n)
c     end interface

c     locals
      integer k, ix, iy
c     makes sure, that we use the correct precision when substracting one
      real one
      parameter ( one = 1. )

      k = 0
      do iy = 1, ny
         do ix = 1, nx
            if ( etamask(ix,iy) .ne. 0 ) then
               k = k+1
               xc(k) = depth(ix,iy)/scaledepth(ix,iy) - one
c               xc(k) = depth(ix,iy) - scaledepth(ix,iy)
c               xc(k) = depth(ix,iy) 
            end if
         end do
      end do
      if ( k .ne. n ) then
         print *, 'map_to_control_vector: ',
     &        'dimensions of control vector are wrong'
         print *, k, ' should be ', n
cju         stop
      end if
      
      end ! subroutine map_to_control_vector

c$openad XXX Template OADrts/ad_template.joint.f
      subroutine map_gradient( n, adxc, grad ) 

      use size
      use pfields

      implicit none

!un #include "size.inc"
!un #include "pfields.inc"

c     interface
      integer n
      real adxc(n)
      real grad(nx,ny)
c     end interface

c     locals
      integer k, ix, iy

      k = 0
      do iy = 1, ny
         do ix = 1, nx
            if ( etamask(ix,iy) .ne. 0 ) then
               k = k+1
               grad(ix,iy) = adxc(k)/scaledepth(ix,iy)
            end if
         end do
      end do
      if ( k .ne. n ) then
         print *, 'map_from_control_vector: ',
     &        'dimensions of control vector are wrong'
         print *, k, ' should be ', n
cju         stop
      end if
      
      end ! subroutine map_gradient

c$openad XXX Template OADrts/ad_template.joint.f
      subroutine length_of_control_vector( n )
      
      use size
      use pfields

      implicit none

!un #include "size.inc"
!un #include "pfields.inc"

c     interface
      integer n
c     end interface

      integer ix, iy, k

      k = 0
      do iy = 1, ny
         do ix = 1, nx
            if ( etamask(ix,iy) .ne. 0 ) then
               k = k+1
            end if
         end do
      end do
      n = k
      print *, 'dimensions of control vector = ', n

      return
      end ! subroutine length_of_control_vector
