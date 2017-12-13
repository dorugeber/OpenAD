program driverFd

 implicit none
 
 double precision   f0,f,h
 double precision, dimension(:), allocatable :: x0,x
 integer numProcs
 integer i
 
 write (*,'(A)',advance='NO') 'enter number of processors used in the parallel case: '
 read *, numProcs
 
 h=1.0e-6
 allocate (x0(numProcs))
 allocate (x(numProcs))
 do i = 1, numProcs
    call init(x0(i),i)
 end do
 x=x0
 call compute(x,f0,numProcs)
 do i = 1, numProcs
    x=x0
    x(i)=x0(i)+h
    call compute(x,f,numProcs)
    print *, i-1, ":", (f-f0)/h
 end do
end
