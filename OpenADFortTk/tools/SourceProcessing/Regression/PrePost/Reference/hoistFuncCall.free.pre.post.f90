module OAD_intrinsics
end module
    program functionComment
      use OAD_active
      use OAD_intrinsics
      real :: x
      real :: oad_ctmp0

      x = 1.5D0
      call oad_s_f(x,oad_ctmp0)
      print *,oad_ctmp0
    end program functionComment

    function f(x)
      use OAD_active
      use OAD_intrinsics
      real :: x,f

      f = 5
    end function f

    subroutine oad_s_f(x,f)
      use OAD_active
      use OAD_intrinsics
      real :: x
      real,intent(out) :: f

      f = 5
    end subroutine oad_s_f
