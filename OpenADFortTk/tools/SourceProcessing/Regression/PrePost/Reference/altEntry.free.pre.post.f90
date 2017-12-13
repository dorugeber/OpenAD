module OAD_intrinsics
end module
function foo1(x)
  use OAD_active
  use OAD_intrinsics
  double precision:: x,foo1,foo2
  foo1=x
  return
entry foo2 ( x )
  foo2 = 2*x
end
subroutine oad_s_foo1(x,foo1)
  use OAD_active
  use OAD_intrinsics
  double precision :: x,foo2
  double precision,intent(out) :: foo1
  foo1=x
  return
entry oad_s_foo2 ( x,foo2 )
  foo2 = 2*x
end subroutine oad_s_foo1

program p
  use OAD_active
  use OAD_intrinsics
  double precision :: x, foo1, foo2
  external oad_s_foo1,oad_s_foo2
  double precision :: oad_ctmp0
  double precision :: oad_ctmp1
  x=2.0
  call oad_s_foo1(x,oad_ctmp0)
  x = oad_ctmp0
  print *,x
  call oad_s_foo2(x,oad_ctmp1)
  x = oad_ctmp1
  print *,x
end
