      program main

      double precision, dimension(2):: x, g 
      double precision :: y
      double precision, dimension(2,2) :: h
      integer i,j

      do i=1,2
         x(i)=i
      end do 
      
      call hess_driver(x,y,g,h)
      do i=1,2
         do j=1,2
            write(2,'(A,I3,A,I3,A,EN26.16E3)') "h(",i,",",j,")=",h(i,j)
         end do
      end do

      end 
