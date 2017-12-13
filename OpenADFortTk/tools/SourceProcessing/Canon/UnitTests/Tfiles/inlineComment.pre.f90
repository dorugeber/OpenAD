      subroutine head(y)
        use OAD_intrinsics
        real y
        external foo
        real :: oad_ctmp0
        call oad_s_foo(y,oad_ctmp0)
        y = oad_ctmp0
      end subroutine head

