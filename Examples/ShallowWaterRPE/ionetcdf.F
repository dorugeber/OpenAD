      subroutine create_netcdf(fname,tname,nx,ny)

      implicit none
#include "netcdf.inc"

c     interface
      character*(80) fname, tname
      integer nx, ny
c     end interface
c     local variables
      integer errmsg
      integer fid, did
      character*24 date
      integer lchar, lastchar
      external lastchar
!nt
      character*(80) :: str1, str2

c     create file
      errmsg = nf_create(fname,nf_clobber,fid)
      str1 = 'nf_create'
      call handle_err( errmsg, str1 )
c     global attributes
      lchar = lastchar(tname)
      str1 = 'title'
      errmsg = nf_put_att_text(fid,nf_global,str1,
     &     lchar,tname(1:lchar))
      str1 = 'nf_put_att_text: title'
      call handle_err( errmsg, str1 )
c
cph      call fdate( date )
cph      errmsg = nf_put_att_text(fid,nf_global,'creation_date',
cph     &     24,date)
cph      call handle_err( errmsg, 'nf_put_att_text: creation_date' )
c
c     create record dimension time
      str1 = 'TIME' ; str2 = 'nf_def_dim: TIME'
      errmsg = nf_def_dim( fid, str1, nf_unlimited, did )
      call handle_err( errmsg, str2 )
c     create dimension x
      str1 = 'X' ; str2 = 'nf_def_dim: X'
      errmsg = nf_def_dim( fid, str1, nx, did )
      call handle_err( errmsg, str2 )
c     create dimension y
      str1 = 'Y'; str2 = 'nf_def_dim: Y'
      errmsg = nf_def_dim( fid, str1, ny, did )
      call handle_err( errmsg, str2 )
c     end define mode
      str1 = 'nf_enddef'
      errmsg = nf_enddef( fid )
      call handle_err( errmsg, str1 )
c     close file
      str1 = 'nf_close'
      errmsg = nf_close( fid )
      call handle_err( errmsg, str1 )

      return
      end! subroutine create_netcdf

      subroutine add_gatta_netcdf( fname, aname, a )

      implicit none
#include "netcdf.inc"

c     interface
      character*(80) a
      character*(80) aname
      character*(80) fname
c     end interface
c     local variables
      integer errmsg
      integer fid
      integer lchar, lastchar
      external lastchar
!nt
      character*(80) :: str1

c     open netcdf file
      str1 = 'nf_open'
      errmsg = nf_open( fname, nf_write, fid )
      call handle_err( errmsg, str1 )
c     put in define mode
      str1 = 'nf_redef'
      errmsg = nf_redef( fid )
      call handle_err( errmsg, str1 )
c     put global attribute
      lchar = lastchar(a)
      if ( lchar .eq. 0 ) then
         lchar = 1
      end if
      str1 = 'nf_put_att_text: '//aname
      errmsg = nf_put_att_text(fid,nf_global,aname,lchar,a(1:lchar))
      call handle_err( errmsg, str1 )
c     close file
      str1 = 'nf_close'
      errmsg = nf_close( fid )
      call handle_err( errmsg, str1 )

      return
      end! subroutine add_gatta_netcdf

      subroutine add_gatti_netcdf( fname, aname, a )
c
c     add an integer value global variable to fname
c
      implicit none
#include "netcdf.inc"

c     interface
      integer a
      character*(80) aname
      character*(80) fname
c     end interface
c     local variables
      integer errmsg
      integer fid
!nt
      character*(80) :: str1

c     open netcdf file
      str1 = 'nf_open'
      errmsg = nf_open( fname, nf_write, fid )
      call handle_err( errmsg, str1 )
c     put in define mode
      str1 = 'nf_redef'
      errmsg = nf_redef( fid )
      call handle_err( errmsg, str1 )
c     put global attribute
      str1 = 'nf_put_att_int '//aname
      errmsg = nf_put_att_int(fid,nf_global,aname,
     &     nf_int,1,a)
      call handle_err( errmsg, str1 )
