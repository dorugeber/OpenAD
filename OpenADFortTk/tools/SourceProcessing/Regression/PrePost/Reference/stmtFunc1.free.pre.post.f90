module OAD_intrinsics
end module
subroutine foo(i)
use OAD_active
use OAD_intrinsics
print *,i
end

program p
use OAD_active
use OAD_intrinsics
implicit integer (f)
integer i
fun(l) = i+l
  integer :: oad_ctmp0
i=3
oad_ctmp0 = (i+2)
call foo(oad_ctmp0)
end

