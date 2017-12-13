module OAD_intrinsics
end module
    program functionComment
      use OAD_active
      use OAD_intrinsics
      real :: x(2)
      real :: oad_ctmp0
      real :: oad_ctmp1

      x(1) = 1.5D0
      x(2) = -3.6D0
      call oad_s_f(x(1),oad_ctmp0)
      call oad_s_f(x(2),oad_ctmp1)
      print *,oad_ctmp0+oad_ctmp1
    end program functionComment

    function f(x)
      use OAD_active
      use OAD_intrinsics
!     blah blah
!     aren't these names interesting?: pomentale and gargantini
      real :: x,f

      if (x<=0) then
        f = 4.0D0
      elseif (x<=39) then
        f = exp(x*(1.0D0/(x+2)))
      else
        f = 0
      end if
    end function f

    subroutine oad_s_f(x,f)
      use OAD_active
      use OAD_intrinsics
!     blah blah
!     aren't these names interesting?: pomentale and gargantini
      real :: x
      real,intent(out) :: f

      if (x<=0) then
        f = 4.0D0
      elseif (x<=39) then
        f = exp(x*(1.0D0/(x+2)))
      else
        f = 0
      end if
    end subroutine oad_s_f

