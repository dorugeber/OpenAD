module oadMPIwrapper

public :: oadGBuf

double precision oadGBuf(1)

end 

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! AWAITALL
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
subroutine  oad_mpi_awaitall ( & 
     count, &
     requests, &
     statuses, &
     ierror)
!$openad xxx template oad_t_awaitall.f
  include 'mpif.h'

  integer count
  integer requests(*)
  integer statuses(MPI_STATUS_SIZE,*)
  integer ierror  

! do nothing

end 

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! ISEND
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
subroutine  oad_mpi_isend ( & 
     buf, & 
     count, &
     datatype, &
     dest, &
     tag, &
     comm, &
     request, &
     ierror)
!$openad xxx template oad_t_isend.f

  use oadMPIwrapper

  double precision buf(*)
  integer count
  integer datatype
  integer dest
  integer tag
  integer comm
  integer request
  integer ierror  

#ifdef OAD_TRANS  
  oadGBuf(1)=buf(1)
#else
  call mpi_isend( &
     buf, & 
     count, &
     datatype, &
     dest, &
     tag, &
     comm, &
     request, &
     ierror)
#endif

end 

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! IRECV
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
subroutine  oad_mpi_irecv ( & 
     buf, & 
     count, &
     datatype, &
     src, &
     tag, &
     comm, &
     request, &
     ierror)
!$openad xxx template oad_t_irecv.f

  use oadMPIwrapper

  double precision buf(*)
  integer count
  integer datatype
  integer src
  integer tag
  integer comm
  integer request
  integer ierror  

#ifdef OAD_TRANS  
  buf(1)=oadGBuf(1)
#else
  call mpi_irecv( &
     buf, & 
     count, &
     datatype, &
     src, &
     tag, &
     comm, &
     request, &
     ierror)
#endif

end 

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! WAITALL
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!$openad xxx template oad_t_waitall.f
subroutine  oad_mpi_waitall ( & 
     count, &
     requests, &
     statuses, &
     ierror)
  include 'mpif.h'

  integer count
  integer requests(*)
  integer statuses(MPI_STATUS_SIZE,*)
  integer ierror  

#ifndef OAD_TRANS
  call mpi_waitall( &
     count, &
     requests, &
     statuses, &
     ierror)
#endif

end 

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! REDUCE
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
subroutine  oad_mpi_reduce ( & 
     sbuf, & 
     rbuf, & 
     count, &
     datatype, &
     op, &
     root, &
     comm, &
     ierror)
!$openad xxx template oad_t_reduce.f

  double precision sbuf(*)
  double precision rbuf(*)
  integer count
  integer datatype
  integer op
  integer root
  integer comm
  integer ierror  

#ifdef OAD_TRANS  
  rbuf(1)=sbuf(1)
#else
  call mpi_reduce( &
     sbuf, & 
     rbuf, & 
     count, &
     datatype, &
     op, &
     root, &
     comm, &
     ierror)
#endif

end 
