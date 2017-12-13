      program main

      double precision, dimension(2):: x, g 
      integer i,j

      do i=1,2
         x(i)=i
      end do 
      
      call grad_driver(x,g)

      do i=1,2
         write(2,'(A,I3,A,EN26.16E3)') "g(",i,")=",g
      end do 

      end 
