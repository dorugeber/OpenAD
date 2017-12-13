module OAD_intrinsics
  interface oad_s_max
    module procedure oad_s_max_d
  end interface
  contains
    subroutine oad_s_max_d(a0,a1,r)
      double precision,intent(in) :: a0
      double precision,intent(in) :: a1
      double precision,intent(out) :: r
      if (a0>a1) then
        r = a0
      else
        r = a1
      end if
    end subroutine
end module
        program maxSpec
        use OAD_intrinsics
        implicit none
        double precision x,y
          double precision :: oad_ctmp0
        x = 3.1415D0
        call oad_s_max(x,8.90D0,oad_ctmp0)
        y = oad_ctmp0
        write(*,*) 'y = ',y
        end program

