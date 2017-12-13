module OAD_intrinsics
end module
subroutine foo()
use OAD_active
use OAD_intrinsics
common / / x
real x
print *,x
end subroutine

program p
use OAD_active
use OAD_intrinsics
common // x
real x
x=.3
call foo()
end program
