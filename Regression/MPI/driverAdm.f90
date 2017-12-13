program driverAdm
 use OAD_active
 use OAD_rev
 use OAD_tape

 ! use mpi
 implicit none ! after 'use' before 'include'
 include 'mpif.h'

 type(active) ::  x, f
 integer myid, ierr
 call MPI_INIT(ierr)
 call MPI_COMM_RANK(MPI_COMM_WORLD, myid, ierr)
 ! init x
 our_rev_mode%plain=.TRUE.
 call init(x%v)
 ! compute f
 our_rev_mode%plain=.FALSE.
 our_rev_mode%tape=.TRUE.
 call compute(x,f)
 ! root prints the function.
 if (myid .eq. 0) then 
    print *, 'function value before starting adjoint: ', f%v
 end if
 call MPI_BARRIER(MPI_COMM_WORLD,ierr)
 our_rev_mode%tape=.FALSE.
 our_rev_mode%adjoint=.TRUE.
 f%d=1.0
 call compute(x,f)
 ! all nodes print their adjoints.
 print *, myid, ":", x%d
 call MPI_FINALIZE(ierr)
end