c     close file
      str1 = 'nf_close'
      errmsg = nf_close( fid )
      call handle_err( errmsg, str1 )

      return
      end! subroutine add_gatti_netcdf

      subroutine add_gattr_netcdf( fname, aname, a )
c
c     add a real value global variable to fname
c
      implicit none
#include "netcdf.inc"

c     interface
      real a
      character*(80) aname
      character*(80) fname
c     end interface
c     local variables
      integer errmsg
      integer fid
      integer nf_type
!nt
      character*(80) :: str1
               
      call set_nf_real_type( nf_type )
c     open netcdf file
      str1 = 'nf_open'
      errmsg = nf_open( fname, nf_write, fid )
      call handle_err( errmsg, str1 )
c     put in define mode
      str1 = 'nf_redef'
      errmsg = nf_redef( fid )
      call handle_err( errmsg, str1 )
c     put global attribute
      str1 = 'nf_put_att_double '//aname
      errmsg = nf_put_att_double(fid,nf_global,aname,
     &     nf_type,1,a)
      call handle_err( errmsg, str1 )
c     close file
      str1 = 'nf_close'
      errmsg = nf_close( fid )
      call handle_err( errmsg, str1 )

      return
      end! subroutine add_gattr_netcdf

      subroutine add_coordinates_netcdf( fname, nx, x, ny, y, units )
      implicit none
#include "netcdf.inc"

c     interface
      integer nx, ny
      real x(nx), y(ny)
      character*(80) fname
      character*(80) units
c     end interface
c     local variables
      integer errmsg
      integer fid, xid, yid, tid, did
      integer nf_type
      integer ix, iy
      real range(2)
      character*(1) grid
!nt
      character*(80) :: str1, str2

      if ( units .eq. 'deg' ) then
         grid = 's'
      else if ( units .eq. 'meters' ) then
         grid = 'c'
      else
         print *, ' no propery x/y units specified'
cju         stop
      end if

      call set_nf_real_type( nf_type )     
c     open netcdf file
      str1 = 'nf_open'
      errmsg = nf_open( fname, nf_write, fid )
      call handle_err( errmsg, str1 )
c     put in define mode
      str1 = 'nf_redef'
      errmsg = nf_redef( fid )
      call handle_err( errmsg, str1 )
c     T-coordinate
c     get dimension id TIME
      str1 = 'TIME' ; str2 = 'nf_inq_dimid: TIME'
      errmsg = nf_inq_dimid( fid, str1, did )
      call handle_err( errmsg, str2 )
c     define variable
      str1 = 'TIME' ; str2 = 'nf_def_var: TIME'
      errmsg = nf_def_var( fid, str1, nf_type, 1, did, tid )
      call handle_err( errmsg, str2 )
c     define some attibutes
      str1 = 'nf_put_att_text: TIME: long_name'
      str2 = 'nf_put_att_text: TIME: units'
      errmsg = nf_put_att_text( fid, tid, 'long_name',
     &     19, 'time of integration' )
      call handle_err( errmsg, str1 )
      errmsg = nf_put_att_text( fid, tid, 'units', 7, 'seconds' )
      call handle_err( errmsg, str2 )
c     X-coordinate
c     get dimension id X
      str1 = 'nf_inq_dimid: X'
      errmsg = nf_inq_dimid( fid, 'X', did )
      call handle_err( errmsg, str1 )
c     define variable
      str1 = 'nf_def_var: X'
      errmsg = nf_def_var( fid, 'X', nf_type, 1, did, xid )
      call handle_err( errmsg, str1 )
