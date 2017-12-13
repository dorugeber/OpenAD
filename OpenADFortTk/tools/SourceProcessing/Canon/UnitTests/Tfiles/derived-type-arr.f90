      subroutine bar(a)
        type baz
           integer :: f1
        end type

        type(baz), dimension(4) :: it

        it(1)%f1 = 3
        it(1)%f1 = 6 + ff(it(2))
      end

