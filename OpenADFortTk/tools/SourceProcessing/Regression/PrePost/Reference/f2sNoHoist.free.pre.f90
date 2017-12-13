module OAD_intrinsics
end module
function f(x)
  use OAD_intrinsics
  x = x*2
  f=x
end
subroutine oad_s_f(x,f)
  use OAD_intrinsics
  x = x*2
  f=x
end subroutine oad_s_f

program p
  use OAD_intrinsics
  real :: oad_ctmp0
  x=3.5
  call oad_s_f(x,oad_ctmp0)
  y = oad_ctmp0
  print *,x,y
end
