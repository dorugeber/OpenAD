program driverAdm
 use OAD_active
 use OAD_rev
 use OAD_tape

 implicit none 

 type(active), dimension(:), allocatable :: x
 type(active) :: f
 integer numProcs
 integer i
 write (*,'(A)',advance='NO') 'enter number of processors used in the parallel case: '
 read *, numProcs

 ! init x
 our_rev_mode%plain=.TRUE.
 allocate (x(numProcs))
 do i = 1, numProcs
    call init(x(i)%v,i)
 end do
 ! compute f
 our_rev_mode%plain=.FALSE.
 our_rev_mode%tape=.TRUE.
 call compute(x,f,numProcs)
 our_rev_mode%tape=.FALSE.
 our_rev_mode%adjoint=.TRUE.
 f%d=1.0
 call compute(x,f,numProcs)
 ! all nodes print the adjoints.
 do i = 1, numProcs
    print *, i-1, ":", x(i)%d
 end do
end
