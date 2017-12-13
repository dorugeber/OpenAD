      subroutine asub(t,a)
        real,intent(in) :: t
        real,intent(out) :: a
        a = t
      end subroutine asub

      program canonicalizeSubunit
        real :: v

        v = 1
        call foo(v)
        print *,v

      contains
        subroutine foo(a)
          real :: a
          call asub(cos(a),a)
        end subroutine foo
      end program canonicalizeSubunit

