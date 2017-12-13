        subroutine head(x1,x2)
          integer :: x1
          double precision :: x2
          x1 = max(x1,0) ! should return int
          x2 = max(10,0) ! should return int
          x2 = max(x2,0) ! should return double
        end subroutine

