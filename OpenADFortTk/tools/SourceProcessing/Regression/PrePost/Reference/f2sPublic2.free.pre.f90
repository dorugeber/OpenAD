module OAD_intrinsics
end module
MODULE m
use OAD_intrinsics
PRIVATE
PUBLIC :: foo
public :: oad_s_foo

CONTAINS

FUNCTION foo(i) RESULT(status)
        INTEGER i
        INTEGER status
        status=1
        return
END FUNCtion

subroutine oad_s_foo(i,status)
        INTEGER i
        integer,intent(out) :: status
        status=1
        return
end subroutine oad_s_foo

end module

program p
use OAD_intrinsics
use m
  integer :: oad_ctmp0
i=2
call oad_s_foo(i,oad_ctmp0)
j = oad_ctmp0
end
