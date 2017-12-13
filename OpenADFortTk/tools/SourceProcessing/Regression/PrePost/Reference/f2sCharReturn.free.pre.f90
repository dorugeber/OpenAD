module OAD_intrinsics
end module
module m
use OAD_intrinsics
contains
  character*16 function foo (l)
    logical :: l
    if (l) then
       foo = '      Yes       '
    else
       foo = '      No        '
    end if
  end function foo
  subroutine oad_s_foo(l,foo)
    logical :: l
    character*16,intent(out) :: foo
    if (l) then
       foo = '      Yes       '
    else
       foo = '      No        '
    end if
  end subroutine oad_s_foo
end

program p
  use OAD_intrinsics
  use m
  logical :: l
  character*16 :: oad_ctmp0
  l=.true.
  call oad_s_foo(l,oad_ctmp0)
  write(*,*) oad_ctmp0
end
