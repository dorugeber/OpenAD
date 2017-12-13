      program main

      double precision, dimension(2):: x 
      double precision :: y
      integer i

      do i=1,2
         x(i)=i
      end do 
      
      call head(x,y)

      do i=1,2
         write(2,'(A,I3,A,EN26.16E3)') "f(",i,")=",y
      end do 

      end 
