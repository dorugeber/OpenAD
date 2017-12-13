        subroutine head(y)
          real :: y
          external foo
          if (foo(y) .eq. 0) then
            y = 0
          endif
        end subroutine

