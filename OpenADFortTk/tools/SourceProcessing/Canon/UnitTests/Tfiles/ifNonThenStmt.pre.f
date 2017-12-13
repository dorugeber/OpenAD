        subroutine head(y)
          use OAD_intrinsics
          real :: y
          external foo
          real :: oad_ctmp0
          call oad_s_foo(y,oad_ctmp0)
          if (oad_ctmp0.eq.0) then
            y = 0
          end if
        end subroutine

