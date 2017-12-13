        subroutine head(y)
          real :: y
          external foo
          if (foo(y) .eq. 0) y = 0
        end subroutine

