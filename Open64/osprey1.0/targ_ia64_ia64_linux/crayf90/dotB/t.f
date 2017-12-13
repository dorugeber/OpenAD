       program main
       integer i
       real*8 a(100), b(100)
       do i=1,100
         a(i)=b(i)*2.0
       enddo
       print *,a(i) 
       end
