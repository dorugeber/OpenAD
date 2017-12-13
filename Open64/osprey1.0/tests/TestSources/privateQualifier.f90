module modA
private :: a
public :: apublic
private :: foo
public :: modAfoo
integer :: a=0, apublic
interface modAfoo
module procedure foo
end interface
contains 
subroutine foo()
  a=a+apublic
end subroutine
integer function getAPrivate()
  getAPrivate=a
end function
end module

program p
use modA
integer a
aPublic=2
call modAfoo()
call modAfoo()
call foo()
a=1
if (getAPrivate()==4)  then 
print *,'OK'
else 
print *,'FAILED'
endif
contains 
  subroutine foo()
  a=a+apublic
  end subroutine
end program
