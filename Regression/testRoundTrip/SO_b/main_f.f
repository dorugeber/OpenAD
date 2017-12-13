      program main

      double precision, dimension(2):: x 
      double precision :: y
      integer i

      do i=1,2
        x(i)=i*1.0
      end do 
      
      call head(x,y)

      write(*,'(A,E25.17E3)') "f=",y

      end 
