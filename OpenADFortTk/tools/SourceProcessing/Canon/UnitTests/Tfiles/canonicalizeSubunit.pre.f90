      subroutine asub(t,a)
        use OAD_intrinsics
        real,intent(in) :: t
        real,intent(out) :: a
        a = t
      end subroutine asub

      program canonicalizeSubunit
        use OAD_intrinsics
        real :: v

        v = 1
        call foo(v)
        print *,v

      contains
        subroutine foo(a)
          real :: a
          real :: oad_ctmp0
          oad_ctmp0 = cos(a)
          call asub(oad_ctmp0,a)
        end subroutine foo
      end program canonicalizeSubunit

