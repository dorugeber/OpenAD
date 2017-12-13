subroutine ring(x,numProcs)
!$openad xxx template ../oad_t_split.f

 implicit none 
 double precision  x(numProcs), oldx(numProcs)
 integer numprocs, i, myId, leftId, rightId
 do i=1,numprocs
    do myId=1,numprocs
       x(myId)=sin(x(myId))
       oldx(myId)=x(myId)
    end do
    do myId=1,numprocs
       leftId=MODULO(myId-2,numprocs)+1
       rightId=MODULO(myId,numprocs)+1
       x(myId)=x(myId)+oldx(rightId)+oldx(leftId)
    end do
 end do
end

subroutine compute(x,f,numProcs)
!$openad xxx template ../oad_t_split.f

 implicit none 
 double precision x(*)
 double precision f
 integer numProcs
 integer i
!$openad independent(x)
 call ring(x,numProcs)
 f=0.0
 do i=1, numProcs
    f=f+x(i)
 end do
!$openad dependent(f)
end
