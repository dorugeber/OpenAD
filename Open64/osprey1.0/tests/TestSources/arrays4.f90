module m1
 integer m1_i
end module
module m2
use m1 
contains 
subroutine foo(foo_arg)
  integer, dimension(m1_i) :: foo_arg
  integer, dimension(m1_i) :: foo_local
  foo_local=foo_arg
  call bar()
  contains 
  subroutine bar()
    if (foo_local(2)==4) print *, 'OK'
  end subroutine 
end subroutine
end module 

program arrays4
  use m1
  use m2
  integer program_local(2)
  m1_i=2
  DATA program_local  / 3,4 /
  call foo(program_local)
end program
