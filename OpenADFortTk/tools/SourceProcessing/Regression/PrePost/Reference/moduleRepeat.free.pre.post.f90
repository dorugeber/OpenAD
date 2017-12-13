module OAD_intrinsics
end module
module m
 use OAD_active
 public
 real :: x(3)
end module

subroutine foo
use OAD_active
use OAD_intrinsics
use m
print *,x
end

subroutine bar
use OAD_active
use OAD_intrinsics
use m
print *,x
end

program p
use OAD_active
use OAD_intrinsics
use m
x=(/.1,.2,.3/)
call foo()
call bar()
end
