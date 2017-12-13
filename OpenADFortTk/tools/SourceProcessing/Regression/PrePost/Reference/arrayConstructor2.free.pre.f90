module OAD_intrinsics
end module
module m
use OAD_intrinsics
 contains
 integer function foo(a)
   integer :: a(3)
   foo=0
   foo=sum(a)
 end function
 subroutine oad_s_foo(a,foo)
   integer :: a(3)
   integer,intent(out) :: foo
   foo=0
   foo=sum(a)
 end subroutine oad_s_foo
end module

program p
 use OAD_intrinsics
 use m
 integer :: i,j,k
  integer :: oad_ctmp0
  integer,dimension(3) :: oad_ctmp1
 i=2
 j=3
 k=4
 oad_ctmp1 = (/i,j,k/)
 call oad_s_foo(oad_ctmp1,oad_ctmp0)
 i = oad_ctmp0
 print *,i
end program
