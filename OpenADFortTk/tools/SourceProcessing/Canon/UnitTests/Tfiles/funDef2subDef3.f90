      program functionComment
        real :: x(2)

        x(1) = 1.5D0
        x(2) = -3.6D0
        print *,f(x(1)) + f(x(2))
      end program functionComment

      function f(x)
!     blah blah
!     aren't these names interesting?: pomentale and gargantini
        real :: x,f

        if ( x <= 0 ) then
           f = 4.0D0
        else if (x<=39) then
           f = exp(x*(1.0D0/(x+2)))
        else
           f = 0
        end if
      end function f

