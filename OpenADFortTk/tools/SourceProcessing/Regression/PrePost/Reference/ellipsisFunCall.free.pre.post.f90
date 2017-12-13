module OAD_intrinsics
  use OAD_active
  interface oad_s_max
    module procedure oad_s_max_r
  end interface
  contains
    subroutine oad_s_max_r(a0,a1,r)
      real,intent(in) :: a0
      real,intent(in) :: a1
      real,intent(out) :: r
      if (a0>a1) then
        r = a0
      else
        r = a1
      end if
    end subroutine
end module
program p
  use OAD_active
  use OAD_intrinsics
  real :: u=1.0,v=1.1,w=1.2,x=0.9,y=0.8,z
  real :: oad_ctmp0
  real :: oad_ctmp1
  real :: oad_ctmp2
  real :: oad_ctmp3
  real :: oad_ctmp4
  call oad_s_max(x,y,oad_ctmp4)
  call oad_s_max(w,oad_ctmp4,oad_ctmp3)
  call oad_s_max(v,oad_ctmp3,oad_ctmp2)
  call oad_s_max(u,oad_ctmp2,oad_ctmp1)
  z = oad_ctmp1
  print *,z
end program
