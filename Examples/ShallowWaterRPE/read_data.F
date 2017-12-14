c$openad XXX Template OADrts/ad_template.split.f
      subroutine read_data( time )

      implicit none
c     begin interface
      real time
c     end interface

cph(
cph choose any of the following
c      call read_eta_data( time )
c      call read_uv_data( time )
c      call read_zonal_transport_data( time )
cph)

      end ! subroutine read_data

c$openad XXX Template OADrts/ad_template.split.f
      subroutine read_eta_data( time )

      use size
      use data
      use parms

      implicit none
c     begin interface
      real time
c     end interface

!un #include "size.inc"
!un #include "data.inc"
!un #include "parms.inc"

c     begin interface
c     real time
c     end interface

c     locals
      integer ix, iy
      real f_in(nx,ny)
!nt
      character*(80) :: myetadata
      character*(80) :: myeta
      integer mynx, myny
      mynx = nx ; myny = ny
      myetadata = 'etadata'
      myeta = 'eta'

      if ( time .eq. 0. ) then
c     assume that there is only one data slab that is to be use at all
c     times
         call read_field_netcdf(ncdatafile,myetadata,mynx,myny,f_in)
         do iy = 1, ny
            do ix = 1, nx
               eta_data(ix,iy) = f_in(ix,iy)
            end do
         end do
      else if ( time .gt. 0. ) then
         call read_snap_netcdf( ncdatafile, time, mynx, myny, 
     &        myeta, f_in )
         do iy = 1, ny
            do ix = 1, nx
               eta_data(ix,iy) = f_in(ix,iy)
            end do
         end do
      else
c     set data to zero
         do iy = 1, ny
            do ix = 1, nx
               eta_data(ix,iy) = 0.
            end do
         end do
      end if

      end ! subroutine read_eta_data

c$openad XXX Template OADrts/ad_template.split.f
      subroutine read_uv_data( time )

      use size
      use data
      use parms

      implicit none
c     begin interface
      real time
c     end interface

!un #include "size.inc"
!un #include "data.inc"
!un #include "parms.inc"

c     begin interface
c     real time
c     end interface

c     locals
      integer ix, iy
      real f_in(nx,ny)
!nt
      integer :: mynx = nx, myny = ny
      character*(80) :: strudata, strvdata
      character*(80) :: strU, strV
      strudata = 'udata' ; strvdata = 'vdata'
      strU = 'U' ; strV = 'V'

      if ( time .eq. 0. ) then
c     assume that there is only one data slab that is to be use at all
c     times
         call read_field_netcdf(ncdatafile,strudata,mynx,myny,f_in)
         do iy = 1, ny
            do ix = 1, nx
               u_data(ix,iy) = f_in(ix,iy)
            end do
         end do
         call read_field_netcdf(ncdatafile,strvdata,mynx,myny,f_in)
         do iy = 1, ny
            do ix = 1, nx
               v_data(ix,iy) = f_in(ix,iy)
            end do
         end do
      else if ( time .gt. 0. ) then
         call read_snap_netcdf(ncdatafile,time,mynx,myny,strU,f_in)
         do iy = 1, ny
            do ix = 1, nx
               u_data(ix,iy) = f_in(ix,iy)
            end do
         end do
         call read_snap_netcdf(ncdatafile,time,mynx,myny,strV,f_in)
         do iy = 1, ny
            do ix = 1, nx
               v_data(ix,iy) = f_in(ix,iy)
            end do
         end do
      else
c     set data to zero
         do iy = 1, ny
            do ix = 1, nx
               u_data(ix,iy) = 0.
               v_data(ix,iy) = 0.
            end do
         end do
      end if

      end ! subroutine read_uv_data

c$openad XXX Template OADrts/ad_template.split.f
      subroutine read_zonal_transport_data( time )

      use size
      use data
      use pfields
      use vars

      implicit none
c     begin interface
      real time
c     end interface

!un #include "size.inc"
!un #include "data.inc"
!un #include "pfields.inc"
!un #include "vars.inc"
c     locals
c     zonal volume transport
      real zonal_transport
cph new from inlining
      integer ix, iy, jx, jy
      parameter ( ix = 6 ) 
      if ( time .eq. 0. ) then
c     hack
         zonal_transport_data = 52.6265e+06
      else
cph(
cph         call calc_zonal_transport( zonal_transport )
cju      ix = 6
      zonal_transport = 0.
      do iy = 1, ny
         zonal_transport = zonal_transport 
     &        + u(ix,iy)*dy(iy)*hu(ix,iy)
c     &       + u(ix,iy)*dy(iy)*( eta(ix-1,iy)+eta(ix,iy) )
      end do
cph)
         zonal_transport_data = zonal_transport
      end if

      end ! subroutine read_zonal_transport_data

c$openad XXX Template OADrts/ad_template.split.f
      subroutine read_depth_data

      use size
      use pfields
      use data

      implicit none
c     interface
c     end interface

!un #include "size.inc"
!un #include "pfields.inc"
!un #include "data.inc"

c     local
      integer ix, iy

      do iy = 1, ny
         do ix = 1, nx
            depth_data(ix,iy) = inidepth(ix,iy)
         end do
      end do
c$$$c     this is another hack!
c$$$      print *, 'warning: depth_data has been set to zero'//
c$$$     &     ' in read_depth_data'
c$$$      do iy = 1, ny
c$$$         do ix = 1, nx
c$$$            depth_data(ix,iy) = 0.
c$$$         end do
c$$$      end do

      end ! subroutine read_depth_data

c$openad XXX Template OADrts/ad_template.split.f
      subroutine determine_data_time( ncdatafile )

      use size
      use data

      implicit none
c     interface
      character*(80) ncdatafile
c     integer nedt
c     end interface

!un #include "size.inc"
!un #include "data.inc"

      integer k
 !nt
      character*(80) :: strtime
      strtime = 'TIME'

      call get_length_netcdf( ncdatafile, strtime, nedt )
      if ( nedt .gt. nedtmax ) then
         print *, 'determine_data_time: too many data,;'
         print *, 'increase nedtmax to ', nedt
cju         stop 'abnormal in determine_data_time'
      else if ( nedt .le. 0 ) then
         print *, 'no time dependent data found in '//ncdatafile
      else
         call read_vector_netcdf( ncdatafile, strtime, 
     &        nedt, eta_data_time )
         print *, 'determine_data_time: # of data times = ', nedt
      end if
      print *, (eta_data_time(k), k=1,nedt)

      end ! subroutine determine_data_time

!nt converted to subroutine
c$openad XXX Template OADrts/ad_template.split.f
      subroutine is_eta_data_time( time, result )

      use size
      use data

      implicit none
c     interface 
      real time
      logical result
c     end interface

!un #include "size.inc"
!un #include "data.inc"
c     locals
      integer it
      logical allDone	

      allDone = .false.
      result = .false.
      it = 1
      do while (.NOT. allDone) 
       if ( abs(eta_data_time(it)-time) .lt. 1.e-8 ) then
         result = .true.
         allDone = .true.
        else
         if ( it .lt. nedt ) then
            it = it + 1
         else
            allDone = .true.
         end if
        end if
      end do
      return
      end subroutine
