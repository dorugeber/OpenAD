      subroutine bar(a)
        use OAD_intrinsics
        type baz
           integer :: f1
        end type

        type(baz), dimension(4) :: it
        real :: oad_ctmp0

        it(1)%f1 = 3
        call oad_s_ff(it(2),oad_ctmp0)
        it(1)%f1 = 6+oad_ctmp0
      end

