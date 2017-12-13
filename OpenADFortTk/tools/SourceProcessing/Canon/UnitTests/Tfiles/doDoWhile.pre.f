        subroutine head(y)
          use OAD_intrinsics
          real :: y
          integer :: i,j
          external foo,bar
          real :: oad_ctmp0
          real :: oad_ctmp1
          do while (y.lt.0)
            call oad_s_foo(y,oad_ctmp0)
            do i = oad_ctmp0,10,2
              call oad_s_bar(y,oad_ctmp1)
              do j = oad_ctmp1,100
                y = y+(i*j)
              enddo
            enddo
          enddo
        end subroutine

