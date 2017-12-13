      program interfaceTest
        interface
           function foo(i)
             real, dimension(2) :: i,foo
           end function foo
           subroutine oad_s_foo(i,foo)
             real,dimension(2),intent(out) :: foo
             real,dimension(2) :: i
           end subroutine
        end interface
      end program interfaceTest
