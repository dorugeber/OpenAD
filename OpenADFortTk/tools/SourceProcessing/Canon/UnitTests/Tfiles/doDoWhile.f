        subroutine head(y)
          real :: y
          integer :: i,j
          external foo,bar
          do while (y .lt. 0)
            do i = foo(y),10,2
              do j = bar(y),100
                y = y + (i * j)
              enddo
            enddo
          enddo
        end subroutine