c     define some attibutes
      if ( grid .eq. 's' ) then
         errmsg = nf_put_att_text( fid, xid, 'long_name',
     &        9, 'longitude' )
      else if ( grid .eq. 'c' ) then
         errmsg = nf_put_att_text( fid, xid, 'long_name',
     &        12, 'X-coordinate' )
      end if
      str1 = 'nf_put_att_text: X: long_name'
      call handle_err( errmsg, str1 )
      if ( grid .eq. 's' ) then
         errmsg = nf_put_att_text( fid, xid, 'units',
     &        len(units)+1, units//'E' )
      else if ( grid .eq. 'c' ) then
         errmsg = nf_put_att_text( fid, xid, 'units',
     &        len(units), units )
      end if
      str1 = 'nf_put_att_text: X: units'
      call handle_err( errmsg, str1 )
      range(1) = 1.e+23
      range(2) = -1.e+23
      do ix = 1, nx
!nt      range(1) = min(range(1),x(ix))
!nt      range(2) = max(range(2),x(ix))
         if (range(1) .gt. x(ix)) then 
            range(1) = x(ix)
         end if
         if (range(2) .lt. x(ix)) then 
            range(2) =x(ix)
         end if
      end do
      errmsg = nf_put_att_double( fid, xid, 'valid_range',
     &        nf_type, 2, range )
      str1 = 'nf_put_att_text: X: valid_range'
      call handle_err( errmsg, str1)
c     Y-coordinate
c     get dimension id Y
      errmsg = nf_inq_dimid( fid, 'Y', did )
      str1 = 'nf_inq_dimid: Y'
      call handle_err( errmsg, str1 )
c     define variable
      str1 = 'nf_def_var: Y'
      errmsg = nf_def_var( fid, 'Y', nf_type, 1, did, yid )
      call handle_err( errmsg, str1 )
c     define some attibutes
      if ( grid .eq. 's' ) then
         errmsg = nf_put_att_text( fid, yid, 'long_name',
     &        8, 'latitude' )
      else if ( grid .eq. 'c' ) then
         errmsg = nf_put_att_text( fid, yid, 'long_name',
     &        12, 'Y-coordinate' )
      end if
      str1 = 'nf_put_att_text: Y: long_name'
      call handle_err( errmsg, str1 )
      if ( grid .eq. 's' ) then
         errmsg = nf_put_att_text( fid, yid, 'units',
     &        len(units)+1, units//'N' )
      else if ( grid .eq. 'c' ) then
         errmsg = nf_put_att_text( fid, yid, 'units',
     &        len(units), units )
      end if
      str1 = 'nf_put_att_text: Y: units'
      call handle_err( errmsg, str1 )
      range(1) = 1.e+23
      range(2) = -1.e+23
      do iy = 1, ny
!nt      range(1) = min(range(1),y(iy))
!nt      range(2) = max(range(2),y(iy))
         if (range(1) .gt. y(iy)) then 
            range(1) = y(iy)
         end if
         if (range(2) .lt. y(iy)) then 
            range(2) = y(iy)
         end if
      end do
      errmsg = nf_put_att_double( fid, yid, 'valid_range',
     &        nf_type, 2, range )

c     end define mode
      errmsg = nf_enddef( fid )
      str1 = 'nf_enddef'
      call handle_err( errmsg, str1 )

c     put the data
      errmsg = nf_put_var_double(fid, xid, x)
      str1 = 'nf_put_var_double: X'
      call handle_err( errmsg, str1 )
      errmsg = nf_put_var_double(fid, yid, y)
      str1 = 'nf_put_var_double: Y'
      call handle_err( errmsg, str1 )

c     close file
      errmsg = nf_close( fid )
      str1 = 'nf_close'
      call handle_err( errmsg, str1 )

      return

      end ! subroutine add_coordinates_netcdf

      subroutine add_pfield_netcdf( fname, nx, ny, u, 
     &     uname, ulongname, uunits )

      implicit none
#include "netcdf.inc"

c     interface
      character*(80) fname
      integer nx, ny
      real u
      character*(80) uname, ulongname, uunits
c     end interface

c     local variables
      integer errmsg
      integer fid, uid
      integer did(2)
      integer nf_type
!nt
      character*(80) :: str1

      call set_nf_real_type( nf_type )
c     open netcdf file
      str1 = 'nf_open'
      errmsg = nf_open( fname, nf_write, fid )
      call handle_err( errmsg, str1 )
c     put in define mode
      str1 = 'nf_redef'
      errmsg = nf_redef( fid )
      call handle_err( errmsg, str1 )
c     get dimension ids
      errmsg = nf_inq_dimid( fid, 'X', did(1) )
      str1 = 'nf_inq_dimid: X'
      call handle_err( errmsg, str1 )
      errmsg = nf_inq_dimid( fid, 'Y', did(2) )
      str1 = 'nf_inq_dimid: Y'
      call handle_err( errmsg, str1 )
c     define variable
      errmsg = nf_def_var( fid, uname, nf_type, 2, did, uid )
      str1 = 'nf_def_var: '//uname
      call handle_err( errmsg, str1 )
c     define some attibutes
      errmsg = nf_put_att_text( fid, uid, 'long_name',
     &     len(ulongname), ulongname )
      str1 = 'nf_put_att_double: '//uname//': long_name'
      call handle_err( errmsg, str1 )
      errmsg = nf_put_att_text( fid, uid, 'units',
     &     len(uunits), uunits )
      str1 = 'nf_put_att_double: '//uname//': units'
      call handle_err( errmsg, str1 )
      errmsg = nf_put_att_double( fid, uid, 'missing_value',
     &     nf_type, 1, -99. )
      str1 = 'nf_put_att_double: '//uname//': missing_value'
      call handle_err( errmsg, str1 )
c     end define mode
      errmsg = nf_enddef( fid )
      str1 = 'nf_enddef'
      call handle_err( errmsg, str1 )
c     write the data
      errmsg = nf_put_var_double( fid, uid, u )
      str1 = 'nf_put_var_double: '//uname 
      call handle_err( errmsg, str1 )

c     close file
      errmsg = nf_close( fid )
      str1 = 'nf_close'
      call handle_err( errmsg, str1 )

      return
      end ! subroutine add_pfield_netcdf
      
      subroutine add_recvar_netcdf( fname, uname, ulongname, uunits )

      implicit none
#include "netcdf.inc"

c     interface
      character*(80) fname, uname, ulongname, uunits
c     end interface

c     local variables
      integer errmsg
      integer fid, uid
      integer did(3)
      integer nf_type
!nt
      character*(80) :: str1
      
      call set_nf_real_type( nf_type )
c     open netcdf file
      errmsg = nf_open( fname, nf_write, fid )
      str1 = 'nf_open'
      call handle_err( errmsg, str1 )
c     put in define mode
      errmsg = nf_redef( fid )
      str1 = 'nf_redef'
      call handle_err( errmsg, str1 )
c     get dimension ids
      errmsg = nf_inq_dimid( fid, 'X', did(1) )
      str1 = 'nf_inq_dimid: X'
      call handle_err( errmsg, str1 )
      errmsg = nf_inq_dimid( fid, 'Y', did(2) )
      str1 = 'nf_inq_dimid: Y'
      call handle_err( errmsg, str1 )
      errmsg = nf_inq_dimid( fid, 'TIME', did(3) )
      str1 = 'nf_inq_dimid: TIME'
      call handle_err( errmsg, str1 )
c     define variables
c     U
      errmsg = nf_def_var( fid, uname, nf_type, 3, did, uid )
      str1 = 'nf_def_var: '//uname
      call handle_err( errmsg, str1 )
c     define some attibutes
      errmsg = nf_put_att_text( fid, uid, 'long_name',
     &     len(ulongname), ulongname )
      str1 = 'nf_put_att_double: '//uname//': '//ulongname
      call handle_err( errmsg, str1 )
      errmsg = nf_put_att_text( fid, uid, 'units',
     &     len(uunits), uunits )
      str1 = 'nf_put_att_double: '//uname//': '//uunits
      call handle_err( errmsg, str1 )
      errmsg = nf_put_att_double( fid, uid, 'missing_value',
     &     nf_type, 1, -99. )
      str1 = 'nf_put_att_double: '//uname//': missing_value'
      call handle_err( errmsg, str1 )
c     end define mode
      errmsg = nf_enddef( fid )
      str1 = 'nf_enddef'
      call handle_err( errmsg, str1 )
c     close file
      errmsg = nf_close( fid )
      str1 = 'nf_close'
      call handle_err( errmsg, str1 )


      return
      end !subroutine add_recvar_netcdf

      subroutine write_state_netcdf( fname, nx, ny, it, uname, u )

      implicit none
#include "netcdf.inc"

c     interface
      integer nx, ny, it
      real u(nx,ny)
      character*(80) fname, uname
c     end interface
c     local variables
      integer errmsg
      integer fid, uid
      integer start(3), count(3)
!nt
      character*(80) :: str1

      start(1) = 1
      start(2) = 1
      start(3) = it
      count(1) = nx
      count(2) = ny
      count(3) = 1

c     open netcdf file
      errmsg = nf_open( fname, nf_write, fid )
      str1 = 'nf_open'
      call handle_err( errmsg, str1 )
c     get variable id
      errmsg = nf_inq_varid( fid, uname, uid )
      str1 = 'nf_inq_varid: '//uname
      call handle_err( errmsg, str1 )
c     put the data
      if ( uname .eq. 'TIME' ) then
         errmsg = nf_put_vara_double( fid, uid, it, 1, u(1,1) )
      else
         errmsg = nf_put_vara_double( fid, uid, start, count, u )
      end if
      str1 = 'nf_put_vara_double: '//uname
      call handle_err( errmsg, str1 )
c     close file
      errmsg = nf_close( fid )
      str1 = 'nf_close'
      call handle_err( errmsg, str1 )

      return
      end! subroutine write_state_netcdf

      subroutine write_time_netcdf( fname, it, uname, u )

      implicit none
#include "netcdf.inc"

c     interface
      real u
      character*(80) fname, uname
      integer it
c     end interface
c     local variables
      integer errmsg
      integer fid, uid
!nt
      character*(80) :: str1

c     open netcdf file
      errmsg = nf_open( fname, nf_write, fid )
      str1 = 'nf_open'
      call handle_err( errmsg, str1 )
c     get variable id
      errmsg = nf_inq_varid( fid, uname, uid )
      str1 = 'nf_inq_varid: '//uname
      call handle_err( errmsg, str1 )
c     put the data
      errmsg = nf_put_vara_double( fid, uid, it, 1, u )
      str1 = 'nf_put_vara_double: '//uname
      call handle_err( errmsg, str1 )
c     close file
      errmsg = nf_close( fid )
      str1 = 'nf_close'
      call handle_err( errmsg, str1 )

      return
      end! subroutine write_time_netcdf

      subroutine read_vector_netcdf( fname, aname, n, a )

      implicit none
#include "netcdf.inc"

c     interface
      character*(80) fname, aname
      integer n
      real a(n)
c     end interface
c     local variables
      integer errmsg
      integer k
      integer fid, aid!, atype, ndim, dids(2), natt
!nt
      character*(80) :: str1

c     open netcdf file
      errmsg = nf_open( fname, nf_nowrite, fid )
      str1 = 'nf_open'
      call handle_err( errmsg, str1 )
c     get variable id
      errmsg = nf_inq_varid( fid, aname, aid )
      if ( errmsg .eq. -49 ) then
         print *, 'variable "'//aname//'" not found in '//fname
         print *, 'set to zero'
         do k = 1, n
            a(k) = 0.
         end do
      else 
         str1 = 'nf_inq_varid: '//aname
         call handle_err( errmsg, str1 )
c     get variable
         errmsg = nf_get_var_double( fid, aid, a )
         str1 = 'nf_get_var_double: '//aname 
         call handle_err( errmsg, str1 )
      end if

c     close file
      errmsg = nf_close( fid )
      str1 = 'nf_close'
      call handle_err( errmsg, str1 )

      return
      end ! read_vector_netcdf

      subroutine read_field_netcdf( fname, aname, nx, ny, a )

      implicit none
#include "netcdf.inc"

c     interface
      character*(80) fname, aname
      integer nx, ny
      real a(nx,ny)
c     end interface
c     local variables
      integer errmsg
      integer ix, iy
      integer fid, aid, ndims, dimlen
      integer lchar, lastchar
      external lastchar
!nt
      character*(80) :: strbogus
      strbogus = 'nf_bogusstring'

      ndims = 0
      dimlen = 0
c     open netcdf file
      errmsg = nf_open( fname, nf_nowrite, fid )
      call handle_err( errmsg, strbogus )
c     get variable id
      errmsg = nf_inq_varid( fid, aname, aid )
      if ( errmsg .eq. -49 ) then
         lchar = lastchar(fname)
         print *, 'variable "'//aname//'" not found in '//fname(1:lchar)
         print *, 'set to zero'
         do ix = 1, nx
            do iy = 1, ny
               a(ix,iy) = 0.
            end do
         end do
      else 
         call handle_err( errmsg, strbogus )
c     get number of dimensions
         errmsg = nf_inq_varndims( fid, aid, ndims )
         call handle_err( errmsg, strbogus )
         if ( ndims .eq. 3 ) then
c     find out about the length of the first (== time) dimension
            errmsg = nf_inq_dimlen( fid, 1, dimlen )
            call handle_err( errmsg, strbogus ) 
            if ( dimlen .eq. 1 ) then
c     read the entire variable since it is only one time slab
               errmsg = nf_get_var_double( fid, aid, a ) 
               call handle_err( errmsg, strbogus ) 
            else
               print *, 'read_field_netcdf: "', aname, 
     &              '" in '//fname(1:lchar)//' has too many dimensions'
               print *, 'setting "', aname, '" to zero'
               do ix = 1, nx
                  do iy = 1, ny
                     a(ix,iy) = 0.
                  end do
               end do
            end if
         else
c     get variable
            errmsg = nf_get_var_double( fid, aid, a )
            call handle_err( errmsg, strbogus )
         end if
      end if

c     close file
      errmsg = nf_close( fid )
      call handle_err( errmsg, strbogus )

      return
      end ! read_field_netcdf

      subroutine read_snap_netcdf( fname, 
     &     snap_time, nx, ny, aname, a )

      implicit none
#include "netcdf.inc"

c     interface
      character*(80) fname, aname
      integer nx, ny
      real snap_time
      real a(nx,ny)
c     end interface
c     local variables
      integer errmsg
      integer ix, iy, nt
      integer fid, tid, did, aid
      integer start(3), count(3)
      real rectime, misval
      integer lchar, lastchar
      external lastchar
!nt
      character*(80) :: strbogus
      strbogus = 'nf_bogusstring'

      start(1) = 1
      start(2) = 1
      count(1) = nx
      count(2) = ny
      count(3) = 1
      lchar = lastchar(fname)
c     open netcdf file
      errmsg = nf_open( fname, nf_nowrite, fid )
      call handle_err( errmsg, strbogus )
c     find out about time
      errmsg = nf_inq_varid( fid, 'TIME', tid )
      call handle_err( errmsg, strbogus )
      errmsg = nf_inq_vardimid( fid, tid, did )
      call handle_err( errmsg, strbogus )
      errmsg = nf_inq_dimlen( fid, did, nt )
      call handle_err( errmsg, strbogus )
c     find proper time index ( start at last record )
      start(3) = nt
 1    errmsg = nf_get_var1_double( fid, tid, start(3), rectime )
      call handle_err( errmsg, strbogus )
      if ( abs( rectime - snap_time ) .gt. 1.e-8 ) then
         start(3) = start(3)-1
         if ( start(3) .gt. 0 ) then
cnt: bogus fix for goto
            call handle_err( errmsg, strbogus )
cnt         goto 1
         else
            print *, 'snap_time does not exist in '//fname(1:lchar)
cnt            pause
         end if
      end if
c     get variable id
      errmsg = nf_inq_varid( fid, aname, aid )
      if ( errmsg .eq. -49 ) then
         lchar = lastchar(fname)
         print *, 'variable "'//aname//'" not found in '//fname(1:lchar)
         print *, 'set to zero'
         do ix = 1, nx
            do iy = 1, ny
               a(ix,iy) = 0.
            end do
         end do
      else 
         call handle_err( errmsg, strbogus )
c     get variable's missing value
         errmsg = nf_get_att_double( fid, aid, 
     &        'missing_value', misval )
         call handle_err( errmsg, strbogus )
c     get variable
         errmsg = nf_get_vara_double( fid, aid, start, count, a )
         call handle_err( errmsg, strbogus )
         do ix = 1, nx
            do iy = 1, ny
               if ( a(ix,iy) .eq. misval ) then
                  a(ix,iy) = 0.
               end if
            end do
         end do
      end if

c     close file
      errmsg = nf_close( fid )
      call handle_err( errmsg, strbogus )

      return
      end ! subroutine read_snap_netcdf

      subroutine read_weight_depth_netcdf( fname, aname, a, nx, ny, n )

      implicit none
c     interface
      character*(80) fname, aname
      integer nx, ny, n
      real a(nx,nx,ny,ny)
c     end interface
#include "netcdf.inc"
c     locals
      integer errmsg
      integer fid, aid
      integer lchar, lastchar
      external lastchar
!nt
      character*(80) :: strbogus
      strbogus = 'nf_bogusstring'
      
      n = 0
      lchar = lastchar(fname)
c     open netcdf file
      errmsg = nf_open( fname, nf_nowrite, fid )
      if ( errmsg .ne. 0 ) then
         print *, 'netcdf function nf_open: '//fname(1:lchar)
cnt         print *, nf_strerror( errmsg )
         print *, 'n is set to -1, and "'//aname//'" is not changed'
         n = -1
      else
c     find out about "vname"
         errmsg = nf_inq_varid( fid, aname, aid )
         if ( errmsg .eq. -49 ) then
            lchar = lastchar(fname)
            print *, 
     &           'variable "'//aname//'" not found in '//fname(1:lchar)
            print *, 'n is set to -1, and "'//aname//'" is not changed'
            n = -1
         else
c     read file
            errmsg = nf_get_var_double( fid, aid, a )
            call handle_err( errmsg, strbogus )
         end if
c     close file
         errmsg = nf_close( fid )
         call handle_err( errmsg, strbogus )
      end if

      return
      end ! subroutine read_weight_depth_netcdf

      subroutine get_length_netcdf( fname, vname, n )

      implicit none
#include "netcdf.inc"

c     interface
      character*(80) fname, vname
      integer n
c     end interface
c     local variables
      integer errmsg
      integer fid, did, vid
      integer lchar, lastchar
      external lastchar
!nt
      character*(80) :: strbogus
      strbogus = 'nf_bogusstring'

      lchar = lastchar(fname)
c     open netcdf file
      errmsg = nf_open( fname, nf_nowrite, fid )
      call handle_err( errmsg, strbogus )
c     find out about "vname"
      errmsg = nf_inq_varid( fid, vname, vid )
      if ( errmsg .eq. -49 ) then
         lchar = lastchar(fname)
         print *, 'variable "'//vname//'" not found in '//fname(1:lchar)
         print *, 'n is set to -1'
         n = -1
      else
         call handle_err( errmsg, strbogus )
         errmsg = nf_inq_vardimid( fid, vid, did )
         call handle_err( errmsg, strbogus )
         errmsg = nf_inq_dimlen( fid, did, n )
         call handle_err( errmsg, strbogus )
      end if
c     close file
      errmsg = nf_close( fid )
      call handle_err( errmsg, strbogus )

      end ! subroutine get_length_netcdf

      subroutine set_nf_real_type( nf_type )
      implicit none
#include "netcdf.inc"
c     interface
      integer nf_type
c     end interface

c      nf_type = nf_real
c       for some reason this must be double, do not ask
c       me why
      nf_type = nf_double

      return
      end! subroutine set_nf_real_type

      subroutine handle_err( errmsg, funcname )
      implicit none
#include "netcdf.inc"
c     interface
      integer errmsg
      character*(80) funcname
c     end interface
      if ( errmsg .ne. nf_noerr ) then
         print *, 'error in netcdf function ', funcname
cnt         print *, nf_strerror( errmsg ) 
cnt         pause
      end if

      return
      end! subroutine handle_err

      integer function lastchar( name )

      implicit none
c     interface
      character*(80) name
c     end interface
      integer n, k
c
!nt
#if 0
      n = len( name )
      k = n
      
      do while ( k .gt. 0 .and. name(k:k) .eq. ' ' ) 
         k = k-1
      end do

      lastchar = k
#endif
      lastchar = INDEX(name,' ')

      return
      end ! function lastchar
