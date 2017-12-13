      real function bar(x)
        use OAD_intrinsics
        real :: x
        bar = x*2
        return
      end function bar
      subroutine oad_s_bar(x,bar)
        use OAD_intrinsics
        real :: x
        real,intent(out) :: bar
        bar = x*2
        return
      end subroutine oad_s_bar
